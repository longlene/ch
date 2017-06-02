#include <thread>
#include <iostream>
#include "chan.hpp"

using namespace std;

void ping(chan<string> &ch)
{
    ch.send("ping");
    //ch << "ping";
}

int main(int argc, char const* argv[])
{
    chan<string> ch;
    thread t(ping, std::ref(ch));

    string s;
    ch.recv(s);
    //ch >> s;
    cout << s << '\n';
    t.join();

    return 0;
}
