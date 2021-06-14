//
// Created by kqbi on 2020/5/29.
//

#ifndef S_WEBSOCKET_UTIL_H
#define S_WEBSOCKET_UTIL_H

#include <string>
#include <boost/utility/string_view.hpp>

namespace S_WS {
    class Utils {
    public:
        static std::string getNewConnectionId();

        static std::string FindField(const char *buf, const char *start, const char *end, int bufSize = 0);

        static std::string base64_encode(std::uint8_t const *data, std::size_t len);

        static std::string base64_encode(boost::string_view s);

        static std::string base64_decode(boost::string_view data);

        static std::string ExePath();

        static std::string ExeDir();

        static std::string ExeName();
    };
}
#endif //S_WEBSOCKET_UTIL_H
