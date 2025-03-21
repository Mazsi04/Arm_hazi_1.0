/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/home_screen_screen/Home_ScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

Home_ScreenViewBase::Home_ScreenViewBase() :
    buttonCallback(this, &Home_ScreenViewBase::buttonCallbackHandler)
{
    __background.setPosition(0, 0, 320, 240);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    image1.setXY(0, 0);
    image1.setBitmap(touchgfx::Bitmap(BITMAP_HATER_ID));
    add(image1);

    Select_device.setXY(105, 110);
    Select_device.setBitmaps(touchgfx::Bitmap(BITMAP_DEVICE_ID), touchgfx::Bitmap(BITMAP_DEVICE_ID));
    add(Select_device);

    Wifi.setXY(228, 16);
    Wifi.setBitmaps(touchgfx::Bitmap(BITMAP_WIFI_ID), touchgfx::Bitmap(BITMAP_WIFI_ID));
    add(Wifi);

    BLE.setXY(228, 61);
    BLE.setBitmaps(touchgfx::Bitmap(BITMAP_BLUTOOTH_ID), touchgfx::Bitmap(BITMAP_BLUTOOTH_ID));
    add(BLE);

    button10.setXY(228, 110);
    button10.setBitmaps(touchgfx::Bitmap(BITMAP_ROOM_SELECT_ID), touchgfx::Bitmap(BITMAP_ROOM_SELECT_ID));
    add(button10);

    Temperature.setXY(11, 16);
    Temperature.setBitmap(touchgfx::Bitmap(BITMAP_HOMERSEKLET_ID));
    add(Temperature);

    image2.setXY(105, 16);
    image2.setBitmap(touchgfx::Bitmap(BITMAP_IDO_ID));
    add(image2);

    image3.setXY(11, 110);
    image3.setBitmap(touchgfx::Bitmap(BITMAP_PARATARTALOM_ID));
    add(image3);

    Settings.setXY(228, 185);
    Settings.setBitmaps(touchgfx::Bitmap(BITMAP_SETTINGS_ID), touchgfx::Bitmap(BITMAP_SETTINGS_ID));
    Settings.setAction(buttonCallback);
    add(Settings);

    About.setXY(274, 185);
    About.setBitmaps(touchgfx::Bitmap(BITMAP_ABOUT_ID), touchgfx::Bitmap(BITMAP_ABOUT_ID));
    About.setAction(buttonCallback);
    add(About);

    digitalClock1.setPosition(105, 34, 110, 45);
    digitalClock1.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    digitalClock1.setTypedText(touchgfx::TypedText(T___SINGLEUSE_7JQQ));
    digitalClock1.displayLeadingZeroForHourIndicator(true);
    digitalClock1.setDisplayMode(touchgfx::DigitalClock::DISPLAY_24_HOUR_NO_SECONDS);
    digitalClock1.setTime24Hour(10, 10, 0);
    add(digitalClock1);
}

Home_ScreenViewBase::~Home_ScreenViewBase()
{

}

void Home_ScreenViewBase::setupScreen()
{

}

void Home_ScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &Settings)
    {
        //settings_clic
        //When Settings clicked change screen to Settings_Screen
        //Go to Settings_Screen with screen transition towards South
        application().gotoSettings_ScreenScreenSlideTransitionSouth();
    }
    if (&src == &About)
    {
        //about
        //When About clicked change screen to About_Screen
        //Go to About_Screen with no screen transition
        application().gotoAbout_ScreenScreenNoTransition();
    }
}

void Home_ScreenViewBase::handleKeyEvent(uint8_t key)
{
    if(0 == key)
    {
        //Interaction1
        //When hardware button 0 clicked change screen to Main_Screen
        //Go to Main_Screen with no screen transition
        application().gotoMain_ScreenScreenNoTransition();
    
    }

    if(48 == key)
    {
        //Button_OK
        //When hardware button 48 clicked call virtual function
        //Call WidgetScrolling
        WidgetScrolling();
    
    }

    if(49 == key)
    {
        //Button_Right
        //When hardware button 49 clicked call virtual function
        //Call WidgetScrolling
        WidgetScrolling();
    
    }

    if(50 == key)
    {
        //Button_Left
        //When hardware button 50 clicked call virtual function
        //Call WidgetScrolling
        WidgetScrolling();
    
    }
}
