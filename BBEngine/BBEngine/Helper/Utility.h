#include <memory>
#include <string>
#include <stdexcept>
#include <stdarg.h>

namespace BBUtility {

    constexpr int MAXPRINTSIZE = 1024;

    //Please don't look at this :c
    //Will replace this later

    template<typename ... Args>
    std::string string_format(const std::string& format, Args ... args)
    {
        int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
        if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
        auto size = static_cast<size_t>(size_s);
        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, format.c_str(), args ...);
        return std::string(buf.get(), buf.get() + size - 1);
    }

    std::string string_format(const std::string& format, va_list list)
    {
        char result[MAXPRINTSIZE];
        vsprintf(result, format.c_str(), list);
        return std::string(result);
    }
}