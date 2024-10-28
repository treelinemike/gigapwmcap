// gigapwmcap.h
// Author:  M. Kokko
// Updated: 27-Oct-2024
// 
// 2-Channel PWM capture for the Arduino Giga (STM32H747XI)
// Use case: capture PWM from to FlySky RC joysticks that are
// already temporally synchronized (we can use just one trigger)
//
// For simplicty users bare metal register manipulation on the STM32
// Note: using TIM-8 b/c Arduino or Mbed OS use TIM-1

#ifndef _GIGA_PWM
#define _GIGA_PWM

// factor for reducing the contribution of the differential
// joystick input to turning motion
#define RC_PWM_DIFF_REDUCTION 0.35F

#include <Arduino.h>
#include <stdio.h>
#include "PeripheralPins.h"
#include "mbed_toolchain.h"
#include <string.h>

void GigaPWMInit();
void GigaPWMPoll(uint16_t *ch1, uint16_t *ch2);
void GigaPWMMotorCmds(int16_t *pwm1, int16_t *pwm2);

#endif