#include <iostream>
#include "chan.hpp"

using namespace std;
int main(int argc, char const* argv[])
{
    chan<int, 1> ch;
    ch << 7;
    int i;
    ch >> i;
    cout << "recv: " << i << '\n';

    return 0;
}

