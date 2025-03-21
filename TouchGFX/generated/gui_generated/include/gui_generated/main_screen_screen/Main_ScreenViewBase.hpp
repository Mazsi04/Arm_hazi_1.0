/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef MAIN_SCREENVIEWBASE_HPP
#define MAIN_SCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/main_screen_screen/Main_ScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

class Main_ScreenViewBase : public touchgfx::View<Main_ScreenPresenter>
{
public:
    Main_ScreenViewBase();
    virtual ~Main_ScreenViewBase();
    virtual void setupScreen();
    virtual void handleKeyEvent(uint8_t key);

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Image image1;
    touchgfx::Button Back_Buton;
    touchgfx::Button Settings_button;
    touchgfx::Button About_button;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<Main_ScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // MAIN_SCREENVIEWBASE_HPP
