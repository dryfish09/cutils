#define CATCH_CONFIG_MAIN
#include "../src/libs/catch.hpp"
#include "cutils.hpp"
#include <sstream>
#include <string>

using namespace fih;

// ---------- helpers ----------
struct CoutCapture {
    std::ostringstream oss;
    std::streambuf* old;
    CoutCapture() : old(std::cout.rdbuf(oss.rdbuf())) {}
    ~CoutCapture() { std::cout.rdbuf(old); }
    std::string str() const { return oss.str(); }
};

struct CerrCapture {
    std::ostringstream oss;
    std::streambuf* old;
    CerrCapture() : old(std::cerr.rdbuf(oss.rdbuf())) {}
    ~CerrCapture() { std::cerr.rdbuf(old); }
    std::string str() const { return oss.str(); }
};

struct CinRedirect {
    std::istringstream iss;
    std::streambuf* old;
    CinRedirect(const std::string& input) : iss(input), old(std::cin.rdbuf(iss.rdbuf())) {}
    ~CinRedirect() { std::cin.rdbuf(old); }
};

// rang: on non-Windows, force ANSI emission even when stdout is not a TTY.
// On Windows rang routes through WinAPI instead of writing escapes to the
// stream, so stream-redirection based ANSI checks are skipped there.
struct ForceColor {
    ForceColor()  { rang::setControlMode(rang::control::Force); }
    ~ForceColor() { rang::setControlMode(rang::control::Auto); }
};

#if !defined(_WIN32)
#  define HAS_ANSI_ESCAPES 1
#else
#  define HAS_ANSI_ESCAPES 0
#endif

// ---------- log ----------
TEST_CASE("log Info/Debug/Warn writes to stdout", "[log]") {
    CoutCapture cap;
    log(Info,  "hello");
    log(Warn,  "careful");
    log(Debug, "dbg");

    auto s = cap.str();
    REQUIRE(s.find("INFO")    != std::string::npos);
    REQUIRE(s.find("WARN")    != std::string::npos);
    REQUIRE(s.find("DEBUG")   != std::string::npos);
    REQUIRE(s.find("hello")   != std::string::npos);
    REQUIRE(s.find("careful") != std::string::npos);
}

TEST_CASE("log Error/Fatal writes to stderr", "[log]") {
    CerrCapture err;
    CoutCapture out;
    log(Error, "oops");
    log(Fatal, "dead");
    REQUIRE(err.str().find("ERROR") != std::string::npos);
    REQUIRE(err.str().find("FATAL") != std::string::npos);
    REQUIRE(out.str().empty());
}

TEST_CASE("log output ends with a newline", "[log]") {
    CoutCapture cap;
    log(Info, "x");
    REQUIRE(cap.str().back() == '\n');
}

// ---------- delay ----------
TEST_CASE("delayForMillisec throws on ms <= 0", "[delay]") {
    REQUIRE_THROWS_AS(delayForMillisec(-9),  std::invalid_argument);
    REQUIRE_THROWS_AS(delayForMillisec(-1), std::invalid_argument);
}

TEST_CASE("delayForSec throws on sec <= 0", "[delay]") {
    REQUIRE_THROWS_AS(delayForSec(-9),  std::invalid_argument);
    REQUIRE_THROWS_AS(delayForSec(-5), std::invalid_argument);
}

TEST_CASE("delayForMillisec does not throw when ms > 0", "[delay]") {
    REQUIRE_NOTHROW(delayForMillisec(1));
}

TEST_CASE("delayForSec exception message contains the invalid value", "[delay]") {
    try {
        delayForSec(-7);
        FAIL("should have thrown");
    } catch (const std::invalid_argument& e) {
        REQUIRE(std::string(e.what()).find("-7") != std::string::npos);
    }
}

TEST_CASE("delayForMillisec exception message contains the invalid value", "[delay]") {
    try {
        delayForMillisec(-42);
        FAIL("should have thrown");
    } catch (const std::invalid_argument& e) {
        REQUIRE(std::string(e.what()).find("-42") != std::string::npos);
    }
}

// ---------- print colored ----------
TEST_CASE("printc writes text with no trailing newline", "[color]") {
    ForceColor fc;
    CoutCapture cap;
    printc("hi", Red);

    auto s = cap.str();
    REQUIRE(s.find("hi") != std::string::npos);
    REQUIRE_FALSE(s.empty());
    REQUIRE(s.back() != '\n');
#if HAS_ANSI_ESCAPES
    REQUIRE(s.find("\033[") != std::string::npos);
#endif
}

TEST_CASE("printcl writes text with a trailing newline", "[color]") {
    ForceColor fc;
    CoutCapture cap;
    printcl("hi", Blue);

    auto s = cap.str();
    REQUIRE(s.find("hi") != std::string::npos);
    REQUIRE_FALSE(s.empty());
    REQUIRE(s.back() == '\n');
#if HAS_ANSI_ESCAPES
    REQUIRE(s.find("\033[") != std::string::npos);
#endif
}

TEST_CASE("printc/printcl handle empty strings", "[color]") {
    ForceColor fc;
    {
        CoutCapture cap;
        printc("", Red);
        REQUIRE(cap.str().find('\n') == std::string::npos);
    }
    {
        CoutCapture cap;
        printcl("", Red);
        auto s = cap.str();
        REQUIRE_FALSE(s.empty());
        REQUIRE(s.back() == '\n');
    }
}

TEST_CASE("setBg followed by resetBg does not crash", "[color]") {
    ForceColor fc;
    CoutCapture cap;
    setBg(Cyan);
    resetBg();
#if HAS_ANSI_ESCAPES
    REQUIRE(cap.str().find("\033[") != std::string::npos);
#endif
}

TEST_CASE("resetAll does not crash", "[color]") {
    ForceColor fc;
    CoutCapture cap;
    resetAll();
#if HAS_ANSI_ESCAPES
    REQUIRE(cap.str().find("\033[") != std::string::npos);
#endif
}

// ---------- ask ----------
TEST_CASE("ask returns the user's input", "[ask]") {
    CinRedirect input("Alice\n");
    CoutCapture output;
    REQUIRE(ask("Name?") == "Alice");
}

TEST_CASE("ask returns empty string on empty input", "[ask]") {
    CinRedirect input("\n");
    CoutCapture output;
    REQUIRE(ask("Name?") == "");
}

TEST_CASE("ask preserves surrounding spaces", "[ask]") {
    CinRedirect input("  Alice Smith  \n");
    CoutCapture output;
    REQUIRE(ask("Name?") == "  Alice Smith  ");
}

TEST_CASE("ask prints the question to stdout", "[ask]") {
    CinRedirect input("x\n");
    CoutCapture output;
    ask("Name?");
    REQUIRE(output.str().find("Name?") != std::string::npos);
}

// ---------- clearConsole ----------
TEST_CASE("clearConsole emits ANSI clear screen", "[console]") {
    CoutCapture cap;
    clearConsole();
    REQUIRE(cap.str().find("\033[2J")   != std::string::npos);
    REQUIRE(cap.str().find("\033[1;1H") != std::string::npos);
}

// ---------- printAnim ----------
TEST_CASE("printAnim with zero delay prints instantly and ends with newline", "[anim]") {
    CoutCapture cap;
    printAnim("abc", 0);
    REQUIRE(cap.str() == "abc\n");
}

TEST_CASE("printAnim preserves message content", "[anim]") {
    CoutCapture cap;
    printAnim("hello", 0);
    REQUIRE(cap.str().find("hello") != std::string::npos);
}

TEST_CASE("printAnim with empty message prints only a newline", "[anim]") {
    CoutCapture cap;
    printAnim("", 0);
    REQUIRE(cap.str() == "\n");
}
TEST_CASE("getCurrentTime returns non-empty string", "[time]") {
    auto s = getCurrentTime();
    REQUIRE(s.size() >= 19);  
}

TEST_CASE("getCurrentTime prints when streamed", "[time]") {
    CoutCapture cap;
    std::cout << getCurrentTime();
    REQUIRE_FALSE(cap.str().empty());
}
TEST_CASE("something work", "[fg") {
    setFg(Red);
    resetFg();
}
