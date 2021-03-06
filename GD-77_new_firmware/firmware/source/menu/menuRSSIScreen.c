/*
 * Copyright (C)2019 Roger Clark. VK3KYY / G4KYF
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
#include "menu/menuSystem.h"
#include "fw_i2c.h"


static void updateScreen();
static void handleEvent(int buttons, int keys, int events);
static void sampleRSSIAndNoise();

static const int NUM_SAMPLES = 256;

int menuRSSIScreen(int buttons, int keys, int events, bool isFirstRun)
{
	if (isFirstRun)
	{
		gMenusCurrentItemIndex=0;
		gMenusStartIndex=0;
		gMenusEndIndex=0;
		//updateScreen(); // Don't draw screen until we've collected enough samples
	}
	else
	{
		if (events!=0 && keys!=0)
		{
			handleEvent(buttons, keys, events);
		}
		if (gMenusCurrentItemIndex++ < NUM_SAMPLES)
		{
			sampleRSSIAndNoise();
		}
		if (gMenusCurrentItemIndex >= NUM_SAMPLES)
		{
			updateScreen();
			gMenusCurrentItemIndex=0;
			gMenusStartIndex=0;
			gMenusEndIndex=0;
		}
	}
	return 0;
}


static void updateScreen()
{

	char buffer[17];

	    gMenusStartIndex /= NUM_SAMPLES;
	    gMenusEndIndex /= NUM_SAMPLES;

		UC1701_clearBuf();
		UC1701_printCentered(0, "RSSI",UC1701_FONT_GD77_8x16);

		sprintf(buffer,"Noise %d", gMenusStartIndex);
		UC1701_printAt(10,16, buffer,UC1701_FONT_GD77_8x16);
		gMenusStartIndex *= 2;
		if (gMenusStartIndex>128)
		{
			gMenusStartIndex=128;
		}
		UC1701_fillRect(0, 32,gMenusStartIndex,8,false);

		sprintf(buffer,"RSSI %d", gMenusEndIndex);
		UC1701_printAt(10,40, buffer,UC1701_FONT_GD77_8x16);

		gMenusEndIndex *= 1.8;
		if (gMenusEndIndex>128)
		{
			gMenusEndIndex=128;
		}
		UC1701_fillRect(0, 56,gMenusEndIndex,8,false);

		UC1701_render();
		displayLightTrigger();

}


static void handleEvent(int buttons, int keys, int events)
{
	if ((keys & KEY_RED)!=0)
	{
		menuSystemPopPreviousMenu();
		return;
	}
}

static void sampleRSSIAndNoise()
{
    uint8_t RX_signal;
    uint8_t RX_noise;

    read_I2C_reg_2byte(I2C_MASTER_SLAVE_ADDR_7BIT, 0x1b, &RX_signal, &RX_noise);
    gMenusStartIndex += RX_noise;
    gMenusEndIndex += RX_signal;
}
