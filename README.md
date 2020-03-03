Netgear R6260 U-Boot
====================

Downloaded from [Netgear GPL sources](https://www.downloads.netgear.com/files/GPL/R6260_R6350_R6850_R6330_V1.1.0.64_gpl_src.tgz).

Config notes:

```
!defined __KERNEL_NAND__
!defined __BOOT_NAND__
!defined MANUAL_CORRECT && ECC_ENABLE
!defined _SC_BUILD_
defined FACT_BBT
!defined MTK_NAND_BMT

```
