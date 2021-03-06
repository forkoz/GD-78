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

#include "fw_HR-C6000.h"
#include "menu/menuUtilityQSOData.h"

#if defined(USE_SEGGER_RTT)
#include <SeggerRTT/RTT/SEGGER_RTT.h>
#endif

TaskHandle_t fwhrc6000TaskHandle;

volatile bool int_sys;
volatile bool int_ts;
volatile int int_timeout;

int slot_state;
int tick_cnt;
int skip_count;

int last_TG;
int last_DMRID;
int qsodata_timer;

void SPI_HR_C6000_init()
{
    // C6000 interrupts
    PORT_SetPinMux(Port_INT_C6000_RF_RX, Pin_INT_C6000_RF_RX, kPORT_MuxAsGpio);
    PORT_SetPinMux(Port_INT_C6000_RF_TX, Pin_INT_C6000_RF_TX, kPORT_MuxAsGpio);
    PORT_SetPinMux(Port_INT_C6000_SYS, Pin_INT_C6000_SYS, kPORT_MuxAsGpio);
    PORT_SetPinMux(Port_INT_C6000_TS, Pin_INT_C6000_TS, kPORT_MuxAsGpio);
    GPIO_PinInit(GPIO_INT_C6000_RF_RX, Pin_INT_C6000_RF_RX, &pin_config_input);
    GPIO_PinInit(GPIO_INT_C6000_RF_TX, Pin_INT_C6000_RF_TX, &pin_config_input);
    GPIO_PinInit(GPIO_INT_C6000_SYS, Pin_INT_C6000_SYS, &pin_config_input);
    GPIO_PinInit(GPIO_INT_C6000_TS, Pin_INT_C6000_TS, &pin_config_input);

    // Connections with C6000
    PORT_SetPinMux(Port_INT_C6000_RESET, Pin_INT_C6000_RESET, kPORT_MuxAsGpio);
    PORT_SetPinMux(Port_INT_C6000_PWD, Pin_INT_C6000_PWD, kPORT_MuxAsGpio);
    GPIO_PinInit(GPIO_INT_C6000_RESET, Pin_INT_C6000_RESET, &pin_config_output);
    GPIO_PinInit(GPIO_INT_C6000_PWD, Pin_INT_C6000_PWD, &pin_config_output);
    GPIO_PinWrite(GPIO_INT_C6000_RESET, Pin_INT_C6000_RESET, 1);
    GPIO_PinWrite(GPIO_INT_C6000_PWD, Pin_INT_C6000_PWD, 1);

    // Wake up C6000
	vTaskDelay(portTICK_PERIOD_MS * 10);
    GPIO_PinWrite(GPIO_INT_C6000_PWD, Pin_INT_C6000_PWD, 0);
	vTaskDelay(portTICK_PERIOD_MS * 10);

	// GD-77 FW V3.1.1 data from 0x76010 / length 0x06
	uint8_t spi_init_values_1[] = { 0xd5, 0xd7, 0xf7, 0x7f, 0xd7, 0x57 };
	// GD-77 FW V3.1.1 data from 0x75F70 / length 0x20
	uint8_t spi_init_values_2[] = { 0x69, 0x69, 0x96, 0x96, 0x96, 0x99, 0x99, 0x99, 0xa5, 0xa5, 0xaa, 0xaa, 0xcc, 0xcc, 0x00, 0xf0, 0x01, 0xff, 0x01, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x10, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	// GD-77 FW V3.1.1 data from 0x75F90 / length 0x10
	uint8_t spi_init_values_3[] = { 0x00, 0x00, 0x14, 0x1e, 0x1a, 0xff, 0x3d, 0x50, 0x07, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	// GD-77 FW V3.1.1 data from 0x75FA0 / length 0x07
	uint8_t spi_init_values_4[] = { 0x00, 0x03, 0x01, 0x02, 0x05, 0x1e, 0xf0 };
	// GD-77 FW V3.1.1 data from 0x75FA8 / length 0x05
	uint8_t spi_init_values_5[] = { 0x00, 0x00, 0xeb, 0x78, 0x67 };
	// GD-77 FW V3.1.1 data from 0x75FB0 / length 0x60
	uint8_t spi_init_values_6[] = { 0x32, 0xef, 0x00, 0x31, 0xef, 0x00, 0x12, 0xef, 0x00, 0x13, 0xef, 0x00, 0x14, 0xef, 0x00, 0x15, 0xef, 0x00, 0x16, 0xef, 0x00, 0x17, 0xef, 0x00, 0x18, 0xef, 0x00, 0x19, 0xef, 0x00, 0x1a, 0xef, 0x00, 0x1b, 0xef, 0x00, 0x1c, 0xef, 0x00, 0x1d, 0xef, 0x00, 0x1e, 0xef, 0x00, 0x1f, 0xef, 0x00, 0x20, 0xef, 0x00, 0x21, 0xef, 0x00, 0x22, 0xef, 0x00, 0x23, 0xef, 0x00, 0x24, 0xef, 0x00, 0x25, 0xef, 0x00, 0x26, 0xef, 0x00, 0x27, 0xef, 0x00, 0x28, 0xef, 0x00, 0x29, 0xef, 0x00, 0x2a, 0xef, 0x00, 0x2b, 0xef, 0x00, 0x2c, 0xef, 0x00, 0x2d, 0xef, 0x00, 0x2e, 0xef, 0x00, 0x2f, 0xef, 0x00 };

	// --- start spi_init_daten_senden()
	write_SPI_page_reg_byte_SPI0(0x04, 0x0b, 0x40);
	write_SPI_page_reg_byte_SPI0(0x04, 0x0c, 0x32);
	write_SPI_page_reg_byte_SPI0(0x04, 0xb9, 0x05);
	write_SPI_page_reg_byte_SPI0(0x04, 0x0a, 0x01);

	write_SPI_page_reg_bytearray_SPI0(0x01, 0x04, spi_init_values_1, 0x06);
	write_SPI_page_reg_bytearray_SPI0(0x01, 0x10, spi_init_values_2, 0x20);
	write_SPI_page_reg_bytearray_SPI0(0x01, 0x30, spi_init_values_3, 0x10);
	write_SPI_page_reg_bytearray_SPI0(0x01, 0x40, spi_init_values_4, 0x07);
	write_SPI_page_reg_bytearray_SPI0(0x01, 0x51, spi_init_values_5, 0x05);
	write_SPI_page_reg_bytearray_SPI0(0x01, 0x60, spi_init_values_6, 0x60);

	write_SPI_page_reg_byte_SPI0(0x04, 0x00, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x10, 0x6E); // (oder 0x02 oder 0x6A)
	write_SPI_page_reg_byte_SPI0(0x04, 0x11, 0x80);
	write_SPI_page_reg_byte_SPI0(0x04, 0x13, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x1F, 0x10); // (dynamisch 16 * byte from mem)
	write_SPI_page_reg_byte_SPI0(0x04, 0x20, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x21, 0xA0);
	write_SPI_page_reg_byte_SPI0(0x04, 0x22, 0x26);
	write_SPI_page_reg_byte_SPI0(0x04, 0x22, 0x86);
	write_SPI_page_reg_byte_SPI0(0x04, 0x25, 0x0E);
	write_SPI_page_reg_byte_SPI0(0x04, 0x26, 0x7D);
	write_SPI_page_reg_byte_SPI0(0x04, 0x27, 0x40);
	write_SPI_page_reg_byte_SPI0(0x04, 0x28, 0x7D);
	write_SPI_page_reg_byte_SPI0(0x04, 0x29, 0x40);
	write_SPI_page_reg_byte_SPI0(0x04, 0x2A, 0x0B);
	write_SPI_page_reg_byte_SPI0(0x04, 0x2B, 0x0B);
	write_SPI_page_reg_byte_SPI0(0x04, 0x2C, 0x17);
	write_SPI_page_reg_byte_SPI0(0x04, 0x2D, 0x05);
	write_SPI_page_reg_byte_SPI0(0x04, 0x2E, 0x04);
	write_SPI_page_reg_byte_SPI0(0x04, 0x2F, 0x0B);
	write_SPI_page_reg_byte_SPI0(0x04, 0x32, 0x02);
	write_SPI_page_reg_byte_SPI0(0x04, 0x33, 0xFF);
	write_SPI_page_reg_byte_SPI0(0x04, 0x34, 0xF0);
	write_SPI_page_reg_byte_SPI0(0x04, 0x35, 0x28);
	write_SPI_page_reg_byte_SPI0(0x04, 0x3E, 0x28);
	write_SPI_page_reg_byte_SPI0(0x04, 0x3F, 0x10);
	write_SPI_page_reg_byte_SPI0(0x04, 0x36, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x37, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x4B, 0x1B);
	write_SPI_page_reg_byte_SPI0(0x04, 0x4C, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x56, 0x00); // / (dynamisch)
	write_SPI_page_reg_byte_SPI0(0x04, 0x5F, 0xC0); // | (dynamisch)
	write_SPI_page_reg_byte_SPI0(0x04, 0x81, 0xFF); // \ (dynamisch)
	write_SPI_page_reg_byte_SPI0(0x04, 0xD1, 0xC4);

	// --- start subroutine spi_init_daten_senden_sub()
	write_SPI_page_reg_byte_SPI0(0x04, 0x01, 0x70); // (dynamisch)
	write_SPI_page_reg_byte_SPI0(0x04, 0x03, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x05, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x12, 0x15); // (dynamisch)
	write_SPI_page_reg_byte_SPI0(0x04, 0xA1, 0x80); // (dynamisch)
	write_SPI_page_reg_byte_SPI0(0x04, 0xC0, 0x0A);
	write_SPI_page_reg_byte_SPI0(0x04, 0x06, 0x21);
	write_SPI_page_reg_byte_SPI0(0x04, 0x07, 0x0B);
	write_SPI_page_reg_byte_SPI0(0x04, 0x08, 0xB8);
	write_SPI_page_reg_byte_SPI0(0x04, 0x09, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x0D, 0x10);
	write_SPI_page_reg_byte_SPI0(0x04, 0x0E, 0x8E);
	write_SPI_page_reg_byte_SPI0(0x04, 0x0F, 0xB8);
	write_SPI_page_reg_byte_SPI0(0x04, 0xC2, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0xE0, 0x8B);
	write_SPI_page_reg_byte_SPI0(0x04, 0xE1, 0x0F);
	write_SPI_page_reg_byte_SPI0(0x04, 0xE2, 0x06);
	write_SPI_page_reg_byte_SPI0(0x04, 0xE3, 0x52);
	write_SPI_page_reg_byte_SPI0(0x04, 0xE4, 0x4A);
	write_SPI_page_reg_byte_SPI0(0x04, 0xE5, 0x1A);
	// --- end subroutine spi_init_daten_senden_sub()

	write_SPI_page_reg_byte_SPI0(0x04, 0x40, 0xC3); // / (dynamisch)
	write_SPI_page_reg_byte_SPI0(0x04, 0x41, 0x40); // \ (dynamisch)
	// --- end spi_init_daten_senden()

	// ------ start spi_more_init
	// --- start sub_1B5A4
	set_clear_SPI_page_reg_byte_with_mask_SPI0(0x04, 0x06, 0xFD, 0x02); // SET
	// --- end sub_1B5A4

	// --- start sub_1B5DC
	// hard coded 128 * 0xAA
	uint8_t spi_values[128];
	for (int i=0; i<128;i++)
	{
		spi_values[i]=0xaa;
	}
	write_SPI_page_reg_bytearray_SPI0(0x03, 0x00, spi_values, 0x80);
	// --- end sub_1B5DC

	// --- start sub_1B5A4
	set_clear_SPI_page_reg_byte_with_mask_SPI0(0x04, 0x06, 0xFD, 0x00); // CLEAR
	// --- end sub_1B5A4

	write_SPI_page_reg_byte_SPI0(0x04, 0x37, 0x9E); // (dynamisch)
	set_clear_SPI_page_reg_byte_with_mask_SPI0(0x04, 0xE4, 0x3F, 0x00); // CLEAR
	// ------ end spi_more_init
}

void SPI_C6000_postinit1()
{
	write_SPI_page_reg_byte_SPI0(0x04, 0x04, 0xE8);
	write_SPI_page_reg_byte_SPI0(0x04, 0x46, 0x37);
	write_SPI_page_reg_byte_SPI0(0x04, 0x48, 0x03);
	write_SPI_page_reg_byte_SPI0(0x04, 0x47, 0xE8);
}

void SPI_C6000_postinit2()
{
	// GD-77 FW V3.1.1 data from 0x76010 / length 0x06
	uint8_t spi_init_values_1[] = { 0xd5, 0xd7, 0xf7, 0x7f, 0xd7, 0x57 };

	write_SPI_page_reg_byte_SPI0(0x04, 0x41, 0x20);
	write_SPI_page_reg_byte_SPI0(0x04, 0x40, 0x03);
	write_SPI_page_reg_byte_SPI0(0x04, 0x41, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x00, 0x3F);
	write_SPI_page_reg_bytearray_SPI0(0x01, 0x04, spi_init_values_1, 0x06);
	write_SPI_page_reg_byte_SPI0(0x04, 0x10, 0x6E);
	write_SPI_page_reg_byte_SPI0(0x04, 0x1F, 0x10);
	write_SPI_page_reg_byte_SPI0(0x04, 0x26, 0x7D);
	write_SPI_page_reg_byte_SPI0(0x04, 0x27, 0x40);
	write_SPI_page_reg_byte_SPI0(0x04, 0x28, 0x7D);
	write_SPI_page_reg_byte_SPI0(0x04, 0x29, 0x40);
	write_SPI_page_reg_byte_SPI0(0x04, 0x2A, 0x0B);
	write_SPI_page_reg_byte_SPI0(0x04, 0x2B, 0x0B);
	write_SPI_page_reg_byte_SPI0(0x04, 0x2C, 0x17);
	write_SPI_page_reg_byte_SPI0(0x04, 0x2D, 0x05);
	write_SPI_page_reg_byte_SPI0(0x04, 0x56, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x5F, 0xC0);
	write_SPI_page_reg_byte_SPI0(0x04, 0x81, 0xFF);
	write_SPI_page_reg_byte_SPI0(0x04, 0x01, 0x70);
	write_SPI_page_reg_byte_SPI0(0x04, 0x03, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x05, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x12, 0x15);
	write_SPI_page_reg_byte_SPI0(0x04, 0xA1, 0x80);
	write_SPI_page_reg_byte_SPI0(0x04, 0xC0, 0x0A);
	write_SPI_page_reg_byte_SPI0(0x04, 0x06, 0x21);
	write_SPI_page_reg_byte_SPI0(0x04, 0x07, 0x0B);
	write_SPI_page_reg_byte_SPI0(0x04, 0x08, 0xB8);
	write_SPI_page_reg_byte_SPI0(0x04, 0x09, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x0D, 0x10);
	write_SPI_page_reg_byte_SPI0(0x04, 0x0E, 0x8E);
	write_SPI_page_reg_byte_SPI0(0x04, 0x0F, 0xB8);
	write_SPI_page_reg_byte_SPI0(0x04, 0xC2, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0xE0, 0x8B);
	write_SPI_page_reg_byte_SPI0(0x04, 0xE1, 0x0F);
	write_SPI_page_reg_byte_SPI0(0x04, 0xE2, 0x06);
	write_SPI_page_reg_byte_SPI0(0x04, 0xE3, 0x52);
	write_SPI_page_reg_byte_SPI0(0x04, 0xE4, 0x4A);
	write_SPI_page_reg_byte_SPI0(0x04, 0xE5, 0x1A);
	write_SPI_page_reg_byte_SPI0(0x04, 0x26, 0x7D);
	write_SPI_page_reg_byte_SPI0(0x04, 0x27, 0x40);
	write_SPI_page_reg_byte_SPI0(0x04, 0x28, 0x7D);
	write_SPI_page_reg_byte_SPI0(0x04, 0x29, 0x40);
	write_SPI_page_reg_byte_SPI0(0x04, 0x41, 0x20);
	write_SPI_page_reg_byte_SPI0(0x04, 0x40, 0xC3);
	write_SPI_page_reg_byte_SPI0(0x04, 0x41, 0x40);
}

void SPI_C6000_postinit3a()
{
	/* sub_328D0
	    spi_TXRX_2(0x84, 1, (int)&v7, a4);
	    if ( sub_21E90() )
	      spi_TX(4, 1, v7 | 0x40);
	    else
	      spi_TX(4, 1, v7 & 0xBF);
	*/

	write_SPI_page_reg_byte_SPI0(0x04, 0x01, 0x70);
}

void SPI_C6000_postinit3b()
{
	/* sub_328D0 (conditional)
	  spi_TXRX_2(0x84, 0x10, (int)&v7, v6);
	  spi_TX(4, 0x10, v7 & 0x7F);
	  spi_TX(4, 0, 0x3F);
	  spi_TX(4, 0xE4, 0x4B);
	*/

	write_SPI_page_reg_byte_SPI0(0x04, 0x10, 0x6E);
	write_SPI_page_reg_byte_SPI0(0x04, 0x00, 0x3F);
	write_SPI_page_reg_byte_SPI0(0x04, 0xE4, 0x4B);
}

void PORTC_IRQHandler(void)
{
    if ((1U << Pin_INT_C6000_SYS) & PORT_GetPinsInterruptFlags(Port_INT_C6000_SYS))
    {
    	int_sys=true;
        PORT_ClearPinsInterruptFlags(Port_INT_C6000_SYS, (1U << Pin_INT_C6000_SYS));
    }
    if ((1U << Pin_INT_C6000_TS) & PORT_GetPinsInterruptFlags(Port_INT_C6000_TS))
    {
    	int_ts=true;
        PORT_ClearPinsInterruptFlags(Port_INT_C6000_TS, (1U << Pin_INT_C6000_TS));
    }

    int_timeout=0;

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
    exception return operation might vector to incorrect interrupt */
    __DSB();
}

void init_HR_C6000_interrupts()
{
	init_digital_state();

    PORT_SetPinInterruptConfig(Port_INT_C6000_SYS, Pin_INT_C6000_SYS, kPORT_InterruptEitherEdge);
    PORT_SetPinInterruptConfig(Port_INT_C6000_TS, Pin_INT_C6000_TS, kPORT_InterruptEitherEdge);

    NVIC_SetPriority(PORTC_IRQn, 3);
}

void init_digital_state()
{
	taskENTER_CRITICAL();
	int_sys=false;
	int_ts=false;
	int_timeout=0;
	taskEXIT_CRITICAL();
	slot_state=0;
	tick_cnt=0;
	skip_count=0;
	last_TG = 0;
	last_DMRID = 0;
	qsodata_timer = 0;
}

void init_digital_DMR_RX()
{
	write_SPI_page_reg_byte_SPI0(0x04, 0x41, 0x20);
	write_SPI_page_reg_byte_SPI0(0x04, 0x41, 0x00);
	write_SPI_page_reg_byte_SPI0(0x04, 0x41, 0x20);
	write_SPI_page_reg_byte_SPI0(0x04, 0x41, 0x40);
}

void init_digital()
{
	GPIO_PinWrite(GPIO_speaker_mute, Pin_speaker_mute, 0);
    GPIO_PinWrite(GPIO_LEDgreen, Pin_LEDgreen, 0);
	init_digital_DMR_RX();
	init_digital_state();
    NVIC_EnableIRQ(PORTC_IRQn);
	init_codec();
}

void terminate_digital()
{
	GPIO_PinWrite(GPIO_speaker_mute, Pin_speaker_mute, 0);
    GPIO_PinWrite(GPIO_LEDgreen, Pin_LEDgreen, 0);
	init_digital_state();
    NVIC_DisableIRQ(PORTC_IRQn);
}

void store_qsodata()
{
	int tmp_last_TG=(tmp_ram[3]<<16)+(tmp_ram[4]<<8)+(tmp_ram[5]<<0);
	int tmp_last_DMRID=(tmp_ram[6]<<16)+(tmp_ram[7]<<8)+(tmp_ram[8]<<0);
	qsodata_timer=2400;
	if ((tmp_last_TG!=last_TG) || (tmp_last_DMRID!=last_DMRID))
	{

#warning HACK ALERT. store qsodata seems to report DMR ID of 0x4ED1E7 when it miss interprets a late start

		//I suspect the C6000 is returning an error code or something similar in these cases in place of the DMR ID,
		// because the value is always the same no matter what the incoming ID actually is
		if (tmp_last_DMRID!=0x4ED1E7)
		{

			last_TG=tmp_last_TG;
			last_DMRID=tmp_last_DMRID;
			lastHeardListUpdate(last_DMRID,last_TG);
		}
	}
}

void init_hrc6000_task()
{
	xTaskCreate(fw_hrc6000_task,                        /* pointer to the task */
				"fw hrc6000 task",                      /* task name for kernel awareness debugging */
				5000L / sizeof(portSTACK_TYPE),      /* task stack size */
				NULL,                      			 /* optional task startup argument */
				5U,                                  /* initial priority */
				fwhrc6000TaskHandle					 /* optional task handle to create */
				);
}

void fw_hrc6000_task()
{
    while (1U)
    {
    	taskENTER_CRITICAL();
    	uint32_t tmp_timer_hrc6000task=timer_hrc6000task;
    	taskEXIT_CRITICAL();
    	if (tmp_timer_hrc6000task==0)
    	{
        	taskENTER_CRITICAL();
        	timer_hrc6000task=10;
        	alive_hrc6000task=true;
        	taskEXIT_CRITICAL();

    		if (melody_play==NULL)
    		{
    	    	if (trxGetMode() == RADIO_MODE_DIGITAL)
    	    	{
    				tick_HR_C6000();
    	    	}
    	    	else if (trxGetMode() == RADIO_MODE_ANALOG)
    	    	{
    	    		trx_check_analog_squelch();
    	    	}
    		}
    	}

		vTaskDelay(0);
    }
}

void tick_HR_C6000()
{
	bool tmp_int_sys=false;
	bool tmp_int_ts=false;
	taskENTER_CRITICAL();
	if (int_sys)
	{
		tmp_int_sys=true;
		int_sys=false;
	}
	if (int_ts)
	{
		tmp_int_ts=true;
		int_ts=false;
	}
	taskEXIT_CRITICAL();

	if (slot_state>0)
	{
		if (int_timeout<200)
		{
			int_timeout++;
			if (int_timeout==200)
			{
	            	init_digital();
	            	slot_state=0;
	            	int_timeout=0;
#if defined(USE_SEGGER_RTT)
            	SEGGER_RTT_printf(0, ">>> INTERRUPT TIMEOUT\r\n");
#endif
			}
		}
	}
	else
	{
		int_timeout=0;
	}

	if (tmp_int_ts)
	{
		// Transmission start/stop state machine
		switch (slot_state)
		{
		case 1: // Start RX of transmission (first step)
			write_SPI_page_reg_byte_SPI0(0x04, 0x41, 0x00);
			GPIO_PinWrite(GPIO_speaker_mute, Pin_speaker_mute, 1);
		    GPIO_PinWrite(GPIO_LEDgreen, Pin_LEDgreen, 1);
			slot_state=2;
			break;
		case 2: // Start RX of transmission (second step)
			write_SPI_page_reg_byte_SPI0(0x04, 0x41, 0x50);
			slot_state=1;
			break;
		case 3: // Stop RX of transmission
			init_digital_DMR_RX();
			GPIO_PinWrite(GPIO_speaker_mute, Pin_speaker_mute, 0);
		    GPIO_PinWrite(GPIO_LEDgreen, Pin_LEDgreen, 0);
			slot_state=0;
			break;
		}

    	if (tick_cnt<10)
    	{
    		// Timeout interrupted transmission
    		tick_cnt++;
            if (tick_cnt==10)
            {
            	slot_state=3;
#if defined(USE_SEGGER_RTT)
            	SEGGER_RTT_printf(0, ">>> TIMEOUT\r\n");
#endif
            }
    	}
	}

	if (tmp_int_sys)
	{
		read_SPI_page_reg_byte_SPI0(0x04, 0x82, &tmp_val_0x82);
		read_SPI_page_reg_byte_SPI0(0x04, 0x86, &tmp_val_0x86);
		read_SPI_page_reg_byte_SPI0(0x04, 0x51, &tmp_val_0x51);
		read_SPI_page_reg_byte_SPI0(0x04, 0x52, &tmp_val_0x52);
		read_SPI_page_reg_byte_SPI0(0x04, 0x57, &tmp_val_0x57);
		read_SPI_page_reg_byte_SPI0(0x04, 0x5f, &tmp_val_0x5f);
		read_SPI_page_reg_bytearray_SPI0(0x02, 0x00, tmp_ram, 0x0c);

		// Check for correct received packet
		int rcrc = (tmp_val_0x51 >> 2) & 0x01;
		int rpi = (tmp_val_0x51 >> 3) & 0x01;
		int cc = (tmp_val_0x52 >> 4) & 0x0f;
        if ((rcrc==0) && (rpi==0) && (cc == 1))
        {
    		if (tmp_val_0x82 & 0x20) // InterSendStop
    		{
    			if (tmp_val_0x86 & 0x10)
    			{
    				send_packet(0x20, 0x10, -1);
    			}
    			if (tmp_val_0x86 & 0x04)
    			{
    				send_packet(0x20, 0x04, -1);
    			}

    			write_SPI_page_reg_byte_SPI0(0x04, 0x83, 0x20);
    		}

    		if (tmp_val_0x82 & 0x10) // InterLateEntry
    		{
    			// Late entry into ongoing transmission
                if ((slot_state==0) && (tmp_ram[0]==0))
                {
                	slot_state=1;
                	store_qsodata();
                	init_codec();
                	skip_count = 2;
#if defined(USE_SEGGER_RTT)
            	SEGGER_RTT_printf(0, ">>> START LATE\r\n");
#endif
                }

#if defined(USE_SEGGER_RTT)
            	SEGGER_RTT_printf(0, "LATE %02x [%02x %02x] %02x %02x %02x %02x SC:%02x RCRC:%02x RPI:%02x RXDT:%02x LCSS:%02x TC:%02x AT:%02x CC:%02x ??:%02x ST:%02x RAM:", slot_state, tmp_val_0x82, tmp_val_0x86, tmp_val_0x51, tmp_val_0x52, tmp_val_0x57, tmp_val_0x5f, (tmp_val_0x51 >> 0) & 0x03, (tmp_val_0x51 >> 2) & 0x01, (tmp_val_0x51 >> 3) & 0x01, (tmp_val_0x51 >> 4) & 0x0f, (tmp_val_0x52 >> 0) & 0x03, (tmp_val_0x52 >> 2) & 0x01, (tmp_val_0x52 >> 3) & 0x01, (tmp_val_0x52 >> 4) & 0x0f, (tmp_val_0x57 >> 2) & 0x01, (tmp_val_0x5f >> 0) & 0x03);
				for (int i=0;i<0x0c;i++)
				{
	            	SEGGER_RTT_printf(0, " %02x", tmp_ram[i]);
				}
            	SEGGER_RTT_printf(0, "\r\n");
#endif

    			send_packet(0x10, 0x00, -1);

    			write_SPI_page_reg_byte_SPI0(0x04, 0x83, 0x10);
    		}

    		if (tmp_val_0x82 & 0x08) // InterRecvData
    		{
    			// Reset transmission timeout
    			tick_cnt = 0;

    			// Start or stop transmission
    			int rxdt = (tmp_val_0x51 >> 4) & 0x0f;
    			int sc = (tmp_val_0x51 >> 0) & 0x03;
                if ((slot_state==0) && (sc==2) && (rxdt==1) && (tmp_ram[0]==0))
                {
                	slot_state=1;
                	store_qsodata();
                	init_codec();
                	skip_count = 0;
#if defined(USE_SEGGER_RTT)
            	SEGGER_RTT_printf(0, ">>> START\r\n");
#endif
                }
                if ((sc==2) && (rxdt==2) && (tmp_ram[0]==0))
                {
                	slot_state=3;
#if defined(USE_SEGGER_RTT)
            	SEGGER_RTT_printf(0, ">>> STOP\r\n");
#endif
                }

            	if ((slot_state!=0) && (skip_count>0) && (sc!=2) && ((rxdt & 0x07) == 0x01))
            	{
            		skip_count--;
            	}

                // Detect/decode voice packet and transfer it into the output soundbuffer
                if ((slot_state!=0) && (skip_count==0) && (sc!=2) && ((rxdt & 0x07) >= 0x01) && ((rxdt & 0x07) <= 0x06))
                {
                	store_qsodata();
                    read_SPI_page_reg_bytearray_SPI1(0x03, 0x00, tmp_ram, 27);
                	tick_codec(tmp_ram);
                    tick_soundbuffer();
                }

#if defined(USE_SEGGER_RTT)
            	SEGGER_RTT_printf(0, "DATA %02x [%02x %02x] %02x %02x %02x %02x SC:%02x RCRC:%02x RPI:%02x RXDT:%02x LCSS:%02x TC:%02x AT:%02x CC:%02x ??:%02x ST:%02x RAM:", slot_state, tmp_val_0x82, tmp_val_0x86, tmp_val_0x51, tmp_val_0x52, tmp_val_0x57, tmp_val_0x5f, (tmp_val_0x51 >> 0) & 0x03, (tmp_val_0x51 >> 2) & 0x01, (tmp_val_0x51 >> 3) & 0x01, (tmp_val_0x51 >> 4) & 0x0f, (tmp_val_0x52 >> 0) & 0x03, (tmp_val_0x52 >> 2) & 0x01, (tmp_val_0x52 >> 3) & 0x01, (tmp_val_0x52 >> 4) & 0x0f, (tmp_val_0x57 >> 2) & 0x01, (tmp_val_0x5f >> 0) & 0x03);
				for (int i=0;i<0x0c;i++)
				{
	            	SEGGER_RTT_printf(0, " %02x", tmp_ram[i]);
				}
            	SEGGER_RTT_printf(0, "\r\n");
#endif

                send_packet(0x08, 0x00, -1);

    			write_SPI_page_reg_byte_SPI0(0x04, 0x83, 0x08);
    		}

    		if (tmp_val_0x82 & 0x01) // InterPHYOnly
    		{
    			send_packet(0x01, 0x00, -1);

    			write_SPI_page_reg_byte_SPI0(0x04, 0x83, 0x01);
    		}

    		if (tmp_val_0x82 & 0xC6)
    		{
    			send_packet(0xFF, 0xFF, -1);

    			write_SPI_page_reg_byte_SPI0(0x04, 0x83, 0xC6);
    		}
        }
        else
        {
#if defined(USE_SEGGER_RTT)
            	SEGGER_RTT_printf(0, "---- %02x [%02x %02x] %02x %02x %02x %02x SC:%02x RCRC:%02x RPI:%02x RXDT:%02x LCSS:%02x TC:%02x AT:%02x CC:%02x ??:%02x ST:%02x RAM:", slot_state, tmp_val_0x82, tmp_val_0x86, tmp_val_0x51, tmp_val_0x52, tmp_val_0x57, tmp_val_0x5f, (tmp_val_0x51 >> 0) & 0x03, (tmp_val_0x51 >> 2) & 0x01, (tmp_val_0x51 >> 3) & 0x01, (tmp_val_0x51 >> 4) & 0x0f, (tmp_val_0x52 >> 0) & 0x03, (tmp_val_0x52 >> 2) & 0x01, (tmp_val_0x52 >> 3) & 0x01, (tmp_val_0x52 >> 4) & 0x0f, (tmp_val_0x57 >> 2) & 0x01, (tmp_val_0x5f >> 0) & 0x03);
				for (int i=0;i<0x0c;i++)
				{
	            	SEGGER_RTT_printf(0, " %02x", tmp_ram[i]);
				}
            	SEGGER_RTT_printf(0, "\r\n");
#endif
        }
	}

	if (qsodata_timer>0)
	{
		qsodata_timer--;
		if (qsodata_timer==0)
		{
			last_TG = 0;
			last_DMRID = 0;
		}
	}
}
