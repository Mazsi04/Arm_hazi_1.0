#include <gui/home_screen_screen/Home_ScreenView.hpp>

Home_ScreenView::Home_ScreenView()
{
}

void Home_ScreenView::setupScreen()
{
    Home_ScreenViewBase::setupScreen();
		digitalHours = digitalClock1.getCurrentHour();
	    digitalMinutes = digitalClock1.getCurrentMinute();
	    digitalSeconds = digitalClock1.getCurrentSecond();
}

void Home_ScreenView::tearDownScreen()
{
    Home_ScreenViewBase::tearDownScreen();
}

void Home_ScreenView::handleTickEvent()
{
    tickCounter++;

    if (tickCounter % 60 == 0)
    {
        if (++digitalSeconds >= 60)
        {
            digitalSeconds = 0;
            if (++digitalMinutes >= 60)
            {
                digitalMinutes = 0;
                if (++digitalHours >= 24)
                {
                    digitalHours = 0;
                }
            }
        }

        // Update the clock
        digitalClock1.setTime24Hour(digitalHours, digitalMinutes, digitalSeconds);
    }
}
