#include "cutils"
int main() {
    // newline
    fih::printcl(":)", fih::Red);
    // no newline
    fih::printc("hi!", fih::Cyan); // supported: Green, Yellow, Black, Magenta, Red,...
    fih::printc("!", fih::Red);
    return 0;
}
