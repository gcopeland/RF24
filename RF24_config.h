
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __RF24_CONFIG_H__
#define __RF24_CONFIG_H__

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include <stddef.h>

// Stuff that is normally provided by Arduino
#ifdef ARDUINO
#include <SPI.h>
#else
#include <stdint.h>
#include <stdio.h>
#include <string.h>
extern HardwareSPI SPI;
#define _BV(x) (1<<(x))
#endif

#if defined(ENERGIA)
#define strlen_P strlen
#define pgm_read_byte(p) (*(p)) 
#define pgm_read_word(p) (*(p)) 
#define _BV(x) (1 << (x))
#define PROGMEM
#define PSTR(s) (s)
#define __FlashStringHelper char

#elif defined(ARDUINO)
// Progmem is Arduino-specific
#include <avr/pgmspace.h>

#else
// RPi maybe?
typedef char const char;
typedef uint16_t prog_uint16_t;
#define PSTR(x) (x)
#define printf_P printf
#define strlen_P strlen
#define PROGMEM
#define pgm_read_word(p) (*(p)) 
#endif

#endif // __RF24_CONFIG_H__
// vim:ai:cin:sts=2 sw=2 ft=cpp
