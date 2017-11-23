ch: Go chan like library for C++11.
===========================
Header file only library.

Example:

[buffered.cpp](example/buffered.cpp)

[job.cpp](example/ch10000.cpp)

[chan_test.cpp](example/chan_test.cpp)

[unbuffered.cpp](example/unbuffered.cpp)


```c++
#include <thread>
#include <iostream>
#include "chan.hpp"

using namespace std;

void worker(chan<string> &ch)
{
    string s;
    int cnt = 0;
    while (ch >> s)
        cout << ++cnt << " -> " << s << '\n';
}

int main(int argc, char const* argv[])
{
    chan<string> ch;
    thread t(worker, std::ref(ch));

    for (int i = 0; i < 10000; ++i) {
        ch << "ping";
    }

    ch.close();
    t.join();
    return 0;
}
```

Inspired by https://github.com/tylertreat/chan
