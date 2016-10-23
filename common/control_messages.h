/*
   Shared between Arduino and STM32
*/
#ifndef __CONTROLMESSAGES_H__
#define __CONTROLMESSAGES_H__

#include <touchgfx/hal/Types.hpp>

enum ControlMessageCode
{
    //STM32 TO ARDUINO

    //Start messages
    EXECUTE_RECIPE          = 65, //Should be followed by doses
    UPDATE_CONFIGURATION    = 66,

    //payload messages
    DOSE            = 80,

    //ARDUINO TO STM32
    INITIALIZE = 97, //Configuration, salt - levels, reservoir level.
    RECIPE_STARTED = 98,
    DOSING_STARTED = 99,
    DOSING_FINISHED = 100,
    RECIPLE_FINISHED = 101,
    //LEVEL

    END_OF_MSG = 127 /* For messages with payload */
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
struct ControlMessage
{
    uint16_t messageCode;
};

struct ControlMessageDose
{
    uint16_t messageCode; //enum ControlMessageCode
    uint16_t mineral;     //enum Mineral
    uint16_t ml;          //amount in ml
};
#endif /* __CONTROLMESSAGES_H__ */
