/*
 * MyButtonController.cpp
 *
 *  Created on: Mar 11, 2025
 *      Author: mazsi
 */

#include <MyButtonController.hpp>
#include <main.h>
#include <touchgfx/hal/HAL.hpp>

extern "C" {
	extern uint8_t User_ButtonState ;
}

void MyButtonController::init()
{

}

bool MyButtonController::sample(uint8_t& key)
{


	if(User_ButtonState == 0x01)
    {

    		User_ButtonState = 0x00;
	        key = 48;
	        return true;
    }
	if(User_ButtonState == 0x02){

		User_ButtonState = 0x00;
        key = 49;
        return true;
	}
	if(User_ButtonState == 0x03){

		User_ButtonState = 0x00;
        key = 50;
        return true;
	}

    return false;
}

