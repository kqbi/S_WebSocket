//
// Created by kqbi on 2020/6/1.
//

#include "Logger.h"
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include "Utils.h"

namespace S_WS {
    Logger &Logger::Instance() {
        static Logger log;
        return log;
    }

    bool Logger::Init(std::string fileName, int type, int level, int maxFileSize, int maxBackupIndex) {
        _formatter =
                boost::log::expressions::stream
                        << "["
                        << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp",
                                                                                               "%Y-%m-%d %H:%M:%S.%f") /*.%f*/
                        << "|"
                        << boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>(
                                "ThreadID")
                        << "]["
                        << boost::log::expressions::format_named_scope("Scope",
                                                                       boost::log::keywords::format = "(%f:%l)",
                                                                       boost::log::keywords::iteration = boost::log::expressions::reverse,
                                                                       boost::log::keywords::depth = 1)
                        << "]["
                        << boost::log::expressions::attr<severity_level>("Severity")
                        << "] "
                        << boost::log::expressions::smessage;

        switch (type) {
            case console: {
                _fileSink = boost::make_shared<file_sink>(
                        boost::log::keywords::file_name =
                                Utils::ExeDir() + fileName,                       // file name pattern
                        boost::log::keywords::target_file_name = "%Y%m%d_%H%M%S_%N.log",   // file name pattern
                        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0,
                                                                                                                    0,
                                                                                                                    0),
                        boost::log::keywords::rotation_size =
                                maxFileSize * 1024 * 1024,                         // rotation size, in characters
                        boost::log::keywords::open_mode = std::ios::out | std::ios::app
                );

                _fileSink->locked_backend()->set_file_collector(boost::log::sinks::file::make_collector(
                        boost::log::keywords::target = Utils::ExeDir() + "logs",        //folder name.
                        boost::log::keywords::max_size = maxFileSize * maxBackupIndex * 1024 *
                                                         1024,    //The maximum amount of space of the folder.
                        boost::log::keywords::min_free_space = 10 * 1024 * 1024,  //Reserved disk space minimum.
                        boost::log::keywords::max_files = 512
                ));

                _fileSink->set_formatter(_formatter);
                _fileSink->locked_backend()->scan_for_files();
                _fileSink->locked_backend()->auto_flush(true);
                boost::log::core::get()->add_sink(_fileSink);
                break;
            }
            case file: {
                boost::shared_ptr<boost::log::sinks::text_ostream_backend> backend = boost::make_shared<boost::log::sinks::text_ostream_backend>();
                _consoleSink = boost::make_shared<console_sink>(backend);

                _consoleSink->set_formatter(_formatter);
                _consoleSink->locked_backend()->add_stream(
                        boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
                boost::log::core::get()->add_sink(_consoleSink);
            }
                break;
            default: {
                _fileSink = boost::make_shared<file_sink>(
                        boost::log::keywords::file_name =
                                Utils::ExeDir() + fileName,                       // file name pattern
                        boost::log::keywords::target_file_name = "%Y%m%d_%H%M%S_%N.log",   // file name pattern
                        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0,
                                                                                                                    0,
                                                                                                                    0),
                        boost::log::keywords::rotation_size =
                                maxFileSize * 1024 * 1024,                         // rotation size, in characters
                        boost::log::keywords::open_mode = std::ios::out | std::ios::app
                );

                _fileSink->locked_backend()->set_file_collector(boost::log::sinks::file::make_collector(
                        boost::log::keywords::target = Utils::ExeDir() + "logs",        //folder name.
                        boost::log::keywords::max_size = maxFileSize * maxBackupIndex * 1024 *
                                                         1024,    //The maximum amount of space of the folder.
                        boost::log::keywords::min_free_space = 10 * 1024 * 1024,  //Reserved disk space minimum.
                        boost::log::keywords::max_files = 512
                ));

                _fileSink->set_formatter(_formatter);
                _fileSink->locked_backend()->scan_for_files();
                _fileSink->locked_backend()->auto_flush(true);
                boost::log::core::get()->add_sink(_fileSink);

                boost::shared_ptr<boost::log::sinks::text_ostream_backend> backend = boost::make_shared<boost::log::sinks::text_ostream_backend>();
                _consoleSink = boost::make_shared<console_sink>(backend);

                _consoleSink->set_formatter(_formatter);
                _consoleSink->locked_backend()->add_stream(
                        boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
                boost::log::core::get()->add_sink(_consoleSink);
            }
                break;
        }
        boost::log::add_common_attributes();
        boost::log::core::get()->add_global_attribute("Scope", boost::log::attributes::named_scope());
        boost::log::core::get()->set_filter(
                boost::log::expressions::attr<severity_level>("Severity") >= level
        );
        return true;
    }

    void Logger::InitSyslog(const std::string &syslog_server_ip, int syslog_server_port, int level) {
        boost::shared_ptr<boost::log::sinks::syslog_udp5424_backend> backend(new boost::log::sinks::syslog_udp5424_backend());
        backend->set_target_address(syslog_server_ip, syslog_server_port);
        boost::log::sinks::syslog::custom_severity_mapping<severity_level> mapping("Severity");
        mapping[debug] = boost::log::sinks::syslog::debug;
        mapping[info] = boost::log::sinks::syslog::info;
        mapping[warning] = boost::log::sinks::syslog::warning;
        mapping[error] = boost::log::sinks::syslog::error;
        mapping[fatal] = boost::log::sinks::syslog::critical;
        backend->set_severity_mapper(mapping);

        _sysSink = boost::make_shared<sys_sink>(backend);
        boost::log::core::get()->add_sink(_sysSink);

        _sysSink->set_formatter(_formatter);
        boost::log::add_common_attributes();
        boost::log::core::get()->add_global_attribute("Scope", boost::log::attributes::named_scope());
        _sysSink->set_filter(boost::log::expressions::attr<severity_level>("Severity") >= level);
    }
}