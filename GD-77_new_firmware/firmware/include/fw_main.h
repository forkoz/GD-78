/*
 * Copyright (C)2019 Kai Ludwig, DG4KLU
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. The name of the author may not be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FW_MAIN_H_
#define _FW_MAIN_H_

#include <stdint.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "virtual_com.h"
#include "fw_usb_com.h"

#include "fw_common.h"
#include "fw_buttons.h"
#include "fw_LEDs.h"
#include "fw_keyboard.h"
#include "fw_display.h"

#include "UC1701.h"

#include "fw_i2c.h"
#include "fw_spi.h"
#include "fw_i2s.h"
#include "fw_AT1846S.h"
#include "fw_HR-C6000.h"
#include "fw_wdog.h"
#include "fw_adc.h"
#include "fw_pit.h"

#include "fw_sound.h"
#include "fw_trx.h"
#include "fw_SPI_Flash.h"
#include "fw_EEPROM.h"

extern int Display_light_Timer;
extern bool Display_light_Touched;
extern const char *FIRMWARE_VERSION_STRING;

void fw_init();

#endif /* _FW_MAIN_H_ */
