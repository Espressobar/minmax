#ifndef MODEL_HPP
#define MODEL_HPP
#include <touchgfx/hal/HAL.hpp>
#include <gui/common/defines.hpp>
#include <../../../common/control_messages.h>

class ModelListener;

class Model
{
public:
	

    Model();

    /**
     * Sets the modelListener to point to the currently active presenter. Called automatically
     * when switching screen.
     */
    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    /**
     * This function will be called automatically every frame. Can be used to e.g. sample hardware
     * peripherals or read events from the surrounding system and inject events to the GUI through
     * the ModelListener interface.
     */
    void tick();
		
		
    void resetTimers();
    void send();
	void sendMessage(void* message, uint16_t length);
	void handleIncomingMessages();
	void handleMessage(ControlMessageCode code, void* message);
    void setSelectedScreen(SubScreen subScreen);
		
protected:
    /**
     * Pointer to the currently active presenter.
     */
    ModelListener* modelListener;
public:

private:
    SubScreen selectedScreen;
};

#endif /* MODEL_HPP */
