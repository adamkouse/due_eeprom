/*
  DueEeprom.cpp - Arduino Due EEPROM library
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


/** Includes */
#include "Arduino.h"
#include "DueEeprom.h"

/** Definitions */

/** Constructors */
DueEepromClass::DueEepromClass(void)
{
    pin = 255;
}

/** User API */
void DueEepromClass::begin(void)
{
    pin = DUE_EEPROM_PIN;
    pinMode(pin, INPUT); // Hack to work around bug where the peripheral clock isn't turned on by default
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);

    return;
}

void DueEepromClass::begin(uint8_t p)
{
    pin = p;
    pinMode(pin, INPUT); // Hack to work around bug where the peripheral clock isn't turned on by default
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);

    return;
}

void DueEepromClass::begin(int p)
{
    begin((uint8_t) p);

    return;
}

char DueEepromClass::read(int address)
{
    uint8_t data;
    uint32_t serial_started = false;

    if (pin == 255) {
        return 0;
    }

    /* Set up the serial port to communicate with the 16u2 if necessary */
    if (!pmc_is_periph_clk_enabled(ID_UART)) {
        Serial.begin(DUE_EEPROM_SPEED);
        serial_started = true;
    }

    /* Make sure the outgoing buffer is empty */
    Serial.flush();

    /* Set the 16u2 control pin so it starts listening */
    digitalWrite(pin, HIGH);
    
    /* Send our read command then the address to read from */
    Serial.write(DUE_EEPROM_READ);
    Serial.write((uint8_t) (address >> 8));     /* MSB */
    Serial.write((uint8_t) (address & 0xff));   /* LSB */
    Serial.write((uint8_t) 0);             /* Garbage to make read and write transfer same number of bytes */

    /* Wait for all of our data to be transmitted before setting the control pin low */
    Serial.flush();

    /* Clear the 16u2 control pin so it stops listening */
    digitalWrite(pin, LOW);
   
    while (!Serial.available());
    data = Serial.read();

    if (serial_started) {
        Serial.end();
    }

	return data;
}

void DueEepromClass::write(int address, uint8_t data)
{
    uint32_t serial_started = false;

    if (pin == 255) {
        return;
    }

    /* Set up the serial port to communicate with the 16u2 if necessary */
    if (!pmc_is_periph_clk_enabled(ID_UART)) {
        Serial.begin(DUE_EEPROM_SPEED);
        serial_started = true;
    }
    
    /* Make sure the outgoing buffer is empty */
    Serial.flush();

    /* Set the 16u2 control pin so it starts listening */
    digitalWrite(pin, HIGH);
    
    /* Send our read command then the address to read from */
    Serial.write(DUE_EEPROM_WRITE);
    Serial.write((uint8_t) (address >> 8));     /* MSB */
    Serial.write((uint8_t) (address & 0xff));   /* LSB */
    Serial.write(data);             /* Data */

    /* Wait for all of our data to be transmitted before setting the control pin low */
    Serial.flush();

    /* Clear the 16u2 control pin so it stops listening */
    digitalWrite(pin, LOW);

    if (serial_started) {
        Serial.end();
    }

    return;
}

DueEepromClass DueEeprom = DueEepromClass();