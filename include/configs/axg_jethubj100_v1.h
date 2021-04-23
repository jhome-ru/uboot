/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration for Amlogic Meson 64bits SoCs
 * (C) Copyright 2016 Beniamino Galvani <b.galvani@gmail.com>
 */

#ifndef __MESON64_CONFIG_H
#define __MESON64_CONFIG_H


#define CONFIG_CPU_ARMV8
#define CONFIG_REMAKE_ELF
#define CONFIG_ENV_SIZE			0x2000
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_MALLOC_LEN		(32 << 20)
#define CONFIG_SYS_CBSIZE               1024

#define CONFIG_SYS_SDRAM_BASE           0
#define CONFIG_SYS_INIT_SP_ADDR         0x20000000
#define CONFIG_SYS_LOAD_ADDR            CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_BOOTM_LEN            (64 << 20) /* 64 MiB */


/* Generic Interrupt Controller Definitions */
#if (defined(CONFIG_MESON_AXG) || defined(CONFIG_MESON_G12A))
#define GICD_BASE			0xffc01000
#define GICC_BASE			0xffc02000
#else /* MESON GXL and GXBB */
#define GICD_BASE			0xc4301000
#define GICC_BASE			0xc4302000
#endif

/* For splashscreen */
#ifdef CONFIG_DM_VIDEO
#define STDOUT_CFG "vidconsole,serial"
#else
#define STDOUT_CFG "serial"
#endif

#ifdef CONFIG_USB_KEYBOARD
#define STDIN_CFG "usbkbd,serial"
#else
#define STDIN_CFG "serial"
#endif

/* +++++++++  ADD FROM U-BOOT 2015.04 +++++++++++++++++ */
/* SMP Definitinos */
#define CPU_RELEASE_ADDR		secondary_boot_func

#define CONFIG_FIT 1
#define CONFIG_OF_LIBFDT 1

/* cpu */
#define CPU_CLOCK					1200 //MHz. Range: 600-1800, should be multiple of 24

/* ddr */
// #define CONFIG_DDR_SIZE					0 //MB //0 means ddr size auto-detect

/* IMPORTANT
 *    DDR clk <= 400MHz, please enable CONFIG_DDR_PLL_BYPASS blow
 *    DDR clk > 400MHz, please disable CONFIG_DDR_PLL_BYPASS blow
 */
// #define CONFIG_DDR_CLK					912  //MHz, Range: 200-1200, should be multiple of 24
// #define CONFIG_DDR4_CLK					912  //MHz, this is for same board with DDR4 chip

#define CONFIG_NR_DRAM_BANKS			1
/* DDR type setting
 *    CONFIG_DDR_TYPE_LPDDR3   : LPDDR3
 *    CONFIG_DDR_TYPE_DDR3     : DDR3
 *    CONFIG_DDR_TYPE_DDR4     : DDR4
 *    CONFIG_DDR_TYPE_AUTO     : DDR3/DDR4 auto detect */
// #define CONFIG_DDR_TYPE					CONFIG_DDR_TYPE_AUTO
/* DDR channel setting, please refer hardware design.
 *    CONFIG_DDR0_16BIT        : DDR0 16bit mode
 *    CONFIG_DDR0_16BIT_2      : DDR0 16bit mode, 2ranks
 *    CONFIG_DDR_CHL_AUTO      : auto detect RANK0 / RANK0+1 */
// #define CONFIG_DDR_CHANNEL_SET			CONFIG_DDR0_16BIT
/* ddr functions */
// #define CONFIG_DDR_FULL_TEST			0 //0:disable, 1:enable. ddr full test
// #define CONFIG_CMD_DDR_D2PLL			0 //0:disable, 1:enable. d2pll cmd
// #define CONFIG_CMD_DDR_TEST				0 //0:disable, 1:enable. ddrtest cmd
// #define CONFIG_DDR_LOW_POWER			1 //0:disable, 1:enable. ddr clk gate for lp
// #define CONFIG_DDR_ZQ_PD				0 //0:disable, 1:enable. ddr zq power down
// #define CONFIG_DDR_USE_EXT_VREF			0 //0:disable, 1:enable. ddr use external vref
// #define CONFIG_DDR4_TIMING_TEST			0 //0:disable, 1:enable. ddr4 timing test function
// #define CONFIG_DDR_PLL_BYPASS			0 //0:disable, 1:enable. ddr pll bypass function
// #define CONFIG_DDR_FUNC_PRINT_WINDOW	0 //0:disable, 1:enable. print ddr training window


// #define CONFIG_CMD_NAND 1
// #define CONFIG_MTD_DEVICE y
// #define CONFIFG_AML_MTDPART	1
// #define CONFIG_CMD_MTDPARTS   1
// #define CONFIG_MTD_PARTITIONS 1
// #define CONFIG_SYS_MAX_NAND_DEVICE  2
// #define CONFIG_SYS_NAND_BASE_LIST   {0}
// #define 	CONFIG_GENERIC_MMC 1
#define 	CONFIG_CMD_MMC 1
#define     CONFIG_CMD_GPT 1
#define	CONFIG_SYS_MMC_ENV_DEV 1
// #define CONFIG_EMMC_DDR52_EN 0
// #define CONFIG_EMMC_DDR52_CLK 35000000

#define 	CONFIG_ENV_OVERWRITE
// defined #define 	CONFIG_CMD_SAVEENV
#define		CONFIG_PARTITIONS 1
// #define 	CONFIG_SYS_NO_FLASH  1

#define CONFIG_CMD_USB 1
#if defined(CONFIG_CMD_USB)
//	#define CONFIG_GXL_XHCI_BASE            0xff500000
//	#define CONFIG_GXL_USB_PHY2_BASE        0xffe09000
//	#define CONFIG_GXL_USB_PHY3_BASE        0xffe09080
	#define CONFIG_USB_STORAGE      1
#endif //#if defined(CONFIG_CMD_USB)
// #define CONFIG_CMD_FASTBOOT 1
// #define CONFIG_FASTBOOT_FLASH_MMC_DEV 1

// #define CONFIG_FASTBOOT_FLASH 1
#define CONFIG_USB_GADGET 1
#define CONFIG_SYS_CACHELINE_SIZE 64
// #define CONFIG_FASTBOOT_MAX_DOWN_SIZE	0xA000000
// #define CONFIG_DEVICE_PRODUCT	"jethome_j100"

// #define CONFIG_AML_V2_FACTORY_BURN              1       //support facotry usb burning
// #define CONFIG_POWER_KEY_NOT_SUPPORTED_FOR_BURN 1       //There isn't power-key for factory sdcard burning
// #define CONFIG_SD_BURNING_SUPPORT_UI            1       //Displaying upgrading progress bar when sdcard/udisk burning

// #define CONFIG_UNIFY_KEY_MANAGE                 1

 
#define CONFIG_SYS_I2C_MESON		1

#if defined(CONFIG_DM_I2C)
#define CONFIG_SYS_I2C_MESON		1
#else
// #define CONFIG_SYS_I2C_AML			1
#define CONFIG_SYS_I2C_SPEED		400000
#define CONFIG_I2C_MULTI_BUS 		1
#endif

// #define CONFIG_EFUSE 1

/* commands */
#define CONFIG_CMD_CACHE 1
#define CONFIG_CMD_BOOTI 1
// #define CONFIG_CMD_EFUSE 1
#define CONFIG_CMD_I2C 1
#define CONFIG_CMD_MEMORY 1
#define CONFIG_CMD_FAT 1
#define CONFIG_CMD_GPIO 1
// defined #define CONFIG_CMD_RUN
// #define CONFIG_CMD_REBOOT 1
#define CONFIG_CMD_ECHO 1
// #define CONFIG_CMD_JTAG	1
// #define CONFIG_CMD_AUTOSCRIPT 1
#define CONFIG_CMD_MISC 1

/*file system*/
// #define CONFIG_MBR_PARTITION 1
#define CONFIG_DOS_PARTITION 1
#define CONFIG_EFI_PARTITION 1

#define CONFIG_MMC 1
#define CONFIG_FS_FAT 1
#define CONFIG_FS_EXT4 1
#define CONFIG_LZO 1

/* other functions */
// #define CONFIG_NEED_BL301	1
// #define CONFIG_NEED_BL32	1
// #define CONFIG_CMD_RSVMEM	1
// #define CONFIG_FIP_IMG_SUPPORT	1
#ifdef CONFIG_BOOTDELAY
#undef CONFIG_BOOTDELAY
#endif
#define CONFIG_BOOTDELAY	2
#define CONFIG_SYS_LONGHELP 1
#define CONFIG_CMD_MISC     1
#define CONFIG_CMD_ITEST    1
// #define CONFIG_CMD_CPU_TEMP 1
#define CONFIG_SYS_MEM_TOP_HIDE 0
// #define CONFIG_MULTI_DTB	1

// #define CONFIG_CMD_CHIPID 1

// #define CONFIG_SECURE_STORAGE 1
#define ETHERNET_INTERNAL_PHY


/* +++++++ END  ADD FROM U-BOOT 2015.04 +++++++++++++++ */


/* ROM USB boot support, auto-execute boot.scr at scriptaddr */
#define BOOTENV_DEV_ROMUSB(devtypeu, devtypel, instance) \
	"bootcmd_romusb=" \
		"if test \"${boot_source}\" = \"usb\" && " \
				"test -n \"${scriptaddr}\"; then " \
			"echo '(ROM USB boot)'; " \
			"source ${scriptaddr}; " \
		"fi\0"

#define BOOTENV_DEV_NAME_ROMUSB(devtypeu, devtypel, instance)	\
		"romusb "

#ifdef CONFIG_CMD_USB
#define BOOT_TARGET_DEVICES_USB(func) func(USB, usb, 0)
#else
#define BOOT_TARGET_DEVICES_USB(func)
#endif

#ifndef BOOT_TARGET_DEVICES
#define BOOT_TARGET_DEVICES(func) \
	func(ROMUSB, romusb, na)  \
	func(MMC, mmc, 0) \
	func(MMC, mmc, 1) \
	func(MMC, mmc, 2) \
	BOOT_TARGET_DEVICES_USB(func) \
	func(PXE, pxe, na) \
	func(DHCP, dhcp, na)
#endif

#include <config_distro_bootcmd.h>

#ifndef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	"stdin=" STDIN_CFG "\0" \
	"stdout=" STDOUT_CFG "\0" \
	"stderr=" STDOUT_CFG "\0" \
	"fdt_addr_r=0x08008000\0" \
	"scriptaddr=0x08000000\0" \
	"kernel_addr_r=0x08080000\0" \
	"pxefile_addr_r=0x01080000\0" \
	"fdtoverlay_addr_r=0x01000000\0" \
	"ramdisk_addr_r=0x13000000\0" \
	"fdtfile=amlogic/" CONFIG_DEFAULT_DEVICE_TREE ".dtb\0" \
	BOOTENV
#endif


#endif /* __MESON64_CONFIG_H */
