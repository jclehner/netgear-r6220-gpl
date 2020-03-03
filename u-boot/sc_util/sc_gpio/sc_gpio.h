#ifndef _SC_GPIO_H_
#define _SC_GPIO_H_

#include "sc_dl_types.h"

#define RALINK_REG(x)	(*(volatile u32_sc_t *)(x))
/* SeRcOmM */

/*************************************************************************
 * Section to put platform dependent definitions for GPIOs
 ************************************************************************/
#define RALINK_SYSCTL_BASE          (0xBE000000)
//#define RALINK_ETH_SW_BASE          (0xB0110000)
#define GPIO_BASE_ADDR              (0xBE000600)

#define CPU_REG(x)					(*((volatile unsigned int *)( x)))

typedef struct {
	u32_sc_t start;
	u32_sc_t end;
	u32_sc_t dir;
	u32_sc_t dat;
} RALINK_GPIO_REG;

#define GPIO_PURPOSE_SELECT         (0xBE000060)

#ifdef _SC_7620_
#define SUTIF_SHARE_MODE_OFFSET     (30)    /* [35,37] */
#define WDT_RST_MODE_OFFSET         (21)    /* [17] */
#define PA_G_GPIO_MODE_OFFSET       (20)    /* [18,21] */
#define ND_SD_GPIO_MODE_OFFSET      (18)    /* [45,59] */
#define PERST_GPIO_MODE_OFFSET      (16)    /* [36] */
#define EPHY_LED_GPIO_MODE_OFFSET   (15)    /* [40,44] */
#define WLED_GPIO_MODE_OFFSET       (13)    /* [72] */
#define SPI_REFCLK0_MODE_OFFSET     (12)    /* [37] */
#define SPI_GPIO_MODE_OFFSET        (11)    /* [3,6] [37,39] */
#define RGMII2_GPIO_MODE_OFFSET     (10)    /* [60,71] */
#define RGMII1_GPIO_MODE_OFFSET     (9)     /* [24,35] */
#define MDIO_GPIO_MODE_OFFSET       (7)     /* [22,23] */
#define JTAG_GPIO_MODE_OFFSET       (6)    
#define UARTL_GPIO_MODE_OFFSET      (5)     /* [15,16] */
#define UARTF_SHARE_MODE_OFFSET     (2)     /* [7,14] */
#define I2C_GPIO_MODE_OFFSET        (0)     /* [1,2] */
#else //7621
#define ESWINT_MODE_OFFSET     		(20)    /* [61] */
#define SDXC_MODE_OFFSET   			(18)    /* [41,48] */
#define SPI_MODE_OFFSET       		(16)    /* [34,40] */
#define RGMII2_MODE_OFFSET     		(15)    /* [22,33] */
#define RGMII1_MODE_OFFSET     		(14)    /* [49,60] */
#define MDIO_MODE_OFFSET        	(12)    /* [20,21] */
#define PERST_MODE_OFFSET     		(10)    /* [19] */
#define WDT_MODE_OFFSET    			(8)     /* [18] */
#define JTAG_MODE_OFFSET       		(7)     /* [13,17] */
#define UART2_MODE_OFFSET       	(5)     /* [9,12]*/ 
#define UART3_MODE_OFFSET      		(3)     /* [5,8] */
#define I2C_MODE_OFFSET     		(2)     /* [3,4] */
#define UART1_MODE_OFFSET        	(1)     /* [1,2] */
#endif
/************************************************************************
 * Section to put project dependent GPIO_ID info here
 *  - GPIO ID is not GPIO PIN number in GPIO registers. It's just a lable
 *    for Button/LED/Other Functions. 
 *  - LED controlled by SHIFT IC are is managed in same manner and please
 *    define '_SC_LED_SHIFT_CTRL_' for it. 
 ************************************************************************/ 

/*
 * The name plese follow this rule
 *      GPIO_XXX_YYY_ZZZ
 *   XXX:The type,just like:LED,BUTTON or OTHER
 *   YYY:The name,just like WPS,POWER ...
 *   ZZZ:The color,just like GREEN,RED...
 *  Change this per project!!!
 */
enum{
    GPIO_NONE_DEFINE = 0,
    GPIO_WIFI_DISABLE,
	GPIO_USB_LED,
	GPIO_WIFI_LED,
	GPIO_INET_LED_GREEN,
	GPIO_PWR_LED_GREEN,
	GPIO_RST_BTN,
	GPIO_WPS_BTN,
	GPIO_USB_ENABLE,
	GPIO_USB_FLAG,
    GPIO_MAX_ID,
};


#ifdef _SC_LED_SHIFT_CTRL_
/*
 * LEDs controller by SHIFT IC(Serial to Parallel) 
 */
#define MAX_SHIFT_LED_NUM	16
#define LED_DELAY           (udelay(1))


enum {
	SC_SHIFT_NONE_DEFINE = 0,
};
#endif


/*
 * Enable Debug or NOT
 */
#if 0
#define SC_GPIO_DEBUG	1
#endif

/*
 * print function 
 */
#ifndef SC_PRINTF
#define SC_PRINTF	printf
#endif

/********************************************************************************/
/****************** DO NOT MODIFY LINES BELOW THIS ******************************/
/********************************************************************************/
#ifndef _SC_LED_ACTION_
#define _SC_LED_ACTION_
typedef enum {
	SC_LED_OFF,
	SC_LED_ON,
} SC_LED_ACTION;

typedef enum {
	SC_GPIO_INPUT_BUTTON,	/* GPIO as input,used for button */
	SC_GPIO_INPUT_OTHERS,	/* Not all input GPIO used for button */
	SC_GPIO_OUTPUT_LED,		/* GPIO as output,used for LED */
	SC_GPIO_OUTPUT_OTHERS,	/* Not all output GPIO used for LED */
	SC_GPIO_CTRL,
	SC_GPIO_BI_DIR,			/* Bi direction GPIO,can be input or output */
} SC_GPIO_DIR;
#endif


struct gpio_data_s {
	u08_sc_t name[20];
	u08_sc_t gpio_id;
	u08_sc_t gpio_pin;	
	u08_sc_t init_state;	/* when we do init,what state we set it.(used for output GPIO) */
	u08_sc_t direction;
	/*
	 * Normal case the low will let the LED on,but some times the hardware design
	 * let the led on when GPIO output HI...
	 */
	u08_sc_t active; 
};

#ifdef _SC_LED_SHIFT_CTRL_
struct led_shift_data_s {
	u08_sc_t shift_led_id;
	u08_sc_t shift_led_pin;
	u08_sc_t init_state;
	u08_sc_t active;
};
#endif

/* 
 * GPIO Generic Functions Protypes 
 */
 u32_sc_t sc_gpio_dat_get(u32_sc_t gpio_pin);
 s32_sc_t sc_gpio_dat_set(u32_sc_t gpio_pin, u32_sc_t hi_lo);
 s32_sc_t sc_gpio_dir_set(u32_sc_t gpio_pin, u32_sc_t dir);
void_sc_t sc_gpio_reg_dump(void_sc_t);

/* 
 * LED/Button Functions Protypes 
 */
void_sc_t sc_gpio_init(void_sc_t);
void_sc_t sc_gpio_pin_mux(void_sc_t);
 s32_sc_t sc_led_ctrl(u08_sc_t gpio_id, u08_sc_t action);
void_sc_t sc_led_ctrl_all(u08_sc_t action);
 s32_sc_t sc_btn_is_pushed(u08_sc_t gpio_id);
 s32_sc_t sc_gpio_get_func(u32_sc_t gpio_id);
s08_sc_t* sc_gpio_get_name(u32_sc_t gpio_id);

#define SC_GPIO_IS_LED(gpio_id) \
    ((sc_gpio_get_func(gpio_id)==SC_GPIO_OUTPUT_LED)?1:0) 
#define SC_GPIO_IS_BTN(gpio_id) \
    ((sc_gpio_get_func(gpio_id)==SC_GPIO_INPUT_BUTTON)?1:0) 

 /*
 * For watchdog
 */
#ifdef _SC_WDI_
void_sc_t sc_feeddog(u08_sc_t *caller_file, u08_sc_t *caller_func, u32_sc_t caller_line);
#endif
#endif /* eof */


