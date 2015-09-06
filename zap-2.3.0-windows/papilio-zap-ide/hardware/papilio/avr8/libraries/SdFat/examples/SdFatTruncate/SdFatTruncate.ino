/*
 * Truncate Example
 *
 * This sketch shows how to use truncate() to remove the last 
 * half of the file created by the SdFatAppend.pde example.
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
SdFile file;

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
  
  // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!card.init(SPI_FULL_SPEED,AL4)) error("card.init failed");
  
  // initialize a FAT volume
  if (!volume.init(&card)) error("volume.init failed");

  // open the root directory
  if (!root.openRoot(&volume)) error("openRoot failed");
  
  char name[] = "APPEND.TXT";
  // open for read and write
  if (!file.open(&root, name, O_RDWR)) {
    PgmPrint("Can't open "); 
    Serial.println(name);
    PgmPrintln("Run the append example to create the file.");
    error("file.open failed");
  }
  // seek to middle of file
  if (!file.seekSet(file.fileSize()/2)) error("file.seekSet failed");
  // find end of line
  int16_t c;
  while ((c = file.read()) > 0 && c != '\n');
  if (c < 0) error("file.read failed");
  // truncate at current position
  if (!file.truncate(file.curPosition())) error("file.truncate failed");
  Serial.print(name);
  PgmPrintln(" truncated.");
}

void loop(void) {}
