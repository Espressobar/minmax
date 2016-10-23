/*
   Shared between Arduino and STM32
*/
#ifndef __CONTROLMESSAGES_H__
#define __CONTROLMESSAGES_H__

#define INPUT_SIZE 30

enum ControlMessages
{
  
  //STM32 TO ARDUINO
  EXECUTE_RECIPE 	= 65,
  CONFIGURE     	= 66,

  //ARDUINO TO STM32
  INITIALIZE 		= 97,
};

//todo we need to configure the sources
struct ControlMessageStart
{
	uint16_t controlMessage;
	uint16_t GH;
	uint16_t KH;
	// Something about
};

struct ControlMessageInitialize
{
	uint16_t controlMessage;
};

// Adjust recipe message
// Configure stuff

enum Component
{
	MG 		= 10,
	CA 		= 11,
	HCO3 	= 12	
};

typedef enum
{
	MGSO4 	= 200,
	MGCL2 	= 201,
	CACL2 	= 203,
	CACO3 	= 204,
} Salt;

#endif /* __CONTROLMESSAGES_H__ */