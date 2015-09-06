/*
 * This sketch reads and prints the file
 * PRINT00.TXT created by SdFatPrint.pde or
 * WRITE00.TXT created by SdFatWrite.pde
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
#include <SdFatUtil.h>

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
  Serial.println("type any character to start");
  while (!Serial.available());
  Serial.println();
  
  // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!card.init(SPI_FULL_SPEED,AL4)) error("card.init failed");
  
  // initialize a FAT volume
  if (!volume.init(&card)) error("volume.init failed");
  
  // open the root directory
  if (!root.openRoot(&volume)) error("openRoot failed");
  
  // open a file
  if (file.open(&root, "PRINT00.TXT", O_READ)) {
    Serial.println("Opened PRINT00.TXT");
  }
  else if (file.open(&root, "WRITE00.TXT", O_READ)) {
    Serial.println("Opened WRITE00.TXT");    
  }
  else{
    error("file.open failed");
  }
  Serial.println();
  
  // copy file to serial port
  int16_t n;
  uint8_t buf[7];// nothing special about 7, just a lucky number.
  while ((n = file.read(buf, sizeof(buf))) > 0) {
    for (uint8_t i = 0; i < n; i++) Serial.print(buf[i]);
  }
  /* easier way
  int16_t c;
  while ((c = file.read()) > 0) Serial.print((char)c);
  */
  Serial.println("\nDone");
}

void loop(void) {}
