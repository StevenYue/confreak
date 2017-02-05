# confreak

Mongodb Requires Super User Access

Nodejs Modules needed
```
npm i body-parser  client-sessions  ejs  express  mongodb  mongoose  nodemon websocket
```

libcurl is needed, to install:
```
sudo apt-get install libcurl4-gnutls-dev
```
To use libcurl, add to the CMakeList (where the lib is installed)
```
include_directories("/usr/lib/x86_64-linux-gnu/")
```
