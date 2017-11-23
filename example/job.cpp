#include <thread>
#include <iostream>
#include "chan.hpp"

using namespace std;

void ping(chan<string> &ch)
{
    string s;
    int cnt = 0;
    while (ch >>s)
        cout << ++cnt << " -> " << s << '\n';
}

int main(int argc, char const* argv[])
{
    chan<string> ch;
    thread t(ping, std::ref(ch));

    string s;
    for (int i = 0; i < 10000; ++i) {
        ch << "ping";
    }

    ch.close();
    t.join();
    return 0;
}
