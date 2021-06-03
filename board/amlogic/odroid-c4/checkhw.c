
/*
 * board/amlogic/txl_skt_v1/txl_skt_v1.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include <common.h>
#include <malloc.h>
#include <errno.h>

#include <env.h>
#include <fdt_support.h>
#include <linux/libfdt.h>
#include <amlogic/cpu_id.h>
#include <asm/arch/secure_apb.h>
//#include <asm/arch/pinctrl_init.h>
#ifdef CONFIG_AML_V2_FACTORY_BURN
#include <amlogic/aml_v2_burning.h>
#endif// #ifdef CONFIG_AML_V2_FACTORY_BURN
//#include <asm/arch/eth_setup.h>
#include <phy.h>
#include <linux/mtd/partitions.h>
#include <linux/sizes.h>
#include <asm-generic/gpio.h>
#include <dm.h>
#include <asm/armv8/mmu.h>

#ifdef CONFIG_MULTI_DTB
int checkhw(char * name)
{
	/*
	 * set aml_dt according to chip and dram capacity
	 */
	unsigned int ddr_size=0;
	char loc_name[64] = {0};
	int i;
	cpu_id_t cpu_id=get_cpu_id();

	for (i=0; i<CONFIG_NR_DRAM_BANKS; i++) {
		ddr_size += gd->bd->bi_dram[i].size;
	}
#if defined(CONFIG_SYS_MEM_TOP_HIDE)
	ddr_size += CONFIG_SYS_MEM_TOP_HIDE;
#endif
	if (MESON_CPU_MAJOR_ID_SM1 == cpu_id.family_id) {
		switch (ddr_size) {
			case 0x80000000:
				strcpy(loc_name, "sm1_ac200_2g\0");
				break;
			case 0x40000000:
				strcpy(loc_name, "sm1_ac200_1g\0");
				break;
			case 0x2000000:
				strcpy(loc_name, "sm1_ac200_512m\0");
				break;
			default:
				strcpy(loc_name, "sm1_ac200_unsupport");
				break;
		}
	}
	else {
		switch (ddr_size) {
			case 0x80000000:
				strcpy(loc_name, "g12a_u200_2g\0");
				break;
			case 0x40000000:
				strcpy(loc_name, "g12a_u200_1g\0");
				break;
			case 0x2000000:
				strcpy(loc_name, "g12a_u200_512m\0");
				break;
			default:
				strcpy(loc_name, "g12a_u200_unsupport");
				break;
		}
	}
	strcpy(name, loc_name);
	env_set("aml_dt", loc_name);
	return 0;
}
#endif
