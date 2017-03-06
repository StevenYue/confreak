//Steeng created at Sat Feb 11 19:27:05 EST 2017
#include <confreak_warden.h>
#include <unistd.h>
#include <confreak_arduinoTranslator.h>
#include <config_loader.h>

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
    int frameRate = (*Config::getInstance())["COMM"]["framerate"].asInt();
    while ( true )
    {
        sleep(frameRate);
        ConfreakRt cr = comm.serialRead();
        if ( cr.rc < 0 )
        {
            LOG_ERROR << "MonitorDuty, serial read error:" << cr.rs << LOG_END; 
            continue;
        }
        LOG_INFO << "Read " << cr.rc << " bytes, content:" << cr.rs << LOG_END;
        try
        {
            //have to catch this one, serial can send anything unparseable
            ConfreakApps cApps;
            cr = arduinoTranslator::toConfreakApps(cr.rs, cApps);
            if ( cr.rc )
            {
                LOG_ERROR << "MonitorDuty, arduino translation error:" << cr.rs << LOG_END; 
                continue;
            }
            for ( auto it = cApps.apps().begin(); it != cApps.apps().end(); ++it )
            {
                Application app = it->second;
                //Only send http update for monitor app
                if ( Application::MONITOR_APP == app.appType()
                        && wardenPtr->apps().apps()[app.appName()] != app )
                {
                    //To avoid making unnecessary http call, don't update data if there is no change 
                    wardenPtr->apps().apps()[app.appName()].updateAppData(app);
                    cr = comm.updateAppData(app);
                    if ( cr.rc < 0 )
                    {
                        LOG_ERROR << "MonitorDuty, update app data error:" << cr.rs << LOG_END; 
                    }
                    else
                    {
                        LOG_INFO << "MonitorDuty, update app data successful:" << cr.rs << LOG_END; 
                    }
                }
                else
                {
                    LOG_INFO << "MonitorDuty, no need to send http update:" << LOG_END; 
                }
            }
        }
        catch (const std::exception& e)
        {
            LOG_ERROR << "Monitor Warden error " << e.what() << LOG_END;
        }
    }
    return NULL;
}

void* controlDuty(void* data)
{
    Warden* wardenPtr = (Warden*)(data);
    LOG_INFO << *wardenPtr << LOG_END;
    int frameRate = (*Config::getInstance())["COMM"]["framerate"].asInt();
    while ( true )
    {
        sleep(frameRate);
        LOG_INFO << *wardenPtr << LOG_END;
    }
    return NULL;    
}

}  //end of namespace confreak
