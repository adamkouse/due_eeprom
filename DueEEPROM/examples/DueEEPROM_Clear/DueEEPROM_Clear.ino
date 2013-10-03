/*
  DueEEPROM Clear

  Created by Adam Kouse
  September, 2013

  A short example showing how to clear the EEPROM.

  The SAM3X that makes up the heart of the DUE doesn't have any EEPROM itself, but
  the Atmel 16U2 that is the bridge between the UART interface on the DUE and the
  USB programming port does. This library works by setting the pin given to
  DueEeprom.begin() HIGH when it wants the 16U2 to perform an EEPROM operation and
  back to LOW when it is done. If no pin is given, it uses pin 90. When the pin is
  driven HIGH, the 16U2 intercepts the incoming serial data and doesn't pass it on
  to the computer. Instead it decodes whether the incoming command is a read or
  write, the address to perform the action on and, in the case of a write, the data
  to put into EEPROM.
  
  When using this library, keep in mind that EEPROM operations share the same serial
  lines as pins 0 and 1 (TX0 and RX0) and potentially also communication to and
  from an attached computer. It also depends on changes to some files in the IDE and
  at a minimum soldering some header pins in the four 16U2 breakout holes next to the
  power jack. They're the bare ones that come pre-filled with solder.

  This code is in the public domain.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#include <DueEeprom.h>


void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  DueEeprom.begin(12);
  
  // Write 255 (the default value) to all the EEPROM bytes.
  for (int i = 0; i < 512; i++) {
    DueEeprom.write(i, 255);
  }
    
  // turn the LED on when we're done
  digitalWrite(13, HIGH);
}

void loop()
{
}
