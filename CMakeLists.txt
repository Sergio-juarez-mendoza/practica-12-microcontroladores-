cmake_minimum_required(VERSION 3.13)

# Nombre del proyecto
project(Proyecto_12)

# Habilitar la Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)
pico_sdk_init()

# Agregar el archivo fuente
add_executable(teclado
    teclado.c
)

# Agregar las bibliotecas estándar de la Pico
target_link_libraries(teclado
    pico_stdlib
    hardware_gpio
    hardware_uart
)

# Habilitar la salida USB para el puerto serie
pico_enable_stdio_usb(teclado 1)
pico_enable_stdio_uart(teclado 0)

# Crear el archivo binario UF2
pico_add_extra_outputs(teclado)
