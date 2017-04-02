#include "urlutil.h"
#include "Request.h"
#include "Response.h"

#include <iostream>

#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

namespace urllib
{

    using std::string;


    int urlConnect(const string& url, bool isHTTP)
    {
        int sockfd;
        struct addrinfo hints, *res, *ressave;
        
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        
        if (::getaddrinfo(url.data(), isHTTP?HTTP_PORT:HTTPS_PORT, &hints, &res) != 0)
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


    Response urlOpen(const Request& req)
    {
        if (req.isHTTP())
            return urlOpenHTTP(req);
        else
            return urlOpenHTTPS(req);
    }


    Response urlOpenHTTP(const Request& req)
    {
        char buf[_8K+1];
        int n, cnt;      
        string resp;      // response content
        
        int connfd = urlConnect(req.host());    // connect to url:80
        
        string requestLine = "GET " + req.uri() + " HTTP/1.1" + CRLF;
        string requestHeaders;
        auto headers = req.headers();
        for (auto& header : headers)
            requestHeaders += header.first + ": " + header.second + CRLF;  
        requestHeaders += CRLF;
        
        n = write(connfd, requestLine.data(), requestLine.size());   // send the request
        n = write(connfd, requestHeaders.data(), requestHeaders.size());  // ignore the return value
        
        // std::cout << requestLine << requestHeaders << std::endl;
        resp.reserve(_8K*50);
        while ((n = read(connfd, buf, sizeof(buf)-1)) > 0)
        {
                // std::cout << n << "------------" << ++cnt << std::endl;
                resp += string(buf, buf+n);
                if (string(resp.end()-20, resp.end()).find("</html>") != string::npos)
                    break;
        }
        
        int statusCode = std::stoi(string(resp.begin()+9, resp.begin()+12));  // get the response status code
        if (statusCode == HTTP_OK)
        {
            close(connfd);
            return Response(std::move(resp));
        }
        else if (statusCode == HTTP_REDIRECT1 || statusCode == HTTP_REDIRECT2)
        {
            auto pos = resp.find("Location");
            if (pos == string::npos)
            {
                std::cout << "Can't not find redirect url." << std::endl;
                exit(2);
            }
            const string newUrl(resp.begin()+pos+10, resp.begin()+resp.find("\r\n", pos));
            close(connfd);
            return urlOpen(newUrl);   // urlopen again
        }
        else
        {
            std::cout << "Error! Http respone status code " << statusCode << "." << std::endl;
            exit(2);
        }
    }



    Response urlOpenHTTPS(const Request& req)
    {
        char buf[_8K+1];
        int n, cnt;      
        string resp;      // response content
        
        int connfd = urlConnect(req.host(), false);    // connect to url:443
        // SSL initial
        SSL_library_init();
        SSL_load_error_strings();
        SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
        if (ctx == NULL)
        {
            ERR_print_errors_fp(stderr);
            exit(1);
        }
        SSL *ssl = SSL_new(ctx);
        if (ssl == NULL)
        {
            ERR_print_errors_fp(stderr);
            exit(1);
        }
        // assioate ssl to fd
        if ((SSL_set_fd(ssl, connfd)) == 0)
        {
            ERR_print_errors_fp(stderr);
            exit(1);
        }
        if ((SSL_connect(ssl)) != 1)
        {
            ERR_print_errors_fp(stderr);
            exit(1);
        }
        string requestLine = "GET " + req.uri() + " HTTP/1.1" + CRLF;
        string requestHeaders;
        auto headers = req.headers();
        for (auto& header : headers)
            requestHeaders += header.first + ": " + header.second + CRLF;  
        requestHeaders += CRLF;
        
        n = SSL_write(ssl, requestLine.data(), requestLine.size());   // send the request
        n = SSL_write(ssl, requestHeaders.data(), requestHeaders.size());  // ignore the return value
        
        // std::cout << requestLine << requestHeaders << std::endl;
        resp.reserve(_8K*50);
        
        while ((n = SSL_read(ssl, buf, sizeof(buf)-1)) > 0)
        {
                // std::cout << n << "------------" << ++cnt << std::endl;
                resp += string(buf, buf+n);
                if (string(resp.end()-20, resp.end()).find("</html>") != string::npos)
                    break;
        }
        
        int statusCode = std::stoi(string(resp.begin()+9, resp.begin()+12));  // get the response status code
        if (statusCode == HTTP_OK)
        {
            close(connfd);
            SSL_free(ssl);
            SSL_CTX_free(ctx);
            ERR_free_strings();
            return Response(resp);
        }
        else if (statusCode == HTTP_REDIRECT1 || statusCode == HTTP_REDIRECT2)
        {
            auto pos = resp.find("Location");
            if (pos == string::npos)
            {
                std::cout << "Can't not find redirect url." << std::endl;
                exit(2);
            }
            const string newUrl(resp.begin()+pos+10, resp.begin()+resp.find("\r\n", pos));
            close(connfd);
            SSL_free(ssl);
            SSL_CTX_free(ctx);
            ERR_free_strings();
            return urlOpen(newUrl);   // urlopen again
        }
        else
        {
            std::cout << "Error! Http respone status code " << statusCode << "." << std::endl;
            exit(2);
        }
    }

} // end of namespace urllib

