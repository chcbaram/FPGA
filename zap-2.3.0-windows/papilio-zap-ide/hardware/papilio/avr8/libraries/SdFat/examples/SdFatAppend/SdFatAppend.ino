/*
 * Append Example
 *
 * This sketch shows how to use open for append and the Arduino Print class
 * with SdFat.
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
  PgmPrint("Appending to: ");
  Serial.println(name);
  
  // clear write error
  file.writeError = false;
  
  for (uint8_t i = 0; i < 100; i++) {
    // O_CREAT - create the file if it does not exist
    // O_APPEND - seek to the end of the file prior to each write
    // O_WRITE - open for write
    if (!file.open(&root, name, O_CREAT | O_APPEND | O_WRITE)) {
      error("open failed");
    }
    // print 100 lines to file
    for (uint8_t j = 0; j < 100; j++) {
      file.print("line ");
      file.print(j, DEC);
      file.print(" of pass ");
      file.print(i, DEC);
      file.print(" millis = ");
      file.println(millis());
    }
    if (file.writeError) error("write failed");
    if (!file.close()) error("close failed");
    if (i > 0 && i%25 == 0)Serial.println();
    Serial.print('.');
  }
  Serial.println();
  Serial.println("Done");
}
void loop(void){}
