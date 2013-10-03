/*  
    Copyright 2013  Adam Kouse (adam [dot] kouse [@] gmail)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef _DUE_EEPROM_H_
#define _DUE_EEPROM_H_

/** Includes */
#include "Lib/LightweightRingBuff.h"


/** Globals */
/* Buffer to store the incoming EEPROM data */
RingBuff_t USARTtoEEPROM_Buffer;

/* Variable to keep track of which buffer to put incoming serial data into */
volatile uint8_t eeprom_op;

/** Macros */
#define DUE_EEPROM_PIN          6 // PB6
#define DUE_EEPROM_DISABLE_PIN  4 // PB4
#define DUE_EEPROM_READ         0xD4
#define DUE_EEPROM_WRITE        0xD5

/** Function Prototypes */
/* Set up everything for EEPROM operations */
void setupEeprom(void);

/* Enable the eeprom control pin interrupt */
void eepromInterruptEnable(void);

/* Disable the eeprom control pin interrupt */
void eepromInterruptDisable(void);

/* Handle any EEPROM requests we've received */
void serviceEepromRequest(void);

#endif /* _DUE_EEPROM_H_ */
