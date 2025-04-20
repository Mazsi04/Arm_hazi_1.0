#ifndef ABOUT_SCREENPRESENTER_HPP
#define ABOUT_SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class About_ScreenView;

class About_ScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    About_ScreenPresenter(About_ScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~About_ScreenPresenter() {}

private:
    About_ScreenPresenter();

    About_ScreenView& view;
};

#endif // ABOUT_SCREENPRESENTER_HPP
