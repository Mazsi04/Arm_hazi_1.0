/*
 * MyButtonController.hpp
 *
 *  Created on: Mar 11, 2025
 *      Author: mazsi
 */

#ifndef APPLICATION_USER_MYBUTTONCONTROLLER_HPP_
#define APPLICATION_USER_MYBUTTONCONTROLLER_HPP_


#include <platform/driver/button/ButtonController.hpp>

class MyButtonController : public touchgfx::ButtonController
{
    virtual void init();
    virtual bool sample(uint8_t& key);

private:
    uint8_t previousState;

};


#endif /* APPLICATION_USER_MYBUTTONCONTROLLER_HPP_ */
