/* New creation */

#ifdef CONFIG_MESON_AXG

#include <asm/arch/clock-axg.h>

#else
#ifdef CONFIG_MESON_G12A

#include <asm/arch/clock-g12a.h>

#else 

#include <asm/arch/clock-gx.h>

#endif
#endif
