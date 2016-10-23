#include <platform/hal/simulator/sdl/HALSDL.hpp>
#include <touchgfx/hal/NoDMA.hpp>
#include <common/TouchGFXInit.hpp>
#include <platform/driver/touch/SDLTouchController.hpp>
#if !defined(USE_BPP) || USE_BPP==16
#include <platform/driver/lcd/LCD16bpp.hpp>
#elif USE_BPP==24
#include <platform/driver/lcd/LCD24bpp.hpp>
#elif USE_BPP==4
#include <platform/driver/lcd/LCD4bpp.hpp>
#elif USE_BPP==2
#include <platform/driver/lcd/LCD2bpp.hpp>
#elif USE_BPP==1
#include <platform/driver/lcd/LCD1bpp.hpp>
#else
#error Unknown USE_BPP
#endif
#include <stdlib.h>

using namespace touchgfx;

int main(int argc, char** argv)
{
    NoDMA dma; //For windows/linux, DMA transfers are simulated
#if !defined(USE_BPP) || USE_BPP==16
    LCD16bpp lcd;
#elif USE_BPP==24
    LCD24bpp lcd;
#elif USE_BPP==4
    LCD4bpp lcd;
#elif USE_BPP==2
    LCD2bpp lcd;
#elif USE_BPP==1
    LCD1bpp lcd;
#else
#error Unknown USE_BPP
#endif
    SDLTouchController tc;

    // Create hardware layer. Use a display size of 480x272.
    HAL& hal = touchgfx_generic_init<HALSDL>(dma, lcd, tc, 800, 480, 0, 0);

    // Simulate hardware running at 60Hz generating a vsync every 16.6667 ms
    static_cast<HALSDL&>(hal).setVsyncInterval(16.6667f);

    // Initialize SDL
    if (!static_cast<HALSDL&>(hal).sdl_init(argc, argv))
    {
        return EXIT_FAILURE;
    }

    touchgfx::HAL::getInstance()->taskEntry(); //Never returns

    return EXIT_SUCCESS;
}
