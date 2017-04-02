#include "Request.h"

#include <algorithm>
#include <iostream>

namespace urllib
{

using std::string;
using std::pair;
using std::vector;
using std::cout;
using std::endl;

Request::Request(const string& url)
    : isHTTP_(true),
      url_(url)
{
    parseUrl();
    initHeaders();
}

Request::Request(const char* url)
    : isHTTP_(true),
      url_(string(url))
{
    parseUrl();
    initHeaders();
}

bool Request::isHTTP() const noexcept
{
    return isHTTP_;
}

const string& Request::url() const noexcept
{
    return url_;
}

const string& Request::host() const noexcept
{
    return host_;
}

const string& Request::uri() const noexcept
{
    return uri_;
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

void Request::parseUrl()
{
    if (url_.find("://") == string::npos)
    {
        cout << url_ << ": is not a valid URL." << endl;
        exit(2);
    }

    if (url_.size() > 1500)
    {
        cout << "url is too long." << endl;
        exit(2);
    }

    if (url_.compare(0, 7, "http://") == 0)
    {
        isHTTP_ = true;    
    }
    else if (url_.compare(0, 8, "https://") == 0)
    {
        isHTTP_ = false;
    }
    else
    {
        cout << "Only HTTP/HTTPS protocol is directly supported." << endl;
        exit(2);
    }

    string tmp;

    if (isHTTP_)
        tmp = string(url_.begin()+7, url_.end());   // remove "http://"
    else
        tmp = string(url_.begin()+8, url_.end());   // remove "https://"

    auto pos = tmp.find("/");
    if (pos == string::npos)
    {
        host_ = tmp;
        uri_ = "/";
    }
    else
    {
        host_ = string(tmp.begin(), tmp.begin()+pos);
        uri_ = string(tmp.begin()+pos, tmp.end());
    }
}

void Request::initHeaders()
{
    headers_.push_back({"Host", host_});
    headers_.push_back({"User-Agent", "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:47.0) Gecko/20100101 Firefox/47.0"});
    headers_.push_back({"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"});
    headers_.push_back({"Accept-Language", "en-US,en;q=0.5"});
}

}  // end of namespace urllib

