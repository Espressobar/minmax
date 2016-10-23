#ifndef __BASE_PRESENTER_HPP__
#define __BASE_PRESENTER_HPP__

#include <mvp/Presenter.hpp>
#include <gui/common/BaseViewInterface.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <gui/model/ModelListener.hpp>
#include <gui/common/defines.hpp>

using namespace touchgfx;

class BasePresenter : public Presenter, public ModelListener
{
public:
    BasePresenter(BaseViewInterface& view);

    virtual void activate();
    virtual void deactivate();

    void gotoScreen(SubScreen subScreen);

    // MCU-LOAD
    virtual void mcuLoadUpdated(uint8_t mcuLoad);

protected:
    BaseViewInterface &viewInterface;

    SubScreen currentScreen;

};

#endif /*__BASE_PRESENTER_HPP__*/
