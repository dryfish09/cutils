# DryFish's Console Utils library
CUtils (Console Utils) is a **simple, ez to use** console utils library for command-line app programming
## Requirement
C++ 17+ (for future updates), C++ projects, all dependencies are included in this library, you just need to add it to your project, then use.
## Usage
### Import to a project
1. Clone this repository:
```bash
git clone git@github.com:dryfish09/cutils
```
Or
```bash
git clone https://github.com/dryfish09/cutils.git
```
2. Copy its source code to your project
```bash
cd cutils/src && cp * ur/project/path
```
3. Coding
### Basic use
1. Logging
```cpp
#include "cutils.hpp"
fih::log(fih::Info, "text");
```
2. Exit (with auto flush stuff before exit)
```cpp
#include "cutils.hpp"
fih::quit(99);
```
3. Ask
```cpp
fih::ask("hi, how are you today?");
```
4. Colorful print
```cpp
fih::printc("msg", fih::Blue);
```
> [!WARNING]
> `fih::quit(int code)` just flush stdout, stderr, exit, it doesn't destruct constructor, free pointers, etc.
>
> Use with you own risk!

**For more example, view [CUtils's code examples](./examples/)**
### Advanced use
Contribute to this project
## Roadmap:
- [ ] Basic console QoL
- [ ] File I/O wrapper
- [ ] JSON
- [ ] Corountines
- [ ] Kotlin/C# features/QoL (not GC)

### License
This library is free to use, under the **MIT license**, iew [License file](LICENSE) for more details.
#### Credit:
This library use `rang` — a simple, lightweigh console color library, license: The Unlicense
<!-- TODO: add author and repo link -->
Thanks C++ for a good language and awesome `std` library, thanks library authors very much, thank you for visiting!

*Made with :fish: by @dryfish09, Thanks for visiting!*
