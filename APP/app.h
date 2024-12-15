#include "stm32f10x.h"
#include "bsp_usart.h"
#include "app_bluetooth.h"
#include "bsp_motor.h"
#include "bsp_ic.h"
#include "bsp_gpio.h"
#include "dht11.h"
#include "taskscheduler.h"
#include "systick.h"
#include "app_dht11.h"
#include "fml_oled.h"
#include "myutils.h"


void AppInit(void);

void oledDispalyTask();
extern sys_info_t sys_parm;















