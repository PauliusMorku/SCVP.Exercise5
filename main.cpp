#include <systemc.h>
#include <iostream>
#include <kpn.h>
#include <kpn_combined.h>

using namespace std;

int sc_main(int argc, char* argv[])
{
    kpn kahn1("kpn");
    //kpn_combined kahn2("kpn_combined");
    sc_start();
    return 0;
}
