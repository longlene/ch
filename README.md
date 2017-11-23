ch: Go chan like library for C++11.
===========================
Header file only library.

Example:

[buffered.cpp](example/buffered.cpp)

[job.cpp](example/ch10000.cpp)

[chan_test.cpp](example/chan_test.cpp)

[unbuffered.cpp](example/unbuffered.cpp)


```c++
#include <iostream>
#include <chan.hpp>

chan<std::string, 1> ch;
ch << "hello";

std::string s;
ch >> s;

std::cout << s << '\n';
```

Inspired by https://github.com/tylertreat/chan
