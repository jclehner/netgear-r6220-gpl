#ifndef _SC_CMD_DEF_
#define _SC_CMD_DEF_
/***********************************************
 * Standalone Dispatcher
 ***********************************************/

typedef enum {
	SC_BOOT = 1,
	SC_DL,
	SC_CMD,
} SC_TASK;

typedef enum {
	SC_CMD_ENDIAN = 1,
	SC_CMD_BTVER,
	SC_CMD_FLMAP,
	SC_CMD_GPIO,
	SC_CMD_LED,
	SC_CMD_LP,
	SC_CMD_NOR,
    SC_CMD_NAND,
	SC_CMD_PHY,
    SC_CMD_MEM,
	SC_CMD_RAMTEST,
	SC_CMD_CPUREG,
	SC_CMD_NMRP,
	SC_CMD_TFTP_REC,
	SC_CMD_ETH_TEST,
	SC_CMD_TIMER,
} SC_CMD_NAME;

#define STANDALONE_COPY_IN_RAM_ADDR (0xA0400000)

#define STANDALONE_LOAD_ADDR        (0xA0100000) //debug in RAM
/*standalone address in *.bin*/
#define STANDALONE_FLASH_OFFSET     (0x40000) //128K
/*standalone length in *.bin*/
#define STANDALONE_LENGTH           (0x20000) //128K
#define STANDALONE_COPY_LENGTH		(0x100000)

/*CMD max argc*/
#define CONFIG_SYS_MAXARGS 16

#define SC_BOOT_STR    "boot"
#define SC_DL_STR      "dl"
#define SC_CMD_STR     "cmd"

#endif

