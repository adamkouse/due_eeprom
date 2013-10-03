/*
  DueEeprom Interactive

  Created by Adam Kouse
  September, 2013
  
  A short example of reading and writing to any of the 512 bytes of EEPROM available.

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

/*
  Get a valid EEPROM address from the user. The 16U2 has 512 bytes of EEPROM.

  Returns an integer between 0 and 511 inclusive.
*/
int getAddress(void) {
  int addr;

  Serial.println("Address: ");
  while(!Serial.available());
  addr = Serial.parseInt();
  while (addr < 0 || addr > 511) {
    Serial.println("Address is out of range. Must be between (0, 512].");
    Serial.println("Address: ");
    while(!Serial.available());
    addr = Serial.parseInt();
  }
  
  return addr;
}

/*
  Reads and prints one byte of data from an address in EEPROM.
*/
void readData(void)
{
  int addr;
  
  addr = getAddress();
  Serial.print("Data: ");
  Serial.println(DueEeprom.read(addr));

  return;
}

/*
  Reads each address from EEPROM and prints it out in a nice format.
*/
void dumpData(void)
{
  for (int i = 0; i < 512; i++) {
    if (i % 16 == 0) {
      Serial.println("");
      Serial.print("0x");
      Serial.print(i, HEX);
      Serial.print("\t");
    }

    Serial.print(DueEeprom.read(i), HEX);
    Serial.print("  ");
  }
  
  Serial.println("");

  return;
}

/*
  Writes one byte of data to a specified address in EEPROM
*/
void writeData(void)
{
  int addr;
  byte data;

  addr = getAddress();
  Serial.println("Data to write: ");
  while (!Serial.available());
  data = Serial.read();
  DueEeprom.write(addr, data);
  
  return;
}

/*
  Prints out the user menu
*/
void printMenu(void)
{
  Serial.println("What would you like to do?");
  Serial.println("(r)ead data");
  Serial.println("(p)rint all EEPROM memory");
  Serial.println("(w)rite data");
  
  return;
}

void setup(void)
{
  DueEeprom.begin(12);
  Serial.begin(9600);
  printMenu();
}

void loop(void)
{
  while(!Serial.available());
  switch(Serial.read()) {
    case 'r':
    case 'R':
      readData();
      break;
    case 'p':
    case 'P':
      dumpData();
      break;
    case 'w':
    case 'W':
      writeData();
      break;
    default:
      Serial.println("Unrecognized command.");
      delay(500);
      break;
  }

  printMenu();
}








