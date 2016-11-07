#ifndef URLLIB_RESPONSE_H
#define URLLIB_RESPONSE_H

#include <string>

namespace urllib
{

class Response
{
 public:
    explicit Response(const std::string& resp);
    explicit Response(std::string&& resp);
    
    // use default copy ctor/assignment, move ctor/assignment, dtor
    
    const std::string& response() const noexcept;
    const std::string& info() const noexcept;
    const std::string& read() const noexcept;
 
 private:
    void parseRespone();

    std::string headers_;
    std::string body_;
    std::string resp_;
};

} // end of namespace urllib

#endif // URLLIB_RESPONSE_H

