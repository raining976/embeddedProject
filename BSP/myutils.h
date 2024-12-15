#ifndef MYUTILS_H
#define MYUTILS_H

#include "bsp_gpio.h"
#include "stdio.h"
#include "string.h"
#include "typedef.h"
#include "bsp_motor.h"
#include "taskscheduler.h"
#include "dht11.h"
#include "app.h"

uint8_t getCmd(uint8_t *data);
void handleCmd(uint8_t cmd);
void handledht11(uint8_t temperature, uint8_t humidity);
uint8_t getnum(uint8_t hex_value);
#endif















