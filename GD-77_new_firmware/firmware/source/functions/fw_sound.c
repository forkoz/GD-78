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

#include "fw_sound.h"

TaskHandle_t fwBeepTaskHandle;

const uint8_t sine_beep[] = { 0x00, 0x00, 0x19, 0x00, 0x32, 0x00, 0x4B, 0x00, 0x65, 0x00, 0x7E, 0x00, 0x97, 0x00, 0xB0, 0x00, 0xC9, 0x00, 0xE2, 0x00, 0xFB, 0x00, 0x14, 0x01, 0x2D, 0x01, 0x47, 0x01, 0x60, 0x01, 0x79, 0x01, 0x92, 0x01, 0xAB, 0x01, 0xC4, 0x01, 0xDD, 0x01, 0xF6, 0x01, 0x0F, 0x02, 0x28, 0x02, 0x42, 0x02, 0x5B, 0x02, 0x74, 0x02, 0x8D, 0x02, 0xA6, 0x02, 0xBF, 0x02, 0xD8, 0x02, 0xF1, 0x02, 0x0A, 0x03, 0x23, 0x03, 0x3C, 0x03, 0x55, 0x03, 0x6E, 0x03, 0x87, 0x03, 0xA0, 0x03, 0xB9, 0x03, 0xD2, 0x03, 0xEB, 0x03, 0x04, 0x04, 0x1D, 0x04, 0x35, 0x04, 0x4E, 0x04, 0x67, 0x04, 0x80, 0x04, 0x99, 0x04, 0xB2, 0x04, 0xCB, 0x04, 0xE4, 0x04, 0xFC, 0x04, 0x15, 0x05, 0x2E, 0x05, 0x47, 0x05, 0x60, 0x05, 0x78, 0x05, 0x91, 0x05, 0xAA, 0x05, 0xC3, 0x05, 0xDB, 0x05, 0xF4, 0x05, 0x0D, 0x06, 0x25, 0x06, 0x3E, 0x06, 0x57, 0x06, 0x6F, 0x06, 0x88, 0x06, 0xA0, 0x06, 0xB9, 0x06, 0xD2, 0x06, 0xEA, 0x06, 0x03, 0x07, 0x1B, 0x07, 0x34, 0x07, 0x4C, 0x07, 0x65, 0x07, 0x7D, 0x07, 0x95, 0x07, 0xAE, 0x07, 0xC6, 0x07, 0xDF, 0x07, 0xF7, 0x07, 0x0F, 0x08, 0x28, 0x08, 0x40, 0x08, 0x58, 0x08, 0x70, 0x08, 0x89, 0x08, 0xA1, 0x08, 0xB9, 0x08, 0xD1, 0x08, 0xE9, 0x08, 0x01, 0x09, 0x1A, 0x09, 0x32, 0x09, 0x4A, 0x09, 0x62, 0x09, 0x7A, 0x09, 0x92, 0x09, 0xAA, 0x09, 0xC2, 0x09, 0xDA, 0x09, 0xF1, 0x09, 0x09, 0x0A, 0x21, 0x0A, 0x39, 0x0A, 0x51, 0x0A, 0x69, 0x0A, 0x80, 0x0A, 0x98, 0x0A, 0xB0, 0x0A, 0xC7, 0x0A, 0xDF, 0x0A, 0xF7, 0x0A, 0x0E, 0x0B, 0x26, 0x0B, 0x3D, 0x0B, 0x55, 0x0B, 0x6C, 0x0B, 0x84, 0x0B, 0x9B, 0x0B, 0xB3, 0x0B, 0xCA, 0x0B, 0xE1, 0x0B, 0xF9, 0x0B, 0x10, 0x0C, 0x27, 0x0C, 0x3F, 0x0C, 0x56, 0x0C, 0x6D, 0x0C, 0x84, 0x0C, 0x9B, 0x0C, 0xB2, 0x0C, 0xC9, 0x0C, 0xE0, 0x0C, 0xF7, 0x0C, 0x0E, 0x0D, 0x25, 0x0D, 0x3C, 0x0D, 0x53, 0x0D, 0x6A, 0x0D, 0x81, 0x0D, 0x97, 0x0D, 0xAE, 0x0D, 0xC5, 0x0D, 0xDB, 0x0D, 0xF2, 0x0D, 0x09, 0x0E, 0x1F, 0x0E, 0x36, 0x0E, 0x4C, 0x0E, 0x63, 0x0E, 0x79, 0x0E, 0x90, 0x0E, 0xA6, 0x0E, 0xBC, 0x0E, 0xD3, 0x0E, 0xE9, 0x0E, 0xFF, 0x0E, 0x15, 0x0F, 0x2B, 0x0F, 0x41, 0x0F, 0x58, 0x0F, 0x6E, 0x0F, 0x84, 0x0F, 0x9A, 0x0F, 0xAF, 0x0F, 0xC5, 0x0F, 0xDB, 0x0F, 0xF1, 0x0F, 0x07, 0x10, 0x1C, 0x10, 0x32, 0x10, 0x48, 0x10, 0x5D, 0x10, 0x73, 0x10, 0x89, 0x10, 0x9E, 0x10, 0xB3, 0x10, 0xC9, 0x10, 0xDE, 0x10, 0xF4, 0x10, 0x09, 0x11, 0x1E, 0x11, 0x33, 0x11, 0x49, 0x11, 0x5E, 0x11, 0x73, 0x11, 0x88, 0x11, 0x9D, 0x11, 0xB2, 0x11, 0xC7, 0x11, 0xDC, 0x11, 0xF0, 0x11, 0x05, 0x12, 0x1A, 0x12, 0x2F, 0x12, 0x43, 0x12, 0x58, 0x12, 0x6C, 0x12, 0x81, 0x12, 0x95, 0x12, 0xAA, 0x12, 0xBE, 0x12, 0xD3, 0x12, 0xE7, 0x12, 0xFB, 0x12, 0x0F, 0x13, 0x24, 0x13, 0x38, 0x13, 0x4C, 0x13, 0x60, 0x13, 0x74, 0x13, 0x88, 0x13, 0x9C, 0x13, 0xAF, 0x13, 0xC3, 0x13, 0xD7, 0x13, 0xEB, 0x13, 0xFE, 0x13, 0x12, 0x14, 0x25, 0x14, 0x39, 0x14, 0x4C, 0x14, 0x60, 0x14, 0x73, 0x14, 0x86, 0x14, 0x9A, 0x14, 0xAD, 0x14, 0xC0, 0x14, 0xD3, 0x14, 0xE6, 0x14, 0xF9, 0x14, 0x0C, 0x15, 0x1F, 0x15, 0x32, 0x15, 0x45, 0x15, 0x57, 0x15, 0x6A, 0x15, 0x7D, 0x15, 0x8F, 0x15, 0xA2, 0x15, 0xB4, 0x15, 0xC7, 0x15, 0xD9, 0x15, 0xEC, 0x15, 0xFE, 0x15, 0x10, 0x16, 0x22, 0x16, 0x34, 0x16, 0x46, 0x16, 0x58, 0x16, 0x6A, 0x16, 0x7C, 0x16, 0x8E, 0x16, 0xA0, 0x16, 0xB2, 0x16, 0xC3, 0x16, 0xD5, 0x16, 0xE7, 0x16, 0xF8, 0x16, 0x0A, 0x17, 0x1B, 0x17, 0x2C, 0x17, 0x3E, 0x17, 0x4F, 0x17, 0x60, 0x17, 0x71, 0x17, 0x82, 0x17, 0x93, 0x17, 0xA4, 0x17, 0xB5, 0x17, 0xC6, 0x17, 0xD7, 0x17, 0xE8, 0x17, 0xF8, 0x17, 0x09, 0x18, 0x19, 0x18, 0x2A, 0x18, 0x3A, 0x18, 0x4B, 0x18, 0x5B, 0x18, 0x6B, 0x18, 0x7B, 0x18, 0x8C, 0x18, 0x9C, 0x18, 0xAC, 0x18, 0xBC, 0x18, 0xCC, 0x18, 0xDB, 0x18, 0xEB, 0x18, 0xFB, 0x18, 0x0B, 0x19, 0x1A, 0x19, 0x2A, 0x19, 0x39, 0x19, 0x49, 0x19, 0x58, 0x19, 0x67, 0x19, 0x77, 0x19, 0x86, 0x19, 0x95, 0x19, 0xA4, 0x19, 0xB3, 0x19, 0xC2, 0x19, 0xD1, 0x19, 0xE0, 0x19, 0xEE, 0x19, 0xFD, 0x19, 0x0C, 0x1A, 0x1A, 0x1A, 0x29, 0x1A, 0x37, 0x1A, 0x46, 0x1A, 0x54, 0x1A, 0x62, 0x1A, 0x70, 0x1A, 0x7F, 0x1A, 0x8D, 0x1A, 0x9B, 0x1A, 0xA8, 0x1A, 0xB6, 0x1A, 0xC4, 0x1A, 0xD2, 0x1A, 0xE0, 0x1A, 0xED, 0x1A, 0xFB, 0x1A, 0x08, 0x1B, 0x16, 0x1B, 0x23, 0x1B, 0x30, 0x1B, 0x3D, 0x1B, 0x4B, 0x1B, 0x58, 0x1B, 0x65, 0x1B, 0x72, 0x1B, 0x7F, 0x1B, 0x8B, 0x1B, 0x98, 0x1B, 0xA5, 0x1B, 0xB1, 0x1B, 0xBE, 0x1B, 0xCA, 0x1B, 0xD7, 0x1B, 0xE3, 0x1B, 0xF0, 0x1B, 0xFC, 0x1B, 0x08, 0x1C, 0x14, 0x1C, 0x20, 0x1C, 0x2C, 0x1C, 0x38, 0x1C, 0x44, 0x1C, 0x4F, 0x1C, 0x5B, 0x1C, 0x67, 0x1C, 0x72, 0x1C, 0x7E, 0x1C, 0x89, 0x1C, 0x94, 0x1C, 0xA0, 0x1C, 0xAB, 0x1C, 0xB6, 0x1C, 0xC1, 0x1C, 0xCC, 0x1C, 0xD7, 0x1C, 0xE2, 0x1C, 0xED, 0x1C, 0xF7, 0x1C, 0x02, 0x1D, 0x0C, 0x1D, 0x17, 0x1D, 0x21, 0x1D, 0x2C, 0x1D, 0x36, 0x1D, 0x40, 0x1D, 0x4A, 0x1D, 0x55, 0x1D, 0x5F, 0x1D, 0x68, 0x1D, 0x72, 0x1D, 0x7C, 0x1D, 0x86, 0x1D, 0x8F, 0x1D, 0x99, 0x1D, 0xA3, 0x1D, 0xAC, 0x1D, 0xB5, 0x1D, 0xBF, 0x1D, 0xC8, 0x1D, 0xD1, 0x1D, 0xDA, 0x1D, 0xE3, 0x1D, 0xEC, 0x1D, 0xF5, 0x1D, 0xFE, 0x1D, 0x06, 0x1E, 0x0F, 0x1E, 0x18, 0x1E, 0x20, 0x1E, 0x29, 0x1E, 0x31, 0x1E, 0x39, 0x1E, 0x41, 0x1E, 0x4A, 0x1E, 0x52, 0x1E, 0x5A, 0x1E, 0x62, 0x1E, 0x69, 0x1E, 0x71, 0x1E, 0x79, 0x1E, 0x81, 0x1E, 0x88, 0x1E, 0x90, 0x1E, 0x97, 0x1E, 0x9E, 0x1E, 0xA6, 0x1E, 0xAD, 0x1E, 0xB4, 0x1E, 0xBB, 0x1E, 0xC2, 0x1E, 0xC9, 0x1E, 0xD0, 0x1E, 0xD6, 0x1E, 0xDD, 0x1E, 0xE4, 0x1E, 0xEA, 0x1E, 0xF1, 0x1E, 0xF7, 0x1E, 0xFD, 0x1E, 0x03, 0x1F, 0x0A, 0x1F, 0x10, 0x1F, 0x16, 0x1F, 0x1C, 0x1F, 0x21, 0x1F, 0x27, 0x1F, 0x2D, 0x1F, 0x32, 0x1F, 0x38, 0x1F, 0x3D, 0x1F, 0x43, 0x1F, 0x48, 0x1F, 0x4D, 0x1F, 0x53, 0x1F, 0x58, 0x1F, 0x5D, 0x1F, 0x62, 0x1F, 0x66, 0x1F, 0x6B, 0x1F, 0x70, 0x1F, 0x75, 0x1F, 0x79, 0x1F, 0x7E, 0x1F, 0x82, 0x1F, 0x86, 0x1F, 0x8B, 0x1F, 0x8F, 0x1F, 0x93, 0x1F, 0x97, 0x1F, 0x9B, 0x1F, 0x9F, 0x1F, 0xA3, 0x1F, 0xA6, 0x1F, 0xAA, 0x1F, 0xAE, 0x1F, 0xB1, 0x1F, 0xB4, 0x1F, 0xB8, 0x1F, 0xBB, 0x1F, 0xBE, 0x1F, 0xC1, 0x1F, 0xC4, 0x1F, 0xC7, 0x1F, 0xCA, 0x1F, 0xCD, 0x1F, 0xD0, 0x1F, 0xD2, 0x1F, 0xD5, 0x1F, 0xD8, 0x1F, 0xDA, 0x1F, 0xDC, 0x1F, 0xDF, 0x1F, 0xE1, 0x1F, 0xE3, 0x1F, 0xE5, 0x1F, 0xE7, 0x1F, 0xE9, 0x1F, 0xEB, 0x1F, 0xEC, 0x1F, 0xEE, 0x1F, 0xF0, 0x1F, 0xF1, 0x1F, 0xF3, 0x1F, 0xF4, 0x1F, 0xF5, 0x1F, 0xF6, 0x1F, 0xF7, 0x1F, 0xF8, 0x1F, 0xF9, 0x1F, 0xFA, 0x1F, 0xFB, 0x1F, 0xFC, 0x1F, 0xFD, 0x1F, 0xFD, 0x1F, 0xFE, 0x1F, 0xFE, 0x1F, 0xFE, 0x1F, 0xFF, 0x1F, 0xFF, 0x1F, 0xFF, 0x1F, 0xFF, 0x1F, 0xFF, 0x1F, 0xFF, 0x1F, 0xFF, 0x1F, 0xFE, 0x1F, 0xFE, 0x1F, 0xFE, 0x1F, 0xFD, 0x1F, 0xFD, 0x1F, 0xFC, 0x1F, 0xFB, 0x1F, 0xFA, 0x1F, 0xF9, 0x1F, 0xF8, 0x1F, 0xF7, 0x1F, 0xF6, 0x1F, 0xF5, 0x1F, 0xF4, 0x1F, 0xF3, 0x1F, 0xF1, 0x1F, 0xF0, 0x1F, 0xEE, 0x1F, 0xEC, 0x1F, 0xEB, 0x1F, 0xE9, 0x1F, 0xE7, 0x1F, 0xE5, 0x1F, 0xE3, 0x1F, 0xE1, 0x1F, 0xDF, 0x1F, 0xDC, 0x1F, 0xDA, 0x1F, 0xD8, 0x1F, 0xD5, 0x1F, 0xD2, 0x1F, 0xD0, 0x1F, 0xCD, 0x1F, 0xCA, 0x1F, 0xC7, 0x1F, 0xC4, 0x1F, 0xC1, 0x1F, 0xBE, 0x1F, 0xBB, 0x1F, 0xB8, 0x1F, 0xB4, 0x1F, 0xB1, 0x1F, 0xAE, 0x1F, 0xAA, 0x1F, 0xA6, 0x1F, 0xA3, 0x1F, 0x9F, 0x1F, 0x9B, 0x1F, 0x97, 0x1F, 0x93, 0x1F, 0x8F, 0x1F, 0x8B, 0x1F, 0x86, 0x1F, 0x82, 0x1F, 0x7E, 0x1F, 0x79, 0x1F, 0x75, 0x1F, 0x70, 0x1F, 0x6B, 0x1F, 0x66, 0x1F, 0x62, 0x1F, 0x5D, 0x1F, 0x58, 0x1F, 0x53, 0x1F, 0x4D, 0x1F, 0x48, 0x1F, 0x43, 0x1F, 0x3D, 0x1F, 0x38, 0x1F, 0x32, 0x1F, 0x2D, 0x1F, 0x27, 0x1F, 0x21, 0x1F, 0x1C, 0x1F, 0x16, 0x1F, 0x10, 0x1F, 0x0A, 0x1F, 0x03, 0x1F, 0xFD, 0x1E, 0xF7, 0x1E, 0xF1, 0x1E, 0xEA, 0x1E, 0xE4, 0x1E, 0xDD, 0x1E, 0xD6, 0x1E, 0xD0, 0x1E, 0xC9, 0x1E, 0xC2, 0x1E, 0xBB, 0x1E, 0xB4, 0x1E, 0xAD, 0x1E, 0xA6, 0x1E, 0x9E, 0x1E, 0x97, 0x1E, 0x90, 0x1E, 0x88, 0x1E, 0x81, 0x1E, 0x79, 0x1E, 0x71, 0x1E, 0x69, 0x1E, 0x62, 0x1E, 0x5A, 0x1E, 0x52, 0x1E, 0x4A, 0x1E, 0x41, 0x1E, 0x39, 0x1E, 0x31, 0x1E, 0x29, 0x1E, 0x20, 0x1E, 0x18, 0x1E, 0x0F, 0x1E, 0x06, 0x1E, 0xFE, 0x1D, 0xF5, 0x1D, 0xEC, 0x1D, 0xE3, 0x1D, 0xDA, 0x1D, 0xD1, 0x1D, 0xC8, 0x1D, 0xBF, 0x1D, 0xB5, 0x1D, 0xAC, 0x1D, 0xA3, 0x1D, 0x99, 0x1D, 0x8F, 0x1D, 0x86, 0x1D, 0x7C, 0x1D, 0x72, 0x1D, 0x68, 0x1D, 0x5F, 0x1D, 0x55, 0x1D, 0x4A, 0x1D, 0x40, 0x1D, 0x36, 0x1D, 0x2C, 0x1D, 0x21, 0x1D, 0x17, 0x1D, 0x0C, 0x1D, 0x02, 0x1D, 0xF7, 0x1C, 0xED, 0x1C, 0xE2, 0x1C, 0xD7, 0x1C, 0xCC, 0x1C, 0xC1, 0x1C, 0xB6, 0x1C, 0xAB, 0x1C, 0xA0, 0x1C, 0x94, 0x1C, 0x89, 0x1C, 0x7E, 0x1C, 0x72, 0x1C, 0x67, 0x1C, 0x5B, 0x1C, 0x4F, 0x1C, 0x44, 0x1C, 0x38, 0x1C, 0x2C, 0x1C, 0x20, 0x1C, 0x14, 0x1C, 0x08, 0x1C, 0xFC, 0x1B, 0xF0, 0x1B, 0xE3, 0x1B, 0xD7, 0x1B, 0xCA, 0x1B, 0xBE, 0x1B, 0xB1, 0x1B, 0xA5, 0x1B, 0x98, 0x1B, 0x8B, 0x1B, 0x7F, 0x1B, 0x72, 0x1B, 0x65, 0x1B, 0x58, 0x1B, 0x4B, 0x1B, 0x3D, 0x1B, 0x30, 0x1B, 0x23, 0x1B, 0x16, 0x1B, 0x08, 0x1B, 0xFB, 0x1A, 0xED, 0x1A, 0xE0, 0x1A, 0xD2, 0x1A, 0xC4, 0x1A, 0xB6, 0x1A, 0xA8, 0x1A, 0x9B, 0x1A, 0x8D, 0x1A, 0x7F, 0x1A, 0x70, 0x1A, 0x62, 0x1A, 0x54, 0x1A, 0x46, 0x1A, 0x37, 0x1A, 0x29, 0x1A, 0x1A, 0x1A, 0x0C, 0x1A, 0xFD, 0x19, 0xEE, 0x19, 0xE0, 0x19, 0xD1, 0x19, 0xC2, 0x19, 0xB3, 0x19, 0xA4, 0x19, 0x95, 0x19, 0x86, 0x19, 0x77, 0x19, 0x67, 0x19, 0x58, 0x19, 0x49, 0x19, 0x39, 0x19, 0x2A, 0x19, 0x1A, 0x19, 0x0B, 0x19, 0xFB, 0x18, 0xEB, 0x18, 0xDB, 0x18, 0xCC, 0x18, 0xBC, 0x18, 0xAC, 0x18, 0x9C, 0x18, 0x8C, 0x18, 0x7B, 0x18, 0x6B, 0x18, 0x5B, 0x18, 0x4B, 0x18, 0x3A, 0x18, 0x2A, 0x18, 0x19, 0x18, 0x09, 0x18, 0xF8, 0x17, 0xE8, 0x17, 0xD7, 0x17, 0xC6, 0x17, 0xB5, 0x17, 0xA4, 0x17, 0x93, 0x17, 0x82, 0x17, 0x71, 0x17, 0x60, 0x17, 0x4F, 0x17, 0x3E, 0x17, 0x2C, 0x17, 0x1B, 0x17, 0x0A, 0x17, 0xF8, 0x16, 0xE7, 0x16, 0xD5, 0x16, 0xC3, 0x16, 0xB2, 0x16, 0xA0, 0x16, 0x8E, 0x16, 0x7C, 0x16, 0x6A, 0x16, 0x58, 0x16, 0x46, 0x16, 0x34, 0x16, 0x22, 0x16, 0x10, 0x16, 0xFE, 0x15, 0xEC, 0x15, 0xD9, 0x15, 0xC7, 0x15, 0xB4, 0x15, 0xA2, 0x15, 0x8F, 0x15, 0x7D, 0x15, 0x6A, 0x15, 0x57, 0x15, 0x45, 0x15, 0x32, 0x15, 0x1F, 0x15, 0x0C, 0x15, 0xF9, 0x14, 0xE6, 0x14, 0xD3, 0x14, 0xC0, 0x14, 0xAD, 0x14, 0x9A, 0x14, 0x86, 0x14, 0x73, 0x14, 0x60, 0x14, 0x4C, 0x14, 0x39, 0x14, 0x25, 0x14, 0x12, 0x14, 0xFE, 0x13, 0xEB, 0x13, 0xD7, 0x13, 0xC3, 0x13, 0xAF, 0x13, 0x9C, 0x13, 0x88, 0x13, 0x74, 0x13, 0x60, 0x13, 0x4C, 0x13, 0x38, 0x13, 0x24, 0x13, 0x0F, 0x13, 0xFB, 0x12, 0xE7, 0x12, 0xD3, 0x12, 0xBE, 0x12, 0xAA, 0x12, 0x95, 0x12, 0x81, 0x12, 0x6C, 0x12, 0x58, 0x12, 0x43, 0x12, 0x2F, 0x12, 0x1A, 0x12, 0x05, 0x12, 0xF0, 0x11, 0xDC, 0x11, 0xC7, 0x11, 0xB2, 0x11, 0x9D, 0x11, 0x88, 0x11, 0x73, 0x11, 0x5E, 0x11, 0x49, 0x11, 0x33, 0x11, 0x1E, 0x11, 0x09, 0x11, 0xF4, 0x10, 0xDE, 0x10, 0xC9, 0x10, 0xB3, 0x10, 0x9E, 0x10, 0x89, 0x10, 0x73, 0x10, 0x5D, 0x10, 0x48, 0x10, 0x32, 0x10, 0x1C, 0x10, 0x07, 0x10, 0xF1, 0x0F, 0xDB, 0x0F, 0xC5, 0x0F, 0xAF, 0x0F, 0x9A, 0x0F, 0x84, 0x0F, 0x6E, 0x0F, 0x58, 0x0F, 0x41, 0x0F, 0x2B, 0x0F, 0x15, 0x0F, 0xFF, 0x0E, 0xE9, 0x0E, 0xD3, 0x0E, 0xBC, 0x0E, 0xA6, 0x0E, 0x90, 0x0E, 0x79, 0x0E, 0x63, 0x0E, 0x4C, 0x0E, 0x36, 0x0E, 0x1F, 0x0E, 0x09, 0x0E, 0xF2, 0x0D, 0xDB, 0x0D, 0xC5, 0x0D, 0xAE, 0x0D, 0x97, 0x0D, 0x81, 0x0D, 0x6A, 0x0D, 0x53, 0x0D, 0x3C, 0x0D, 0x25, 0x0D, 0x0E, 0x0D, 0xF7, 0x0C, 0xE0, 0x0C, 0xC9, 0x0C, 0xB2, 0x0C, 0x9B, 0x0C, 0x84, 0x0C, 0x6D, 0x0C, 0x56, 0x0C, 0x3F, 0x0C, 0x27, 0x0C, 0x10, 0x0C, 0xF9, 0x0B, 0xE1, 0x0B, 0xCA, 0x0B, 0xB3, 0x0B, 0x9B, 0x0B, 0x84, 0x0B, 0x6C, 0x0B, 0x55, 0x0B, 0x3D, 0x0B, 0x26, 0x0B, 0x0E, 0x0B, 0xF7, 0x0A, 0xDF, 0x0A, 0xC7, 0x0A, 0xB0, 0x0A, 0x98, 0x0A, 0x80, 0x0A, 0x69, 0x0A, 0x51, 0x0A, 0x39, 0x0A, 0x21, 0x0A, 0x09, 0x0A, 0xF1, 0x09, 0xDA, 0x09, 0xC2, 0x09, 0xAA, 0x09, 0x92, 0x09, 0x7A, 0x09, 0x62, 0x09, 0x4A, 0x09, 0x32, 0x09, 0x1A, 0x09, 0x01, 0x09, 0xE9, 0x08, 0xD1, 0x08, 0xB9, 0x08, 0xA1, 0x08, 0x89, 0x08, 0x70, 0x08, 0x58, 0x08, 0x40, 0x08, 0x28, 0x08, 0x0F, 0x08, 0xF7, 0x07, 0xDF, 0x07, 0xC6, 0x07, 0xAE, 0x07, 0x95, 0x07, 0x7D, 0x07, 0x65, 0x07, 0x4C, 0x07, 0x34, 0x07, 0x1B, 0x07, 0x03, 0x07, 0xEA, 0x06, 0xD2, 0x06, 0xB9, 0x06, 0xA0, 0x06, 0x88, 0x06, 0x6F, 0x06, 0x57, 0x06, 0x3E, 0x06, 0x25, 0x06, 0x0D, 0x06, 0xF4, 0x05, 0xDB, 0x05, 0xC3, 0x05, 0xAA, 0x05, 0x91, 0x05, 0x78, 0x05, 0x60, 0x05, 0x47, 0x05, 0x2E, 0x05, 0x15, 0x05, 0xFC, 0x04, 0xE4, 0x04, 0xCB, 0x04, 0xB2, 0x04, 0x99, 0x04, 0x80, 0x04, 0x67, 0x04, 0x4E, 0x04, 0x35, 0x04, 0x1D, 0x04, 0x04, 0x04, 0xEB, 0x03, 0xD2, 0x03, 0xB9, 0x03, 0xA0, 0x03, 0x87, 0x03, 0x6E, 0x03, 0x55, 0x03, 0x3C, 0x03, 0x23, 0x03, 0x0A, 0x03, 0xF1, 0x02, 0xD8, 0x02, 0xBF, 0x02, 0xA6, 0x02, 0x8D, 0x02, 0x74, 0x02, 0x5B, 0x02, 0x42, 0x02, 0x28, 0x02, 0x0F, 0x02, 0xF6, 0x01, 0xDD, 0x01, 0xC4, 0x01, 0xAB, 0x01, 0x92, 0x01, 0x79, 0x01, 0x60, 0x01, 0x47, 0x01, 0x2D, 0x01, 0x14, 0x01, 0xFB, 0x00, 0xE2, 0x00, 0xC9, 0x00, 0xB0, 0x00, 0x97, 0x00, 0x7E, 0x00, 0x65, 0x00, 0x4B, 0x00, 0x32, 0x00, 0x19, 0x00, 0x00, 0x00, 0xE7, 0xFF, 0xCE, 0xFF, 0xB5, 0xFF, 0x9B, 0xFF, 0x82, 0xFF, 0x69, 0xFF, 0x50, 0xFF, 0x37, 0xFF, 0x1E, 0xFF, 0x05, 0xFF, 0xEC, 0xFE, 0xD3, 0xFE, 0xB9, 0xFE, 0xA0, 0xFE, 0x87, 0xFE, 0x6E, 0xFE, 0x55, 0xFE, 0x3C, 0xFE, 0x23, 0xFE, 0x0A, 0xFE, 0xF1, 0xFD, 0xD8, 0xFD, 0xBE, 0xFD, 0xA5, 0xFD, 0x8C, 0xFD, 0x73, 0xFD, 0x5A, 0xFD, 0x41, 0xFD, 0x28, 0xFD, 0x0F, 0xFD, 0xF6, 0xFC, 0xDD, 0xFC, 0xC4, 0xFC, 0xAB, 0xFC, 0x92, 0xFC, 0x79, 0xFC, 0x60, 0xFC, 0x47, 0xFC, 0x2E, 0xFC, 0x15, 0xFC, 0xFC, 0xFB, 0xE3, 0xFB, 0xCB, 0xFB, 0xB2, 0xFB, 0x99, 0xFB, 0x80, 0xFB, 0x67, 0xFB, 0x4E, 0xFB, 0x35, 0xFB, 0x1C, 0xFB, 0x04, 0xFB, 0xEB, 0xFA, 0xD2, 0xFA, 0xB9, 0xFA, 0xA0, 0xFA, 0x88, 0xFA, 0x6F, 0xFA, 0x56, 0xFA, 0x3D, 0xFA, 0x25, 0xFA, 0x0C, 0xFA, 0xF3, 0xF9, 0xDB, 0xF9, 0xC2, 0xF9, 0xA9, 0xF9, 0x91, 0xF9, 0x78, 0xF9, 0x60, 0xF9, 0x47, 0xF9, 0x2E, 0xF9, 0x16, 0xF9, 0xFD, 0xF8, 0xE5, 0xF8, 0xCC, 0xF8, 0xB4, 0xF8, 0x9B, 0xF8, 0x83, 0xF8, 0x6B, 0xF8, 0x52, 0xF8, 0x3A, 0xF8, 0x21, 0xF8, 0x09, 0xF8, 0xF1, 0xF7, 0xD8, 0xF7, 0xC0, 0xF7, 0xA8, 0xF7, 0x90, 0xF7, 0x77, 0xF7, 0x5F, 0xF7, 0x47, 0xF7, 0x2F, 0xF7, 0x17, 0xF7, 0xFF, 0xF6, 0xE6, 0xF6, 0xCE, 0xF6, 0xB6, 0xF6, 0x9E, 0xF6, 0x86, 0xF6, 0x6E, 0xF6, 0x56, 0xF6, 0x3E, 0xF6, 0x26, 0xF6, 0x0F, 0xF6, 0xF7, 0xF5, 0xDF, 0xF5, 0xC7, 0xF5, 0xAF, 0xF5, 0x97, 0xF5, 0x80, 0xF5, 0x68, 0xF5, 0x50, 0xF5, 0x39, 0xF5, 0x21, 0xF5, 0x09, 0xF5, 0xF2, 0xF4, 0xDA, 0xF4, 0xC3, 0xF4, 0xAB, 0xF4, 0x94, 0xF4, 0x7C, 0xF4, 0x65, 0xF4, 0x4D, 0xF4, 0x36, 0xF4, 0x1F, 0xF4, 0x07, 0xF4, 0xF0, 0xF3, 0xD9, 0xF3, 0xC1, 0xF3, 0xAA, 0xF3, 0x93, 0xF3, 0x7C, 0xF3, 0x65, 0xF3, 0x4E, 0xF3, 0x37, 0xF3, 0x20, 0xF3, 0x09, 0xF3, 0xF2, 0xF2, 0xDB, 0xF2, 0xC4, 0xF2, 0xAD, 0xF2, 0x96, 0xF2, 0x7F, 0xF2, 0x69, 0xF2, 0x52, 0xF2, 0x3B, 0xF2, 0x25, 0xF2, 0x0E, 0xF2, 0xF7, 0xF1, 0xE1, 0xF1, 0xCA, 0xF1, 0xB4, 0xF1, 0x9D, 0xF1, 0x87, 0xF1, 0x70, 0xF1, 0x5A, 0xF1, 0x44, 0xF1, 0x2D, 0xF1, 0x17, 0xF1, 0x01, 0xF1, 0xEB, 0xF0, 0xD5, 0xF0, 0xBF, 0xF0, 0xA8, 0xF0, 0x92, 0xF0, 0x7C, 0xF0, 0x66, 0xF0, 0x51, 0xF0, 0x3B, 0xF0, 0x25, 0xF0, 0x0F, 0xF0, 0xF9, 0xEF, 0xE4, 0xEF, 0xCE, 0xEF, 0xB8, 0xEF, 0xA3, 0xEF, 0x8D, 0xEF, 0x77, 0xEF, 0x62, 0xEF, 0x4D, 0xEF, 0x37, 0xEF, 0x22, 0xEF, 0x0C, 0xEF, 0xF7, 0xEE, 0xE2, 0xEE, 0xCD, 0xEE, 0xB7, 0xEE, 0xA2, 0xEE, 0x8D, 0xEE, 0x78, 0xEE, 0x63, 0xEE, 0x4E, 0xEE, 0x39, 0xEE, 0x24, 0xEE, 0x10, 0xEE, 0xFB, 0xED, 0xE6, 0xED, 0xD1, 0xED, 0xBD, 0xED, 0xA8, 0xED, 0x94, 0xED, 0x7F, 0xED, 0x6B, 0xED, 0x56, 0xED, 0x42, 0xED, 0x2D, 0xED, 0x19, 0xED, 0x05, 0xED, 0xF1, 0xEC, 0xDC, 0xEC, 0xC8, 0xEC, 0xB4, 0xEC, 0xA0, 0xEC, 0x8C, 0xEC, 0x78, 0xEC, 0x64, 0xEC, 0x51, 0xEC, 0x3D, 0xEC, 0x29, 0xEC, 0x15, 0xEC, 0x02, 0xEC, 0xEE, 0xEB, 0xDB, 0xEB, 0xC7, 0xEB, 0xB4, 0xEB, 0xA0, 0xEB, 0x8D, 0xEB, 0x7A, 0xEB, 0x66, 0xEB, 0x53, 0xEB, 0x40, 0xEB, 0x2D, 0xEB, 0x1A, 0xEB, 0x07, 0xEB, 0xF4, 0xEA, 0xE1, 0xEA, 0xCE, 0xEA, 0xBB, 0xEA, 0xA9, 0xEA, 0x96, 0xEA, 0x83, 0xEA, 0x71, 0xEA, 0x5E, 0xEA, 0x4C, 0xEA, 0x39, 0xEA, 0x27, 0xEA, 0x14, 0xEA, 0x02, 0xEA, 0xF0, 0xE9, 0xDE, 0xE9, 0xCC, 0xE9, 0xBA, 0xE9, 0xA8, 0xE9, 0x96, 0xE9, 0x84, 0xE9, 0x72, 0xE9, 0x60, 0xE9, 0x4E, 0xE9, 0x3D, 0xE9, 0x2B, 0xE9, 0x19, 0xE9, 0x08, 0xE9, 0xF6, 0xE8, 0xE5, 0xE8, 0xD4, 0xE8, 0xC2, 0xE8, 0xB1, 0xE8, 0xA0, 0xE8, 0x8F, 0xE8, 0x7E, 0xE8, 0x6D, 0xE8, 0x5C, 0xE8, 0x4B, 0xE8, 0x3A, 0xE8, 0x29, 0xE8, 0x18, 0xE8, 0x08, 0xE8, 0xF7, 0xE7, 0xE7, 0xE7, 0xD6, 0xE7, 0xC6, 0xE7, 0xB5, 0xE7, 0xA5, 0xE7, 0x95, 0xE7, 0x85, 0xE7, 0x74, 0xE7, 0x64, 0xE7, 0x54, 0xE7, 0x44, 0xE7, 0x34, 0xE7, 0x25, 0xE7, 0x15, 0xE7, 0x05, 0xE7, 0xF5, 0xE6, 0xE6, 0xE6, 0xD6, 0xE6, 0xC7, 0xE6, 0xB7, 0xE6, 0xA8, 0xE6, 0x99, 0xE6, 0x89, 0xE6, 0x7A, 0xE6, 0x6B, 0xE6, 0x5C, 0xE6, 0x4D, 0xE6, 0x3E, 0xE6, 0x2F, 0xE6, 0x20, 0xE6, 0x12, 0xE6, 0x03, 0xE6, 0xF4, 0xE5, 0xE6, 0xE5, 0xD7, 0xE5, 0xC9, 0xE5, 0xBA, 0xE5, 0xAC, 0xE5, 0x9E, 0xE5, 0x90, 0xE5, 0x81, 0xE5, 0x73, 0xE5, 0x65, 0xE5, 0x58, 0xE5, 0x4A, 0xE5, 0x3C, 0xE5, 0x2E, 0xE5, 0x20, 0xE5, 0x13, 0xE5, 0x05, 0xE5, 0xF8, 0xE4, 0xEA, 0xE4, 0xDD, 0xE4, 0xD0, 0xE4, 0xC3, 0xE4, 0xB5, 0xE4, 0xA8, 0xE4, 0x9B, 0xE4, 0x8E, 0xE4, 0x81, 0xE4, 0x75, 0xE4, 0x68, 0xE4, 0x5B, 0xE4, 0x4F, 0xE4, 0x42, 0xE4, 0x36, 0xE4, 0x29, 0xE4, 0x1D, 0xE4, 0x10, 0xE4, 0x04, 0xE4, 0xF8, 0xE3, 0xEC, 0xE3, 0xE0, 0xE3, 0xD4, 0xE3, 0xC8, 0xE3, 0xBC, 0xE3, 0xB1, 0xE3, 0xA5, 0xE3, 0x99, 0xE3, 0x8E, 0xE3, 0x82, 0xE3, 0x77, 0xE3, 0x6C, 0xE3, 0x60, 0xE3, 0x55, 0xE3, 0x4A, 0xE3, 0x3F, 0xE3, 0x34, 0xE3, 0x29, 0xE3, 0x1E, 0xE3, 0x13, 0xE3, 0x09, 0xE3, 0xFE, 0xE2, 0xF4, 0xE2, 0xE9, 0xE2, 0xDF, 0xE2, 0xD4, 0xE2, 0xCA, 0xE2, 0xC0, 0xE2, 0xB6, 0xE2, 0xAB, 0xE2, 0xA1, 0xE2, 0x98, 0xE2, 0x8E, 0xE2, 0x84, 0xE2, 0x7A, 0xE2, 0x71, 0xE2, 0x67, 0xE2, 0x5D, 0xE2, 0x54, 0xE2, 0x4B, 0xE2, 0x41, 0xE2, 0x38, 0xE2, 0x2F, 0xE2, 0x26, 0xE2, 0x1D, 0xE2, 0x14, 0xE2, 0x0B, 0xE2, 0x02, 0xE2, 0xFA, 0xE1, 0xF1, 0xE1, 0xE8, 0xE1, 0xE0, 0xE1, 0xD7, 0xE1, 0xCF, 0xE1, 0xC7, 0xE1, 0xBF, 0xE1, 0xB6, 0xE1, 0xAE, 0xE1, 0xA6, 0xE1, 0x9E, 0xE1, 0x97, 0xE1, 0x8F, 0xE1, 0x87, 0xE1, 0x7F, 0xE1, 0x78, 0xE1, 0x70, 0xE1, 0x69, 0xE1, 0x62, 0xE1, 0x5A, 0xE1, 0x53, 0xE1, 0x4C, 0xE1, 0x45, 0xE1, 0x3E, 0xE1, 0x37, 0xE1, 0x30, 0xE1, 0x2A, 0xE1, 0x23, 0xE1, 0x1C, 0xE1, 0x16, 0xE1, 0x0F, 0xE1, 0x09, 0xE1, 0x03, 0xE1, 0xFD, 0xE0, 0xF6, 0xE0, 0xF0, 0xE0, 0xEA, 0xE0, 0xE4, 0xE0, 0xDF, 0xE0, 0xD9, 0xE0, 0xD3, 0xE0, 0xCE, 0xE0, 0xC8, 0xE0, 0xC3, 0xE0, 0xBD, 0xE0, 0xB8, 0xE0, 0xB3, 0xE0, 0xAD, 0xE0, 0xA8, 0xE0, 0xA3, 0xE0, 0x9E, 0xE0, 0x9A, 0xE0, 0x95, 0xE0, 0x90, 0xE0, 0x8B, 0xE0, 0x87, 0xE0, 0x82, 0xE0, 0x7E, 0xE0, 0x7A, 0xE0, 0x75, 0xE0, 0x71, 0xE0, 0x6D, 0xE0, 0x69, 0xE0, 0x65, 0xE0, 0x61, 0xE0, 0x5D, 0xE0, 0x5A, 0xE0, 0x56, 0xE0, 0x52, 0xE0, 0x4F, 0xE0, 0x4C, 0xE0, 0x48, 0xE0, 0x45, 0xE0, 0x42, 0xE0, 0x3F, 0xE0, 0x3C, 0xE0, 0x39, 0xE0, 0x36, 0xE0, 0x33, 0xE0, 0x30, 0xE0, 0x2E, 0xE0, 0x2B, 0xE0, 0x28, 0xE0, 0x26, 0xE0, 0x24, 0xE0, 0x21, 0xE0, 0x1F, 0xE0, 0x1D, 0xE0, 0x1B, 0xE0, 0x19, 0xE0, 0x17, 0xE0, 0x15, 0xE0, 0x14, 0xE0, 0x12, 0xE0, 0x10, 0xE0, 0x0F, 0xE0, 0x0D, 0xE0, 0x0C, 0xE0, 0x0B, 0xE0, 0x0A, 0xE0, 0x09, 0xE0, 0x08, 0xE0, 0x07, 0xE0, 0x06, 0xE0, 0x05, 0xE0, 0x04, 0xE0, 0x03, 0xE0, 0x03, 0xE0, 0x02, 0xE0, 0x02, 0xE0, 0x02, 0xE0, 0x01, 0xE0, 0x01, 0xE0, 0x01, 0xE0, 0x01, 0xE0, 0x01, 0xE0, 0x01, 0xE0, 0x01, 0xE0, 0x02, 0xE0, 0x02, 0xE0, 0x02, 0xE0, 0x03, 0xE0, 0x03, 0xE0, 0x04, 0xE0, 0x05, 0xE0, 0x06, 0xE0, 0x07, 0xE0, 0x08, 0xE0, 0x09, 0xE0, 0x0A, 0xE0, 0x0B, 0xE0, 0x0C, 0xE0, 0x0D, 0xE0, 0x0F, 0xE0, 0x10, 0xE0, 0x12, 0xE0, 0x14, 0xE0, 0x15, 0xE0, 0x17, 0xE0, 0x19, 0xE0, 0x1B, 0xE0, 0x1D, 0xE0, 0x1F, 0xE0, 0x21, 0xE0, 0x24, 0xE0, 0x26, 0xE0, 0x28, 0xE0, 0x2B, 0xE0, 0x2E, 0xE0, 0x30, 0xE0, 0x33, 0xE0, 0x36, 0xE0, 0x39, 0xE0, 0x3C, 0xE0, 0x3F, 0xE0, 0x42, 0xE0, 0x45, 0xE0, 0x48, 0xE0, 0x4C, 0xE0, 0x4F, 0xE0, 0x52, 0xE0, 0x56, 0xE0, 0x5A, 0xE0, 0x5D, 0xE0, 0x61, 0xE0, 0x65, 0xE0, 0x69, 0xE0, 0x6D, 0xE0, 0x71, 0xE0, 0x75, 0xE0, 0x7A, 0xE0, 0x7E, 0xE0, 0x82, 0xE0, 0x87, 0xE0, 0x8B, 0xE0, 0x90, 0xE0, 0x95, 0xE0, 0x9A, 0xE0, 0x9E, 0xE0, 0xA3, 0xE0, 0xA8, 0xE0, 0xAD, 0xE0, 0xB3, 0xE0, 0xB8, 0xE0, 0xBD, 0xE0, 0xC3, 0xE0, 0xC8, 0xE0, 0xCE, 0xE0, 0xD3, 0xE0, 0xD9, 0xE0, 0xDF, 0xE0, 0xE4, 0xE0, 0xEA, 0xE0, 0xF0, 0xE0, 0xF6, 0xE0, 0xFD, 0xE0, 0x03, 0xE1, 0x09, 0xE1, 0x0F, 0xE1, 0x16, 0xE1, 0x1C, 0xE1, 0x23, 0xE1, 0x2A, 0xE1, 0x30, 0xE1, 0x37, 0xE1, 0x3E, 0xE1, 0x45, 0xE1, 0x4C, 0xE1, 0x53, 0xE1, 0x5A, 0xE1, 0x62, 0xE1, 0x69, 0xE1, 0x70, 0xE1, 0x78, 0xE1, 0x7F, 0xE1, 0x87, 0xE1, 0x8F, 0xE1, 0x97, 0xE1, 0x9E, 0xE1, 0xA6, 0xE1, 0xAE, 0xE1, 0xB6, 0xE1, 0xBF, 0xE1, 0xC7, 0xE1, 0xCF, 0xE1, 0xD7, 0xE1, 0xE0, 0xE1, 0xE8, 0xE1, 0xF1, 0xE1, 0xFA, 0xE1, 0x02, 0xE2, 0x0B, 0xE2, 0x14, 0xE2, 0x1D, 0xE2, 0x26, 0xE2, 0x2F, 0xE2, 0x38, 0xE2, 0x41, 0xE2, 0x4B, 0xE2, 0x54, 0xE2, 0x5D, 0xE2, 0x67, 0xE2, 0x71, 0xE2, 0x7A, 0xE2, 0x84, 0xE2, 0x8E, 0xE2, 0x98, 0xE2, 0xA1, 0xE2, 0xAB, 0xE2, 0xB6, 0xE2, 0xC0, 0xE2, 0xCA, 0xE2, 0xD4, 0xE2, 0xDF, 0xE2, 0xE9, 0xE2, 0xF4, 0xE2, 0xFE, 0xE2, 0x09, 0xE3, 0x13, 0xE3, 0x1E, 0xE3, 0x29, 0xE3, 0x34, 0xE3, 0x3F, 0xE3, 0x4A, 0xE3, 0x55, 0xE3, 0x60, 0xE3, 0x6C, 0xE3, 0x77, 0xE3, 0x82, 0xE3, 0x8E, 0xE3, 0x99, 0xE3, 0xA5, 0xE3, 0xB1, 0xE3, 0xBC, 0xE3, 0xC8, 0xE3, 0xD4, 0xE3, 0xE0, 0xE3, 0xEC, 0xE3, 0xF8, 0xE3, 0x04, 0xE4, 0x10, 0xE4, 0x1D, 0xE4, 0x29, 0xE4, 0x36, 0xE4, 0x42, 0xE4, 0x4F, 0xE4, 0x5B, 0xE4, 0x68, 0xE4, 0x75, 0xE4, 0x81, 0xE4, 0x8E, 0xE4, 0x9B, 0xE4, 0xA8, 0xE4, 0xB5, 0xE4, 0xC3, 0xE4, 0xD0, 0xE4, 0xDD, 0xE4, 0xEA, 0xE4, 0xF8, 0xE4, 0x05, 0xE5, 0x13, 0xE5, 0x20, 0xE5, 0x2E, 0xE5, 0x3C, 0xE5, 0x4A, 0xE5, 0x58, 0xE5, 0x65, 0xE5, 0x73, 0xE5, 0x81, 0xE5, 0x90, 0xE5, 0x9E, 0xE5, 0xAC, 0xE5, 0xBA, 0xE5, 0xC9, 0xE5, 0xD7, 0xE5, 0xE6, 0xE5, 0xF4, 0xE5, 0x03, 0xE6, 0x12, 0xE6, 0x20, 0xE6, 0x2F, 0xE6, 0x3E, 0xE6, 0x4D, 0xE6, 0x5C, 0xE6, 0x6B, 0xE6, 0x7A, 0xE6, 0x89, 0xE6, 0x99, 0xE6, 0xA8, 0xE6, 0xB7, 0xE6, 0xC7, 0xE6, 0xD6, 0xE6, 0xE6, 0xE6, 0xF5, 0xE6, 0x05, 0xE7, 0x15, 0xE7, 0x25, 0xE7, 0x34, 0xE7, 0x44, 0xE7, 0x54, 0xE7, 0x64, 0xE7, 0x74, 0xE7, 0x85, 0xE7, 0x95, 0xE7, 0xA5, 0xE7, 0xB5, 0xE7, 0xC6, 0xE7, 0xD6, 0xE7, 0xE7, 0xE7, 0xF7, 0xE7, 0x08, 0xE8, 0x18, 0xE8, 0x29, 0xE8, 0x3A, 0xE8, 0x4B, 0xE8, 0x5C, 0xE8, 0x6D, 0xE8, 0x7E, 0xE8, 0x8F, 0xE8, 0xA0, 0xE8, 0xB1, 0xE8, 0xC2, 0xE8, 0xD4, 0xE8, 0xE5, 0xE8, 0xF6, 0xE8, 0x08, 0xE9, 0x19, 0xE9, 0x2B, 0xE9, 0x3D, 0xE9, 0x4E, 0xE9, 0x60, 0xE9, 0x72, 0xE9, 0x84, 0xE9, 0x96, 0xE9, 0xA8, 0xE9, 0xBA, 0xE9, 0xCC, 0xE9, 0xDE, 0xE9, 0xF0, 0xE9, 0x02, 0xEA, 0x14, 0xEA, 0x27, 0xEA, 0x39, 0xEA, 0x4C, 0xEA, 0x5E, 0xEA, 0x71, 0xEA, 0x83, 0xEA, 0x96, 0xEA, 0xA9, 0xEA, 0xBB, 0xEA, 0xCE, 0xEA, 0xE1, 0xEA, 0xF4, 0xEA, 0x07, 0xEB, 0x1A, 0xEB, 0x2D, 0xEB, 0x40, 0xEB, 0x53, 0xEB, 0x66, 0xEB, 0x7A, 0xEB, 0x8D, 0xEB, 0xA0, 0xEB, 0xB4, 0xEB, 0xC7, 0xEB, 0xDB, 0xEB, 0xEE, 0xEB, 0x02, 0xEC, 0x15, 0xEC, 0x29, 0xEC, 0x3D, 0xEC, 0x51, 0xEC, 0x64, 0xEC, 0x78, 0xEC, 0x8C, 0xEC, 0xA0, 0xEC, 0xB4, 0xEC, 0xC8, 0xEC, 0xDC, 0xEC, 0xF1, 0xEC, 0x05, 0xED, 0x19, 0xED, 0x2D, 0xED, 0x42, 0xED, 0x56, 0xED, 0x6B, 0xED, 0x7F, 0xED, 0x94, 0xED, 0xA8, 0xED, 0xBD, 0xED, 0xD1, 0xED, 0xE6, 0xED, 0xFB, 0xED, 0x10, 0xEE, 0x24, 0xEE, 0x39, 0xEE, 0x4E, 0xEE, 0x63, 0xEE, 0x78, 0xEE, 0x8D, 0xEE, 0xA2, 0xEE, 0xB7, 0xEE, 0xCD, 0xEE, 0xE2, 0xEE, 0xF7, 0xEE, 0x0C, 0xEF, 0x22, 0xEF, 0x37, 0xEF, 0x4D, 0xEF, 0x62, 0xEF, 0x77, 0xEF, 0x8D, 0xEF, 0xA3, 0xEF, 0xB8, 0xEF, 0xCE, 0xEF, 0xE4, 0xEF, 0xF9, 0xEF, 0x0F, 0xF0, 0x25, 0xF0, 0x3B, 0xF0, 0x51, 0xF0, 0x66, 0xF0, 0x7C, 0xF0, 0x92, 0xF0, 0xA8, 0xF0, 0xBF, 0xF0, 0xD5, 0xF0, 0xEB, 0xF0, 0x01, 0xF1, 0x17, 0xF1, 0x2D, 0xF1, 0x44, 0xF1, 0x5A, 0xF1, 0x70, 0xF1, 0x87, 0xF1, 0x9D, 0xF1, 0xB4, 0xF1, 0xCA, 0xF1, 0xE1, 0xF1, 0xF7, 0xF1, 0x0E, 0xF2, 0x25, 0xF2, 0x3B, 0xF2, 0x52, 0xF2, 0x69, 0xF2, 0x7F, 0xF2, 0x96, 0xF2, 0xAD, 0xF2, 0xC4, 0xF2, 0xDB, 0xF2, 0xF2, 0xF2, 0x09, 0xF3, 0x20, 0xF3, 0x37, 0xF3, 0x4E, 0xF3, 0x65, 0xF3, 0x7C, 0xF3, 0x93, 0xF3, 0xAA, 0xF3, 0xC1, 0xF3, 0xD9, 0xF3, 0xF0, 0xF3, 0x07, 0xF4, 0x1F, 0xF4, 0x36, 0xF4, 0x4D, 0xF4, 0x65, 0xF4, 0x7C, 0xF4, 0x94, 0xF4, 0xAB, 0xF4, 0xC3, 0xF4, 0xDA, 0xF4, 0xF2, 0xF4, 0x09, 0xF5, 0x21, 0xF5, 0x39, 0xF5, 0x50, 0xF5, 0x68, 0xF5, 0x80, 0xF5, 0x97, 0xF5, 0xAF, 0xF5, 0xC7, 0xF5, 0xDF, 0xF5, 0xF7, 0xF5, 0x0F, 0xF6, 0x26, 0xF6, 0x3E, 0xF6, 0x56, 0xF6, 0x6E, 0xF6, 0x86, 0xF6, 0x9E, 0xF6, 0xB6, 0xF6, 0xCE, 0xF6, 0xE6, 0xF6, 0xFF, 0xF6, 0x17, 0xF7, 0x2F, 0xF7, 0x47, 0xF7, 0x5F, 0xF7, 0x77, 0xF7, 0x90, 0xF7, 0xA8, 0xF7, 0xC0, 0xF7, 0xD8, 0xF7, 0xF1, 0xF7, 0x09, 0xF8, 0x21, 0xF8, 0x3A, 0xF8, 0x52, 0xF8, 0x6B, 0xF8, 0x83, 0xF8, 0x9B, 0xF8, 0xB4, 0xF8, 0xCC, 0xF8, 0xE5, 0xF8, 0xFD, 0xF8, 0x16, 0xF9, 0x2E, 0xF9, 0x47, 0xF9, 0x60, 0xF9, 0x78, 0xF9, 0x91, 0xF9, 0xA9, 0xF9, 0xC2, 0xF9, 0xDB, 0xF9, 0xF3, 0xF9, 0x0C, 0xFA, 0x25, 0xFA, 0x3D, 0xFA, 0x56, 0xFA, 0x6F, 0xFA, 0x88, 0xFA, 0xA0, 0xFA, 0xB9, 0xFA, 0xD2, 0xFA, 0xEB, 0xFA, 0x04, 0xFB, 0x1C, 0xFB, 0x35, 0xFB, 0x4E, 0xFB, 0x67, 0xFB, 0x80, 0xFB, 0x99, 0xFB, 0xB2, 0xFB, 0xCB, 0xFB, 0xE3, 0xFB, 0xFC, 0xFB, 0x15, 0xFC, 0x2E, 0xFC, 0x47, 0xFC, 0x60, 0xFC, 0x79, 0xFC, 0x92, 0xFC, 0xAB, 0xFC, 0xC4, 0xFC, 0xDD, 0xFC, 0xF6, 0xFC, 0x0F, 0xFD, 0x28, 0xFD, 0x41, 0xFD, 0x5A, 0xFD, 0x73, 0xFD, 0x8C, 0xFD, 0xA5, 0xFD, 0xBE, 0xFD, 0xD8, 0xFD, 0xF1, 0xFD, 0x0A, 0xFE, 0x23, 0xFE, 0x3C, 0xFE, 0x55, 0xFE, 0x6E, 0xFE, 0x87, 0xFE, 0xA0, 0xFE, 0xB9, 0xFE, 0xD3, 0xFE, 0xEC, 0xFE, 0x05, 0xFF, 0x1E, 0xFF, 0x37, 0xFF, 0x50, 0xFF, 0x69, 0xFF, 0x82, 0xFF, 0x9B, 0xFF, 0xB5, 0xFF, 0xCE, 0xFF, 0xE7, 0xFF };
int sine_beep_freq;
int sine_beep_duration;

int melody_generic[512];
int melody_poweron[] = { 440, 500, 466, 500, 494, 500, -1, -1 };
int melody_key_beep[] = { 880, 100, -1, -1 };
int melody_sk1_beep[] = { 466, 100, 0, 100, 466, 100, -1, -1 };
int melody_sk2_beep[] = { 494, 100, 0, 100, 494, 100, -1, -1 };
int melody_orange_beep[] = { 440, 100, 494, 100, 440, 100, 494, 100, -1, -1 };
int melody_ACK_beep[] = { 466, 200, 494, 200, -1, -1 };
int melody_NACK_beep[] = { 494, 200, 466, 200, -1, -1 };
int melody_ERROR_beep[] = { 440, 50, 0, 50, 440, 50, 0, 50, 440, 50, -1, -1 };
int *melody_play = NULL;
int melody_idx = 0;

const uint8_t melody1[] = { 21, 4, 21, 2, 26, 10, 26, 2, 26, 4, 26, 2, 26, 6, 21, 6, 18, 4, 19, 2, 21, 10, 18, 2, 16, 4, 19, 2, 18, 6, 0, 6, 18, 4, 18, 2, 26, 10, 26, 2, 26, 4, 25, 2, 25, 6, 23, 6, 25, 4, 26, 2, 28, 10, 25, 2, 25, 4, 23, 2, 21, 6, 0, 6, 21, 4, 21, 2, 26, 10, 26, 2, 26, 4, 26, 2, 26, 6, 23, 6, 23, 4, 23, 2, 28, 10, 28, 2, 28, 4, 23, 2, 25, 6, 0, 6, 21, 4, 21, 2, 30, 10, 30, 2, 30, 4, 30, 2, 30, 6, 28, 6, 26, 4, 23, 2, 21, 10, 22, 2, 23, 4, 19, 2, 18, 6, 0, 6, 21, 4, 21, 2, 30, 10, 30, 2, 30, 4, 30, 2, 33, 6, 31, 6, 23, 4, 22, 2, 21, 10, 26, 2, 28, 4, 26, 2, 26, 12, 0, 0 };
const uint8_t melody2[] = { 21, 4, 26, 6, 18, 2, 21, 4, 26, 4, 28, 4, 30, 4, 26, 4, 26, 4, 25, 6, 23, 2, 21, 4, 20, 4, 23, 4, 21, 3, 0, 5, 21, 4, 26, 6, 18, 2, 21, 4, 26, 4, 28, 4, 30, 4, 26, 4, 26, 4, 25, 6, 23, 2, 21, 4, 20, 4, 23, 4, 21, 3, 0, 5, 21, 4, 28, 6, 25, 2, 26, 4, 23, 4, 21, 6, 19, 2, 18, 4, 21, 4, 28, 6, 25, 2, 26, 4, 23, 4, 21, 6, 19, 2, 18, 4, 21, 2, 26, 2, 30, 6, 26, 2, 21, 4, 18, 4, 16, 4, 16, 4, 23, 4, 23, 4, 21, 6, 26, 2, 30, 6, 28, 2, 28, 8, 26, 4, 25, 3, 23, 1, 21, 4, 28, 3, 30, 1, 26, 4, 25, 3, 23, 1, 21, 4, 28, 3, 30, 1, 26, 4, 23, 4, 21, 4, 26, 4, 30, 6, 28, 2, 28, 8, 26, 3, 0, 0 };
const uint8_t melody3[] = { 21, 4, 30, 4, 26, 4, 23, 3, 25, 1, 26, 3, 23, 1, 21, 3, 18, 1, 21, 4, 0, 2, 21, 2, 23, 2, 25, 2, 26, 6, 26, 2, 30, 6, 28, 2, 28, 8, 0, 4, 28, 4, 31, 6, 28, 2, 25, 2, 25, 2, 23, 2, 25, 2, 28, 4, 26, 4, 0, 2, 26, 2, 25, 2, 23, 2, 28, 6, 25, 2, 25, 4, 23, 4, 21, 8, 0, 4, 21, 4, 28, 4, 25, 4, 21, 2, 21, 2, 26, 2, 28, 2, 30, 4, 26, 4, 0, 2, 26, 2, 28, 2, 30, 2, 31, 6, 28, 2, 30, 6, 26, 2, 30, 4, 28, 4, 0, 4, 21, 4, 30, 6, 28, 2, 26, 2, 25, 2, 28, 3, 26, 1, 25, 4, 23, 4, 0, 2, 31, 2, 30, 2, 28, 2, 26, 6, 26, 2, 28, 6, 28, 2, 30, 12, 21, 4, 30, 6, 28, 2, 26, 2, 25, 2, 28, 3, 26, 1, 25, 4, 23, 4, 0, 2, 31, 2, 30, 2, 28, 2, 33, 6, 26, 2, 30, 4, 28, 4, 26, 12, 0, 0 };
const uint8_t melody4[] = { 18, 2, 22, 2, 25, 4, 25, 4, 25, 2, 27, 2, 23, 2, 20, 2, 18, 4, 22, 4, 22, 4, 0, 2, 22, 2, 20, 6, 22, 2, 23, 4, 25, 4, 23, 4, 22, 4, 0, 4, 25, 4, 25, 6, 22, 2, 30, 6, 29, 2, 29, 6, 27, 2, 27, 4, 27, 4, 25, 6, 27, 2, 25, 2, 22, 2, 23, 2, 20, 2, 20, 4, 18, 4, 0, 2, 22, 4, 20, 4, 25, 4, 25, 4, 29, 4, 29, 2, 27, 2, 30, 2, 29, 2, 27, 4, 20, 4, 20, 4, 27, 4, 27, 4, 30, 4, 30, 2, 29, 2, 32, 2, 30, 2, 29, 2, 27, 2, 25, 2, 23, 2, 25, 6, 22, 2, 30, 6, 29, 2, 29, 2, 27, 2, 27, 4, 0, 4, 27, 4, 25, 6, 27, 2, 25, 2, 22, 2, 23, 2, 20, 2, 20, 4, 18, 4, 0, 4, 23, 2, 25, 6, 22, 2, 30, 6, 29, 2, 29, 2, 27, 2, 27, 4, 0, 2, 27, 4, 25, 6, 27, 2, 25, 2, 22, 2, 23, 2, 20, 2, 20, 4, 18, 4, 0, 0 };

void set_melody(int *melody)
{
	sine_beep_freq=0;
	sine_beep_duration=0;
	melody_play=melody;
	melody_idx=0;
}

int get_freq(int tone)
{
    int freq = 0;
    switch (tone)
    {
        case 11:
            freq = 196;
            break;
        case 12:
            freq = 208;
            break;
        case 13:
            freq = 220;
            break;
        case 14:
            freq = 233;
            break;
        case 15:
            freq = 247;
            break;
        case 16:
            freq = 262;
            break;
        case 17:
            freq = 277;
            break;
        case 18:
            freq = 294;
            break;
        case 19:
            freq = 311;
            break;
        case 20:
            freq = 330;
            break;
        case 21:
            freq = 349;
            break;
        case 22:
            freq = 370;
            break;
        case 23:
            freq = 392;
            break;
        case 24:
            freq = 415;
            break;
        case 25:
            freq = 440;
            break;
        case 26:
            freq = 466;
            break;
        case 27:
            freq = 494;
            break;
        case 28:
            freq = 523;
            break;
        case 29:
            freq = 554;
            break;
        case 30:
            freq = 587;
            break;
        case 31:
            freq = 622;
            break;
        case 32:
            freq = 659;
            break;
        case 33:
            freq = 698;
            break;
    }

    return freq;
}

void create_song(const uint8_t *melody)
{
	int song_idx = 0;
	for (int i=0;i<256;i++)
	{
		if (melody[2*i+1]!=0)
		{
			melody_generic[song_idx]=get_freq(melody[2*i]);
			melody_generic[song_idx+1]=melody[2*i+1]*180;
			melody_generic[song_idx+2]=0;
			melody_generic[song_idx+3]=melody[2*i+1]*20;
			song_idx=song_idx+4;
		}
		else
		{
			melody_generic[song_idx]=-1;
			melody_generic[song_idx+1]=-1;
			song_idx=song_idx+2;
			break;
		}
	}
}

void fw_init_beep_task()
{
	sine_beep_freq = 0;
	sine_beep_duration = 0;

	xTaskCreate(fw_beep_task,                        /* pointer to the task */
				"fw beep task",                      /* task name for kernel awareness debugging */
				1000L / sizeof(portSTACK_TYPE),      /* task stack size */
				NULL,                      			 /* optional task startup argument */
				5U,                                  /* initial priority */
				fwBeepTaskHandle					 /* optional task handle to create */
				);
}

volatile uint8_t wavbuffer[WAV_BUFFER_COUNT][WAV_BUFFER_SIZE];
int wavbuffer_read_idx;
int wavbuffer_write_idx;
volatile int wavbuffer_count;
uint8_t tmp_wavbuffer[WAV_BUFFER_SIZE];

uint8_t spi_sound1[WAV_BUFFER_SIZE*2];
uint8_t spi_sound2[WAV_BUFFER_SIZE*2];
uint8_t spi_sound3[WAV_BUFFER_SIZE*2];
uint8_t spi_sound4[WAV_BUFFER_SIZE*2];

volatile bool g_TX_SAI_in_use = false;

void init_sound()
{
    g_TX_SAI_in_use = false;
    SAI_TxSoftwareReset(I2S0, kSAI_ResetAll);
	SAI_TxEnable(I2S0, true);
	wavbuffer_read_idx = 0;
	wavbuffer_write_idx = 0;
	wavbuffer_count = 0;
}

void terminate_sound()
{
    SAI_TransferTerminateSendEDMA(I2S0, &g_SAI_TX_Handle);
}

void store_soundbuffer()
{
	taskENTER_CRITICAL();
	int tmp_wavbuffer_count = wavbuffer_count;
	taskEXIT_CRITICAL();

	if (tmp_wavbuffer_count<WAV_BUFFER_COUNT)
	{
		for (int wav_idx=0;wav_idx<WAV_BUFFER_SIZE;wav_idx++)
		{
			wavbuffer[wavbuffer_write_idx][wav_idx]=tmp_wavbuffer[wav_idx];
		}
		wavbuffer_write_idx++;
		if (wavbuffer_write_idx>=WAV_BUFFER_COUNT)
		{
			wavbuffer_write_idx=0;
		}

		taskENTER_CRITICAL();
		wavbuffer_count++;
		taskEXIT_CRITICAL();
	}
}

void send_sound_data()
{
	if (wavbuffer_count>0)
	{
		for (int i=0; i<(WAV_BUFFER_SIZE/2); i++)
		{
			switch(g_SAI_TX_Handle.queueUser)
			{
			case 0:
				spi_sound1[4*i+3]=wavbuffer[wavbuffer_read_idx][2*i+1];
				spi_sound1[4*i+2]=wavbuffer[wavbuffer_read_idx][2*i];
				spi_sound1[4*i+1]=wavbuffer[wavbuffer_read_idx][2*i+1];
				spi_sound1[4*i]=wavbuffer[wavbuffer_read_idx][2*i];
				break;
			case 1:
				spi_sound2[4*i+3]=wavbuffer[wavbuffer_read_idx][2*i+1];
				spi_sound2[4*i+2]=wavbuffer[wavbuffer_read_idx][2*i];
				spi_sound2[4*i+1]=wavbuffer[wavbuffer_read_idx][2*i+1];
				spi_sound2[4*i]=wavbuffer[wavbuffer_read_idx][2*i];
				break;
			case 2:
				spi_sound3[4*i+3]=wavbuffer[wavbuffer_read_idx][2*i+1];
				spi_sound3[4*i+2]=wavbuffer[wavbuffer_read_idx][2*i];
				spi_sound3[4*i+1]=wavbuffer[wavbuffer_read_idx][2*i+1];
				spi_sound3[4*i]=wavbuffer[wavbuffer_read_idx][2*i];
				break;
			case 3:
				spi_sound4[4*i+3]=wavbuffer[wavbuffer_read_idx][2*i+1];
				spi_sound4[4*i+2]=wavbuffer[wavbuffer_read_idx][2*i];
				spi_sound4[4*i+1]=wavbuffer[wavbuffer_read_idx][2*i+1];
				spi_sound4[4*i]=wavbuffer[wavbuffer_read_idx][2*i];
				break;
			}
		}

		wavbuffer_read_idx++;
		if (wavbuffer_read_idx>=WAV_BUFFER_COUNT)
		{
			wavbuffer_read_idx=0;
		}
		wavbuffer_count--;

		sai_transfer_t xfer;
		switch(g_SAI_TX_Handle.queueUser)
		{
		case 0:
			xfer.data = spi_sound1;
			break;
		case 1:
			xfer.data = spi_sound2;
			break;
		case 2:
			xfer.data = spi_sound3;
			break;
		case 3:
			xfer.data = spi_sound4;
			break;
		}
		xfer.dataSize = WAV_BUFFER_SIZE*2;

		SAI_TransferSendEDMA(I2S0, &g_SAI_TX_Handle, &xfer);

		g_TX_SAI_in_use = true;
	}
}

void tick_soundbuffer()
{
	taskENTER_CRITICAL();
	bool tmp_g_TX_SAI_in_use = g_TX_SAI_in_use;
	taskEXIT_CRITICAL();
    if (!tmp_g_TX_SAI_in_use)
    {
    	taskENTER_CRITICAL();
    	send_sound_data();
    	taskEXIT_CRITICAL();
    }
}

void tick_melody()
{
	if (melody_play!=NULL)
	{
		if (sine_beep_duration==0)
		{
			if (melody_play[melody_idx]==-1)
			{
				GPIO_PinWrite(GPIO_speaker_mute, Pin_speaker_mute, 0);
				if (current_mode == MODE_ANALOG)
				{
				    GPIO_PinWrite(GPIO_RX_audio_mux, Pin_RX_audio_mux, 1);
				}
			    set_melody(NULL);
			}
			else
			{
				if (melody_idx==0)
				{
				    GPIO_PinWrite(GPIO_speaker_mute, Pin_speaker_mute, 1);
					if (current_mode == MODE_ANALOG)
					{
					    GPIO_PinWrite(GPIO_RX_audio_mux, Pin_RX_audio_mux, 0);
					}
				}
				sine_beep_freq=melody_play[melody_idx];
				sine_beep_duration=melody_play[melody_idx+1];
				melody_idx=melody_idx+2;
			}
		}
	}
}

void fw_beep_task()
{
	// beep mode stuff
	uint8_t tmp_val;
	int beep_idx = 0;
	bool beep = false;

    while (1U)
    {
		if (sine_beep_duration>0)
		{
			if (!beep)
			{
				set_clear_SPI_page_reg_byte_with_mask_SPI0(0x04, 0x06, 0xFD, 0x02); // SET
				beep = true;
			}

			read_SPI_page_reg_byte_SPI0(0x04, 0x88, &tmp_val);
			if ( !(tmp_val & 1) )
			{
				uint8_t spi_sound[32];
				for (int i=0; i<16 ;i++)
				{
					spi_sound[2*i+1]=sine_beep[2*beep_idx];
					spi_sound[2*i]=sine_beep[2*beep_idx+1];
					if (sine_beep_freq!=0)
					{
						beep_idx=beep_idx+(sine_beep_freq/3.915f);
						if (beep_idx>=0x0800)
						{
							beep_idx=beep_idx-0x0800;
						}
					}
				}
				write_SPI_page_reg_bytearray_SPI0(0x03, 0x00, spi_sound, 0x20);
			}

			sine_beep_duration--;
		}
		else
		{
			if (beep)
			{
				set_clear_SPI_page_reg_byte_with_mask_SPI0(0x04, 0x06, 0xFD, 0x00); // CLEAR
				beep = false;
			}
		}

		vTaskDelay(portTICK_PERIOD_MS);
    }
}
