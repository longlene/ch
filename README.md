ch: Go chan like library for C++11.
===========================
Header file only library.

Example:
[buffered.cpp](example/buffered.cpp)

[ch10000.cpp](example/ch10000.cpp)

[chan_test.cpp](example/chan_test.cpp)

[unbuffered.cpp](example/unbuffered.cpp)


```c++
#include <iostream>
#include <chan.hpp>

chan<std::string, 1> ch;
ch.send("hello");

std::string s;
ch.recv(s);

std::cout << s << '\n';
```
