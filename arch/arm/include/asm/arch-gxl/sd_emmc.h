
/*
 * arch/arm/include/asm/arch-gxb/sd_emmc.h
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

#ifndef __SD_EMMC_H__
#define __SD_EMMC_H__

#include "cpu_sdio.h"

#define SD_EMMC_BASE_A 0xd0070000
#define SD_EMMC_BASE_B 0xd0072000
#define SD_EMMC_BASE_C 0xd0074000

#define NEWSD_IRQ_ALL				    0x3fff

#define SD_EMMC_CLKSRC_24M 	24000000
#define SD_EMMC_CLKSRC_DIV2	1000000000

#define NEWSD_IRQ_EN_ALL_INIT
#define NEWSD_MAX_DESC_MUN					512
#define NEWSD_BOUNCE_REQ_SIZE		(512*1024)
#define MAX_BLOCK_COUNTS	256
#define SD_EMMC_CLKSRC 24000000

#define MMC_RSP_136_NUM					4
#define MAX_RESPONSE_BYTES     4

#define RESPONSE_R1_R3_R6_R7_LENGTH     6
#define RESPONSE_R2_CID_CSD_LENGTH      17
#define RESPONSE_R4_R5_NONE_LENGTH      0

#define SDIO_PORT_A    0
#define SDIO_PORT_B    1
#define SDIO_PORT_C    2

#define CARD_SD_SDIO_INIT          (1<<0)
#define CARD_SD_SDIO_DETECT        (1<<1)
#define CARD_SD_SDIO_PWR_PREPARE   (1<<2)
#define CARD_SD_SDIO_PWR_ON        (1<<3)
#define CARD_SD_SDIO_PWR_OFF       (1<<4)

typedef enum _SD_Error_Status_t {
	SD_NO_ERROR                 = 0,
	SD_ERROR_OUT_OF_RANGE,                  //Bit 31
	SD_ERROR_ADDRESS,                       //Bit 30
	SD_ERROR_BLOCK_LEN,                     //Bit 29
	SD_ERROR_ERASE_SEQ,                     //Bit 28
	SD_ERROR_ERASE_PARAM,                   //Bit 27
	SD_ERROR_WP_VIOLATION,                  //Bit 26
	SD_ERROR_CARD_IS_LOCKED,                    //Bit 25
	SD_ERROR_LOCK_UNLOCK_FAILED,                //Bit 24
	SD_ERROR_COM_CRC,                       //Bit 23
	SD_ERROR_ILLEGAL_COMMAND,               //Bit 22
	SD_ERROR_CARD_ECC_FAILED,                   //Bit 21
	SD_ERROR_CC,                                //Bit 20
	SD_ERROR_GENERAL,                       //Bit 19
	SD_ERROR_Reserved1,                         //Bit 18
	SD_ERROR_Reserved2,                         //Bit 17
	SD_ERROR_CID_CSD_OVERWRITE,             //Bit 16
	SD_ERROR_AKE_SEQ,                           //Bit 03
	SD_ERROR_STATE_MISMATCH,
	SD_ERROR_HEADER_MISMATCH,
	SD_ERROR_DATA_CRC,
	SD_ERROR_TIMEOUT,
	SD_ERROR_DRIVER_FAILURE,
	SD_ERROR_WRITE_PROTECTED,
	SD_ERROR_NO_MEMORY,
	SD_ERROR_SWITCH_FUNCTION_COMUNICATION,
	SD_ERROR_NO_FUNCTION_SWITCH,
	SD_ERROR_NO_CARD_INS
} SD_Error_Status_t;


typedef enum _SD_Bus_Width
{
	SD_BUS_SINGLE                   = 1,        //only DAT0
	SD_BUS_WIDE                     = 4         //use DAT0-4
} SD_Bus_Width_t;

//LSB -> MSB, structrue for SD Card Status
typedef struct _SD_Card_Status
{
	unsigned Reserved3: 2;
	unsigned Reserved4: 1;
	unsigned AKE_SEQ_ERROR: 1;                  //Error in the sequence of authentication process.
	unsigned Reserved5: 1;
	unsigned APP_CMD: 1;                        //The card will expect ACMD, or indication that the command has been interpreted as ACMD.
	unsigned NotUsed: 2;

	unsigned READY_FOR_DATA: 1;                 //Corresponds to buffer empty signalling on the bus.
	unsigned CURRENT_STATE: 4;                  //The state of the card when receiving the command.
	unsigned ERASE_RESET: 1;                    //An erase sequence was cleared beforem executing because an out of erase sequence command was received.
	unsigned CARD_ECC_DISABLED: 1;              //The command has been executed without using the internal ECC.
	unsigned WP_ERASE_SKIP: 1;                  //Only partial address space was erased due to existing write protected blocks.

	unsigned CID_CSD_OVERWRITE: 1;              //Can be either one of the following errors:
	unsigned Reserved1: 1;
	unsigned Reserved2: 1;
	unsigned ERROR: 1;                          //A general or an unknown error occurred during the operation.
	unsigned CC_ERROR: 1;                       //Internal card controller error
	unsigned CARD_ECC_FAILED: 1;                //Card internal ECC was applied but failed to correct the data.
	unsigned ILLEGAL_COMMAND: 1;                //Command not legal for the card state
	unsigned COM_CRC_ERROR: 1;                  //The CRC check of the previous command failed.

	unsigned LOCK_UNLOCK_FAILED: 1;             //Set when a sequence or password error has been detected in lock/ unlock card command or if there was an attempt to access a locked card

	unsigned CARD_IS_LOCKED: 1;                 //When set, signals that the card is locked by the host
	unsigned WP_VIOLATION: 1;                   //Attempt to program a write-protected block.
	unsigned ERASE_PARAM: 1;                    //An invalid selection of write-blocks for erase occurred.
	unsigned ERASE_SEQ_ERROR: 1;                //An error in the sequence of erase commands occurred.
	unsigned BLOCK_LEN_ERROR: 1;                //The transferred block length is not allowed for this card, or the number of transferred bytes does not match the block length.
	unsigned ADDRESS_ERROR: 1;                  //A misaligned address that did not match the block length was used in the command.
	unsigned OUT_OF_RANGE: 1;                   //The command??s argument was out of the allowed range for this card.

} SD_Card_Status_t;

//structure for response
typedef struct _SD_Response_R1
{
	SD_Card_Status_t card_status;               //card status
} SD_Response_R1_t;


struct aml_card_sd_info
{
	unsigned sd_emmc_port;				 //0: sdioa, 1:sdiob, 2:sdioc
	unsigned sd_emmc_reg_base;
	char * name;
	int inited_flag;
	int removed_flag;
	int init_retry;
	int single_blk_failed;
	char* desc_buf;
	struct mmc_config cfg;
	struct sd_emmc_global_regs *sd_emmc_reg;
	dma_addr_t		desc_dma_addr;
#ifdef AML_CARD_SD_INFO_DETAILED
	int  (* sd_emmc_init)(unsigned port,struct aml_card_sd_info *sdio);
	int  (* sd_emmc_detect)(unsigned port,struct aml_card_sd_info *sdio);
	void (* sd_emmc_pwr_prepare)(unsigned port,struct aml_card_sd_info *sdio);
	void (* sd_emmc_pwr_on)(unsigned port,struct aml_card_sd_info *sdio);
	void (* sd_emmc_pwr_off)(unsigned port,struct aml_card_sd_info *sdio);
	unsigned int sdio_pwr_flag;
#else
	int  (* sd_emmc_init)(unsigned port);
	int  (* sd_emmc_detect)(unsigned port);
	void (* sd_emmc_pwr_prepare)(unsigned port);
	void (* sd_emmc_pwr_on)(unsigned port);
	void (* sd_emmc_pwr_off)(unsigned port);
#endif
};

extern struct aml_card_sd_info * cpu_sd_emmc_get(unsigned port);
extern int                cpu_sd_emmc_init(unsigned port);
extern void               cpu_sd_emmc_pwr_prepare(unsigned port);
struct mmc;//mmc is struct mmc , to avoid include mmc.h , declare it
extern void               sd_emmc_register(struct aml_card_sd_info *);
extern void sd_emmc_para_config(struct sd_emmc_global_regs *reg,
		unsigned int clock, unsigned int port);
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2016 Carlo Caione <carlo@caione.org>
 */

#include <mmc.h>

#define SDIO_PORT_A			0
#define SDIO_PORT_B			1
#define SDIO_PORT_C			2

#define SD_EMMC_CLKSRC_24M		24000000	/* 24 MHz */
#define SD_EMMC_CLKSRC_DIV2		1000000000	/* 1 GHz */

#define MESON_SD_EMMC_CLOCK		0x00
#define CLK_MAX_DIV   GENMASK(5, 0)
#define CLK_MAX_SRC   GENMASK(7, 6)
#define	Cfg_div 	0
#define Cfg_src		6
#define Cfg_co_phase	8
#define	Cfg_tx_phase	10
#define	Cfg_rx_phase	12
#define	Cfg_sram_pd		14
#define	Cfg_tx_delay	16
#define	Cfg_rx_delay	22
#define	Cfg_always_on	28
#define	Cfg_irq_sdio_sleep   29
#define Cfg_irq_sdio_sleep_ds		30

#define MESON_SD_EMMC_DELAY1	0x4
#define DLY_D0_MASK	GENMASK(5, 0)
#define DLY_D1_MASK	GENMASK(11, 6)
#define DLY_D2_MASK	GENMASK(17, 12)
#define DLY_D3_MASK	GENMASK(23, 18)
#define DLY_D4_MASK	GENMASK(31, 24)
#define Dly_d0	0
#define Dly_d1	6
#define Dly_d2	12
#define Dly_d3	18
#define Dly_d4	24

#define MESON_SD_EMMC_DELAY2	0x8
#define DLY_D5_MASK	GENMASK(5, 0)
#define DLY_D6_MASK	GENMASK(11, 6)
#define DLY_D7_MASK	GENMASK(17, 12)
#define DLY_D8_MASK	GENMASK(23, 18)
#define DLY_D9_MASK	GENMASK(31, 24)
#define Dly_d5	0
#define Dly_d6	6
#define Dly_d7	12
#define Dly_d8	18
#define Dly_d9	24

#define MESON_SD_EMMC_ADJUST	0xC
#define CALI_SEL_MASK	GENMASK(11, 8)
#define ADJ_DLY_MASK	GENMASK(21, 16)
#define Cfg_cali_sel	8
#define Cfg_cali_en		12
#define Cfg_adj_en		13
#define Cfg_cali_rise	14
#define Cfg_ds_en		15
#define Cfg_adj_dly		16
#define Cfg_adj_auto	22
#define Cfg_adj_init	23

#define MESON_SD_EMMC_START		0x40
#define   CFG_DESC_INIT			BIT(0)
#define   CFG_DESC_BUSY			BIT(1)
#define   CFG_DESC_ADDR			2

#define MESON_SD_EMMC_CFG		0x44
#define   CFG_BUS_WIDTH_MASK		GENMASK(1, 0)
#define   CFG_BUS_WIDTH_1		0
#define   CFG_BUS_WIDTH_4		1
#define   CFG_BUS_WIDTH_8		2
#define   CFG_DDR				BIT(2)
#define   CFG_BL_LEN_MASK		GENMASK(7, 4)
#define   CFG_BL_LEN_SHIFT		4
#define   CFG_BL_LEN_512		(9 << 4)
#define   CFG_RESP_TIMEOUT_MASK		GENMASK(11, 8)
#define   CFG_RESP_TIMEOUT_256		(8 << 8)
#define   CFG_RC_CC_MASK		GENMASK(15, 12)
#define   CFG_RC_CC_16			(4 << 12)
#define   CFG_SDCLK_ALWAYS_ON		BIT(18)
#define   CFG_AUTO_CLK			BIT(23)

#define MESON_SD_EMMC_STATUS		0x48
#define   STATUS_MASK			GENMASK(15, 0)
#define   STATUS_ERR_MASK		GENMASK(12, 0)
#define   STATUS_RXD_ERR_MASK		GENMASK(7, 0)
#define   STATUS_TXD_ERR		BIT(8)
#define   STATUS_DESC_ERR		BIT(9)
#define   STATUS_RESP_ERR		BIT(10)
#define   STATUS_RESP_TIMEOUT		BIT(11)
#define   STATUS_DESC_TIMEOUT		BIT(12)
#define   STATUS_END_OF_CHAIN		BIT(13)

#define MESON_SD_EMMC_IRQ_EN		0x4c

#define MESON_SD_EMMC_CMD_CFG		0x50
#define   CMD_CFG_LENGTH_MASK		GENMASK(8, 0)
#define   CMD_CFG_BLOCK_MODE		BIT(9)
#define   CMD_CFG_R1B			BIT(10)
#define   CMD_CFG_END_OF_CHAIN		BIT(11)
#define   CMD_CFG_TIMEOUT_4S		(12 << 12)
#define   CMD_CFG_NO_RESP		BIT(16)
#define   CMD_CFG_NO_CMD		BIT(17)
#define   CMD_CFG_DATA_IO		BIT(18)
#define   CMD_CFG_DATA_WR		BIT(19)
#define   CMD_CFG_RESP_NOCRC		BIT(20)
#define   CMD_CFG_RESP_128		BIT(21)
#define   CMD_CFG_RESP_NUM		BIT(22)
#define   CMD_CFG_DATA_NUM		BIT(23)
#define   CMD_CFG_CMD_INDEX_MASK	GENMASK(29, 24)
#define   CMD_CFG_CMD_INDEX_SHIFT	24
#define   CMD_CFG_ERR			BIT(30)
#define   CMD_CFG_OWNER			BIT(31)

#define MESON_SD_EMMC_CMD_ARG		0x54
#define MESON_SD_EMMC_CMD_DAT		0x58
#define MESON_SD_EMMC_CMD_RSP		0x5c
#define MESON_SD_EMMC_CMD_RSP1		0x60
#define MESON_SD_EMMC_CMD_RSP2		0x64
#define MESON_SD_EMMC_CMD_RSP3		0x68

#define SD_EMMC_RXD_ERROR               (1 << 0)
#define SD_EMMC_TXD_ERROR               (1 << 1)
#define SD_EMMC_DESC_ERROR              (1 << 2)
#define SD_EMMC_RESP_CRC_ERROR          (1 << 3)
#define SD_EMMC_RESP_TIMEOUT_ERROR      (1 << 4)
#define SD_EMMC_DESC_TIMEOUT_ERROR      (1 << 5)

#endif
