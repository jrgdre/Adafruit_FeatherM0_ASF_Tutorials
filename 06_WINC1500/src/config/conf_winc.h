/**
 *
 * \file
 *
 * \brief WINC1500 configuration.
 *
 * Copyright (c) 2016-2017 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef CONF_WINC_H_INCLUDED
#define CONF_WINC_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "board.h"

/* Adafruit FeatherM0 WiFi settings */

/*
   ---------------------------------
   ---------- PIN settings ---------
   ---------------------------------
*/

#define CONF_WINC_PIN_RESET				PIN_PA08
#define CONF_WINC_PIN_CHIP_ENABLE		PIN_PA14
#define CONF_WINC_PIN_WAKE				0

/*
   ---------------------------------
   ---------- SPI settings ---------
   ---------------------------------
*/

#define CONF_WINC_USE_SPI				(1)

/** SPI pin and instance settings. */
#define CONF_WINC_SPI_MODULE			SERCOM4
#define CONF_WINC_SPI_SERCOM_MUX		SPI_SIGNAL_MUX_SETTING_E  /* PAD[0]:MISO, PAD[1]:unused; PAD[2]:SCK, PAD[2]:MOSI, PAD[3]:SCK */
#define CONF_WINC_SPI_PINMUX_PAD0		PINMUX_PA12D_SERCOM4_PAD0 /* MISO */
#define CONF_WINC_SPI_PINMUX_PAD1		PINMUX_UNUSED 
#define CONF_WINC_SPI_PINMUX_PAD2		PINMUX_PB10D_SERCOM4_PAD2 /* MOSI */
#define CONF_WINC_SPI_PINMUX_PAD3		PINMUX_PB11D_SERCOM4_PAD3 /* SCK  */
#define CONF_WINC_SPI_CS_PIN			PIN_PA06

#define CONF_WINC_SPI_MISO				PIN_PA12
#define CONF_WINC_SPI_MOSI				PIN_PB10
#define CONF_WINC_SPI_SCK				PIN_PB11
#define CONF_WINC_SPI_SS				PIN_PA06

/** SPI interrupt pin. */
#define CONF_WINC_SPI_INT_PIN			PIN_PA21A_EIC_EXTINT5
#define CONF_WINC_SPI_INT_MUX			MUX_PA21A_EIC_EXTINT5
#define CONF_WINC_SPI_INT_EIC			(5)

/** SPI clock. */
#define CONF_WINC_SPI_CLOCK				(4000000) /* max. external_clock / 2 */

/*
   ---------------------------------
   --------- Debug Options ---------
   ---------------------------------
*/

#define CONF_WINC_DEBUG					(1)
#define CONF_WINC_PRINTF				printf

#ifdef __cplusplus
}
#endif

#endif /* CONF_WINC_H_INCLUDED */
