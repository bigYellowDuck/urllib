#ifndef URLLIB_URLUTIL_H
#define URLLIB_URLUTIL_H

#include <string>

namespace urllib
{
    
    const int IPV4_ADDRLEN = 32;
    const int WEB_DEFAULT_PORT = 80;

    bool url2ip(const std::string& url, char* addr);

    int urlConnect(const std::string& addr);
    int urlConnect2(const std::string& url);

    int urlOpen(const std::string& url);

}  // end of namespace urllib


#endif // URLLIB_URLUTIL_H
