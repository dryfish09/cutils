#include "cutils.hpp"
#include <string>
int main() {
    std::string time = fih::getCurrentTime();
    fih::printcl("time: " + time, Cyan);
    return 0;
}
