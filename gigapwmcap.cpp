// gigapwmcap.cpp
// Author:  M. Kokko
// Updated: 27-Oct-2024

#include "gigapwmcap.h"

void GigaPWMInit()
{
    // prescaler for TIM1 and TIM8
    uint32_t Prescaler = (uint16_t)(SystemCoreClock / 10000000) - 1;

    /////////////////// TIMER 8 PWM CAPTURE /////////////////////

    // CONFIGURE TIMER 8 COUNTER
    RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
    TIM8->ARR = 0xFFFF;
    TIM8->PSC = Prescaler;
    TIM8->CR1 |= TIM_CR1_CEN;

    // set mode of PJ8 (Arduino D4) to alternate function AF3
    GPIOJ->MODER &= ~GPIO_MODER_MODE8;
    GPIOJ->MODER |= GPIO_MODER_MODE8_1;
    GPIOJ->AFR[1] &= ~GPIO_AFRH_AFSEL8;
    GPIOJ->AFR[1] |= (GPIO_AFRH_AFSEL8_1 | GPIO_AFRH_AFSEL8_0);       // AF3

    // configure TIM8 for PWM capture from CH1
    TIM8->TISEL &= ~TIM_TISEL_TI1SEL;

    // CONFIGURE TIM8_CH1->IC1 SIGNAL (ACTIVE RISING EDGE)
    TIM8->CCER &= ~TIM_CCER_CC1E;                                     // ensure channel is off so we can write config
    TIM8->CCMR1 &= ~TIM_CCMR1_CC1S;                                   // clear CH1 settings
    TIM8->CCMR1 |= TIM_CCMR1_CC1S_0;                                  // CC1S = 0b01: Use TI1 (TIM8_CH1) (RM0399 pg1748)
    TIM8->CCMR1 &= ~TIM_CCMR1_IC1PSC;                                 // no prescaler
    TIM8->CCMR1 &= ~TIM_CCMR1_IC1F;                                   // no filtering
    TIM8->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NE | TIM_CCER_CC1NP); // set CC1P/CC1NP = 0b00 active on RISING edge

    // CONFIGURE TIM8_CH1->IC2 SIGNAL (ACTIVE FALLING EDGE)
    TIM8->CCER &= ~TIM_CCER_CC2E;                                     // ensure channel is off so we can write config
    TIM8->CCMR1 &= ~TIM_CCMR1_CC2S;                                   // clear CH2 settings
    TIM8->CCMR1 |= TIM_CCMR1_CC2S_1;                                  // CC1S = 0b10: Use TI1 (TIM8_CH1) (RM0399 pg1748)
    TIM8->CCMR1 &= ~TIM_CCMR1_IC2PSC;                                 // no prescaler
    TIM8->CCMR1 &= ~TIM_CCMR1_IC2F;                                   // no filtering
    TIM8->CCER &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NE | TIM_CCER_CC2NP); // set CC1P/CC1NP = 0b10 active on falling edge
    TIM8->CCER |= TIM_CCER_CC2P;

    // set mode of PI2 (Arduino D71) to alternate function AF3
    GPIOI->MODER &= ~GPIO_MODER_MODE2;
    GPIOI->MODER |= GPIO_MODER_MODE2_1;
    GPIOI->AFR[0] &= ~GPIO_AFRL_AFSEL2;
    GPIOI->AFR[0] |= (GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_0);       // AF3

    // configure TIM8 for PWM capture from CH4
    TIM8->TISEL &= ~TIM_TISEL_TI4SEL;

    // CONFIGURE TIM8_CH1->IC1 SIGNAL (ACTIVE RISING EDGE)
    TIM8->CCER &= ~TIM_CCER_CC3E;                                     // ensure channel is off so we can write config
    TIM8->CCMR2 &= ~TIM_CCMR2_CC3S;                                   // clear CH3 settings
    TIM8->CCMR2 |= TIM_CCMR2_CC3S_1;                                  // CC3S = 0b20: Use TI4 (TIM8_CH4) (RM0399 pg1748)
    TIM8->CCMR2 &= ~TIM_CCMR2_IC3PSC;                                 // no prescaler
    TIM8->CCMR2 &= ~TIM_CCMR2_IC3F;                                   // no filtering
    TIM8->CCER &= ~(TIM_CCER_CC3P | TIM_CCER_CC3NE | TIM_CCER_CC3NP); // set CC3P/CC3NP = 0b00 active on RISING edge

    // CONFIGURE TIM8_CH1->IC2 SIGNAL (ACTIVE FALLING EDGE)
    TIM8->CCER &= ~TIM_CCER_CC2E;                                     // ensure channel is off so we can write config
    TIM8->CCMR2 &= ~TIM_CCMR2_CC4S;                                   // clear CH4 settings
    TIM8->CCMR2 |= TIM_CCMR2_CC4S_0;                                  // CC4S = 0b01: Use TI4 (TIM8_CH4) (RM0399 pg1748)
    TIM8->CCMR2 &= ~TIM_CCMR2_IC4PSC;                                 // no prescaler
    TIM8->CCMR2 &= ~TIM_CCMR2_IC4F;                                   // no filtering
    TIM8->CCER &= ~(TIM_CCER_CC4P | TIM_CCER_CC4NP);                  // set CC1P/CC1NP = 0b10 active on falling edge
    TIM8->CCER |= TIM_CCER_CC4P;

    // select valid trigger input
    // because we're using synchronized PWM signals we can get away with a single STM32 timer and triggering
    // on just one channel... in general this wouldn't necessarily be posssible
    TIM8->SMCR &= ~TIM_SMCR_TS;
    TIM8->SMCR |= (TIM_SMCR_TS_2 | TIM_SMCR_TS_0);                    // 0b00101 = trigger on filtered TI1 (pg1741)
    TIM8->SMCR &= ~TIM_SMCR_SMS;
    TIM8->SMCR |= TIM_SMCR_SMS_2;                                     // 0b0100 = reset mode (pg1742)

    // enable channels 1-4
    TIM8->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E);

    /////////////////////////////////////////////////////////////
}

void GigaPWMPoll(uint16_t *ch1, uint16_t *ch2)
{
    *ch1 = (TIM8->CCR4 & 0xFFFF);
    *ch2 = (TIM8->CCR2 & 0xFFFF);
}

void GigaPWMMotorCmds(int16_t *pwm1, int16_t *pwm2)
{
    uint16_t ch1, ch2;
    GigaPWMPoll(&ch1, &ch2);

    double meanrc = 0.5*(double)(ch1+ch2);
    double deltarc = (ch1-ch2);

    *pwm1 = (int16_t)((( (meanrc + RC_PWM_DIFF_REDUCTION*0.5F*deltarc) - 7500.0F) / 2500.0F) * 400.0F);
    *pwm2 = (int16_t)((( (meanrc - RC_PWM_DIFF_REDUCTION*0.5F*deltarc) - 7500.0F) / 2500.0F) * 400.0F);
}