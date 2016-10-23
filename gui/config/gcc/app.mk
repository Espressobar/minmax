# Relative location of the TouchGFX framework from root of application
touchgfx_path := ../../../touchgfx

# Optional additional compiler flags
user_cflags := -DUSE_BPP=16

# Settings for image converter output format
alpha_dither := yes
opaque_image_format := RGB565
non_opaque_image_format := ARGB8888

# Settings for image converter screen orientation (empty string = default value, -rotate90 rotates the image 90 degrees)
screen_orientation :=

#Settings for Hardware accelerated text rendering on STM32F4 and F7 devices. Must correspond to value of bitsPerPixel for font to have any effect.
#If A4 blitcap is enabled for target specific HAL the fontconverter must generate compliant data format, otherwise resulting in a DMA Controller Configuration Error.
text_data_format :=
