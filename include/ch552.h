#pragma once

#include <compiler.h>
#include <stdint.h>

SFR(SAFE_MOD, 0xA1);
SFR(CLOCK_CFG, 0xB9);

#define bOSC_EN_INT 0x80
#define bOSC_EN_XT 0x40

void ConfigClock(void);

void delay(uint16_t ms);
void delayMicroseconds(uint16_t us);
