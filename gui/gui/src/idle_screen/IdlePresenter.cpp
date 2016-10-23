#include <gui//idle_screen/IdlePresenter.hpp>
#include <gui//idle_screen/IdleView.hpp>
#include <gui/common/FrontendApplication.hpp>

IdlePresenter::IdlePresenter(IdleView& v)
    : BasePresenter(v), view(v)
{
}

void IdlePresenter::activate()
{
  
}

void IdlePresenter::deactivate()
{

}
