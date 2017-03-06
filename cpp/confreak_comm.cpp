#include <confreak_comm.h>
#include <config_loader.h>

namespace confreak {

const int SERIAL_MODE_WR_NO_TERMINAL_CONTROL = O_RDWR | O_NOCTTY;

Comm::Comm(const std::string& baseUrl, const std::string& serialPortName, const Args& args):
d_baseUrl(baseUrl), d_args(args), d_ec()
{
    d_serialfd = openSerial(serialPortName.c_str(), SERIAL_MODE_WR_NO_TERMINAL_CONTROL);
    if ( d_serialfd == -1 )
    {
        throw std::runtime_error("Error opening serial port");
    }
    if ( 0 != configSerial(d_serialfd) )
    {
        throw std::runtime_error("Error config serial port");
    }
    LOG_INFO << "Serial port: " << serialPortName << " opened successfully" << LOG_END;
};

Comm::Comm()
{}

Comm::~Comm()
{
    //closeSerial(d_serialfd);
}

ConfreakRt Comm::getWithArgs(const std::string& reqName, const Args& args)
{
    std::string url = d_baseUrl;
    url += reqName;
    for ( auto it = args.begin(); it != args.end(); ++it )
    {
        if ( it == args.begin() )
        {
            url += "?";
        }
        else
        {
            url += "&";
        }
        url += it->first + "=" + it->second;
    }
    ConfreakRt cr;
    LOG_INFO << "URL:" << url << LOG_END;
    cr.rc = d_ec.httpGet(url, cr.rs);
    return cr;
}

ConfreakRt Comm::loadAppData(const Args& args)
{
    return getWithArgs("loadAllApplications", args); 
}

ConfreakRt Comm::loadAppData()
{
    return loadAppData(d_args); 
}

ConfreakRt Comm::updateAppData(Application& app)
{
    Args args = d_args;
    args["appname"]     = app.appName();
    args["data"]        = app.dataAsString();
    args["apptype"]     = app.appTypeAsString();
    return getWithArgs("updateAppData", args);
}

int Comm::serialWrite(const std::string& data)
{
    return writeSerial(d_serialfd, data.c_str(), data.length());
}

ConfreakRt Comm::serialRead()
{
    ConfreakRt cr;
    char buf[1024];
    cr.rc = readSerial(d_serialfd, buf, 1024);
    if ( cr.rc < 0 )
    {
        cr.rs = std::string("Serial Read error, read negative bytes");
    }
    else
    {
        cr.rs = std::string(buf, cr.rc);
    }
    return cr;
}

} // end of namespace confreak
