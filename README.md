# DryFish's Console Utilities library

CUtils (Console Utilities) is a **simple, easy-to-use** console utilities library for command-line app programming.

## Requirements

C++17+ (for future updates), C++ projects. All dependencies are included in this library — you just need to add it to your project, then use it.

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
cd cutils/src && cp -r * ur/project/path
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
> `fih::quit(int code)` flushes `std::cout` and `std::cerr`, then calls `std::exit(code)`. As a result:
> - destructors of objects with **automatic storage duration** (local/stack variables) are **not** run;
> - destructors of objects with **static storage duration** (globals, static locals) **are** run, and `std::atexit` handlers are called.
>
> If your cleanup relies on local destructors (RAII), prefer returning from `main()` instead of calling `quit()`.

**For more examples, view [CUtils's code examples](./examples/)**

> [!CAUTION]
> `fih::setStyle(Style style)` may not work correctly on old devices. For more details, view [rang library's original repository](https://github.com/agauniyal/rang)

### Advanced use

Contribute to this project.

## Roadmap:

- [ ] Basic console QoL
- [ ] File I/O wrapper
- [ ] JSON
- [ ] Coroutines
- [ ] Kotlin/C# features/QoL (not GC)

### License

This library is free to use, under the **MIT license**. View [License file](LICENSE) for more details.

#### Credit:

This library uses `rang` — a simple, lightweight console color library, license: The Unlicense.

I also use Catch2 for testing, thanks Catch2 team!

Thanks C++ for a good language and awesome `std` library, thanks library authors very much, thank you for visiting!

*Made with :fish: by @dryfish09, Thanks for visiting!*

