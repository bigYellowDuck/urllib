#ifndef URLLIB_REQUEST_H
#define URLLIB_REQUEST_H

#include <string>
#include <vector>
#include <utility>

namespace urllib
{

class Request
{
 public:
    explicit Request(const std::string& url);
    Request(const char* url);

    // using defualt copy ctor/assignment, move ctor/assignment, dtor
 
    const std::string& url() const noexcept;
    const std::string& host() const noexcept;
    const std::string& uri() const noexcept;
    const std::vector<std::pair<const std::string, std::string>>& headers() const noexcept;

    bool addHeader(const std::string& key, const std::string& value);
 
 private:
    void parseUrl();
    void initHeaders();

    std::string url_;
    std::string host_;
    std::string uri_;
    std::vector<std::pair<const std::string, std::string>> headers_;
};

}  // end of namespace urllib

#endif // URLLIB_REQUEST_H

