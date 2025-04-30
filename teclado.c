#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define ROWS 4
#define COLS 4
#define LED_VERDE 1
#define LED_ROJO 2

const uint8_t row_pins[ROWS] = {6, 7, 8, 9};
const uint8_t col_pins[COLS] = {10, 11, 12, 13};
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
const char PASSWORD[] = "369376";
char input[7];
int index = 0;
int incorrect_attempts = 0;

void setup() {
    stdio_init_all();
    printf("Bienvenido, ingrese la contraseña:\n");
    for (int i = 0; i < ROWS; i++) {
        gpio_init(row_pins[i]);
        gpio_set_dir(row_pins[i], GPIO_OUT);
    }
    for (int i = 0; i < COLS; i++) {
        gpio_init(col_pins[i]);
        gpio_set_dir(col_pins[i], GPIO_IN);
        gpio_pull_up(col_pins[i]);
    }
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_init(LED_ROJO);
    gpio_set_dir(LED_ROJO, GPIO_OUT);
}

char get_key() {
    for (int row = 0; row < ROWS; row++) {
        gpio_put(row_pins[row], 0);
        for (int col = 0; col < COLS; col++) {
            if (gpio_get(col_pins[col]) == 0) {
                while (gpio_get(col_pins[col]) == 0);
                gpio_put(row_pins[row], 1);
                return keys[row][col];
            }
        }
        gpio_put(row_pins[row], 1);
    }
    return 0;
}

void lock_device() {
    printf("\nDispositivo bloqueado. Espere 15 segundos.\n");
    for (int i = 0; i < 15; i++) {
        gpio_put(LED_ROJO, 1);
        sleep_ms(500);
        gpio_put(LED_ROJO, 0);
        sleep_ms(500);
    }
    incorrect_attempts = 0;
    printf("\nIngrese la contraseña nuevamente:\n");
}

void loop() {
    char key = get_key();
    if (key) {
        printf("%c", key);
        if (key == '#') {
            input[index] = '\0';
            if (strcmp(input, PASSWORD) == 0) {
                printf("\nContraseña correcta!\n");
                gpio_put(LED_VERDE, 1);
                gpio_put(LED_ROJO, 0);
                sleep_ms(5000);
                gpio_put(LED_VERDE, 0);
                incorrect_attempts = 0;
            } else {
                printf("\nContraseña incorrecta!\n");
                gpio_put(LED_VERDE, 0);
                gpio_put(LED_ROJO, 1);
                sleep_ms(5000);
                gpio_put(LED_ROJO, 0);
                incorrect_attempts++;
            }
            index = 0;
            if (incorrect_attempts >= 3) {
                lock_device();
            } else {
                printf("\nIngrese la contraseña nuevamente:\n");
            }
        } else if (index < 6) {
            input[index++] = key;
        }
    }
    sleep_ms(200);
}

int main() {
    setup();
    while (1) {
        loop();
    }
}

