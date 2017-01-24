#include <iostream>
#include <config_loader.h>
#include <curl_helper.h>
#include <msgpack.hpp>
#include <vector>
#include <string>

int main(int argc, char* argv[])
{
    if ( argc != 2 )
    {
        std::cout << "Confreak Usage: confreak.tsk confreak.cfg" << std::endl;
        exit(1);
    }
    try
    {
        Config* config = Config::getInstance(argv[1]);
        std::cout << "email:    " << (*config)["LOGIN"]["email"].asString() << std::endl;
        std::cout << "password: " << (*config)["LOGIN"]["password"].asString() << std::endl;
        std::cout << "password: " << (*config)["LOGIN"]["password"].asInt() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception Caught: " << e.what() << std::endl;
        exit(1);
    }

    EasyCurl ec;
    std::string s = "http://localhost:8000/updateAppData?email=s@s&password=111111&appname=HAHA&apptype=1&data=777";
    std::cout << "RES:" << ec.httpGet(s) << std::endl;
    
    // serializes this object.
    std::vector<std::string> vec;
    vec.push_back("Hello");
    vec.push_back("MessagePack");

    // serialize it into simple buffer.
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, vec);

    // deserialize it.
    msgpack::object_handle oh =
        msgpack::unpack(sbuf.data(), sbuf.size());

    // print the deserialized object.
    msgpack::object obj = oh.get();
    std::cout << obj << std::endl;  //=> ["Hello", "MessagePack"]

    // convert it into statically typed object.
    std::vector<std::string> rvec;
    obj.convert(rvec);
    
    return 0;
}
