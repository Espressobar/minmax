#ifndef TEMPLATE_VIEW_HPP
#define TEMPLATE_VIEW_HPP

#include <mvp/View.hpp>
#include <gui/splash_screen/SplashPresenter.hpp>
#include <touchgfx/mixins/Draggable.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <gui/common/Util.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

using namespace touchgfx;

/**
 * The View for a template screen. In the MVP paradigm, the view is a
 * passive interface that only displays the user interface and routes
 * any events from the user interface to the presenter. Normally it is the
 * responsibility of the presenter to act upon those events.
 *
 * By deriving from View, this class has a presenter pointer, which is configured
 * automatically.
 */
class SplashView : public View<SplashPresenter>
{
public:
    /**
     * The view constructor. Note that the constructor runs before the new screen is
     * completely initialized, so do not place code here that causes anything to be drawn.
     * Use the setupScreen function instead.
     */
    SplashView();

    virtual ~SplashView() { }

    /**
     * This function is called automatically when the view is activated. This function
     * should add widgets to the root container, configure widget sizes and event
     * callbacks, et cetera.
     */
    virtual void setupScreen();

    /**
     * This function is called automatically when transitioning to a different screen
     * than this one. Can optionally be used to clean up.
     */
    virtual void tearDownScreen();

	virtual void handleClickEvent(const ClickEvent& evt)
	{
        backgroundImage.startFadeAnimation(0, 40, EasingEquations::linearEaseIn);
        start.setVisible(false);
        logo.setMoveAnimationDelay(40);
        logo.startMoveAnimation(logo.getX(), touchgfx::HAL::DISPLAY_HEIGHT / 2 - logo.getHeight() / 2, 80, EasingEquations::linearEaseNone, EasingEquations::cubicEaseOut);

        //Move this to a button in another view.
/*        if(evt.getType() == ClickEvent::PRESSED)
        {
	        presenter->send();
		    View::handleClickEvent(evt);
        }
*/
	}
		
    void moveCursor();
    void test() 
    {
        marker.moveTo(200, 200);
    }
private:
    Callback<SplashView, const MoveAnimator<Image>& > moveAnimationEndedCallback;
    void moveAnimationEndedHandler(const MoveAnimator<Image>& element);

    FadeAnimator<Image> backgroundImage;
    Box backgroundBox;

    MoveAnimator<Image> logo;
    PulseAnimator<TextArea> start;

    //Image backdrop;

    //Marker - Should just be one single container that reports back a few things
    //1) Released event (update position)
    //2) internal: When mg or ca has been pressed - update the visuals.

    Draggable<Box> marker;
};

#endif // TEMPLATE_VIEW_HPP
