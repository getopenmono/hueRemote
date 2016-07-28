#include "hue.h"

const char *HueLight::stateRegex = "\"on\":(true|false),";
const char *HueLight::briRegex = "\"bri\":\\d+,";
const char *HueLight::stateFormat = "http://%s/api/%s/lights/%i/state?method=PUT&body=%%7B%%22on%%22%%3A%s%%7D";
const char *HueLight::briFormat = "http://%s/api/%s/lights/%i/state?method=PUT&body=%%7B%%22bri%%22%%3A%i%%7D";

HueLight::HueLight(String ip, String userName, int lampNumber)
{
    lampNo = lampNumber;
    hueIp = "10.0.41.190";
    username = userName;
}

// HueLight::HueLight(const HueLight &other)
// {
//     printf("copy constructor!");
//     lampNo = other.lampNo;
//     hueIp = other.hueIp;
//     username = other.username;
//     client = other.client;
// }

// HueLight &HueLight::operator=(const HueLight &other)
// {
//     lampNo = other.lampNo;
//     hueIp = other.hueIp;
//     username = other.username;
//     client = other.client;
//     printf("Assign (malloced: %i) ptr from: 0x%x to 0x%x, user ptr: 0x%x\r\n", this->hueIp.malloced, other.hueIp.stringData, this->hueIp.stringData, this->username.stringData);
//
//     return *this;
// }

void HueLight::setState(bool on)
{
    const char *state = on ? "true" : "false";
    String url = String::Format(stateFormat, hueIp(), username(), lampNo, state);
    printf("Querying: %s\r\n", url());
    client = network::HttpClient(url);
}
