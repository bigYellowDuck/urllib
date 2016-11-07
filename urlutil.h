#ifndef URLLIB_URLUTIL_H
#define URLLIB_URLUTIL_H

#include <string>

namespace urllib
{
    
    class Request;
    class Response;
    const int IPV4_ADDRLEN = 32;
    const int WEB_DEFAULT_PORT = 80;
    const int _8K = 8192;
    const int HTTP_OK = 200;
    const int HTTP_REDIRECT1 = 301;
    const int HTTP_REDIRECT2 = 302;
    const std::string CRLF("\r\n");
    
    bool url2ip(const std::string& url, char* addr);

    int urlConnect(const std::string& addr);
    
    int urlConnect2(const std::string& url);

    Response urlOpen(const Request& req);

}  // end of namespace urllib


#endif // URLLIB_URLUTIL_H
