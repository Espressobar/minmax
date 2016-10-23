#ifndef BASE_VIEW_INTERFACE_HPP
#define BASE_VIEW_INTERFACE_HPP

#include <touchgfx/hal/Types.hpp>
#include <gui/common/Defines.hpp>
#include <gui/common/Time.hpp>

using namespace touchgfx;

class BaseViewInterface
{
public:
    virtual ~BaseViewInterface() {}

    // MCU-LOAD
    virtual void updateProcessorLoad(uint8_t mcuLoadInProcent) {}
    //Update time
    //....
};
 
#endif
