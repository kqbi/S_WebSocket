//
// Created by kqbi on 2020/6/1.
//

#ifndef UTILS_LOGGER_H
#define UTILS_LOGGER_H

//#include <stdexcept>
#include <string>
//#include <iostream>
//#include <fstream>
#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/asio.hpp>
#include <boost/log/attributes.hpp>
#include <boost/lexical_cast.hpp>

namespace boost {
    BOOST_LOG_OPEN_NAMESPACE
            namespace sinks {
                class syslog_udp5424_backend :
                        public basic_formatted_sink_backend< char >
                {
                    //! Base type
                    typedef basic_formatted_sink_backend< char > base_type;
                    typedef boost::log::aux::light_function< syslog::level (record_view const&) > severity_mapper_type;
                public:
                    /*!
                     * Constructor. Creates a UDP socket-based backend with <tt>syslog::user</tt> facility code.
                     * IPv4 protocol will be used.
                     */
                    BOOST_LOG_API syslog_udp5424_backend() : m_socket(m_service)
                    {

                    }
                    /*!
                     * Destructor
                     */
                    BOOST_LOG_API ~syslog_udp5424_backend()
                    {

                    }

                    BOOST_LOG_API void set_severity_mapper(severity_mapper_type const& mapper)
                    {
                        m_level_mapper = mapper;
                    }

                    BOOST_LOG_API void set_target_address(std::string const& addr, unsigned short port = 514)
                    {
                        m_process_name = boost::log::aux::get_process_name();
                        if(m_process_name.empty())
                        {
                            m_process_name = "-";
                        }
                        m_process_id = boost::lexical_cast<std::string>(boost::log::aux::this_process::get_id().native_id());

                        boost::system::error_code ec;
                        m_local_hostname = asio::ip::host_name(ec);
                        if(m_local_hostname.empty())
                        {
                            m_local_hostname = "-";
                        }
                        boost::asio::ip::address vaddr = boost::asio::ip::address::from_string(addr);
                        m_target_host = asio::ip::udp::endpoint(vaddr, port);
                        if(vaddr.is_v4())
                        {
                            m_socket.open(boost::asio::ip::udp::v4());
                        }
                        else
                        {
                            m_socket.open(boost::asio::ip::udp::v6());
                        }
                    }

                    BOOST_LOG_API void consume(record_view const& rec, string_type const& formatted_message)
                    {
                        send(m_level_mapper.empty() ? syslog::info : m_level_mapper(rec),
                             formatted_message);
                    }

                private:
                    void send(syslog::level lev, string_type const& formatted_message)
                    {
//                        int pri = m_facility | static_cast<int>(lev);
//                        std::time_t t = std::time(nullptr);
//                        std::tm ts;
//                        std::tm* time_stamp = boost::date_time::c_time::localtime(&t, &ts);
#if 0
                        // The packet size is mandated in RFC5424, plus one for the terminating zero

            //# 一条信息的构成
            //SYSLOG-MSG = HEADER SP STRUCTURED-DATA [SP MSG]  # 最后的MSG是可省略的
            //# HEADER = 优先级 版本 空格 时间戳 空格 主机名 空格 应用名 空格 进程id 空格 信息id
            //HEADER = PRI VERSION SP TIMESTAMP SP HOSTNAME
            //SP APP-NAME SP PROCID SP MSGID
            //# PRI优先级
            //PRI = "<" PRIVAL ">" # 优先级 <0>
            //# PRI优先级的值
            //PRIVAL = 1*3DIGIT ; range 0 .. 191 # 3位数字, 0到191
            //# syslog版本号
            //VERSION = NONZERO-DIGIT 0*2DIGIT # 默认为 RFC5424默认为1
            //# 主机名
            //HOSTNAME = NILVALUE / 1*255PRINTUSASCII # - 或 255位可打印ASCII值
            //# 应用名
            //APP-NAME = NILVALUE / 1*48PRINTUSASCII # - 或 48位可打印ASCII值
            //# 进程ID
            //PROCID = NILVALUE / 1*128PRINTUSASCII # - 或 128位可打印ASCII值
            //# 信息ID
            //MSGID = NILVALUE / 1*32PRINTUSASCII # - 或 32位可打印ASCII值
            //# 时间戳
            //TIMESTAMP = NILVALUE / FULL-DATE "T" FULL-TIME # - 或 "0000-00-00"
            //# 完整日期格式
            //FULL-DATE = DATE-FULLYEAR "-" DATE-MONTH "-" DATE-MDAY # "0000-00-00"
            //# 年
            //DATE-FULLYEAR = 4DIGIT # 四位数字
            //# 月
            //DATE-MONTH = 2DIGIT ; 01-12 # 两位数字
            //# 日
            //DATE-MDAY = 2DIGIT ; 01-28, 01-29, 01-30, 01-31 based on month/year
            //# 完整时间（带时区）
            //FULL-TIME = PARTIAL-TIME TIME-OFFSET
            //# 时间（不带时区）
            //PARTIAL-TIME = TIME-HOUR ":" TIME-MINUTE ":" TIME-SECOND # 23:59:59
            //[TIME-SECFRAC]
            //# 小时
            //TIME-HOUR = 2DIGIT ; 00-23 # 两位数字
            //# 分
            //TIME-MINUTE = 2DIGIT ; 00-59 # 两位数字
            //# 秒
            //TIME-SECOND = 2DIGIT ; 00-59 # 两位数字
            //# 时间的小数部分
            //TIME-SECFRAC = "." 1*6DIGIT # 6位数字
            //TIME-OFFSET = "Z" / TIME-NUMOFFSET # 相对于标准时区的偏移， "Z" 或 +/- 23:59
            //# 相对于便准时区的偏移
            //TIME-NUMOFFSET = ("+" / "-") TIME-HOUR ":" TIME-MINUTE # +/- 23:59
            //# 结构化数据
            //STRUCTURED-DATA = NILVALUE / 1*SD-ELEMENT # - 或 SD-ELEMENT
            //SD-ELEMENT = "[" SD-ID *(SP SD-PARAM) "]" # [SD-ID*( PARAM-NAME="PARAM-VALUE")]
            //SD-PARAM = PARAM-NAME "=" %d34 PARAM-VALUE %d34 # PARAM-NAME="PARAM-VALUE"
            //SD-ID = SD-NAME # SD-ID
            //PARAM-NAME = SD-NAME # 参数名
            //PARAM-VALUE = UTF-8-STRING # utf-8字符， '"', '\' 和 ']'必须被转义
            //SD-NAME = 1*32PRINTUSASCII # 1到32位可打印ascii值，除了'=',空格, ']', 双引号(")
            //MSG = MSG-ANY / MSG-UTF8 # 信息
            //MSG-ANY = *OCTET ; not starting with BOM # 八进制字符串 不以BOM开头
            //MSG-UTF8 = BOM UTF-8-STRING # utf-8格式字符串
            //BOM = %xEF.BB.BF # 表明编码方式，以 EF BB BF开头表明utf-8编码
            //UTF-8-STRING = *OCTET # RFC 3629规定的字符
            //OCTET = %d00-255 # ascii
            //SP = %d32 # 空格
            //PRINTUSASCII = %d33-126 # ascii值的33-126，即数字、大小写字母、标点符号
            //NONZERO-DIGIT = %d49-57 # ascii的49-57
            //DIGIT = %d48 / NONZERO-DIGIT # ascii的48-57
            //NILVALUE = "-" # 无对应值
#endif
//                        char packet[2048];
//                        int n = boost::log::aux::snprintf
//                                (
//                                        packet,
//                                        sizeof(packet),
//                                        "<%d>1 %4d-%02d-%02dT%02d:%02d:%02d %s %s %s %s - %s",
//                                        pri,
//                                        time_stamp->tm_year + 1900,
//                                        time_stamp->tm_mon + 1,
//                                        time_stamp->tm_mday,
//                                        time_stamp->tm_hour,
//                                        time_stamp->tm_min,
//                                        time_stamp->tm_sec,
//                                        m_local_hostname.c_str(),
//                                        m_process_name.c_str(),
//                                        m_process_id.c_str(),
//                                        m_msg_id.c_str(),
//                                        formatted_message.c_str()
//                                );
//                        if (n > 0)

                        //            std::size_t packet_size = static_cast<std::size_t>(n) >= sizeof(packet) ? sizeof(packet) - 1u : static_cast< std::size_t >(n);
                        m_socket.send_to(asio::buffer(formatted_message.c_str(), formatted_message.size()), m_target_host);
                        //}
                    }

                private:
                    boost::asio::io_service m_service;
                    boost::asio::ip::udp::socket m_socket;
                    severity_mapper_type m_level_mapper;
                    boost::asio::ip::udp::endpoint m_target_host;
                    std::string m_local_hostname;
                    std::string m_process_name;
                    std::string m_process_id;
                    std::string m_msg_id = "-";
                    int m_facility = syslog::user;

                };
            }
    BOOST_LOG_CLOSE_NAMESPACE
}


namespace S_WS {
    enum severity_level {
        trace,
        debug,
        info,
        warning,
        error,
        fatal
    };

    static std::ostream &operator<<(std::ostream &strm, severity_level level) {
        static const char *strings[] =
                {
                        "trace",
                        "debug",
                        "info",
                        "warning",
                        "error",
                        "fatal"};

        if (static_cast<std::size_t>(level) < sizeof(strings) / sizeof(*strings))
            strm << strings[level];
        else
            strm << static_cast<int>(level);

        return strm;
    }

    class Logger {
    public:

        typedef boost::log::sinks::asynchronous_sink<boost::log::sinks::text_file_backend, boost::log::sinks::unbounded_fifo_queue> file_sink;
        typedef boost::log::sinks::asynchronous_sink<boost::log::sinks::text_ostream_backend, boost::log::sinks::unbounded_fifo_queue> console_sink;
        typedef boost::log::sinks::asynchronous_sink<boost::log::sinks::syslog_udp5424_backend, boost::log::sinks::unbounded_fifo_queue> sys_sink;
        enum LoggerType {
            console = 0,
            file,
        };


        Logger() : _fileSink(0), _consoleSink(0),_sysSink(0) {}

        ~Logger() {
            if (_fileSink) {

                boost::log::core::get()->remove_sink(_fileSink);
                // Break the feeding loop
                _fileSink->stop();
                // Flush all log records that may have left buffered
                _fileSink->flush();
            }
            if (_consoleSink) {
                boost::log::core::get()->remove_sink(_consoleSink);
                // Break the feeding loop
                _consoleSink->stop();
                // Flush all log records that may have left buffered
                _consoleSink->flush();
            }
            if (_sysSink) {
                boost::log::core::get()->remove_sink(_sysSink);
                // Break the feeding loop
                _sysSink->stop();
                // Flush all log records that may have left buffered
                _sysSink->flush();
            }
        }

        static Logger &Instance();

        bool Init(std::string fileName, int type, int level, int maxFileSize, int maxBackupIndex);

        void InitSyslog(const std::string &syslog_server_ip, int syslog_server_port, int level);

        boost::log::sources::severity_logger<boost::log::trivial::severity_level> _logger;

        boost::log::formatter _formatter;

        boost::shared_ptr<file_sink> _fileSink;

        boost::shared_ptr<console_sink> _consoleSink;

        boost::shared_ptr<sys_sink> _sysSink;
    };

#define S_LOG_TRACE(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::trace) << logEvent;

#define S_LOG_DEBUG(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::debug) << logEvent;

#define S_LOG_INFO(logEvent)   BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::info) << logEvent;

#define S_LOG_WARN(logEvent)   BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::warning) << logEvent;

#define S_LOG_ERROR(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::error) << logEvent;

#define S_LOG_FATAL(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::fatal) << logEvent;
}
#endif //UTILS_LOGGER_H
