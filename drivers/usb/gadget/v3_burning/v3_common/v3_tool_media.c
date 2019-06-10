/*
 * \file        v3_tool_media.c
 * \brief       operations on media for v3tool
 *
 * \version     1.0.0
 * \date        2018/06/01
 * \author      Sam.Wu <yihui.wu@amlgic.com>
 *
 * Copyright (c) 2018 Amlogic. All Rights Reserved.
 *
 */
#include "../include/v3_tool_def.h"
#include <fdtdec.h>
#define DWN_ERR FB_ERR
#define BOOTLOADER_MAX_SZ   (0x2<<20)
#define DTB_MAX_SZ          (256<<10)
#define DWN_DBG FB_DBG
#define DWN_ERR FB_ERR
#define DWN_MSG FB_MSG
#define DWN_WRN FB_WRN

DECLARE_GLOBAL_DATA_PTR;

//make sure logic part sz == dtb configured sz
static int _assert_logic_partition_cap(const char* thePartName, const uint64_t nandPartCap)
{
    extern struct partitions * part_table;

    int partIndex                   = 0;
    struct partitions * thePart     = NULL;
    if (NULL == part_table)
        return 0;
    for (thePart = part_table; partIndex < MAX_PART_NUM; ++thePart, ++partIndex)
    {
        const uint64_t partSzInBytes = thePart->size;
        if (memcmp(thePartName, thePart->name, strlen(thePartName))) continue;

        FB_DBG("cfg partSzInBytes %llx for part(%s)\n", partSzInBytes, thePartName);
        if (NAND_PART_SIZE_FULL == partSzInBytes) {return 0;}
        if (partSzInBytes > nandPartCap) {
            FB_EXIT("partSz of logic part(%s): sz dts %llx > Sz flash %llx\n",
                    thePartName, partSzInBytes, nandPartCap);
        }

        return 0;
    }

    FB_EXIT("Can't find your download part(%s)\n", thePartName);
}

int v3tool_media_check_image_size(int64_t imgSz, const char* partName)
{
    int ret = 0;
    u64 partCap = 0;

    if (!strcmp("bootloader", partName)) {
        const unsigned bootSz = store_boot_copy_size("bootloader");
        if (imgSz > bootSz)
            FB_EXIT("imgsz 0x%llx > copy sz 0x%x\n", imgSz, bootSz);
        return 0;
    }

    if (!strcmp("_aml_dtb", partName)) {
        const unsigned dtbCap = store_rsv_size("dtb");
        if (imgSz >= dtbCap)
            FB_EXIT("imgsz 0x%llx >= max sz 0x%x\n", imgSz, dtbCap);
        return 0;
    }

    partCap = store_part_size(partName);
    if (!partCap) {
        DWN_ERR("Fail to get size for part %s\n", partName);
        return __LINE__;
    }
    DWN_MSG("flash LOGIC partCap 0x%llxB\n", partCap);
    if (imgSz > partCap) {
        DWN_ERR("imgSz 0x%llx out of cap 0x%llx\n", imgSz, partCap);
        return __LINE__;
    }
    ret = _assert_logic_partition_cap(partName, partCap);
    if (ret) {
        DWN_ERR("Fail in _assert_logic_partition_cap\n");
        return __LINE__;
    }

    return 0;
}

static int _optimusWorkMode = V3TOOL_WORK_MODE_NONE;

int v3tool_work_mode_get(void)
{
    return _optimusWorkMode;
}

int v3tool_work_mode_set(int workmode)
{
    _optimusWorkMode = workmode;
    return 0;
}


static int _disk_intialed_ok = 0;

int v3tool_is_flash_erased(void)
{
    return _disk_intialed_ok>>16;
}

static int should_load_env(void)
{
#ifdef CONFIG_OF_CONTROL
	return fdtdec_get_config_int(gd->fdt_blob, "load-environment", 1);
#elif defined CONFIG_DELAY_ENVIRONMENT
	return 0;
#else
	return 1;
#endif
}

static int initr_env(void)
{
	/* initialize environment */
	if (should_load_env())
		env_relocate();

	return 0;
}

int v3tool_storage_init(const int eraseFlash, unsigned dtbImgSz)
{
    int ret = 0;
    unsigned char* dtbLoadedAddr = (unsigned char*)V3_DTB_LOAD_ADDR;
    int dtb_valid = 0;

    if (V3TOOL_WORK_MODE_USB_PRODUCE != v3tool_work_mode_get()) {//Already inited in other work mode
        /*DWN_MSG("Exit before re-init\n");*/
        /*store_exit();*/
    }

    if (dtbImgSz) {
#ifdef CONFIG_AML_MTD
        if ( BOOT_NAND_MTD == store_get_type() ) {
            extern int check_valid_dts(unsigned char *buffer);
            ret = check_valid_dts(dtbLoadedAddr);
        } else
#endif // #ifdef CONFIG_AML_MTD
        ret = get_partition_from_dts(dtbLoadedAddr);
        if (ret) FBS_EXIT(_ACK, "Failed at check dts\n");
        dtb_valid = 1;
    }

    int initFlag = 0;
    switch (eraseFlash) {
        case 0://NO erase
        case 5://NO erase
            initFlag = 1;
            break;

        case 3://erase all(with key)
            {
                if (store_rsv_protect("key", false))
                    FBS_EXIT(_ACK, "Fail in disprotect key\n");
            }
        case 1://normal erase, store init 3
            initFlag = 3;
            break;

        case 4: {//force erase all
                if (store_rsv_protect(NULL, false))
                    FBS_EXIT(_ACK, "Fail in disprotect all rsv\n");
            }
        case 2:
            initFlag = 4;
            break;

        default:
            FBS_EXIT(_ACK, "Unsupported erase flag %d\n", eraseFlash);
    }

    ret = store_init(initFlag);
    if (ret <= 0)
        FBS_EXIT(_ACK, "Fail in store init %d, ret %d\n", initFlag, ret);

    if (5 == eraseFlash) {//erase key only
        ret = store_rsv_erase("key");
        if (ret) FBS_EXIT(_ACK, "disk_initial 5, Fail in erase key\n");
    }

    if (V3TOOL_WORK_MODE_USB_PRODUCE == v3tool_work_mode_get()) {
        if (!_disk_intialed_ok) {//last disk_initial also okay
            DWN_MSG("usb producing env_relocate\n");
            initr_env();//can only be called once
        }
    }
    _disk_intialed_ok  = 1;
    if (eraseFlash && eraseFlash < 5) _disk_intialed_ok += (1 <<16);

    FB_DBG("dtb_valid %d, dtbImgSz 0x%x\n", dtb_valid, dtbImgSz);
    if (dtb_valid)//for key init, or fail when get /unifykey
    {
        unsigned long fdtAddr = (unsigned long)dtbLoadedAddr;
#ifdef CONFIG_MULTI_DTB
        fdtAddr = get_multi_dt_entry(fdtAddr);
#endif// #ifdef CONFIG_MULTI_DTB
        ret = fdt_check_header((char*)fdtAddr);
        if (ret) FBS_EXIT(_ACK, "Fail in fdt check header\n");

        unsigned fdtsz    = fdt_totalsize((char*)fdtAddr);
        if (fdtAddr != (unsigned long)dtbLoadedAddr)
            memmove((char*)dtbLoadedAddr, (char*)fdtAddr, fdtsz);
    }

    return ret;
}

static int _v3tool_is_busy = 0;
static char* _v3tool_media_busy_info = "";
void v3tool_media_set_busy(const char* busyInfo)
{
    _v3tool_is_busy = 1;
    _v3tool_media_busy_info = fb_response_str - 4;
    strncpy(_v3tool_media_busy_info, "INFO", 4 + 1);//add terminated 0
    if (busyInfo)
        strncpy(_v3tool_media_busy_info + 4, busyInfo, 64);
}
void v3tool_media_set_free(const char* info)
{
    _v3tool_is_busy = 0;
    _v3tool_media_busy_info = fb_response_str - 4;
    strncpy(_v3tool_media_busy_info, "OKAY", 4 + 1);
    if (info)
        strncpy(_v3tool_media_busy_info + 4, info, 64);
}
int v3tool_media_is_busy(void)
{
    return _v3tool_is_busy;
}
