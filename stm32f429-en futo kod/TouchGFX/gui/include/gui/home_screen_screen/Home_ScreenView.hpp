#ifndef HOME_SCREENVIEW_HPP
#define HOME_SCREENVIEW_HPP


#include <gui_generated/home_screen_screen/Home_ScreenViewBase.hpp>
#include <gui/home_screen_screen/Home_ScreenPresenter.hpp>
#include "WidgetSelection.h"


//ezt ki kell szedni ha touch gfx be mókolok
extern "C" int tempValue;
extern "C" int humValue;

typedef enum{
	ButtonOK,
	ButtonRight,
	ButtonLeft
} MyAplicationButton;

class Home_ScreenView : public Home_ScreenViewBase
{
public:
    Home_ScreenView();
    virtual ~Home_ScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

    virtual void WidgetScrolling();


    virtual void handleKeyEvent(uint8_t key);

    MyAplicationButton currentButton;

    WidgetSelection currentWidget;

    Unicode::UnicodeChar myTextBuffer[10]; // Buffer a szám tárolására
    void updateText(int newValue);


    Unicode::UnicodeChar myTextBuffer_hum[20]; // Buffer a szám tárolására
    void updateTextHumidity(int newValue);

    //virtual void TempUpdate();

protected:
    	int tickCounter;
        int digitalHours;
        int digitalMinutes;
        int digitalSeconds;


        void updateButtonHighlight();


};

#endif // HOME_SCREENVIEW_HPP
