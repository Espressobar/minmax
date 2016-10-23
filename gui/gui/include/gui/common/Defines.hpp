#ifndef __DEFINES_HPP__
#define __DEFINES_HPP__

#include <touchgfx/hal/Types.hpp>
#include <gui/common/Time.hpp>


#define BLACK     (Color::getColorFrom24BitRGB( 0x0,  0x0,  0x0))
#define WHITE     (Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF))
#define RED			  (Color::getColorFrom24BitRGB(0xDC, 0x2B, 0x27))
#define BLUE		  (Color::getColorFrom24BitRGB(0x00, 0x90, 0xDA))

enum SubScreen
{
  IDLE = 0,
  SPLASH,
  NUMBER_OF_SUB_SCREENS
  //More...
};

#endif /* __DEFINES_HPP__ */
