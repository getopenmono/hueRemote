
#ifndef app_controller_h
#define app_controller_h

#include <mono.h>
#include <SPI.h>
#include <wireless/redpine_module.h>
#include <wireless/module_communication.h>
#include <SDFileSystem.h>
#include "hue.h"

using namespace mono;
using namespace mono::ui;

#define WIFI_SSID "SSID"
#define WIFI_PASSPHRASE "PASSPHRASE"
#define HUE_HOST "SERVER_HOST"
#define HUE_USER "HUE_BRIDGE_USER"

class HeaderView : public View {
protected:
    TextLabelView txtView;
    mono::display::Color textColor, bgColor;

public:

    HeaderView(geo::Rect rct, String text);

    void repaint();

};

class BrightnessWell : public ResponderView {
protected:

    float stepSize;
    bool touchActive, showHighlight;
    int32_t currentTouchYVal, prevYVal;
    mbed::FunctionPointerArg1<void, uint8_t> brightnessHandler;
    
    virtual void TouchBegin(TouchEvent &event);
    virtual void TouchEnd(TouchEvent &event);
    virtual void TouchMove(TouchEvent &event);

public:

    mono::display::Color topColor, bottomColor;

    BrightnessWell(geo::Rect rct);

    template <typename Context>
    void setBrightnessSelectCallback(Context *cnxt, void(Context::*memPtr)(uint8_t))
    {
        brightnessHandler.attach<Context>(cnxt, memPtr);
    }

    void repaint();
};

class Toucher : public mono::TouchResponder
{
public:

    mbed::FunctionPointer touchHandler;

    void RespondTouchBegin(TouchEvent &event);
};

class AppController : public mono::IApplication {

    BackgroundView bgView;
    TextLabelView messageLbl;
    HeaderView hueHead;
    OnOffButtonView stateBtn;
    BrightnessWell bWell;
    ButtonView calBtn;
    TouchCalibrateView calView;

    //SDFileSystem fs;
    mbed::SPI spi;
    redpine::ModuleSPICommunication spiComm;

    HueLight *hue;
    bool allGood;

    static const int dimBrightness;
    Timer dimmer, sleeper;
    Toucher toucher;

public:

    AppController();

    void wifiReady();
    void toggleState();
    void clearMessage();
    void beginTouchCal();
    void touchCalEnded();

    void dim();
    void undim();

    void monoWakeFromReset();
    void monoWillGotoSleep();
    void monoWakeFromSleep();

};



#endif /* app_controller_h */
