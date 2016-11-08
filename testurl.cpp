#include <iostream>
#include <fstream>

#include "Request.h"
#include "Response.h"
#include "urlutil.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << "usage: testurl <url>" << endl;
        return 0;
    }

    const string url(argv[1]);
    
    urllib::Request req(url);
//    req.addHeader("Accept-Language", "zh-CN,zh; q=0.8");
    auto resp = urllib::urlOpen(req);
    cout << resp.info() << endl;
    ofstream fcout("xxx");
    fcout << resp.read();
    return 0;
}
