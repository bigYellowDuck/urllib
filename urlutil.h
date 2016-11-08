#ifndef URLLIB_URLUTIL_H
#define URLLIB_URLUTIL_H

#include <string>

namespace urllib
{
    
    class Request;
    class Response;
    const int _8K = 8192;
    const int HTTP_OK = 200;
    const int HTTP_REDIRECT1 = 301;
    const int HTTP_REDIRECT2 = 302;
    const char* const HTTP_PORT = "80";
    const std::string CRLF("\r\n");
    
    int urlConnect(const std::string& url);

    Response urlOpen(const Request& req);

}  // end of namespace urllib


#endif // URLLIB_URLUTIL_H

