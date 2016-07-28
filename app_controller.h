
#ifndef app_controller_h
#define app_controller_h

#include <mono.h>
#include <wireless/redpine_module.h>
#include <wireless/module_communication.h>
#ifndef EMUNO
//#include <SDFileSystem.h>
#endif
#include "hue.h"

using namespace mono;
using namespace mono::ui;

class AppController : public mono::IApplication {

    TextLabelView messageLbl;
    TextLabelView hueLbl;
    ButtonView stateBtn;
#ifndef EMUNO
    //SDFileSystem fs;
    mbed::SPI spi;
    redpine::ModuleSPICommunication spiComm;
#endif

    HueLight *hue;
    bool allGood;

public:

    AppController();

    void wifiReady();
    void toggleState();
    void clearMessage();

    void monoWakeFromReset();
    void monoWillGotoSleep();
    void monoWakeFromSleep();

};

#endif /* app_controller_h */
