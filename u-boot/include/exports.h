#ifndef __EXPORTS_H__
#define __EXPORTS_H__

#ifndef __ASSEMBLY__

#include <common.h>
#include <command.h>

/* These are declarations of exported functions available in C code */
unsigned long get_version(void);
int  getc(void);
int  tstc(void);
void putc(const char);
void puts(const char*);
void printf(const char* fmt, ...);
void free_hdlr(int);
void *malloc(size_t);
void free(void*);
__attribute__((nomips16)) void udelay(unsigned long);
__attribute__((nomips16)) unsigned long get_timer(unsigned long);
void vprintf(const char *, va_list);
void do_reset (void);
int memcmp(const void * cs,const void * ct,size_t count);
void * memcpy(void * dest,const void *src,size_t count);
void * memset(void * s,int c,size_t count);
size_t strlen(const char * s);
void get_nand_info(struct mtd_info *mtd , struct nand_chip *nand_chip); 
int ranand_erase(unsigned int offs, int len);
int ranand_write(char *buf, unsigned int to, int datalen);
int ranand_read(char *buf, unsigned int from, int datalen);
#ifndef _SC_BUILD_
static int mtk_nand_read_oob_raw(struct mtd_info *mtd, uint8_t * buf, int page_addr, int len);
#else
int mtk_nand_read_oob_raw(struct mtd_info *mtd, uint8_t * buf, int page_addr, int len);
#endif

#ifndef _SC_BUILD_
static int mtk_nand_erase(struct mtd_info *mtd, int page);
#else
int mtk_nand_erase(struct mtd_info *mtd, int page);
#endif

#ifndef _SC_BUILD_
static int mtk_nand_write_oob_raw(struct mtd_info *mtd, const uint8_t * buf, int page_addr, int len);
#else
int mtk_nand_write_oob_raw(struct mtd_info *mtd, const uint8_t * buf, int page_addr, int len);
#endif
#ifndef _SC_BUILD_
static int mtk_nand_read_oob(struct mtd_info *mtd, struct nand_chip *chip, int page, int sndcmd); 
#else
int mtk_nand_read_oob(struct mtd_info *mtd, struct nand_chip *chip, int page, int sndcmd);
#endif

signed char  TFTP_REC_main(void);
void eth_halt(void);
struct eth_device *eth_get_dev(void);
int sc_NetLoop(void);
unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base);

int	vsprintf(char *buf, const char *fmt, va_list args);
int mtk_nand_write_oob(struct mtd_info *mtd, struct nand_chip *chip, int page);


unsigned char *sc_rt2880_eth_recv(struct eth_device *dev, unsigned int *len);

int eth_send(volatile void *packet, int length);
void init_rx_buf(void);

u32 mii_mgr_read(u32 phy_addr, u32 phy_register, u32 *read_data);
u32 mii_mgr_write(u32 phy_addr, u32 phy_register, u32 write_data);
int do_sc_regs (cmd_tbl_t *cmd , int flag, int argc, char *argv[]);
int do_sc_ramtest (cmd_tbl_t *cmd ,int flag, int argc, char *argv[]);


void app_startup(char **);

#endif    /* ifndef __ASSEMBLY__ */

enum {
#define EXPORT_FUNC(x) XF_ ## x ,
#include <_exports.h>
#undef EXPORT_FUNC

	XF_MAX
};

#define XF_VERSION	2

#if defined(CONFIG_I386)
extern gd_t *global_data;
#endif

#endif	/* __EXPORTS_H__ */
