#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <stdint.h>

#ifdef SIMULATOR
#include <stdio.h>
#else
extern "C"
{
#include "bsp_ngdf_uart.h"
#include "bsp_ngdf_conf.h"
}
#endif

Model::Model() : modelListener(0)
{
}

void Model::tick()
{
	  //check message queue
    handleIncomingMessages();
}


void Model::send()
{
	//initialize - test
	ControlMessageInitialize initMsg;
  initMsg.messageCode = INITIALIZE;
  sendMessage(&initMsg, sizeof(initMsg));	
}

void Model::handleIncomingMessages()
{
#ifndef SIMULATOR
    uint32_t length = 0;
    char* data = 0;
    while ((data = runUART(&length)) != 0)
    {
        if (data)
        {
            ControlMessageCode code = (ControlMessageCode)data[0];
            handleMessage(code, (void*)data);
        }
    }
#endif
}

void Model::handleMessage(ControlMessageCode code, void* message)
{
	switch(code)
	{
		case INITIALIZE:
			modelListener->initialize();
			break;
		//CTRL messages
		case EXECUTE_RECIPE:
		case UPDATE_CONFIGURATION:
			break;
	};
}

void Model::sendMessage(void* message, uint16_t length)
{
#ifdef SIMULATOR
	printf("sending command...\n");
#else
	sendUART(message, length);
#endif
}
