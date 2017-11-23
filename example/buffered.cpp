#include <iostream>
#include <string>
#include "chan.hpp"

using namespace std;

int main(int argc, char const* argv[])
{
    chan<string, 2> ch;
    ch << "buffered" << "ch";

    string s;
    ch >> s;
    cout << s << '\n';

    ch >> s;
    cout << s << '\n';


    return 0;
}

