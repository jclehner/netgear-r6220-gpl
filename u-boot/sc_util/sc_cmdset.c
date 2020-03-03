#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>
#include <../sc_include/sc_cmd_def.h>

#ifdef CONFIG_MODEM_SUPPORT
#include <malloc.h>		/* for free() prototype */
#endif

#ifdef CONFIG_SYS_HUSH_PARSER
#include <hush.h>
#endif

#include <post.h>

/***********************************************************************
 * Sercomm StandAlone Code Entry
 ***********************************************************************/
#ifndef _SC_NAND_SUPPORT_
extern int raspi_read(char *buf, unsigned int from, int len);
#else
int ranand_read(char *buf, unsigned int from, int datalen);
#endif //_SC_NAND_SUPPORT_
/* HEX to string */
static void hex2str(ulong num, char *p)
{
    int i;

    p[0] = '0';
    p[1] = 'x';
    for(i=9; i>=2; i--)
    {
        ulong x = num&0xf;
        p[i] = (x < 0xa) ? x+'0':x-0xa+'A';
        num = num>>4;
    }
    p[10] = '\0';
}

void sc_standalone(SC_TASK task, SC_CMD_NAME cmd, char argc, char *argv[])
{	
    char *local_args[10];
	char local_argc;
    char cmd_s[11];
    void (*call_fun)(int argc, char *argv[]);
    int i = 0;

    memset(STANDALONE_LOAD_ADDR, 0xFF, 0x100000);
    memcpy((char *)STANDALONE_LOAD_ADDR, (char *)STANDALONE_COPY_IN_RAM_ADDR, STANDALONE_LENGTH);
    
    switch(task) {
	case SC_BOOT:
		local_args[0] = SC_BOOT_STR;
        local_args[1] = argv[0];
		local_argc    = 2;
		break;
	case SC_DL:
		local_args[0] = SC_DL_STR;
		local_args[1] = NULL;
	
		for(i=0; i<argc; i++)
        {
			local_args[2+i] = argv[i];
        }

		local_argc    = argc + 2;
		break;
	case SC_CMD:
		local_args[0] = SC_CMD_STR;
        hex2str(cmd, cmd_s);
		local_args[1] = cmd_s;

		for(i=0; i<argc; i++)
        {
			local_args[2+i] = argv[i];
        }

		local_argc    = argc + 2;
		break;
	default:
		printf("Do not know what to do ... \n");
		return;
	}

    /* 
     * The ation loading standalone code had been done when Uboot running.
     * So, if need to enter standalone mode, only jump the certain memory address. 
     */

   /*Get function address in standalone code and call upgrade function */
   call_fun = (void (*)(int argc, char *argv[]))(STANDALONE_LOAD_ADDR);
   call_fun(local_argc, local_args);

}
#ifdef _SC_UPGRADE_
/***********************************************************************
 * Sercomm Debug Command Set
 ***********************************************************************/
/* 
 * CMD : sc_dl
 * 		SC upgrade command
 */
int do_sc_dl (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_DL, 0, argc, argv);
    return 0;
}

U_BOOT_CMD(
	sc_dl,	2,	1,	do_sc_dl,
	"sc_dl		-SC: sercomm upgrade \n",
	"sc_dl [debug_level], debug_level = <0..3>, 0 - all, 1 - warn, 2 - info, 3 - error, defaule is 2\n"
);
#endif //_SC_UPGRADE_

#if 0
/*
 * CMD : sc_endian 
 * 		Uesd to test current platform endian.
 */
int do_sc_endian_test (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_CMD, SC_CMD_ENDIAN, argc, argv);
    return 0;	
}


U_BOOT_CMD(
	sc_endian,	3,	1,	do_sc_endian_test,
	"sc_endian   -SC: help to test current system endian\n",
	""
);

#endif // 0
#if 1
/*
 * CMD: sc_lp
 */
int do_sc_lp (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_CMD, SC_CMD_LP, argc, argv);
	return 0;
}


U_BOOT_CMD(
	sc_lp, CONFIG_SYS_MAXARGS,	1,	do_sc_lp,
	"sc_lp       -SC: SC loopback sample command\n",
	"usage:       sc_lp <loopback_times>"
);
#endif
#ifdef _SC_NMRP_
/*
 * CMD: sc_nmrp
 */
int do_sc_nmrp (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_CMD, SC_CMD_NMRP, 0, NULL);
    return 0;
}

U_BOOT_CMD(
		sc_nmrp, 1, 1, do_sc_nmrp,
		"sc_nmrp\t- Sercomm nmrp\n",
		"sc_nmrp usage:\n"
);

#endif
#ifdef _SC_TFTP_REC_
int do_sc_tftp_rec (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_CMD,SC_CMD_TFTP_REC,0,NULL);
	return 0;
}
U_BOOT_CMD(
    sc_tftp_rec, 1, 1, do_sc_tftp_rec,
    "sc_tftp_rec\t- Sercomm nmrp\n",
    "sc_tftp_rec usage:\n"
);
#endif 
/*
 * CMD : sc_fl_map
 * 		Sercomm Flash Map dump
 */

int do_sc_flmap (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_CMD, SC_CMD_FLMAP, argc, argv);
	return 0; 
}

U_BOOT_CMD(
	sc_fl_map, CONFIG_SYS_MAXARGS,	1,	do_sc_flmap,
	"sc_fl_map   -SC: SC Defined Flash Map Dump\n",
	""
);
/* 
 * CMD : sc_btver
 *		Show Bootloader Sercomm Version
 */

int do_sc_btver (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_CMD, SC_CMD_BTVER, argc, argv);
	return 0; 
}

U_BOOT_CMD(
	sc_btver, CONFIG_SYS_MAXARGS,	1,	do_sc_btver,
	"sc_btver    -SC: SC Show Bootloader Version(SC), and dump compile timestamp\n",
	""
);
#ifdef _SC_GPIO_
/*
 * CMD : sc_led
 * 		sc debug command for led & buttons
 */

int do_sc_led (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_CMD, SC_CMD_LED, argc, argv);
	return 0; 
}

U_BOOT_CMD(
	sc_led, 3,	2,	do_sc_led,
	"sc_led      -SC: sc debug command for led & buttons\n",
	"usage:\n"
    "\tsc_led on all : set all leds on\n"
	"\tsc_led off all : set all leds off\n"
	"\tsc_led on <led_num> : set certain led on\n"
	"\tsc_led off <led_num> : set certain led off\n"
	"\tsc_led info : dump led & button maps w/ GPIO\n"
);
/*
 * CMD : sc_gpio
 * 		sc debug for gpio control.
 */
int do_sc_gpio (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_CMD, SC_CMD_GPIO, argc, argv);
	return 0; 
}

U_BOOT_CMD(
	sc_gpio, 4,	2,	do_sc_gpio,
	"sc_gpio     -SC: sc debug command for gpio operations\n",
	"usage:\n"
    "\tsc_gpio d : display gpio register values\n"
	"\tsc_gpio i <gpio_num> : set certain gpio as input\n"
	"\tsc_gpio o <gpio_num> : set certain gpio as output\n"
	"\tsc_gpio w <gpio_num> <1|0> : set gpio high or low\n"
	"\tsc_gpio r <gpio_num> : get gpio input\n"
);
#endif // _SC_GPIO_

#if 1
#ifdef _SC_RAMTEST_
/*
 * CMD : sc_ramtest
 * 		sc ramtest command
 */

int do_sc_ramtest (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_CMD, SC_CMD_RAMTEST, argc, argv);
	return 0; 
}

U_BOOT_CMD(
	sc_ramtest, CONFIG_SYS_MAXARGS,	1,	do_sc_ramtest,
	"sc_ramtest  -SC: sc debug command for memory test\n",
	"usage:\n"
    "\tsc_ramtest <start> <end> [times]\n [0xA0300000 , 0xA1000000]\n"
);
#endif //_SC_RAMTEST_
#endif
#if 0 //Picasso has no switch
/*
 * CMD : sc-sw
 *      sercomm R/W switch register
 */
int do_sc_sw(cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
    sc_standalone(SC_CMD, SC_CMD_SW, argc, argv);
    return 0;
}

U_BOOT_CMD(
        sc_sw, 4, 2, do_sc_sw,
        "sc_sw       -SC: sercomm R/W switch register\n",
        "usage:\n"
 	    "\nsc_sw r [REG_ADDR(HEX)]  \n"
     	"sc_sw w [REG_ADDR(HEX)] [data(HEX)] \n"
);
/*
 * CMD : sc_sw_init
 * 		init QCA8337 giga switch  
 */
int do_sc_sw_init (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_CMD, SC_CMD_SW_INIT, argc, argv);
	return 0; 
}
U_BOOT_CMD(
	sc_sw_init, 1,	1,	do_sc_sw_init,
	"sc_sw_init   -SC: init QCA8035 giga switch\n",
	"usage:\n"
    "\tsc_sw_init\n"
);
#endif
/*
 * CMD : sc_phy
 * 		sc phy debug command 
 */
int do_sc_phy (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_CMD, SC_CMD_PHY, argc, argv);
	return 0; 
}
U_BOOT_CMD(
 	sc_phy,5,4,	do_sc_phy,
 	"sc_phy  - read/write 8337 phy register \n",
 	"\nsc_phy <w|r> <PHY_ADDR> <REG_ADDR> [DATA] \n"
 	""
);
/*
 * CMD : sc_eth_test
 * 	    enter to test mode  QCA8337 giga switch  
 */
int do_sc_eth_test (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_CMD, SC_CMD_ETH_TEST, argc, argv);
	return 0; 
}
U_BOOT_CMD(
	sc_eth_test, 3,	3,	do_sc_eth_test,
	"sc_eth_test   -SC: enter to test mode of QCA8337 giga switch\n",
	"usage:\n"
    "\tsc_eth_test <mode_num> <PHY_NUM> : no phy_num means all phy \n"
    "\t phy_num :[0,4] ,WAN port is phy 0\n"
);

int do_sc_regs (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_CMD, SC_CMD_CPUREG, argc, argv);
	return 0; 
}

U_BOOT_CMD(
	sc_cpureg, CONFIG_SYS_MAXARGS,	1,	do_sc_regs,
	"sc_cpureg   -SC: sc debug for read/write cpu registers\n",
	"usage:\n"
    "\tsc_cpureg <r|w> <address> [data]\n"
);


#ifdef _SC_NAND_SUPPORT_
int do_sc_nand (cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
    sc_standalone(SC_CMD, SC_CMD_NAND, argc, argv);
    return 0;
}

U_BOOT_CMD(
        sc_nand, 4, 2, do_sc_nand,
        "sc_nand      -SC: sc nand flash\n",
        "usage:\n"
        "\tsc_nand r <offset>  : dump page data and oob by given offset(in hex)\n"
        "\tsc_nand w <offset> <data>  : write one byte data to flash\n"
        "\tsc_nand e <offset>  : erase block\n"
        "\tsc_nand markbad <offset> : mark one block as bad block \n"
        "\tsc_nand clearbad <offset>: clear bad block tag \n"
        "\tsc_nand b [offset] : scan bad block\n"
        "\tsc_nand oob : dump oob of first page for whole flash\n"
        );
#endif //_SC_NAND_SUPPORT_


int do_sc_timer(cmd_tbl_t * cmd, int flag, int argc, char *argv[])
{
	sc_standalone(SC_CMD,SC_CMD_TIMER,argc,argv);
	return 0;
}

U_BOOT_CMD(
	sc_timer, 4, 1, do_sc_timer,
	"sc_timer\t- Sercomm timer command\n",
	"sc_timer usage:\n"
	"sc_timer\n"
);
