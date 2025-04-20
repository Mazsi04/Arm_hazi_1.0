#include <gui/home_screen_screen/Home_ScreenView.hpp>
#include <stdio.h> // sprintf használatához

Home_ScreenView::Home_ScreenView()
{
}

void Home_ScreenView::setupScreen()
{
    Home_ScreenViewBase::setupScreen();
		digitalHours = digitalClock1.getCurrentHour();
	    digitalMinutes = digitalClock1.getCurrentMinute();
	    digitalSeconds = digitalClock1.getCurrentSecond();

	    currentWidget = WIDGET_WIFI;
	    // Frissítsük a megjelenítést
	    updateButtonHighlight();

	    //TempPresenter.updateText(100);
	    updateText(0);

	    updateTextHumidity(0);
}

void Home_ScreenView::tearDownScreen()
{
    Home_ScreenViewBase::tearDownScreen();
}

//extern "C" int tempValue;

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
    //ezt ki kell szedni ha touch gfx be mókolok
    updateText(tempValue);
    updateTextHumidity(humValue);
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
    // Frissítsük a gombok vizuális állapotát
    updateButtonHighlight();
}

void Home_ScreenView::updateButtonHighlight()
{
    // Az összes gombot visszaállítjuk az eredeti állapotba
	SELECT_DEVICE.setAlpha(255);
	ROOM_SELECT.setAlpha(255);
    WIFI.setAlpha(255);
    BLE.setAlpha(255);
    SETTINGS.setAlpha(255);
    ABOUT.setAlpha(255);

    // A kiválasztott gomb halványabb lesz
    switch (currentWidget)
    {
    case WIDGET_WIFI:
    	WIFI.setAlpha(150);
        break;
    case WIDGET_BLE:
        BLE.setAlpha(150);
        break;
    case WIDGET_ROOM_SELECT:
    	ROOM_SELECT.setAlpha(150); // Halványítjuk
        break;
    case WIDGET_SELECT_DEVICE:
    	SELECT_DEVICE.setAlpha(150); // Halványítjuk
        break;
    case WIDGET_SETTINGS:
    	SETTINGS.setAlpha(150);
        break;
    case WIDGET_ABOUT:
    	ABOUT.setAlpha(150);
        break;
    default:
        break;
    }

    // A módosítások érvénybe léptetése
    SELECT_DEVICE.invalidate();
    ROOM_SELECT.invalidate();
    WIFI.invalidate();
    BLE.invalidate();
    SETTINGS.invalidate();
    ABOUT.invalidate();
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


/*void Home_ScreenView::updateText(int newValue)
{
	Unicode::snprintf(myTextBuffer, sizeof(myTextBuffer) / sizeof(Unicode::UnicodeChar), "%d\u00B0C", newValue); // A számot szöveggé alakítjuk
    data_temp.setWildcard(myTextBuffer);  // Beállítjuk a szöveget
    data_temp.invalidate();               // Frissítjük a kijelzőt
}*/

void Home_ScreenView::updateText(int newValue)
{
    //int len = 0;
    // Ha negatív az érték, akkor külön formázd le a mínusz jellel
    if(newValue < 0)
    {
        /*len = */Unicode::snprintf(myTextBuffer, sizeof(myTextBuffer) / sizeof(Unicode::UnicodeChar), "-%d", -newValue);
    }
    else
    {
        /*len =*/ Unicode::snprintf(myTextBuffer, sizeof(myTextBuffer) / sizeof(Unicode::UnicodeChar), "%d", newValue);
    }

    // Ellenőrizzük, hogy van-e elegendő hely a fokjelnek és a 'C'-nek
    /*len = Unicode::strlen(myTextBuffer);  // Meghatározzuk a szöveg hosszát
    if(len < (int)((sizeof(myTextBuffer)/sizeof(Unicode::UnicodeChar)) - 2))
    {
        myTextBuffer[len]     = 0x00B0;  // Fokjel Unicode értéke
        myTextBuffer[len +1] = 'C';     // 'C' karakter
        myTextBuffer[len + 2] = '\0';       // null-terminátor
    }
    else
    {
        myTextBuffer[(sizeof(myTextBuffer)/sizeof(Unicode::UnicodeChar)) - 1] = 0;
    }*/

    data_temp.setWildcard(myTextBuffer);
    data_temp.invalidate();
}

void Home_ScreenView::updateTextHumidity(int newValue)
{

    if(newValue < 0)
    {
        Unicode::snprintf(myTextBuffer_hum, 10  /*sizeof(myTextBuffer_hum) / sizeof(Unicode::UnicodeChar)*/, "-%d\0", -newValue);
    }
    else
    {
       Unicode::snprintf(myTextBuffer_hum, 10 , "%d\0", newValue);
    }

    /*for (int i = 0; i < 20; ++i)
    {
        printf("%c\r\n", (char)myTextBuffer_hum[i]);
    }*/

    humidty_data.setWildcard(myTextBuffer_hum);
    humidty_data.invalidate();
}

