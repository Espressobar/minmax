#include <new>
#include <gui/common/FrontendApplication.hpp>
#include <gui/idle_screen/IdleView.hpp>
#include <gui/idle_screen/IdlePresenter.hpp>
#include <mvp/View.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <gui/splash_screen/SplashView.hpp>
#include <gui/splash_screen/SplashPresenter.hpp>
#include <gui/common/FrontendHeap.hpp>

using namespace touchgfx;


FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : touchgfx::MVPApplication(),
      transitionCallback(),
      frontendHeap(heap),
      model(m)
{
}

void FrontendApplication::gotoTemplateScreen()
{
    transitionCallback = touchgfx::Callback< FrontendApplication >(this, &FrontendApplication::gotoTemplateScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoTemplateScreenImpl()
{
    makeTransition< SplashView, SplashPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoIdle()
{ 
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoIdleImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoIdleImpl()
{
    makeTransition< IdleView, IdlePresenter, NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
