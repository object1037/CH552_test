#pragma once

#include <compiler.h>
#include <stdint.h>

SFR(T2MOD, 0xC9);
#define bTMR_CLK 0x80
#define bT0_CLK 0x10

SFR(SAFE_MOD, 0xA1);
SFR(CLOCK_CFG, 0xB9);
#define bOSC_EN_INT 0x80
#define bOSC_EN_XT 0x40

SFR(TKEY_CTRL, 0xC3);
#define bTKC_IF 0x80

void ConfigClock(void);

void delay(uint16_t ms);
void delayMicroseconds(uint16_t us);
