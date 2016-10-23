#ifndef IDLE_PRESENTER_HPP
#define IDLE_PRESENTER_HPP

#include <gui/common/BasePresenter.hpp>

class IdleView;

class IdlePresenter : public BasePresenter
{
public:
    IdlePresenter(IdleView& v);
    virtual void activate();
    virtual void deactivate();
    virtual ~IdlePresenter() {};
private:
    IdlePresenter();

    IdleView& view;
};

#endif
