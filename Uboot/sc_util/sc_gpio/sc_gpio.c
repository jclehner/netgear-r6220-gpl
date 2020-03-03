/************************************************************************
 * This file is part of Sercomm GPIO/LED control module;
 *  -   which should NOT modified.
 ***********************************************************************/
/* SeRcOmM */

/*
 * Header File
 */
#include "sc_gpio.h"


/* 
 * Extern Data
 */
#ifdef _SC_LED_SHIFT_CTRL_
extern struct led_shift_data_s shift_data[]; 
#endif
extern struct gpio_data_s gpio_data[];


/*
 * Static Function Declaration
 */
static s32_sc_t sc_get_index_by_gpio_id(u08_sc_t gpio_id);

#ifdef _SC_LED_SHIFT_CTRL_
static u32_sc_t sc_shift_led_state = 0;
static s32_sc_t sc_get_index_by_shift_id(u08_sc_t shift_id);
static void_sc_t sc_shift_output(u08_sc_t value);
static void_sc_t sc_shift_update(void_sc_t);
#endif

//******************************************************************************
//* Description:
//*     LED Operation Interface
//*         - to control LED by given "GPIO_ID" and "Action", GPIO_ID can be
//*             found by command "sc_led info".
//* Input:
//*     @gpio_id    - GPIO ID
//*     @action     - SC_LED_ON or SC_LED_OFF
//* Output:
//*     void_sc_t.
//* Return:
//*     @SC_ERROR:      - operation failed.
//*     @SC_SUCCESS:    - operation ok.
//*
//*****************************************************************************/
s32_sc_t sc_led_ctrl(u08_sc_t gpio_id, u08_sc_t action)
{
	s32_sc_t index;
	u08_sc_t gpio_pin;
	u08_sc_t active;

#ifdef _SC_LED_SHIFT_CTRL_	
	if( (gpio_id & 0x80) == 0)
#endif		
	{		
		index = sc_get_index_by_gpio_id(gpio_id);
		if(index != SC_ERROR)
		{
			if((gpio_data[index].direction == SC_GPIO_INPUT_BUTTON) 
                    || (gpio_data[index].direction == SC_GPIO_INPUT_OTHERS)) 
			{
				SC_PRINTF("sw can not ctl INPUT GPIO\n");
				return SC_ERROR;
			}
		
			gpio_pin = gpio_data[index].gpio_pin;
			active = gpio_data[index].active;
			
			if(action == SC_LED_ON)
			{
				if(active == SC_GPIO_LO)
				{
					sc_gpio_dat_set(gpio_pin, SC_GPIO_LO);
				}
				else
				{
					sc_gpio_dat_set(gpio_pin, SC_GPIO_HI);
				}
			}
			else /* action == SC_LED_OFF */
			{
				if(active == SC_GPIO_LO)
				{
					sc_gpio_dat_set(gpio_pin, SC_GPIO_HI);
				}
				else
				{
					sc_gpio_dat_set(gpio_pin, SC_GPIO_LO);
				}
			}
			return SC_SUCCESS;
		}
		else /* index == SC_ERROR */
		{
			SC_PRINTF("gpio_id invalid\n");
			return SC_ERROR;
		}
	}
#ifdef _SC_LED_SHIFT_CTRL_	
	else
	{
		index = sc_get_index_by_shift_id(gpio_id);
		if(index != SC_ERROR)
		{

			gpio_pin = shift_data[index].shift_led_pin;
			active = shift_data[index].active;
			if(action == SC_LED_ON)
			{
				if(active == SC_GPIO_LO)
				{
					sc_shift_led_state &= ~(1 << gpio_pin);
				}
				else
				{
					sc_shift_led_state |=  (1 << gpio_pin);
				}
			}
			else /* action == SC_LED_OFF */
			{
				if(active == SC_GPIO_LO)
				{
					sc_shift_led_state |=  (1 << gpio_pin);
				}
				else
				{
					sc_shift_led_state &= ~(1 << gpio_pin);
				}
			}
			
			sc_shift_update();
			return SC_SUCCESS;
		}
		else /* index == SC_ERROR */
		{
			SC_PRINTF("gpio_id invalid\n");
			return SC_ERROR;
		}		
	}
#endif	
}

//******************************************************************************
//* Description:
//*     LED Operation Interface
//*         - to control all LED by given "Action"
//* Input:
//*     @action     - SC_LED_ON or SC_LED_OFF
//* Output:
//*     void_sc_t.
//* Return:
//*     void_sc_t.
//*****************************************************************************/
void_sc_t sc_led_ctrl_all(u08_sc_t action)
{
	u32_sc_t i;

	for(i = 0; gpio_data[i].gpio_id != 0; i++)
	{
		if(gpio_data[i].direction == SC_GPIO_OUTPUT_LED)
		{
			sc_led_ctrl(gpio_data[i].gpio_id, action);
		}
	}
#ifdef _SC_LED_SHIFT_CTRL_
	for(i = 0; shift_data[i].shift_led_id != 0; i++)
	{
		sc_led_ctrl(shift_data[i].shift_led_id, action);
	}	
#endif		
}

//******************************************************************************
//* Description:
//*     Check Button Status
//* Input:
//*     @gpio_id     - gpio_id of specific button
//* Output:
//*     void_sc_t.
//* Return:
//*     @SC_ERROR   - this button can not be found.
//*     @SC_TRUE    - pressed
//*     @SC_FALSE   - released
//*****************************************************************************/
s32_sc_t sc_btn_is_pushed(u08_sc_t gpio_id)
{
	s32_sc_t index;

	index = sc_get_index_by_gpio_id(gpio_id);
	if(index == SC_ERROR)
	{
		return SC_ERROR;
	}

	if((gpio_data[index].direction == SC_GPIO_INPUT_BUTTON) ||  \
			(gpio_data[index].direction == SC_GPIO_INPUT_OTHERS))
	{
		if( ((gpio_data[index].active == SC_GPIO_LO) && (!sc_gpio_dat_get(gpio_data[index].gpio_pin)))
			|| ((gpio_data[index].active == SC_GPIO_HI) && (sc_gpio_dat_get(gpio_data[index].gpio_pin))) )
		{
			return SC_TRUE;
		}
		else
		{
			return SC_FALSE;
		}
	}
	else /* this led id is output */
	{
		return SC_ERROR;
	}
}

//******************************************************************************
//* Description:
//*     Return the function of given GPIO ID
//* Input:
//*     @gpio_id     - gpio_id 
//* Output:
//*     void_sc_t.
//* Return:
//*     @function of this gpio id.
//******************************************************************************
s32_sc_t sc_gpio_get_func(u32_sc_t gpio_id)
{
    s32_sc_t index;

    index = sc_get_index_by_gpio_id(gpio_id);

    return gpio_data[index].direction;
}

//******************************************************************************
//* Description:
//*     Return the name string of given GPIO ID
//* Input:
//*     @gpio_id     - gpio_id 
//* Output:
//*     void_sc_t.
//* Return:
//*     @name string of this gpio id.
//******************************************************************************
s08_sc_t* sc_gpio_get_name(u32_sc_t gpio_id)
{
    s32_sc_t index;

    index = sc_get_index_by_gpio_id(gpio_id);

    return gpio_data[index].name;
}

//******************************************************************************
//* Description:
//*     Get the Table Index by given GPIO_ID
//* Input:
//*     @gpio_id     - gpio_id 
//* Output:
//*     void_sc_t.
//* Return:
//*     @table index 
//*     @SC_ERROR   - not found.
//******************************************************************************
static s32_sc_t sc_get_index_by_gpio_id(u08_sc_t gpio_id)
{
	u08_sc_t i;

	for(i = 0; gpio_data[i].gpio_id != 0; i++)
	{
		if(gpio_data[i].gpio_id == gpio_id)
		{
#ifdef SC_GPIO_DEBUG
			SC_PRINTF("found index %d for gpio_id\n", i , gpio_id);
#endif
			return i;
		}	
	}

	SC_PRINTF("gpio_id:%d no found\n", gpio_id);
	return SC_ERROR;
}

//******************************************************************************
//* Description:
//*     Get the Table Index by given GPIO_ID, for shift IC control LEDs, the 
//*         GPIO_ID must start from 0x80..
//* Input:
//*     @gpio_id     - gpio_id 
//* Output:
//*     void_sc_t.
//* Return:
//*     @table index 
//*     @SC_ERROR   - not found.
//******************************************************************************
#ifdef _SC_LED_SHIFT_CTRL_
static s32_sc_t sc_get_index_by_shift_id(u08_sc_t shift_led_id)
{
	u08_sc_t i;

	for(i = 0; shift_data[i].shift_led_id != 0; i++)
	{
		if(shift_data[i].shift_led_id == shift_led_id)
		{
#ifdef SC_GPIO_DEBUG
			SC_PRINTF("found index %d for shift_led_id\n", i , shift_led_id);
#endif
			return i;
		}	
	}

	SC_PRINTF("shift_led_id:%d no found\n", shift_led_id);
	return SC_ERROR;
}

//******************************************************************************
//* Description:
//*     Shitf IC LED controll bit set
//* Input:
//*     @value  - bitmask standing hi or lo.
//* Output:
//*     void_sc_t.
//* Return:
//******************************************************************************
static void_sc_t sc_shift_output(u08_sc_t value)
{
    /* init clk */
    sc_gpio_dat_set(GPIO_SERIAL_CLK, SC_GPIO_LO);
    LED_DELAY;

    /* prepare data */
    if(value) {
    	sc_gpio_dat_set(GPIO_SERIAL_DATA, SC_GPIO_HI);
    } else {
        sc_gpio_dat_set(GPIO_SERIAL_DATA, SC_GPIO_LO);
    }
    LED_DELAY;

    /* pull high clk, data will set to D0 */
    sc_gpio_dat_set(GPIO_SERIAL_CLK, SC_GPIO_HI);
    LED_DELAY;
}

//******************************************************************************
//* Description:
//*     Shitf IC LED controll bit set, but no really action.
//* Input:
//*     @value  - bitmask standing hi or lo.
//* Output:
//*     void_sc_t.
//* Return:
//******************************************************************************
static void_sc_t sc_shift_update(void_sc_t)
{
    s32_sc_t i;
    for(i = MAX_SHIFT_LED_NUM - 1; i >= 0; i--) {
       sc_shift_output(((sc_shift_led_state & (1 << i)) >> i));
    }	
}
#endif


//******************************************************************************
//* Description:
//*     SC GPIO init function
//* Input:
//* Output:
//*     void_sc_t.
//* Return:
//******************************************************************************
void_sc_t sc_gpio_init(void_sc_t)
{
	u32_sc_t i;
	u08_sc_t gpio_pin;

    sc_gpio_pin_mux();

	for(i = 0; gpio_data[i].gpio_id != 0; i++)
	{
		gpio_pin = gpio_data[i].gpio_pin;

		if((gpio_data[i].direction == SC_GPIO_OUTPUT_LED) || 
				(gpio_data[i].direction == SC_GPIO_OUTPUT_OTHERS))		
		{
			/*
			 * 1:output
			 * 0:input
			 */	
            sc_gpio_dir_set(gpio_pin, SC_GPIO_OUTPUT);
		}
		else /* we init the BI_DIRECTION as input gpio too */
		{
			/* if it input,we don't care the data register when we do init. */
		    sc_gpio_dir_set(gpio_pin, SC_GPIO_INPUT);	
            continue;
		}

		if(gpio_data[i].init_state == SC_LED_OFF)
		{
			if(gpio_data[i].active == SC_GPIO_LO)
			{
				sc_gpio_dat_set(gpio_data[i].gpio_pin, SC_GPIO_HI);
			}
			else
			{
				sc_gpio_dat_set(gpio_data[i].gpio_pin, SC_GPIO_LO);
			}
		}
		else /* gpio_data[i].init_state == SC_LED_ON */
		{
			if(gpio_data[i].active == SC_GPIO_LO)
			{
				sc_gpio_dat_set(gpio_data[i].gpio_pin, SC_GPIO_LO);
			}
			else
			{
				sc_gpio_dat_set(gpio_data[i].gpio_pin, SC_GPIO_HI);
			}
		}	
	} /* end for(...) */

#ifdef _SC_WDI_
	sc_feeddog(__FILE__, __func__, __LINE__);
#endif
}


//******************************************************************************
//* Description:
//*     SC GPIO usage information dump(LEDs, Buttons, etc)
//* Input:
//* Output:
//*     void_sc_t.
//* Return:
//******************************************************************************
void_sc_t sc_gpio_map(void_sc_t)
{
	u08_sc_t i;

	SC_PRINTF("--------------------------------------------\n");
	SC_PRINTF("%-20s%-7s%-7s\n", "Name", "ID", "GPIO_PIN");
	SC_PRINTF("--------------------------------------------\n");
	

	for(i = 0; gpio_data[i].gpio_id != 0; i++)
	{
		SC_PRINTF("%-20s%-7d%-7d\n", gpio_data[i].name, gpio_data[i].gpio_id, gpio_data[i].gpio_pin);
	}
}




