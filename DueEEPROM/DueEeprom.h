/*
  DueEeprom.h - Arduino Due EEPROM library
  Adam Kouse (adam [dot] kouse [@] gmail)
  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _DUE_EEPROM_H_
#define _DUE_EEPROM_H_


/** Includes */
#include <inttypes.h>

/** Macros */
#define DUE_EEPROM_PIN      0x5a
#define DUE_EEPROM_READ     0xd4
#define DUE_EEPROM_WRITE    0xd5
#define DUE_EEPROM_SPEED    9600

/** Classes */
class DueEepromClass
{
  public:
    DueEepromClass(void);
    void begin(void);
    void begin(uint8_t p);
    void begin(int p);
    char read(int address);
    void write(int address, uint8_t data);

  private:
    uint8_t pin; /* 16u2 Control Pin */
};

extern DueEepromClass DueEeprom;

#endif /* _DUE_EEPROM_H_ */
