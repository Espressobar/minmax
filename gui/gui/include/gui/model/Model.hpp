#ifndef MODEL_HPP
#define MODEL_HPP
#include <touchgfx/hal/HAL.hpp>

class ModelListener;

/**
 * The Model class defines the data model in the model-view-presenter paradigm.
 * The Model is a singular object used across all presenters. The currently active
 * presenter will have a pointer to the Model through deriving from ModelListener.
 *
 * The Model will typically contain UI state information that must be kept alive
 * through screen transitions. It also usually provides the interface to the rest
 * of the system (the backend). As such, the Model can receive events and data from
 * the backend and inform the current presenter of such events through the modelListener
 * pointer, which is automatically configured to point to the current presenter.
 * Conversely, the current presenter can trigger events in the backend through the Model.
 */
class Model
{
public:
	
	enum ControlMessageCode
	{
		//STM32 TO ARDUINO
		EXECUTE_RECIPE 	        = 65, //Should be followed by doses
        DOSE_MINERAL            = 66,
		UPDATE_CONFIGURATION    = 67,


		//ARDUINO TO STM32
		INITIALIZE 			= 97, //Configuration, salt - levels, reservoir level.
        RECIPE_STARTED      = 98,
        DOSING_STARTED      = 99,
        DOSING_FINISHED     = 100,
        RECIPLE_FINISHED    = 101
        //LEVEL
	};
			
    enum Solution
    {
        MGSO4 = 0, //GH 100 solution
        MGCL2,     //GH 100 solution
        CACL2,     //GH 100 solution

        //CA + HCO3-
        CACO3,   //GH 100 solution. Molar mass 100 mg/mmol

        //HCO3- + (NA)
        NAHCO3   //KH 100 solution
    };

    /* uint8 instead? */
  struct ControlMessageInitialize
	{
		uint16_t messageCode;
	};

  struct ControlMessageExecuteRecipe
  {
      uint16_t messageCode;
   };

  struct ControlMessageDose
  {
      uint16_t messageCode; //enum ControlMessageCode
      uint16_t mineral;     //enum Mineral
      uint16_t ml;          //amount in ml
  };

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
		
		
	  void send();
		void sendMessage(void* message, uint16_t length);
		void handleIncomingMessages();
		void handleMessage(ControlMessageCode code, void* message);
		

		

		
protected:
    /**
     * Pointer to the currently active presenter.
     */
    ModelListener* modelListener;
};

#endif /* MODEL_HPP */
