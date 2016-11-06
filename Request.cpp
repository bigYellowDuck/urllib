#include "Request.h"

#include <algorithm>

namespace urllib
{

using std::string;
using std::pair;
using std::vector;

Request::Request(const string& url)
    : url_(url)
{
    initHeaders();
}

Request::Request(const char* url)
    : url_(string(url))
{
    initHeaders();
}

const string& Request::url() const noexcept
{
    return url_;
}

const vector<pair<const string, string>>& Request::headers() const noexcept
{
    return headers_;
}

bool Request::addHeader(const string& key, const string& value)
{
    auto iter = std::find_if(headers_.begin(), headers_.end(),
            [key](pair<const string, string> header)
            { return header.first == key; });

    if (iter == headers_.end())
    {
        pair<const string, string> header(key, value);
        headers_.push_back(header);
    }
    else
    {
        iter->second = value;
    }

    return true;
}

void Request::initHeaders()
{
    headers_.push_back({"Host", url_});
    headers_.push_back({"User-Agent", "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:47.0) Gecko/20100101 Firefox/47.0"});
    headers_.push_back({"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"});
    headers_.push_back({"Accept-Language", "en-US,en;q=0.5"});
}

}  // end of namespace urllib

