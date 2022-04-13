cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from submodule
# note: this must happen before project()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(flash)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(lib/Config)
add_subdirectory(lib/Infrared)
add_subdirectory(lib/OLED)
add_subdirectory(lib/Icm20948)
add_subdirectory(lib/LCD)
add_subdirectory(lib/Fonts)
add_subdirectory(lib/GUI)
add_subdirectory(examples)

include_directories(examples)
include_directories(./lib/Config)
include_directories(./lib/GUI)
include_directories(./lib/Icm20948)
include_directories(./lib/LCD)
include_directories(./lib/Infrared)
include_directories(./lib/OLED)

add_executable(
        flash
        flash.c
        )

target_link_libraries(
        flash
        pico_stdlib
        hardware_flash
        hardware_sync
        LCD
        OLED
        Infrared
        Icm20948
        GUI
        Fonts
        Config
        hardware_spi
        )


pico_enable_stdio_uart(flash 0)
pico_enable_stdio_usb(flash 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(flash)

# add url via pico_set_program_url
# example_auto_set_url(flash)
