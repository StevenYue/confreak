#include <confreak_comm.h>

namespace confreak {

std::string Comm::loadAppData(const std::string& reqName, Args& args)
{
    std::string url = d_baseUrl;
    url += reqName;
    for ( int i = 0; i < args.size(); ++i )
    {
        if ( i == 0 )
        {
            url += "/?";
        }
        else
        {
            url += "&";
        }
        url += args[i].name + "=" + args[i].value;
    }
    return d_ec.httpGet(url);
}


} // end of namespace confreak
