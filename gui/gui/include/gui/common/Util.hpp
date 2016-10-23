#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <touchgfx/mixins/FadeAnimator.hpp>

using namespace touchgfx;

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



#endif
