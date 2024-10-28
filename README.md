2-Channel PWM capture for the Arduino Giga (STM32H747XI)
Use case: capture PWM from to FlySky RC joysticks that are already temporally synchronized (we can use just one trigger)

For simplicty users bare metal register manipulation on the STM32
Note: using TIM-8 b/c Arduino or Mbed OS use TIM-1
