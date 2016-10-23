#ifndef TEMPLATE_PRESENTER_HPP
#define TEMPLATE_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SplashView;

/**
 * The Presenter for a template screen. In the MVP paradigm, the presenter acts upon
 * events from the model and the view. By deriving from ModelListener, the presenter
 * implements the interface which the model uses to inform the current presenter of
 * events and model state changes. The presenter also contains a reference to the
 * associated view, and uses this to update the view accordingly.
 *
 */
class SplashPresenter : public Presenter, public ModelListener
{
public:
    SplashPresenter(SplashView& v);

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
    
    virtual ~SplashPresenter() {};
	virtual void initialize();
			
	void send()
	{
	    model->send();
	}

private:
    SplashPresenter();

    SplashView& view;
};

#endif // TEMPLATE_PRESENTER_HPP
