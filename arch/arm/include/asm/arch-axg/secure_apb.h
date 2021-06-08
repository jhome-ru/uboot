/* New creation */

#include <asm/arch/regs.h>

#ifdef REGISTER_H
#else
#define REGISTER_H
#endif

#define		P_AO_SEC_SD_CFG8				(volatile uint32_t *)(0xff800000 + (0x088 << 2))
#define		SEC_AO_SEC_GP_CFG0                              (0xff800000 + (0x090 << 2))