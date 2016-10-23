#ifndef IDLE_VIEW_HPP
#define IDLE_VIEW_HPP

#include <gui/common/BaseView.hpp>
#include <gui/idle_screen/IdlePresenter.hpp>

class IdleView : public BaseView<IdlePresenter>
{
public:
    IdleView();
    virtual ~IdleView();
    virtual void setupScreen();
    virtual void tearDownScreen();
};

#endif
