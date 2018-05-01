

#ifndef TIMER_H_
#define TIMER_H_

/* Function declaration for timer A congiguration function */
void timer_a0_config(void);

/* Function declaration for timer A configuration function for PWM */
void timer_a0_pwm_config();

/* Function declaration for timer A handler, more details need to be added */
void TA0_0_IRQHandler(void);

//function definition for setting up the systic timer
void systic_config(void);

// function defintion for making the pwm signal increase to 12%
void pwm_up();

// function definition for making the pwm signal decrease to 3%
void pwm_down();

#endif /* TIMER_H_ */
