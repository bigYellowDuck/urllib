#include "urlutil.h"

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>

namespace urllib
{

    using std::string;

    bool url2ip(const string& url, char* addr)
    {
        struct hostent* hptr;
        
        if ((hptr = ::gethostbyname(url.data())) == NULL)
            return false;
        
        for (char** pptr = hptr->h_addr_list; *pptr != NULL; ++pptr)
        {   
            if (inet_ntop(hptr->h_addrtype, *pptr, addr, IPV4_ADDRLEN) == NULL)
                return false;
        }

        return true;
    }

    int urlConnect(const string& addr)
    {
        struct sockaddr_in servaddr;
        
        int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(WEB_DEFAULT_PORT);
        inet_pton(AF_INET, addr.data(), &servaddr.sin_addr);

        ::connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

        return sockfd;
    }

    int urlConnect2(const string& url)
    {
        int sockfd;
        const char* port = "80"; 
        struct addrinfo hints, *res, *ressave;
        
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if (::getaddrinfo(url.data(), port, &hints, &res) != 0)
            return -1;
        ressave = res;
        
        do {
            sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
            if (sockfd < 0)
                continue;
        
            if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
                break;
            
            ::close(sockfd);
        } while((res = res->ai_next) != NULL); 

        if (res == NULL)
            return -1;

        ::freeaddrinfo(ressave);
        
        return sockfd;
    }

    int urlOpen(const string& url)
    {
        return sizeof(url);
    }

} // end of namespace urllib





