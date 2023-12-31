/*
 * timers.c
 *
 * Created: 4/8/2023 5:02:04 PM
 *  Author: Team 2
 */ 
#include "timers.h"
static unsigned int u16_gs_t0_initial_value= 0;
static unsigned int u16_gs_t2_initial_value= 0;

/********************************************************************************************/
/*Description: Set timer 0 control register to normal mode by clearing bits COM00 & COM01.  */
/*@param void                                                                               */	
/********************************************************************************************/
void TIMER0_normalMode(void)
{
    clear_bit(TCCR0,4);
    clear_bit(TCCR0,5);
    
}

/****************************************************************************************/
/*Description: Setting the initial value for the timer 0 counter register               */
/*@param value initial value to start timer 0 from                                      */	
/****************************************************************************************/
void TIMER0_initialValue(uint8_t u8_a_value)
{
    TCNT0 = u8_a_value;
    
}

/************************************************************************/
/*Description: Sets the prescaler mode.                                 */
/*Choose a prescaler mode from this list => TIMER_OFF_                  */
/*                                       => NO_PRESCALE_                */
/*                                       => PRESCALE_8_	                */
/*                                       => PRESCALE_64_                */
/*                                       => PRESCALE_256_               */
/*                                       => PRESCALE_1024_              */
/*                                       => PRESCALE_EXT_FALLING_       */
/*                                       => PRESCALE_EXT_RISING_        */
/*@param prescaler prescaler mode                                       */	
/************************************************************************/
void TIMER0_prescalerMode(unsigned int u16_a_prescaler)
{
    switch(u16_a_prescaler)
    {
        case 0: //Timer off
            clear_bit(TCCR0,0);
            clear_bit(TCCR0,1);
            clear_bit(TCCR0,2);
            break;
            
        case 1: //No prescaling
            set_bit(TCCR0,0);
            clear_bit(TCCR0,1);
            clear_bit(TCCR0,2);
            break;
        
        case 8: //Prescaling set to clk/8
            clear_bit(TCCR0,0);
            set_bit(TCCR0,1);
            clear_bit(TCCR0,2);
            break;
            
        case 64: //Prescaling set to clk/64
            set_bit(TCCR0,0);
            set_bit(TCCR0,1);
            clear_bit(TCCR0,2);
            break;						

        case 256: //Prescaling set to clk/256
            clear_bit(TCCR0,0);
            clear_bit(TCCR0,1);
            set_bit(TCCR0,2);
            break;
            
        case 1024: //Prescaling set to clk/8
            set_bit(TCCR0,0);
            clear_bit(TCCR0,1);
            set_bit(TCCR0,2);
            break;
            
        case 11: //Prescaling set to external clock source on T0 pin. clock on falling edge
            set_bit(TCCR0,0);
            set_bit(TCCR0,1);
            clear_bit(TCCR0,2);
            break;								

        case 12: //Prescaling set to external clock source on T0 pin. clock on rising edge
            set_bit(TCCR0,0);
            set_bit(TCCR0,1);
            set_bit(TCCR0,2);
            break;

        default: //Timer off
            clear_bit(TCCR0,0);
            clear_bit(TCCR0,1);
            clear_bit(TCCR0,2);
            break;			
    }
    
}

/************************************************************************/
/*Description: Stays in a busy loop until the delay is met              */
/*@param f_a_delayInMillis  delay time needed in ms                     */	
/************************************************************************/
void TIMER0_delay(float f_a_delayInMillis)
{
    //variable initialization
    unsigned int u16_l_numberOfOVF;
    unsigned int u16_l_count = 0;
    
    //get no of overflows needed to achieve the required delay
    u16_l_numberOfOVF = TIMER0_getInitialValue(f_a_delayInMillis);
    
    //Sets the prescaler mode to the mode chosen by the user (defined as a global variable in the timers.h file)

    
    //keep looping until achieving the number of overflows required
    while(u16_l_count<u16_l_numberOfOVF)	
    {	
        //busy loop until the timer interrupt flag is up
        TCNT0 = u16_gs_t0_initial_value;
        TIMER0_prescalerMode(PRESCALER_MODE);
        while( (TIFR & (1<<0) ) ==0 );
        TCCR0 = 0;		
        set_bit(TIFR,0);
        //clear flag
        u16_l_count++;
    }
    //close the timer
    TCCR0 = 0;    
    
}

/************************************************************************************************************************************************************************/
/*Description: Gets The timer initial value according to the delay needed, prescaler mode and CPU frequency, then sets the timer initial value accordingly.             */
/*@param f_a_delayInMillis delay time needed in ms                                                                                                                      */
/************************************************************************************************************************************************************************/
unsigned int TIMER0_getInitialValue(float f_a_delayInMillis)
{
    //variable initialization
    volatile unsigned int u16_l_numberOfOVF=0;
    //Initial value calculations
    f_a_delayInMillis = f_a_delayInMillis * (0.001);
    u16_l_numberOfOVF = ceil(f_a_delayInMillis / t0_max_delay );
    u16_gs_t0_initial_value = 256.0 - ceil( (f_a_delayInMillis/t_tick) / u16_l_numberOfOVF );
    return u16_l_numberOfOVF;
}



/************************************************************************/
/* Timer 2 Driver                                                       */
/************************************************************************/

/********************************************************************************************/
/*Description: Set timer 2 control register to normal mode by clearing bits COM00 & COM01.  */
/*@param void                                                                               */	
/********************************************************************************************/
void TIMER2_normalMode(void)
{
    clear_bit(TCCR2,4);
    clear_bit(TCCR2,5);
    
}

/****************************************************************************************/
/*Description: Setting the initial value for the timer 2 counter register               */
/*@param value initial value to start timer 2 from                                      */
/****************************************************************************************/
void TIMER2_initialValue(uint8_t u8_a_value)
{
    TCNT2 = u8_a_value;
    
}

/************************************************************************/
/*Description: Sets the prescaler mode.                                 */
/*Choose a prescaler mode from this list => TIMER_OFF_                  */
/*                                       => NO_PRESCALE_                */
/*                                       => PRESCALE_8_	                */
/*                                       => PRESCALE_64_                */
/*                                       => PRESCALE_256_               */
/*                                       => PRESCALE_1024_              */
/*                                       => PRESCALE_EXT_FALLING_       */
/*                                       => PRESCALE_EXT_RISING_        */
/*@param prescaler prescaler mode                                       */	
/************************************************************************/
void TIMER2_perscalerMode(unsigned int u16_a_prescaler)
{
    switch(u16_a_prescaler)
    {
        case 0: //Timer off
            clear_bit(TCCR2,0);
            clear_bit(TCCR2,1);
            clear_bit(TCCR2,2);
            break;

        case 1: //No prescaling
            set_bit(TCCR2,0);
            clear_bit(TCCR2,1);
            clear_bit(TCCR2,2);
            break;

        case 8: //Prescaling set to clk/8
            clear_bit(TCCR2,0);
            set_bit(TCCR2,1);
            clear_bit(TCCR2,2);
            break;

        case 64: //Prescaling set to clk/64
            set_bit(TCCR2,0);
            set_bit(TCCR2,1);
            clear_bit(TCCR2,2);
            break;

        case 256: //Prescaling set to clk/256
            clear_bit(TCCR2,0);
            clear_bit(TCCR2,1);
            set_bit(TCCR2,2);
            break;

        case 1024: //Prescaling set to clk/8
            set_bit(TCCR2,0);
            clear_bit(TCCR2,1);
            set_bit(TCCR2,2);
            break;

        case 11: //Prescaling set to external clock source on T0 pin. clock on falling edge
            set_bit(TCCR2,0);
            set_bit(TCCR2,1);
            clear_bit(TCCR2,2);
            break;

        case 12: //Prescaling set to external clock source on T0 pin. clock on rising edge
            set_bit(TCCR2,0);
            set_bit(TCCR2,1);
            set_bit(TCCR2,2);
            break;

        default: //Timer off
            clear_bit(TCCR2,0);
            clear_bit(TCCR2,1);
            clear_bit(TCCR2,2);
        break;
    }
    
}

/************************************************************************/
/*Description: Stays in a busy loop until the delay is met              */
/*@param f_a_delayInMillis delay time needed in ms                      */
/************************************************************************/
void TIMER2_delay(float f_a_delayInMillis)
{
    //variable initialization
    unsigned int u16_l_numberOfOVF;
    unsigned int u16_l_count = 0;

    //get no of overflows needed to achieve the required delay
    u16_l_numberOfOVF = TIMER2_getInitialValue(f_a_delayInMillis);

    //keep looping until achieving the number of overflows required
    while(u16_l_count<u16_l_numberOfOVF)
    {
        //busy loop until the timer interrupt flag is up
        TCNT2 = u16_gs_t2_initial_value;
        TIMER2_perscalerMode(PRESCALER_MODE);
        while( (TIFR & (1<<6) ) ==0 );
        TCCR2 = 0;
        set_bit(TIFR,6);
        //clear flag
        u16_l_count++;
    }
    //close the timer
    TCCR2 = 0;
    
}

/************************************************************************************************************************************************************************/
/*Description: Gets The timer initial value according to the delay needed, prescaler mode and CPU frequency, then sets the timer initial value accordingly.             */
/*@param f_a_delayInMillis delay time needed in ms                                                                                                                      */
/************************************************************************************************************************************************************************/
unsigned int TIMER2_getInitialValue(float f_a_delayInMillis)
{
    //variable initialization
    volatile unsigned int u16_l_numberOfOVF=0;
    //Initial value calculations
    f_a_delayInMillis = f_a_delayInMillis * (0.001);
    u16_l_numberOfOVF = ceil( f_a_delayInMillis / t2_max_delay );
    u16_gs_t2_initial_value = 256.0 - ceil( (f_a_delayInMillis/t_tick) / u16_l_numberOfOVF );
    return u16_l_numberOfOVF;
}