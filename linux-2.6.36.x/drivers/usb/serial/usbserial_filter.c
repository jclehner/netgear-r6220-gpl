
/* fix KCode attached as usd serial driver issue. --bill Guo. */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>
#include <linux/uaccess.h>

struct sc_filter_entry {
    char *name;
    u_int16_t vid;
    u_int16_t pid;
    u_int16_t pid2;
};

extern int (*sc_usbserial_filter_hook)(struct usb_device *dev);

/* this list is auto generated by 'dongle_monitor -l' command. */
static struct sc_filter_entry dongle_list[] = {
    {"ZTE-MF637U", 0x19d2, 0x2000, 0x0031},
    {"ZTE-MF637", 0x19d2, 0x2000, 0x0031},
    {"ZTE-MF626", 0x19d2, 0x2000, 0x0031},
    {"ZTE-MF622", 0x19d2, 0x2000, 0x0001},
    {"VODAFONE-K3565-Z", 0x19d2, 0x2000, 0x0063},
    {"OVATION-MC950D", 0x1410, 0x5010, 0x4400},
    {"SIERRA-880U", 0x1199, 0x0fff, 0x6855},
    {"SIERRA-881U", 0x1199, 0x0fff, 0x6856},
    {"SIERRA-C597", 0x1199, 0x0fff, 0x0023},
    {"HUAWEI-E180", 0x12d1, 0x1001, 0x1001},
    {"HUAWEI-E220", 0x12d1, 0x1003, 0x1003},
    {"MOVISTAR-E226", 0x12d1, 0x1003, 0x1003},
    {"HUAWEI-E160", 0x12d1, 0x1003, 0x1003},
    {"HUAWEI-E160G", 0x12d1, 0x1003, 0x1003},
    {"HUAWEI-E1690", 0x12d1, 0x1446, 0x1001},
    {"HUAWEI-E261", 0x12d1, 0x1446, 0x140c},
    {"HUAWEI-E353s", 0x12d1, 0x14fe, 0x1506},
    {"HUAWEI-E156G", 0x12d1, 0x1003, 0x1003},
    {"HUAWEI-E1550", 0x12d1, 0x1446, 0x1001},
    {"HUAWEI-EC122", 0x12d1, 0x1505, 0x140c},
    {"HUAWEI-EC189", 0x12d1, 0x1505, 0x140c},
    {"HUAWEI-EC1270",0x12d1, 0x1446, 0x140c},
    {"HUAWEI-E392",0x12d1, 0x1505, 0x1506},
	{"HUAWEI-E303",0x12d1, 0x1446, 0x140c},
    {"HUAWEI-E303C",0x12d1, 0x14fe, 0x1506},
    {"HUAWEI-E303F",0x12d1, 0x1446, 0x140c},
    {"HUAWEI-E3121",0x12d1, 0x1446, 0x140c}, 
    {"YOTA-LU150",0x1076, 0x8001, 0x8002},    
    {"YOTA-LU150",0x1076, 0x8001, 0x8003},    
    {"YOTA-WLTUBA107",0x15a9, 0x002a, 0x002a},   
    {"YOTA-WLTUBA1071",0x216f, 0x0043, 0x0043},   
    {"ITON_CBP71", 0x15eb, 0x7153, 0x7152},
    {"ITON_BU580", 0x05c6, 0x6000, 0x6000},
     {"ITON_BU580", 0x05c6, 0x1000, 0x6000},
    {"T-MOBILE-E170", 0x12d1, 0x1003, 0x1003},
    {"Orange-iCON_7_2", 0x0af0, 0x1000, 0x6901},
    {"VERIZON_USB720", 0x1410, 0x2110, 0x2110},
    {"VODAFONE-K3520", 0x12d1, 0x1001, 0x1001},
    {"VODAFONE-K3565", 0x12d1, 0x1003, 0x1003},
    {"VODAFONE-E172", 0x12d1, 0x1003, 0x1003},
    {"VODAFONE-E272", 0x12d1, 0x1003, 0x1003},
    {"O2-GI0431", 0x0af0, 0x7501, 0x7501},
    {"ORANGE-GI0225", 0x0af0, 0x6971, 0x6971},
    {"OPTION-GI0201", 0x0af0, 0x6911, 0x6911},
    {"OPTION-GI0401", 0x0af0, 0x7401, 0x7401},
    {"DM9601", 0x0fe6, 0x8101, 0x8101},
	{"NETGEAR-AC329U", 0x2077, 0xf000, 0xa002},
    {"NETGEAR-AC327U", 0x2077, 0xf000, 0xa003},
	{"MICROMAX-MMX353W", 0x1c9e, 0x98ff, 0x9803},
    {"MICROMAX-MMX353G", 0x1c9e, 0xf000, 0x9605},
    {"MICROMAX-MMX377G", 0x2020, 0x0002, 0x4010},
    {NULL, 0, 0}
};

static int sc_usbserial_filter(struct usb_device *dev)
{
    struct sc_filter_entry *entry;
    u_int16_t pid;

    for (entry = &dongle_list[0]; entry->name; entry++) {
        if (entry->vid == le16_to_cpu(dev->descriptor.idVendor)
            && ((pid = le16_to_cpu(dev->descriptor.idProduct)) == entry->pid || pid == entry->pid2)) {
            printk("sc fileter match <%s> \n", entry->name);
            return 1;
        }
    }
    return 0;
}

static int __init usbserial_filter_init(void)
{
    sc_usbserial_filter_hook = sc_usbserial_filter;
    return 0;
}

static void __exit usbserial_filter_exit(void)
{
    sc_usbserial_filter_hook = NULL;
    return;
}
module_init(usbserial_filter_init);
module_exit(usbserial_filter_exit);




