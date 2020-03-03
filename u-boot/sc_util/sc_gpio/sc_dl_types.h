#ifndef _SC_DL_TYPES_H_
#define _SC_DL_TYPES_H_


typedef unsigned char u08_sc_t;
typedef char s08_sc_t;
typedef unsigned int u32_sc_t;
typedef int s32_sc_t;
typedef unsigned short u16_sc_t;
typedef short s16_sc_t;
typedef void  void_sc_t;

#ifdef _SC_USE_SYSTEM_U64_
typedef unsigned long long u64_sc_t;
#else
#ifdef BIG_ENDIAN_USE
typedef struct 
{
    u32_sc_t hi;
    u32_sc_t lo; 
} u64_sc_t;
#else

typedef struct 
{
    u32_sc_t lo;
    u32_sc_t hi; 
} u64_sc_t;

#endif
#endif


/*
 * Flash Type
 */
typedef enum
{
    SC_NOR = 0,
    SC_NAND,
    SC_FL_TYPE_MAX,
} SC_FL_TYPE;

/*
 * Flash Operation Status
 */
typedef enum
{
    SC_FL_ERR = 0,
    SC_FL_OK,
} SC_FL_RET;

/*
 * SC DL Status
 */
typedef enum
{
    SC_DL_ERR,
    SC_DL_OK,
} SC_DL_RET;

/*
 * SC DL Verify Status
 */
typedef enum
{
    SC_DL_VERIFY_ERR,
    SC_DL_VERIFY_OK,
    SC_DL_VERIFY_NAND,
} SC_DL_VERIFY_RET;

#ifndef NULL
#define NULL (0)
#endif
/*
 * TRUE, FALSE
 */
typedef enum
{
    SC_FALSE = 0,
    SC_TRUE,
} SC_TURE_FALSE;

/*
 * RET VALUE
 */
typedef enum
{
    SC_ERROR = -1,
    SC_SUCCESS,
} SC_RET;


/*
 * GPIO HI, LO 
 */
typedef enum
{
    SC_GPIO_LO = 0,
    SC_GPIO_HI,
} SC_GPIO_HILO;

/*
 * GPIO DIR
 */
typedef enum
{
    SC_GPIO_OUTPUT = 0,
    SC_GPIO_INPUT,
} SC_GPIO_SET_DIR;

/*
 * OOB Information
 */
typedef struct 
{
	u32_sc_t offset;
	u32_sc_t length;
} SC_DL_OOB_FREE;

#ifdef MTD_MAX_OOBFREE_ENTRIES
#define SC_DL_MAX_OOBFREE_ENTRIES	MTD_MAX_OOBFREE_ENTRIES
#else
#define SC_DL_MAX_OOBFREE_ENTRIES	8
#endif

/*
 * ECC Algorithm
 */
typedef enum
{
    SC_ECC_HAMMING = 0,
    SC_ECC_RS,
    SC_ECC_BCH,
} SC_DL_ECC_ALGO;


/*
 * ECC layout control structure. Exported to userspace for
 * diagnosis and to allow creation of raw images
 */
typedef struct
{
    SC_DL_ECC_ALGO eccalgo; // ECC algorithm 
    u32_sc_t oob_protected; // OOB bitflips can be corrected by ECC or not? 
    u32_sc_t oob_corrected; // OOB read api can correct bitflips by Nand ECC engine or not?
	u32_sc_t eccbytes;
	u32_sc_t eccpos[128];
	SC_DL_OOB_FREE oobfree[SC_DL_MAX_OOBFREE_ENTRIES];
} SC_OOB_LAYOUT, *PSC_OOB_LAYOUT;


/***************************************************************
*       DO NOT TOUCH ANY STUFF BELOW THIS LINE         *
****************************************************************/
#define ERASE_EALL              (3)
#define ERASE_UPGRADE           (4)
#define ERASE_BOOT          (5)

#define VERSION_LENGTH      (4)

#ifndef MAC_ADDR_LENGTH
#define MAC_ADDR_LENGTH     (6)
#endif


#endif


