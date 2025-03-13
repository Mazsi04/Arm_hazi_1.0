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


void Home_ScreenView::WidgetScrolling()
{
    switch(currentButton)
    {
    case ButtonOK:
        // OK gomb: widget aktiválása, például képernyőváltás
        // Feltételezzük, hogy a presenter-ben van egy gotoNextScreen() metódus
    	presenter->gotoNextScreen(currentWidget);
        break;

    case ButtonRight:
        // Jobbra gomb: növeljük az indexet, és frissítjük a fókuszt
        if (currentWidget < /*WidgetEnums::*/WIDGET_COUNT - 1) {
            currentWidget = static_cast</*WidgetEnums::*/WidgetSelection>(currentWidget + 1);
        }
        break;
        // Itt hívd meg azt a függvényt, ami a fókuszváltást végzi, pl.
        // updateWidgetFocus();


    case ButtonLeft:
        // Balra gomb: csökkentjük az indexet (ellenőrizve, hogy ne legyen negatív)
        if (currentWidget > 0) {
            currentWidget = static_cast</*WidgetEnums::*/WidgetSelection>(currentWidget - 1);
        }
        break;
        // Frissítsd a fókuszt ennek megfelelően
        // updateWidgetFocus();

    default:
        break;
    }
}


void Home_ScreenView::handleKeyEvent(uint8_t key)
{
    // A generált kódban a key értékek:
    // 48 -> OK, 49 -> Jobbra, 50 -> Balra
    switch(key)
    {
    case 48:
        currentButton = ButtonOK;
        break;
    case 49:
        currentButton = ButtonRight;
        break;
    case 50:
        currentButton = ButtonLeft;
        break;
    default:
        break;
    }
    // Hívjuk meg a generált implementációt, ami majd meghívja a WidgetScrolling()-et
    Home_ScreenViewBase::handleKeyEvent(key);
}
