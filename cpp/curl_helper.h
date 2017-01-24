#ifndef INCLUDED_CURL_HELPER
#define INCLUDED_CURL_HELPER
#include <curl/curl.h>
#include <iostream>

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
    {
        curl_easy_cleanup(d_curl);
    }

    static size_t writefunc(void *contents, size_t size, size_t nmemb, void* s)
    {
        ((std::string*)s)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    std::string httpGet(const string& url)
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
            std::cerr << "Error Curl: " << curl_easy_strerror(res) << std::endl;
        }
        return s;
    }
};

#endif
