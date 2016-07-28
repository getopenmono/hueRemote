#ifndef __hue_h__
#define __hue_h__

#include <mono.h>
#include <http_client.h>

using namespace mono;

class HueLight
{
    int lampNo;
    String hueIp;
    String username;

#ifndef EMUNO
    network::HttpClient client;
#endif

    static const char *stateRegex;
    static const char *briRegex;
    static const char *stateFormat;
    static const char *briFormat;

    void httpDataReady(network::HttpClient::HttpResponseData const &data);

public:
    HueLight(String ip, String userName, int lampNumber);

    HueLight(const HueLight &other);

    HueLight &operator=(const HueLight &other);

    void setState(bool on);

    void setBrightness(uint8_t bright);
};

#endif
