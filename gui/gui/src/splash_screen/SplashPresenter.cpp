#include <gui/splash_screen/SplashPresenter.hpp>
#include <gui/splash_screen/SplashView.hpp>

SplashPresenter::SplashPresenter(SplashView& v)
    : view(v)
{
}

void SplashPresenter::activate()
{

}

void SplashPresenter::deactivate()
{

}

void SplashPresenter::initialize()
{
    view.test();
}
