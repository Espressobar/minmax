#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <touchgfx/mixins/FadeAnimator.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/Color.hpp>

using namespace touchgfx;

//todo: move this to util
class DisabledButton : public ButtonWithLabel
{
public:
    DisabledButton()
    {}

    virtual ~DisabledButton() {}

    void setBitmaps(const Bitmap& bmpReleased, const Bitmap& bmpPressed, const Bitmap& bmpDisabled)
    {
        pressed = bmpPressed;
        released = bmpReleased;
        disabled = bmpDisabled;

        enable();
    }

    void disable()
    {
        Button::setBitmaps(disabled, disabled);
        ButtonWithLabel::setLabelColor(Color::getColorFrom24BitRGB(0x7a, 0x7a, 0x7a));
        finalize(false);
    }

    void enable()
    {
        Button::setBitmaps(released, pressed);
        ButtonWithLabel::setLabelColor(Color::getColorFrom24BitRGB(0xff, 0xff, 0xff));
        finalize(true);
    }

private:
    Bitmap disabled;
    Bitmap released;
    Bitmap pressed;

    void finalize(bool touchable)
    {
        Button::setTouchable(touchable);
        Button::invalidate();
    }
};


template <class T>
class PulseAnimator : public FadeAnimator<T>
{
public:
    PulseAnimator() :
        FadeAnimator<T>(),
        pulseUp(true),
        startAlpha(0),
        endAlpha(0),
        fadeAnimationEndedCallback(this, &PulseAnimator::fadeAnimationEndedHandler)
    {
    }

    virtual ~PulseAnimator() {}

    void stopAnimation()
    {
        FadeAnimator<T>::cancelFadeAnimation();
    }

    void setup(uint8_t _startAlpha, uint8_t _endAlpha, uint16_t _duration, EasingEquation _alphaProgressionEquation = &EasingEquations::linearEaseNone)
    {
        startAlpha = _startAlpha;
        endAlpha = _endAlpha;
        duration = _duration;
        alphaProgressionEquation = _alphaProgressionEquation;

        FadeAnimator<T>::setFadeAnimationEndedAction(fadeAnimationEndedCallback);
    }

    bool isAnimating()
    {
        return FadeAnimator<T>::isRunning();
    }

    void startAnimation()
    {
        FadeAnimator<T>::startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    }

    void fadeAnimationEndedHandler(const FadeAnimator<T>& element)
    {
        //reverse pulse
        pulseUp = !pulseUp;
        uint8_t temp = startAlpha;
        startAlpha = endAlpha;
        endAlpha = temp;

        //start animation again
        startAnimation();
    }

    bool getPulseDirection()
    {
        return pulseUp;
    }

private:

    bool pulseUp;
    uint8_t startAlpha;
    uint8_t endAlpha;
    uint16_t duration;
    EasingEquation alphaProgressionEquation;
    Callback<PulseAnimator, const FadeAnimator<T>& > fadeAnimationEndedCallback;
};


/************************************************************************/
// State IF
/************************************************************************/
class Context;
class State
{
public:
    State() {}
    virtual ~State() {}

    //Manual signals
    virtual void next(Context* context) {}
    virtual void previous(Context* context) {}

    //external signals - I.e. we'd like to receive a signal from the outside and just throw it at the gui - but without the context knowing about the signal? getCurrentState().recordingDone() ?
    virtual void recordingDone() {}


    //gui related
    virtual void show() = 0;
    virtual void hide() = 0;

};

/************************************************************************/
// Next/Prev State Machine Context 
/************************************************************************/
class Context /* Context */
{
public:
    //todo: something that is able to set a state in the middle of everything
    void setState(State* state)
    {
        currentState = state;
        currentState->show();
    }

    //Are these really necessary? Maybe we'd like to just call this on the currentstate.
    //Maybe by calling next on the state, it updates the current state in the context automatically, and they call their own hide().
    void goNext()
    {
        currentState->hide();
        currentState->next(this);
    }

    void goPrev()
    {
        currentState->hide();
        currentState->previous(this);
    }

    State* getCurrent()
    {
        return currentState;
    }

    State* currentState;
};

/************************************************************************/
// View state  (todo: define concrete states instead of just using them raw?)                                                                   
/************************************************************************/
template<int size>
class ViewState : public State
{
public:

    ViewState(Container* _parent)
    {
        parent = _parent;
    }

    void add(Drawable &element, bool addToRoot = true)
    {
        element.setVisible(false); //default invisible
        elements.add(&element);
        if (addToRoot)
        {
            parent->add(element);
        }
    }

    virtual void show()
    {
        setVisibility(true);
    }

    virtual void hide()
    {
        setVisibility(false);
    }

    /************************************************************************/
    //  interface methods
    /************************************************************************/
    virtual void next(Context* context)
    {
        context->setState(nextState);
    }

    virtual void previous(Context* context)
    {
        context->setState(prevState);
    }

    void setVisibility(bool show)
    {
        for (int i = 0; i < elements.size(); i++)
        {
            elements[i]->setVisible(show);
            elements[i]->invalidate();
        }
    }

    //This is a simplified state table where the state always knows what's next or behind it.
    ViewState* nextState;
    ViewState* prevState;

private:

    Container* parent;
    touchgfx::Vector<Drawable*, size> elements;
};

#endif
