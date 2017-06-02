#include <iostream>
#include <string>
#include "../include/chan.hpp"

using namespace std;

int main(int argc, char const* argv[])
{
    chan<string, 2> ch;
    ch.send("buffered");
    //ch << "buffered" << "ch";
    ch.send("ch");

    string s;
    ch.recv(s);
    cout << s << '\n';

    ch.recv(s);
    cout << s << '\n';


    return 0;
}

