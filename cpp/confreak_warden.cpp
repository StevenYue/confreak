//Steeng created at Sat Feb 11 19:27:05 EST 2017
#include <confreak_warden.h>
#include <unistd.h>

namespace confreak { 

Warden::Warden(Application::AppType appType, Duty duty, 
            const std::string& baseUrl, const std::string serialPort,
            const Comm::Args& args):
d_duty(duty), d_comm(baseUrl, serialPort, args)
{
    confreak::ConfreakRt cr = d_comm.loadAppData();
    assert ( cr.rc == 0 );
    d_apps = ConfreakApps(cr.rs, appType);
    LOG_INFO << "Warden " << Application::AppTypeStrMap[appType] << " badged in" << LOG_END;
}

Warden::Warden()
{}

Warden::~Warden()
{}

Warden::Duty& Warden::duty()
{
    return d_duty;
}

Comm& Warden::comm()
{
    return d_comm;
}
    
ConfreakApps& Warden::apps()
{
    return d_apps;
}

void* monitorDuty(void* data)
{
    Warden* wardenPtr = (Warden*)(data);
    LOG_INFO << *wardenPtr << LOG_END;
    Comm& comm = wardenPtr->comm();
    while ( true )
    {
        sleep(2);
        ConfreakRt cr = comm.serialRead();
        if ( cr.rc < 0 )
        {
            LOG_ERROR << "MonitorDuty, serial read error:" << cr.rs << LOG_END; 
            continue;
        }
        LOG_INFO << "Read " << cr.rc << " bytes, content:" << cr.rs << LOG_END;
        AppTracker at(cr.rs);
        if ( at.info.size() == 0 )
        {
            continue;
        }
        cr = comm.updateAppData(at.info["appname"], at.info["data"], Application::MONITOR_APP);
        if ( cr.rc < 0 )
        {
            LOG_ERROR << "MonitorDuty, update app data error:" << cr.rs << LOG_END; 
        }
    }
    return NULL;    
}

void* controlDuty(void* data)
{
    Warden* wardenPtr = (Warden*)(data);
    LOG_INFO << *wardenPtr << LOG_END;
    while ( true )
    {
        sleep(2);
        LOG_INFO << *wardenPtr << LOG_END;
    }
    return NULL;    
}

}  //end of namespace confreak
