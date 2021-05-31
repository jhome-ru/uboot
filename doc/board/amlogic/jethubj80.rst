.. SPDX-License-Identifier: GPL-2.0+

U-Boot for JetHub J80
=======================

JetHub J80 is a single board computer (home automation device) manufactured by JetHome with the following
specifications:

 - Amlogic S905X ARM Cortex-A53 quad-core SoC @ 1.5GHz
 - ARM Mali 450 GPU
 - 2GB DDR3 SDRAM
 - 10/100 Ethernet
 - HDMI 2.0 4K/60Hz display
 - 2 x USB 2.0 Host
 - eMMC, microSD
 - Infrared receiver
 - SDIO WiFi Module
 - CVBS+Stereo Audio Jack

Schematics are available from Amlogic on demand.

More information [JetHub H1 wiki](http://wiki.jethome.ru/jethub_h1) (Russian)

- 4-core CPU Amlogic S905W (ARM Cortex-A53) 1,5 Ghz;
- RAM 1024 MB DDR3;
- eMMC flash 8 Gbyte.

## Interfaces

- WiFi/Bluetooth Realtek RTL8822CS
- MicroSD 2.x/3.x/4.x DS/HS card slot
- Ethernet IEEE 802.3 10/100 Mbps
- ZigBee TI CC2538 + CC2592
- 2 x USB 2.0 high-speed

## MMC

MMC support is provided transparently. Just flash the image to the MMC board by http://update.jethome.ru/armbian/USB_Burning_Tool_v2.2.3.zip

## Console

By default, console access is granted over the serial header. Certain startup messages will only appear on the serial console by default. 

## USB

A long-standing kernel bug currently results in some odd behavior. To use the USB, a device must be plugged into one of the USB ports at hard boot. If all devices are removed from the USB ports, the USB will cease to function until a reboot.

### OTG

The OTG USB is untested.

## GPIO

Work in progress

## MMC




U-Boot compilation
------------------

.. code-block:: bash

    $ export CROSS_COMPILE=aarch64-none-elf-
    $ make p212_defconfig
    $ make

Image creation
--------------

Amlogic doesn't provide sources for the firmware and for tools needed
to create the bootloader image, so it is necessary to obtain them from
the git tree published by the board vendor:

.. code-block:: bash

    $ wget https://releases.linaro.org/archive/13.11/components/toolchain/binaries/gcc-linaro-aarch64-none-elf-4.8-2013.11_linux.tar.xz
    $ wget https://releases.linaro.org/archive/13.11/components/toolchain/binaries/gcc-linaro-arm-none-eabi-4.8-2013.11_linux.tar.xz
    $ tar xvfJ gcc-linaro-aarch64-none-elf-4.8-2013.11_linux.tar.xz
    $ tar xvfJ gcc-linaro-arm-none-eabi-4.8-2013.11_linux.tar.xz
    $ export PATH=$PWD/gcc-linaro-aarch64-none-elf-4.8-2013.11_linux/bin:$PWD/gcc-linaro-arm-none-eabi-4.8-2013.11_linux/bin:$PATH
    $ git clone https://github.com/BayLibre/u-boot.git -b n-amlogic-openlinux-20170606 amlogic-u-boot
    $ cd amlogic-u-boot
    $ make gxl_p212_v1_defconfig
    $ make
    $ export FIPDIR=$PWD/fip

Go back to mainline U-boot source tree then :

.. code-block:: bash

    $ mkdir fip

    $ cp $FIPDIR/gxl/bl2.bin fip/
    $ cp $FIPDIR/gxl/acs.bin fip/
    $ cp $FIPDIR/gxl/bl21.bin fip/
    $ cp $FIPDIR/gxl/bl30.bin fip/
    $ cp $FIPDIR/gxl/bl301.bin fip/
    $ cp $FIPDIR/gxl/bl31.img fip/
    $ cp u-boot.bin fip/bl33.bin

    $ $FIPDIR/blx_fix.sh \
    	fip/bl30.bin \
    	fip/zero_tmp \
    	fip/bl30_zero.bin \
    	fip/bl301.bin \
    	fip/bl301_zero.bin \
    	fip/bl30_new.bin \
    	bl30

    $ $FIPDIR/acs_tool.pyc fip/bl2.bin fip/bl2_acs.bin fip/acs.bin 0

    $ $FIPDIR/blx_fix.sh \
    	fip/bl2_acs.bin \
    	fip/zero_tmp \
    	fip/bl2_zero.bin \
    	fip/bl21.bin \
    	fip/bl21_zero.bin \
    	fip/bl2_new.bin \
    	bl2

    $ $FIPDIR/gxl/aml_encrypt_gxl --bl3enc --input fip/bl30_new.bin
    $ $FIPDIR/gxl/aml_encrypt_gxl --bl3enc --input fip/bl31.img
    $ $FIPDIR/gxl/aml_encrypt_gxl --bl3enc --input fip/bl33.bin
    $ $FIPDIR/gxl/aml_encrypt_gxl --bl2sig --input fip/bl2_new.bin --output fip/bl2.n.bin.sig
    $ $FIPDIR/gxl/aml_encrypt_gxl --bootmk \
    		--output fip/u-boot.bin \
    		--bl2 fip/bl2.n.bin.sig \
    		--bl30 fip/bl30_new.bin.enc \
    		--bl31 fip/bl31.img.enc \
    		--bl33 fip/bl33.bin.enc

and then write the image to SD with:

.. code-block:: bash

    $ DEV=/dev/your_sd_device
    $ dd if=fip/u-boot.bin.sd.bin of=$DEV conv=fsync,notrunc bs=512 skip=1 seek=1
    $ dd if=fip/u-boot.bin.sd.bin of=$DEV conv=fsync,notrunc bs=1 count=444
