#include "Response.h"

namespace urllib
{
using std::string;

Response::Response(const string& resp)
    : resp_(resp) 
{
    parseRespone();
}


Response::Response(string&& resp)
    : resp_(std::move(resp))
{
    parseRespone();
}

const string& Response::response() const noexcept
{
    return resp_;
}

const string& Response::info() const noexcept
{
    return headers_;
}

const string& Response::read() const noexcept
{
    return body_;
}

void Response::parseRespone()
{
    auto pos1 = resp_.find("\r\n");   // ignore HTTP respone line
    auto pos2 = resp_.find("\r\n\r\n");
    headers_ = std::move(string(resp_.begin()+pos1+2, resp_.begin()+pos2+4));
    body_ = std::move(string(resp_.begin()+pos2+4,resp_.end()));
}

} // end of namespace urllib

