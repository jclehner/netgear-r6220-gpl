/************************************************************************
 * This file is part of Sercomm GPIO/LED control module, which should be 
 * modified when porting to various projects.
 ***********************************************************************/
/* SeRcOmM */


#include "sc_gpio.h"

/*
 * For easy access of RALINK GPIO registers
 */
static RALINK_GPIO_REG sc_ralink[] =
{
	{0,  31, 0x0, 0x20},
	{32, 63, 0x4, 0x24},
	{64, 95, 0x8, 0x28}, 
};

/*
 * Core Information Data, for LED/BUTTON/etc definitions
 *  - modified per projects
 */
struct gpio_data_s gpio_data[] = {
    /* NAME             			LED_ID              		GPIO_PIN  INIT            DIR                     ACTIVE */
	{ "WPS_BUTTON",       		    GPIO_WPS_BTN, 		     	7,        SC_LED_OFF,     SC_GPIO_INPUT_BUTTON,   SC_GPIO_LO},
	{ "RST_BUTTON",        			GPIO_RST_BTN,  		     	14,       SC_LED_OFF,     SC_GPIO_INPUT_BUTTON,   SC_GPIO_LO},
	{ "INET_GREEN_LED",        		GPIO_INET_LED_GREEN,        13,       SC_LED_OFF,     SC_GPIO_OUTPUT_LED,     SC_GPIO_LO},
	{ "USB_LED",        			GPIO_USB_LED,       		15,       SC_LED_OFF,     SC_GPIO_OUTPUT_LED,     SC_GPIO_LO},
	{ "WIFI_LED",        			GPIO_WIFI_LED,  	     	16,       SC_LED_OFF,     SC_GPIO_OUTPUT_LED,     SC_GPIO_LO},
	{ "PWR_LED_GREEN",        		GPIO_PWR_LED_GREEN,  	    18,       SC_LED_OFF,     SC_GPIO_OUTPUT_LED,     SC_GPIO_LO},
	{ "5G_WIFI_DISABLE",        	GPIO_WIFI_DISABLE,      	6, 	      SC_LED_OFF,     SC_GPIO_CTRL,		      SC_GPIO_LO},
	{ "USB_PWR_RNABLE",        		GPIO_USB_ENABLE,	       	10,       SC_LED_OFF,     SC_GPIO_CTRL,  	      SC_GPIO_HI},
	{ "USB_FLAG", 		       		GPIO_USB_FLAG,       		12,       SC_LED_OFF,     SC_GPIO_CTRL,		      SC_GPIO_LO},
    /* Don't remove this line,it used to indicate this table end */
    { "\0",             			GPIO_NONE_DEFINE,   0,              0,              0,                      0}
};

#ifdef _SC_LED_SHIFT_CTRL_
/*
 * Core Information Data, for LED/BUTTON/etc definitions
 *  - modified per projects, for using SHIFT IC
 */
struct led_shift_data_s shift_data[] = {
	/* Don't remove this line,it used to indicate this table end */    
	{SC_SHIFT_NONE_DEFINE, 	0, 0, 0},
};
#endif
     


//******************************************************************************
//* Description:
//*     DUMP ALL GPIO-related registers value, for debug purpose.
//* Input:
//* Output:
//*     void_sc_t.
//* Return:
//******************************************************************************
void_sc_t sc_gpio_reg_dump(void_sc_t)
{
	s32_sc_t i = 0;
    SC_PRINTF("\n*********************************************************\n");
	SC_PRINTF("GPIO_MUX\r\t\t\[0x%08x]:\t 0x%08x\n", GPIO_PURPOSE_SELECT, *(u32_sc_t *)GPIO_PURPOSE_SELECT);

	for(i=0; i<sizeof(sc_ralink)/sizeof(RALINK_GPIO_REG); i++)
	{
		SC_PRINTF("GPIO%d_%d_DIR\r\t\t[0x%08x]:\t 0x%08x\n", 
				sc_ralink[i].end, 
				sc_ralink[i].start, 
				GPIO_BASE_ADDR + sc_ralink[i].dir,
				RALINK_REG(GPIO_BASE_ADDR + sc_ralink[i].dir));
		SC_PRINTF("GPIO%d_%d_DATA\r\t\t[0x%08x]:\t 0x%08x\n", 
				sc_ralink[i].end, 
				sc_ralink[i].start, 
				GPIO_BASE_ADDR + sc_ralink[i].dat,
				RALINK_REG(GPIO_BASE_ADDR + sc_ralink[i].dat));
	}
 	SC_PRINTF("*********************************************************\n");

    return;
}

//******************************************************************************
//* Description:
//*     set given "gpio_pin" direction by given direction.
//* Input:
//*     @gpio_in    - the platform gpio number.
//*     @dir        - SC_GPIO_INPUT or SC_GPIO_OUTPUT
//* Output:
//*     void_sc_t.
//* Return:
//*     @SC_ERROR   - operatin failed.
//*     @SC_SUCCESS - operation ok.
//******************************************************************************
s32_sc_t sc_gpio_dir_set(u32_sc_t gpio_pin, u32_sc_t dir)
{
	u32_sc_t ralink_index;

	for(ralink_index = 0; ralink_index < sizeof(sc_ralink)/sizeof(RALINK_GPIO_REG); ralink_index++)
	{
		if((gpio_pin >= sc_ralink[ralink_index].start) 
				&& (gpio_pin <= sc_ralink[ralink_index].end))
		{
			break;
		}
	}

	if(ralink_index == sizeof(sc_ralink)/sizeof(RALINK_GPIO_REG))
	{
		SC_PRINTF("Error: can not find correct GPIO pin %x\n", gpio_pin);
		return SC_ERROR;
	}
	
	if(dir == SC_GPIO_OUTPUT)
	{ 
		/* set 1 for output */
		RALINK_REG(GPIO_BASE_ADDR + sc_ralink[ralink_index].dir)	|= (1 << (gpio_pin - sc_ralink[ralink_index].start));
	}
	else
	{
		/* set 0 for input */
		RALINK_REG(GPIO_BASE_ADDR + sc_ralink[ralink_index].dir)	&= ~(1 << (gpio_pin - sc_ralink[ralink_index].start));
	}
	return SC_SUCCESS;
}

//******************************************************************************
//* Description:
//*     set given "gpio_pin" output by given hi or lo.
//* Input:
//*     @gpio_in    - the platform gpio number.
//*     @hi_lo      - SC_GPIO_HI or SC_GPIO_LO
//* Output:
//*     void_sc_t.
//* Return:
//*     @SC_ERROR   - operatin failed.
//*     @SC_SUCCESS - operation ok.
//******************************************************************************
s32_sc_t sc_gpio_dat_set(u32_sc_t gpio_pin, u32_sc_t hi_lo)
{
	u32_sc_t ralink_index;

	for(ralink_index = 0; ralink_index < sizeof(sc_ralink)/sizeof(RALINK_GPIO_REG); ralink_index++)
	{
		if((gpio_pin >= sc_ralink[ralink_index].start) 
				&& (gpio_pin <= sc_ralink[ralink_index].end))
		{
			break;
		}
	}

	if(ralink_index == sizeof(sc_ralink)/sizeof(RALINK_GPIO_REG))
	{
		SC_PRINTF("Error: can not find correct GPIO pin %x\n", gpio_pin);
		return SC_ERROR;
	}

#if 0
    /*
     * Work-around solution for controling GPIO38, GPIO39
     *       set SPI bus as GPIO mode
     */
    if(gpio_pin == 38 || gpio_pin == 39)
    {
        RALINK_REG(GPIO_PURPOSE_SELECT) |= ( 1 << SPI_GPIO_MODE_OFFSET ); 
    }
#endif	

	if(hi_lo == SC_GPIO_HI)
	{ 
		RALINK_REG(GPIO_BASE_ADDR + sc_ralink[ralink_index].dat)	|= (1 << (gpio_pin - sc_ralink[ralink_index].start));
	}
	else
	{
		RALINK_REG(GPIO_BASE_ADDR + sc_ralink[ralink_index].dat)	&= ~(1 << (gpio_pin - sc_ralink[ralink_index].start));
	}

#if 0
    /*  Clear SPI GPIO Mode */
    if(gpio_pin == 38 || gpio_pin == 39)
    {
        RALINK_REG(GPIO_PURPOSE_SELECT) &= ~( 1 << SPI_GPIO_MODE_OFFSET );    
    }
#endif
	return SC_SUCCESS;


}
//******************************************************************************
//* Description:
//*     get given "gpio_pin" value.
//* Input:
//*     @gpio_in    - the platform gpio number.
//* Output:
//*     void_sc_t.
//* Return:
//*     @SC_ERROR   - operatin failed.
//*     @SC_GPIO_HI - high.
//*     @SC_GPIO_LO - low.
//******************************************************************************
u32_sc_t sc_gpio_dat_get(u32_sc_t gpio_pin)
{
	u32_sc_t ralink_index;

	for(ralink_index = 0; ralink_index < sizeof(sc_ralink)/sizeof(RALINK_GPIO_REG); ralink_index++)
	{
		if((gpio_pin >= sc_ralink[ralink_index].start) 
				&& (gpio_pin <= sc_ralink[ralink_index].end))
		{
			break;
		}
	}

	if(ralink_index == sizeof(sc_ralink)/sizeof(RALINK_GPIO_REG))
	{
		SC_PRINTF("Error: can not find correct GPIO pin %x\n", gpio_pin);
		return SC_ERROR;
	}
	
	if( (RALINK_REG(GPIO_BASE_ADDR + sc_ralink[ralink_index].dat)) 
			& (1 << (gpio_pin - sc_ralink[ralink_index].start)))
	{
		return SC_GPIO_HI;
	}
	else
	{
		return SC_GPIO_LO;
	}
}
//******************************************************************************
//* Description:
//*     GPIO pin mux setting, will be called by "sc_gpio_init". 
//* Input:
//* Output:
//*     void_sc_t.
//* Return:
//******************************************************************************
void_sc_t sc_gpio_pin_mux(void_sc_t)
{
    /* gpio mux settings first */
	RALINK_REG(GPIO_PURPOSE_SELECT) |= (
		( 1 << WDT_MODE_OFFSET )  |
		( 1 << JTAG_MODE_OFFSET ) |
		( 1 << UART2_MODE_OFFSET) |
		( 1 << UART3_MODE_OFFSET) );
	//RALINK_REG(GPIO_PURPOSE_SELECT) &= ~(2 << WDT_MODE_OFFSET);  
}

#ifdef _SC_WDI_
typedef struct 
{
    u32_sc_t spec_min;
    u32_sc_t spec_max;
    u32_sc_t threshold;
    u64_sc_t time_stamp;
} SC_WDI_DATA;

static SC_WDI_DATA sc_wdt =
{
    .spec_min = 300,    // ms
    .spec_max = 800,    // ms
    .threshold = 700,   // ms
    .time_stamp = 0,
};


void_sc_t sc_feed_action(void_sc_t)
{
    /*
     * implemented base on Hardware Design
     */
}

///******************************************************************************
//* Description:
//*     watchdog triggier function 
//* Input:
//*     @caller_file:   called in which file;
//*     @caller_func:   called in which function;
//*     @caller_line:   called in which line;
//* Output:
//*     void_sc_t.
//* Return:
//******************************************************************************
#if 0
#define _SC_WDI_DEBUG_
#endif
void_sc_t sc_feeddog(u08_sc_t *caller_file, u08_sc_t *caller_func, u32_sc_t caller_line)
{
    u64_sc_t current;

    if(sc_wdt.time_stamp == 0)
    {
        printf("First Time to Feed!\n");
        sc_wdt.time_stamp = sc_get_time(0);
#ifdef _SC_WDI_DEBUG_
        printf("max %x, min %x, threshold %x, sc_wdt.time_stamp %x\n", sc_wdt.spec_max, sc_wdt.spec_min, sc_wdt.threshold, sc_wdt.time_stamp);
#endif
        sc_feed_action();
    }
    else
    {
        current = sc_get_time(0);
#ifdef _SC_WDI_DEBUG_
        printf("current %x time_stamp %x\n", current, sc_wdt.time_stamp);
#endif
        if( (current - sc_wdt.time_stamp) > (sc_wdt.spec_max*SC_TIMER_1S_UNIT/1000) )
        {
            /*
             * ERROR: too later!
             */    
            printf("Error: Feed Dog Slow! %s, %s, %d\n", caller_file, caller_func, caller_line);
        }
        else if( (current - sc_wdt.time_stamp) > (sc_wdt.threshold*SC_TIMER_1S_UNIT/1000) )
        {
#ifdef _SC_WDI_DEBUG_
            printf("Feed!\n");
#endif
            sc_wdt.time_stamp = sc_get_time(0);
            sc_feed_action();
        }
        else
        {
            /*
             * skip. do nothing
             */
#ifdef _SC_WDI_DEBUG_
            printf("Nothing!\n");
#endif
        }
    }
}
#endif

