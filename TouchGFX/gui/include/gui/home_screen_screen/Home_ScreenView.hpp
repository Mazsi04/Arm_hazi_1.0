#ifndef HOME_SCREENVIEW_HPP
#define HOME_SCREENVIEW_HPP

#include <gui_generated/home_screen_screen/Home_ScreenViewBase.hpp>
#include <gui/home_screen_screen/Home_ScreenPresenter.hpp>

class Home_ScreenView : public Home_ScreenViewBase
{
public:
    Home_ScreenView();
    virtual ~Home_ScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
protected:
    	int tickCounter;
        int digitalHours;
        int digitalMinutes;
        int digitalSeconds;
};

#endif // HOME_SCREENVIEW_HPP
