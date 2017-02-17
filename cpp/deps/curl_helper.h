#ifndef INCLUDED_CURL_HELPER
#define INCLUDED_CURL_HELPER
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

class EasyCurl{
private:
    CURL* d_curl;

public:
    EasyCurl()
    {
        d_curl = curl_easy_init();
        if ( !d_curl ) 
        {
            throw std::runtime_error("Error initializing curl");
        }
    }
    
    ~EasyCurl()
    {}

    void EasyCurlCleanUp()
    {
        if ( !d_curl )
        {
            curl_easy_cleanup(d_curl);
        }
    }

    static size_t writefunc(void *contents, size_t size, size_t nmemb, void* s)
    {
        ((std::string*)s)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    //return 0 on success, -1 on failure
    int httpGet(const std::string& url, std::string& rstr)
    {
        CURLcode res;
        std::string s;
        curl_easy_setopt(d_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(d_curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(d_curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(d_curl, CURLOPT_WRITEDATA, &s);
        res = curl_easy_perform(d_curl);
        if ( res != CURLE_OK )
        {
            std::ostringstream os;
            os << "Error Curl: " << curl_easy_strerror(res);
            rstr = os.str();
            return -1;
        }
        else
        {
            rstr = s;
            return 0;
        }
    }
};

#endif
