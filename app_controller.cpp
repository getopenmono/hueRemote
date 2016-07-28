
#include "app_controller.h"
#include <io/File.h>

using namespace mono::geo;

AppController::AppController() :
    messageLbl(Rect(0,200,176,20),""),
    hueLbl(Rect(0,50,176,20),"hue"),
    stateBtn(Rect(20,130,136,40),"Toggle")
#ifndef EMUNO
//    ,fs(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_CLK, SD_SPI_CS, "sd")
    ,spi(RP_SPI_MOSI, RP_SPI_MISO, RP_SPI_CLK),
    spiComm(spi, RP_SPI_CS, RP_nRESET, RP_INTERRUPT)
#endif
{
    allGood = false;
    messageLbl.setAlignment(TextLabelView::ALIGN_CENTER);
    stateBtn.setClickCallback<AppController>(this, &AppController::toggleState);
    hueLbl.setAlignment(TextLabelView::ALIGN_CENTER);
    hueLbl.setText(display::BelizeHoleColor);
    hueLbl.show();
}

void AppController::monoWakeFromReset()
{
    String ssid = io::File::readFirstLine("/sd/ssid.txt");
    String pass = io::File::readFirstLine("/sd/pass.txt");
    String hueHost = io::File::readFirstLine("/sd/hue.txt");
    String hueUser = io::File::readFirstLine("/sd/user.txt");

    messageLbl.show();

    if (ssid.Length() == 0)
    {
        printf("No ssid.txt file!\r\n");
        messageLbl.setText("No ssid.txt");
        return;
    }
    if (pass.Length() == 0)
    {
        printf("No pass.txt file!\r\n");
        messageLbl.setText("No pass.txt");
        return;
    }
    if (hueHost.Length() == 0)
    {
        printf("No hue.txt file!\r\n");
        messageLbl.setText("No hue.txt");
        return;
    }
    if (hueUser.Length() == 0)
    {
        printf("No user.txt file!\r\n");
        messageLbl.setText("No user.txt");
        return;
    }

    printf("Mono Hue App\r\n");
    printf("Conf: Hue Forwarder Host: %s\r\nHue username: %s\r\n", hueHost(), hueUser());

#ifndef EMUNO
    allGood = redpine::Module::initialize(&spiComm);
    allGood &= redpine::Module::setupWifiOnly(ssid, pass);
#else
    allGood = true;
#endif

    if (allGood)
    {
        messageLbl.setText("init wifi...");
#ifndef EMUNO
        redpine::Module::setNetworkReadyCallback<AppController>(this, &AppController::wifiReady);
#else
        Timer::callOnce<AppController>(1000, this, &AppController::wifiReady);
#endif
        printf("assign with host: %s, user: %s\r\n", hueHost(), hueUser());
        hue = new HueLight(hueHost, hueUser, 3);
    }
    else
        messageLbl.setText("Wifi error!");
}

void AppController::wifiReady()
{
    messageLbl.setText("Connected");
    stateBtn.show();
    Timer::callOnce<AppController>(1000, this, &AppController::clearMessage);
}

void AppController::toggleState()
{
    static bool s = false;
    printf("setting state to: %i\r\n",!s);
    hue->setState(!s);
    s = !s;
}

void AppController::clearMessage()
{
    messageLbl.setText("");
}

void AppController::monoWillGotoSleep()
{
}

void AppController::monoWakeFromSleep()
{
    IApplicationContext::SoftwareResetToApplication();
}
