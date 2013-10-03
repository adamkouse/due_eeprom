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


/** Includes */
#include "Due-eeprom.h"

#include "Lib/LightweightRingBuff.h"

#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include <LUFA/Drivers/Peripheral/Serial.h>


/** Functions */
/* Set up everything for EEPROM operations */
void setupEeprom(void)
{
    uint8_t pins;

    eeprom_op = 0;
    RingBuffer_InitBuffer(&USARTtoEEPROM_Buffer);

    /*  Enable the pullup. If this pin is grounded it'll read low and
        EEPROM functions will be disabled. This is useful if you have
        a sketch run amok or you accidentally tell your control pin
        to be driven high and it causes all serial data to be dumped
        to EEPROM, thus effectively disabling new sketch uploading. */
    PORTB |= (1 << DUE_EEPROM_DISABLE_PIN);
    /*  There MUST be at least a nop between setting the pullups and
        reading the value. See datasheet page figure 12.4 */
    asm("nop");
    pins = PINB;
    if (pins & (1 << DUE_EEPROM_DISABLE_PIN)) {
        /* All PortB pins are set as inputs by default, no pullup */
        /* Set up the pin change interrupt */
        PCICR |= (1 << PCIE0);
        eepromInterruptEnable();
    }

    return;
}

/* Enable the eeprom control pin interrupt */
void eepromInterruptEnable(void)
{
    PCMSK0 |= (1 << DUE_EEPROM_PIN);

    return;
}

/* Disable the eeprom control pin interrupt */
void eepromInterruptDisable(void)
{
    PCMSK0 &= ~(1 << DUE_EEPROM_PIN);

    return;
}

/* Handle any EEPROM requests we've received */
void serviceEepromRequest(void)
{
    if (RingBuffer_IsEmpty(&USARTtoEEPROM_Buffer)) {
        return;
    }

    RingBuff_Count_t count = RingBuffer_GetCount(&USARTtoEEPROM_Buffer);
    if (count < 4) {
        return; // We don't have enough bytes to perform an operation yet
    }

    RingBuff_Data_t data = RingBuffer_Remove(&USARTtoEEPROM_Buffer);
    uint16_t addr;

    switch (data) {
        case DUE_EEPROM_READ:
            /* Get the address to read from EEPROM */
            addr = (RingBuffer_Remove(&USARTtoEEPROM_Buffer) << 8); /* MSB */
            addr |= RingBuffer_Remove(&USARTtoEEPROM_Buffer);       /* LSB */
            data = RingBuffer_Remove(&USARTtoEEPROM_Buffer);        /* Placeholder Byte */
            data = eeprom_read_byte((uint8_t *) addr);
            Serial_TxByte(data);
            break;
        case DUE_EEPROM_WRITE:
             /* Get the address to write to EEPROM */
            addr = RingBuffer_Remove(&USARTtoEEPROM_Buffer) << 8; /* MSB */
            addr |= RingBuffer_Remove(&USARTtoEEPROM_Buffer);     /* LSB */
            data = RingBuffer_Remove(&USARTtoEEPROM_Buffer);      /* Data to Write */
            eeprom_write_byte((uint8_t *) addr, data);
            break;
        default:
            break;
            /* We somehow got some bogus data. Discard it. */
    }

    return;
}

/* ISR to set how the serial ISR should handle incoming bytes */
ISR(PCINT0_vect, ISR_BLOCK)
{
    if (PINB & (1 << DUE_EEPROM_PIN)) {
        eeprom_op = 1; // Serial data should be put into USARTtoEEPROM_Buffer
    } else {
        eeprom_op = 0; // Serial data should be put into USARTtoUSB_Buffer
    }
}

