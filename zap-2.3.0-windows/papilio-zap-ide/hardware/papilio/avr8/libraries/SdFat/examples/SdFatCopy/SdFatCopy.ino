/*
 * Copy Example - only runs on chips with 2K or more RAM
 *
 * This sketch copies the file APPEND.TXT, created by the
 * SdFatAppend.pde example, to the file ACOPY.TXT.
 *
 * Modified October 5, 2010 by Jack Gassett for the Papilio Platform
 *
 * Wing Configuration:
 * Connect BPW5002 MicroSD Wing to Wing Slot AL
 * To move the MicroSD Wing to other Wing Slots change all instances of AL to the desired Wing Slot designator. (AH, BL, BH, CL, CH)
 * For more information about the BPW5002 MicroSD Wing visit the project page:
 * http://gadgetforge.gadgetfactory.net/gf/project/bpw5002-usd/
 */
 
#include <SdFat.h>
#include <SdFatUtil.h> // use functions to print strings from flash memory

Sd2Card card;
SdVolume volume;
SdFile root;
SdFile from; // read file
SdFile copy; // write file

// large buffer to test for bugs. 512 bytes runs much faster.
char buf[600];

// store error strings in flash to save RAM
#define error(s) error_P(PSTR(s))

void error_P(const char* str) {
  PgmPrint("error: ");
  SerialPrintln_P(str);
  if (card.errorCode()) {
    PgmPrint("SD error: ");
    Serial.print(card.errorCode(), HEX);
    Serial.print(',');
    Serial.println(card.errorData(), HEX);
  }
  while(1);
}

void setup(void) {
  //Move the SPI pins to the desired Wing Slot
  SPI1_mosi_LOC=AL3;	//AL3 for Wing Slot AL
  SPI1_miso_LOC=AL1;	//AL1 for Wing Slot AL
  SPI1_sck_LOC=AL2;	//AL2 for Wing Slot AL
  
  Serial.begin(9600);
  Serial.println();
  PgmPrintln("Type any character to start");
  while (!Serial.available());
  
  PgmPrint("FreeRam: ");
  Serial.println(FreeRam());
  
  // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!card.init(SPI_FULL_SPEED,AL4)) error("card.init failed");

  // initialize a FAT volume
  if (!volume.init(&card)) error("volume.init failed");

  // open the root directory
  if (!root.openRoot(&volume)) error("openRoot failed");
  
  strcpy_P(buf, PSTR("APPEND.TXT"));
  // open for read
  if (!from.open(&root, buf, O_READ)) {
    PgmPrint("Can't open "); 
    Serial.println(buf);
    PgmPrintln("Run the append example to create the file.");
    error("from.open failed");
  }
  strcpy_P(buf, PSTR("ACOPY.TXT"));
  // create if needed, truncate to zero length, open for write
  if (!copy.open(&root, buf, O_CREAT | O_TRUNC | O_WRITE)) {
    error("copy.open failed");
  }
  // count for printing periods
  uint16_t p = 0;
  int16_t n;  
  while ((n = from.read(buf, sizeof(buf))) > 0) {
    if (copy.write(buf, n) != n) error("write failed");
    // print progress periods
    if (!(p++ % 25)) Serial.print('.');
    if (!(p % 500)) Serial.println();
  }
  Serial.println();
  if (n != 0) error ("read");
  // force write of directory entry and last data
  if (!copy.close()) error("copy.close failed");
  PgmPrintln("Copy done.");
}

void loop(void) {}
