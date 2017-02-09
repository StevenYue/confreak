#include <confreak_comm.h>

namespace confreak {

const int SERIAL_MODE_WR_NO_TERMINAL_CONTROL = O_RDWR | O_NOCTTY;

Comm::Comm(const std::string& baseUrl, const std::string& serialPortName):
d_baseUrl(baseUrl)
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
    std::cout << "Serial port: " << serialPortName << " opened successfully" << std::endl;
};

Comm::~Comm()
{
    closeSerial(d_serialfd);
}

ConfreakRt Comm::getWithArgs(const std::string& reqName, Args& args)
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
    ConfreakRt cr;
    cr.rc = d_ec.httpGet(url, cr.rs);
    return cr;
}

ConfreakRt Comm::loadAppData(Args& args)
{
    return getWithArgs("loadAllApplications", args); 
}


int Comm::serialWrite(const std::string& data)
{
    return writeSerial(d_serialfd, data.c_str());
}

ConfreakRt Comm::serialRead()
{
    ConfreakRt cr;
    char buf[255];
    cr.rc = readSerial(d_serialfd, buf, 255);
    cr.rs = std::string(buf);
    return cr;
}

} // end of namespace confreak
