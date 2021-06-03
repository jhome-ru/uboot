.. SPDX-License-Identifier: GPL-2.0+

U-Boot for JetHub H1 (J80)
=======================

JetHub H1 is a single board computer (home automation device) manufactured by JetHome with the following
specifications:

 - Amlogic S905W ARM Cortex-A53 quad-core SoC @ 1.5GHz
 - 1GB DDR3 SDRAM
 - 10/100 Ethernet
 - 2 x USB 2.0 Host
 - integrated eMMC 8Gb
 - WiFi/Bluetooth Realtek RTL8822CS
 - MicroSD 2.x/3.x/4.x DS/HS card slot
 - ZigBee TI CC2538 + CC2592

U-Boot compilation
------------------

.. code-block:: bash

    $ export CROSS_COMPILE=aarch64-none-elf-
    $ make gxl_jethubj80_v1_defconfig
    $ make

Image creation
--------------

Amlogic doesn't provide sources for the firmware and for tools needed
to create the bootloader image, so it is necessary to obtain them from
the git tree published by the board vendor:

.. code-block:: bash

    $ git clone https://github.com/jethome-ru/jethub-aml-tools.git
    $ cd jethub-aml-tools
    $ ln -s _path_to_mainline_u-boot_ u-boot
    $ ./mk_new.sh gxl

and then write the image to SD with:

.. code-block:: bash

    $ DEV=/dev/your_sd_device
    $ dd if=build/u-boot.bin.sd.bin of=$DEV conv=fsync,notrunc bs=512 skip=1 seek=1
    $ dd if=build/u-boot.bin.sd.bin of=$DEV conv=fsync,notrunc bs=1 count=444
