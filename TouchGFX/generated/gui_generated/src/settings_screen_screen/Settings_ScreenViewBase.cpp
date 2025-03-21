/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/settings_screen_screen/Settings_ScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>

Settings_ScreenViewBase::Settings_ScreenViewBase() :
    buttonCallback(this, &Settings_ScreenViewBase::buttonCallbackHandler)
{
    __background.setPosition(0, 0, 320, 240);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    image1.setXY(0, 0);
    image1.setBitmap(touchgfx::Bitmap(BITMAP_ACTIVAIHOME_320X240_ID));
    image1.setAlpha(200);
    add(image1);

    back_button.setXY(0, 190);
    back_button.setBitmaps(touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_NAVIGATION_ARROW_BACK_50_50_000000_SVG_ID), touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_NAVIGATION_ARROW_BACK_50_50_000000_SVG_ID));
    back_button.setAction(buttonCallback);
    add(back_button);

    button1.setXY(90, 45);
    button1.setBitmaps(touchgfx::Bitmap(BITMAP_WIFI_SETTINGS_ON_ID), touchgfx::Bitmap(BITMAP_WIFI_SETTINGS_OFF_ID));
    add(button1);
}

Settings_ScreenViewBase::~Settings_ScreenViewBase()
{

}

void Settings_ScreenViewBase::setupScreen()
{

}

void Settings_ScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &back_button)
    {
        //Back_button
        //When back_button clicked change screen to Main_Screen
        //Go to Main_Screen with screen transition towards West
        application().gotoMain_ScreenScreenSlideTransitionWest();
    }
}
