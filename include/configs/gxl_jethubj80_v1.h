/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration for Amlogic Meson 64bits SoCs
 * (C) Copyright 2016 Beniamino Galvani <b.galvani@gmail.com>
 */

#ifndef __MESON64_CONFIG_H
#define __MESON64_CONFIG_H

/* import from v2015-amlogic */
//#define CONFIG_AML_V2_FACTORY_BURN              1       //support facotry usb burning
//#define CONFIG_AML_FACTORY_BURN_LOCAL_UPGRADE   1       //support factory sdcard burning
//#define CONFIG_POWER_KEY_NOT_SUPPORTED_FOR_BURN 1       //There isn't power-key for factory sdcard burning
//#define CONFIG_SD_BURNING_SUPPORT_UI            1       //Displaying upgrading progress bar when sdcard/udisk burning

/* ------------------------- */
/* import from v2019-amlogic */
#define PACKAGE_ID_MASK			0xFF
#define CPU_ID_REG			P_AO_SEC_SD_CFG8
/* ------------------------- */


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

#define CONFIG_CPU_ARMV8
#define CONFIG_REMAKE_ELF
#define CONFIG_SYS_MAXARGS		32
#define CONFIG_SYS_MALLOC_LEN		(32 << 20)
#define CONFIG_SYS_CBSIZE		1024

#define CONFIG_SYS_SDRAM_BASE		0
#define CONFIG_SYS_INIT_SP_ADDR		0x20000000
#define CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_BOOTM_LEN		(64 << 20) /* 64 MiB */

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

#ifdef CONFIG_CMD_NVME
	#define BOOT_TARGET_NVME(func) func(NVME, nvme, 0)
#else
	#define BOOT_TARGET_NVME(func)
#endif

#ifndef BOOT_TARGET_DEVICES
#define BOOT_TARGET_DEVICES(func) \
	func(ROMUSB, romusb, na)  \
	func(MMC, mmc, 0) \
	func(MMC, mmc, 1) \
	func(MMC, mmc, 2) \
	BOOT_TARGET_DEVICES_USB(func) \
	BOOT_TARGET_NVME(func) \
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
