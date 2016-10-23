#include <gui/common/BasePresenter.hpp>

BasePresenter::BasePresenter(BaseViewInterface& view) :
    viewInterface(view)
{
}

void BasePresenter::activate()
{
//    statusBarInformationUpdated();
}

void BasePresenter::deactivate()
{
}

void BasePresenter::gotoScreen(SubScreen subScreen)
{
    if (currentScreen == subScreen)
    {
        return;
    }

    model->setSelectedScreen(subScreen);

	switch(subScreen)
	{
	case IDLE:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoIdle();
		break;
	case NUMBER_OF_SUB_SCREENS:
	default:
		break;
	}
}

void BasePresenter::mcuLoadUpdated(uint8_t mcuLoad)
{
    viewInterface.updateProcessorLoad(mcuLoad);
}

