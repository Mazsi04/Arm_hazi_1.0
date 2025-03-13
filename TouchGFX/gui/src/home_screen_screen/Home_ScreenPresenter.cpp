#include <gui/home_screen_screen/Home_ScreenView.hpp>
#include <gui/home_screen_screen/Home_ScreenPresenter.hpp>

Home_ScreenPresenter::Home_ScreenPresenter(Home_ScreenView& v)
    : view(v)
{

}

void Home_ScreenPresenter::activate()
{

}

void Home_ScreenPresenter::deactivate()
{

}

void Home_ScreenPresenter::gotoNextScreen(/*WidgetEnums::*/WidgetSelection selectedWidget)
{
    switch (selectedWidget)
    {
    case /*WidgetEnums::*/WIDGET_SELECT_DEVICE:
        //application().gotoDevice_ScreenScreenNoTransition();
        break;

    case /*WidgetEnums::*/WIDGET_WIFI:
        //application().gotoWiFi_ScreenScreenNoTransition();
        break;

    case /*WidgetEnums::*/WIDGET_BLE:
        //application().gotoBLE_ScreenScreenNoTransition();
        break;

    case /*WidgetEnums::*/WIDGET_ROOM_SELECT:
        //application().gotoRoom_SelectScreenNoTransition();
        break;

    case /*WidgetEnums::*/WIDGET_SETTINGS:
        //application().gotoHome_ScreenScreenNoTransition();
    	application().gotoSettings_ScreenScreenSlideTransitionSouth();
        break;

    case /*WidgetEnums::*/WIDGET_ABOUT:
    	//application().gotoHome_ScreenScreenNoTransition();
        application().gotoAbout_ScreenScreenNoTransition();
        break;

    default:
        break;
    }
}
