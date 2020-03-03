#include <common.h>
#include <exports.h>

static void dummy(void)
{
}
unsigned long get_version(void)
{
	return XF_VERSION;
}

void jumptable_init (void)
{
	DECLARE_GLOBAL_DATA_PTR;
	int i;

	gd->jt = (void **) malloc (XF_MAX * sizeof (void *));
	for (i = 0; i < XF_MAX; i++)
		gd->jt[i] = (void *) dummy;
	gd->jt[XF_get_version] = (void *) get_version;
	gd->jt[XF_getc] = (void *) getc;
	gd->jt[XF_tstc] = (void *) tstc;
	gd->jt[XF_putc] = (void *) putc;
	gd->jt[XF_puts] = (void *) puts;
	gd->jt[XF_printf] = (void *) printf;
	gd->jt[XF_malloc] = (void *) malloc;
	gd->jt[XF_free] = (void *) free;
	gd->jt[XF_udelay] = (void *)udelay;
	gd->jt[XF_get_timer] = (void *)get_timer;
	gd->jt[XF_vprintf] = (void *)vprintf;
	gd->jt[XF_do_reset] = (void *)do_reset;
#ifdef _SC_BUILD_
	gd->jt[XF_memcmp] = (void *)memcmp;
	gd->jt[XF_memcpy] = (void *)memcpy;
	gd->jt[XF_memset] = (void *)memset;
	gd->jt[XF_strlen] = (void *)strlen;
	gd->jt[XF_get_nand_info] = (void *)get_nand_info;
	gd->jt[XF_ranand_erase] = (void *)ranand_erase;
	gd->jt[XF_ranand_write] = (void *)ranand_write;
	gd->jt[XF_ranand_read] = (void *)ranand_read;
	gd->jt[XF_mtk_nand_read_oob_raw] = (void *)mtk_nand_read_oob_raw;
	gd->jt[XF_mtk_nand_erase] = (void *)mtk_nand_erase;
	gd->jt[XF_mtk_nand_write_oob_raw] = (void *)mtk_nand_write_oob_raw;
	gd->jt[XF_mtk_nand_read_oob] = (void *)mtk_nand_read_oob;
	gd->jt[XF_eth_halt] = (void *)eth_halt;
	gd->jt[XF_sc_NetLoop] = (void *)sc_NetLoop;
	gd->jt[XF_simple_strtoul] = (void *)simple_strtoul;
	gd->jt[XF_vsprintf] = (void *)vsprintf;
	gd->jt[XF_mtk_nand_write_oob] = (void *)mtk_nand_write_oob;
	gd->jt[XF_sc_rt2880_eth_recv] = (void *)sc_rt2880_eth_recv;
	gd->jt[XF_eth_send] = (void *)eth_send;
	gd->jt[XF_init_rx_buf] = (void *)init_rx_buf;
	gd->jt[XF_mii_mgr_read] = (void *)mii_mgr_read;
	gd->jt[XF_mii_mgr_write] = (void *)mii_mgr_write;
	gd->jt[XF_do_sc_regs] = (void *)do_sc_regs;
	gd->jt[XF_do_sc_ramtest] = (void *)do_sc_ramtest;
#endif
}
