#ifndef ABOUT_SCREENVIEW_HPP
#define ABOUT_SCREENVIEW_HPP

#include <gui_generated/about_screen_screen/About_ScreenViewBase.hpp>
#include <gui/about_screen_screen/About_ScreenPresenter.hpp>

class About_ScreenView : public About_ScreenViewBase
{
public:
    About_ScreenView();
    virtual ~About_ScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // ABOUT_SCREENVIEW_HPP
