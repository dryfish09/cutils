// cutils.hpp - simple console utils for command-line app programming
// usage: #include "cutils.hpp"
// NOTE: THIS LIB IS IN DEVELOPMENT, you know...

#pragma once 
#include "./libs/rang.hpp"
#include <string>
#include <iostream>
#include <string_view>
#include <cstdlib>
#include <thread>
#include <chrono>
namespace fih {
// log type enum
enum logType {
    Info,
    Debug,
    Warn,
    Error,
    Fatal
};
enum color {
    Green,
    Blue,
    Cyan,
    Gray,
    Red,
    Yellow,
    Magenta,
    Black
};
inline const char* enumToString(logType type) {
    switch(type) {
        case Info:
        return "INFO";
        case Debug:
        return "DEBUG";
        case Warn:
        return "WARN";
        case Error:
        return "ERROR";
        case Fatal:
        return "FATAL";
        default:
        return "UNKNOWN";
    }
}
        
/* 
    log(type, string text);
    example:
    - log(Info, "Initializing");
    - log(Warn, "Additional deps not found");
*/
inline void log(logType typ, std::string_view text) {
    const char* type = enumToString(typ);
    if (typ == Info) {
        std::cout << rang::fg::cyan << "[" << type << ": " << text << "]"<< rang::style::reset << '\n';
    }
    else if (typ == Error || typ == Fatal) {
        std::cerr << rang::fg::red << "[" << type << ": " << text << "]"<< rang::style::reset << '\n';
    }
    else if (typ == Warn) {
        std::cout << rang::fg::yellow << "[" << type << ": " << text << "]"<< rang::style::reset << '\n';
    }
    else {
        std::cout << "[" << type << "]" << ": " << text << '\n';
    }
}
// std::exit wrapper
// usage: quit(0);
inline void quit(int code) {
    std::cout.flush();
    std::cerr.flush();
    std::exit(code);
}
// ask()
// usage: std::string name = ask("What's ur name);
inline std::string ask(std::string_view question) {
    std::cout << rang::fg::green << question << rang::style::reset << '\n';
    std::string answer;
    std::getline(std::cin, answer);
    return answer;
}
// print colorful (printc)
// usage: printc("text", Red);
inline void printc(std::string_view msg, color col) {
    switch (col) {
        case Red:
        std::cout << rang::fg::red;
        break;
        case Green:
        std::cout << rang::fg::green;
        break;
        case Blue:
        std::cout << rang::fg::blue;
        break;
        case Gray:
        std::cout << rang::fg::gray;
        break;
        case Cyan:
        std::cout << rang::fg::cyan;
        break;
        case Yellow:
        std::cout << rang::fg::yellow;
        break;
        case Magenta:
        std::cout << rang::fg::magenta;
        break;
        case Black:
        std::cout << rang::fg::black;
        break;
    }
    std::cout << msg << rang::style::reset << '\n';
}
inline void setBg(color col) {
    switch (col) {
        case Red:
        std::cout << rang::bg::red;
        break;
        case Green:
        std::cout << rang::bg::green;
        break;
        case Blue:
        std::cout << rang::bg::blue;
        break;
        case Gray:
        std::cout << rang::bg::gray;
        break;
        case Cyan:
        std::cout << rang::bg::cyan;
        break;
        case Yellow:
        std::cout << rang::bg::yellow;
        break;
        case Magenta:
        std::cout << rang::bg::magenta;
        break;
        case Black:
        std::cout << rang::bg::black;
        break;
    }
}
inline void resetBg() {
    std::cout << rang::bg::reset;
}

inline void delayForMillisec(int ms) {
    if (ms > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    } else {
        log(Fatal, "Invalid delay agrument");
        quit(1);
    }
}

inline void delayForSec(int sec) {
    if (sec > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(sec));
    } else {
        log(Fatal, "Invalid delay agrument");
        quit(1);
    }
}
inline void printAnim(std::string_view msg, int char_per_ms) {
    for (char c : msg) {
        if(char_per_ms <= 0) {
            std::cout << c << std::flush;
        }
        std::cout << c << std::flush; 
        delayForMillisec(char_per_ms);
    }
    std::cout << '\n';
}
} // namespace fih
