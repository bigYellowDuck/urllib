#include <iostream>
#include <fstream>

#include "Request.h"
#include "Response.h"
#include "urlutil.h"

using namespace std;

int main(int argc, char **argv)
{
    auto resp = urllib::urlOpen(argv[1]);
    cout << resp.info() << endl;
    ofstream fcout("xxx");
    fcout << resp.read();



    return 0;
}
