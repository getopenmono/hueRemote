
#include "app_controller.h"
#include <io/File.h>

using namespace mono::geo;

HeaderView::HeaderView(geo::Rect rct, String text) :
View(rct),
txtView(rct, text)
{
    bgColor = display::BlackColor;
    textColor = display::EmeraldColor;

    uint32_t yOff = rct.Y() + (rct.Height()/2 - txtView.TextPixelHeight()/2 + 8);
    txtView.setPosition(geo::Point(rct.X(),yOff));
    txtView.setAlignment(TextLabelView::ALIGN_CENTER);
    txtView.setBackground(bgColor);
    txtView.setText(textColor);
}

void HeaderView::repaint()
{
    painter.setBackgroundColor(bgColor);
    painter.drawFillRect(viewRect,true);
    txtView.repaint();
}

AppController::AppController() :
    messageLbl(Rect(0,200,176,20),""),
    hueHead(Rect(0,0,176,30),"hue"),
    stateBtn(Rect(60,155,96,40)),
    bWell(Rect(5,50,35,145)),
    calBtn(Rect(150,5,21,21),"")
//    ,fs(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_CLK, SD_SPI_CS, "sd")
    ,spi(RP_SPI_MOSI, RP_SPI_MISO, RP_SPI_CLK),
    spiComm(spi, RP_SPI_CS, RP_nRESET, RP_INTERRUPT)
{
    allGood = false;

    View::StandardBorderColor = display::MidnightBlueColor;
    View::StandardTextColor = display::WetAsphaltColor;
    View::StandardBackgroundColor = display::CloudsColor;

    messageLbl.setAlignment(TextLabelView::ALIGN_CENTER);
    messageLbl.setText(View::StandardTextColor);
    messageLbl.setBackground(View::StandardBackgroundColor);

    stateBtn.setBackground(View::StandardBackgroundColor);
    stateBtn.setBorder(View::StandardBorderColor);
    stateBtn.setClickCallback<AppController>(this, &AppController::toggleState);

    bgView.setBackgroundColor(View::StandardBackgroundColor);

    bgView.show();
    hueHead.show();
    calBtn.setBackground(display::BlackColor);
    calBtn.setBorder(display::BlackColor);
    calBtn.setClickCallback<AppController>(this, &AppController::beginTouchCal);
    calBtn.show();

    calView.setCalibrationDoneCallback<AppController>(this, &AppController::touchCalEnded);
}

void AppController::monoWakeFromReset()
{
//    String ssid = io::File::readFirstLine("/sd/ssid.txt");
//    String pass = io::File::readFirstLine("/sd/pass.txt");
//    String hueHost = io::File::readFirstLine("/sd/hue.txt");
//    String hueUser = io::File::readFirstLine("/sd/user.txt");
    String ssid(WIFI_SSID);
    String pass(WIFI_PASSPHRASE);
    String hueHost(HUE_HOST);
    String hueUser(HUE_USER);

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

    allGood = redpine::Module::initialize(&spiComm);
    allGood &= redpine::Module::setupWifiOnly(ssid, pass);

    if (allGood)
    {
        messageLbl.setText("init wifi...");

        redpine::Module::setNetworkReadyCallback<AppController>(this, &AppController::wifiReady);

        printf("assign with host: %s, user: %s\r\n", hueHost(), hueUser());
        hue = new HueLight(hueHost, hueUser, 3);
        bWell.setBrightnessSelectCallback<HueLight>(hue, &HueLight::setBrightness);
    }
    else
        messageLbl.setText("Wifi error!");
}

void AppController::wifiReady()
{
    messageLbl.setText("Connected");
    stateBtn.show();
    bWell.show();
    Timer::callOnce<AppController>(2000, this, &AppController::clearMessage);
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

void AppController::beginTouchCal()
{
    bgView.hide();
    calBtn.hide();
    hueHead.hide();
    messageLbl.hide();
    stateBtn.hide();
    bWell.hide();

    calView.StartNewCalibration();
    calView.show();
}

void AppController::touchCalEnded()
{
    bgView.show();
    hueHead.show();
    calBtn.show();
    messageLbl.show();

    if (redpine::Module::IsNetworkReady())
    {
        stateBtn.show();
        bWell.show();
    }
}

void AppController::monoWillGotoSleep()
{
}
void AppController::monoWakeFromSleep()
{
    IApplicationContext::SoftwareResetToApplication();
}


// MARK: Brightness Well

BrightnessWell::BrightnessWell(geo::Rect rct) : ResponderView(rct)
{
    touchActive = showHighlight = false;
    currentTouchYVal = prevYVal = 0;
    topColor = display::Color(253, 255, 209);
    bottomColor = display::Color(100, 100, 100);

    stepSize = 255.0/rct.Height();
}

// MARK: BrightnessWell Touch

void BrightnessWell::TouchBegin(TouchEvent &event)
{
    if (!event.IsScreenCoords)
        ToScreenCoords(&event);
    prevYVal = currentTouchYVal;
    currentTouchYVal = event.Position.Y();
    touchActive = true;
    showHighlight = true;
    scheduleRepaint();
}
void BrightnessWell::TouchEnd(TouchEvent &event)
{
    touchActive = false;
    showHighlight = false;
    scheduleRepaint();

    if (viewRect.contains(event.Position))
    {
        if (currentTouchYVal < viewRect.Y())
            currentTouchYVal = viewRect.Y();
        else if(currentTouchYVal > viewRect.Y2())
            currentTouchYVal = viewRect.Y2();

        float blend = stepSize*(currentTouchYVal-viewRect.Y());
        brightnessHandler.call(255-blend);
    }
    else
    {
        currentTouchYVal = prevYVal;
    }
}
void BrightnessWell::TouchMove(TouchEvent &event)
{
    if (!event.IsScreenCoords)
        ToScreenCoords(&event);

    if (viewRect.contains(event.Position))
    {
        touchActive = true;
        currentTouchYVal = event.Position.Y();
        //if (abs(currentTouchYVal - event.Position.Y()) > 3)

    }
    else
    {
        touchActive = false;
    }

    //scheduleRepaint();
}

void BrightnessWell::repaint()
{

    if (currentTouchYVal < viewRect.Y())
        currentTouchYVal = viewRect.Y();
    else if(currentTouchYVal > viewRect.Y2())
        currentTouchYVal = viewRect.Y2();

    float currentBlend = stepSize*(currentTouchYVal-viewRect.Y());

    if (false && touchActive)
    {
        painter.setBackgroundColor(topColor.alphaBlend(255-currentBlend, bottomColor));
        painter.drawFillRect(viewRect, true);
    }
    else
    {
        geo::Point left(viewRect.UpperLeft());
        //left.appendX(1);
        geo::Point right(viewRect.UpperRight());
        //right.appendX(-1);

        float blend = 0;
        for (int l=viewRect.Y(); l<viewRect.Y2(); l++) {
            left.setY(l); right.setY(l);

            painter.setBackgroundColor(topColor.alphaBlend(255-blend, bottomColor));
            painter.drawLine(left, right, true);
            
            blend += stepSize;
        }
    }

    geo::Rect slider(viewRect.X()+1, currentTouchYVal-4, viewRect.Width()-2, 8);
    slider = slider.crop(viewRect);
    painter.setForegroundColor(topColor.alphaBlend(255-currentBlend, bottomColor));
    painter.drawFillRect(slider);
    painter.setForegroundColor(painter.ForegroundColor().alphaBlend(176, StandardBorderColor));
    painter.drawRect(slider);

    painter.setForegroundColor(showHighlight ? StandardHighlightColor : StandardBorderColor);
    painter.drawRect(viewRect);
}


