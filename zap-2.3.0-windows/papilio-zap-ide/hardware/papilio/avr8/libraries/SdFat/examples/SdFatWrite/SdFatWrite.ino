/*
 * Write Example
 *
 * This sketch creates a new file and writes 100 lines to the file.
 * No error checks on write in this example.
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
/*
 * Write CR LF to a file
 */
void writeCRLF(SdFile& f) {
  f.write((uint8_t*)"\r\n", 2);
}
/*
 * Write an unsigned number to a file
 */
void writeNumber(SdFile& f, uint32_t n) {
  uint8_t buf[10];
  uint8_t i = 0;
  do {
    i++;
    buf[sizeof(buf) - i] = n%10 + '0';
    n /= 10;
  } while (n);
  f.write(&buf[sizeof(buf) - i], i);
}
/*
 * Write a string to a file
 */
void writeString(SdFile& f, char *str) {
  uint8_t n;
  for (n = 0; str[n]; n++);
  f.write((uint8_t *)str, n);
}

void setup(void) {
  //Move the SPI pins to the desired Wing Slot
  SPI1_mosi_LOC=AL3;	//AL3 for Wing Slot AL
  SPI1_miso_LOC=AL1;	//AL1 for Wing Slot AL
  SPI1_sck_LOC=AL2;	//AL2 for Wing Slot AL

  Serial.begin(9600);
  Serial.println();
  Serial.println("Type any character to start");
  while (!Serial.available());
  
  // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!card.init(SPI_FULL_SPEED,AL4)) error("card.init failed");
  
  // initialize a FAT volume
  if (!volume.init(&card)) error("volume.init failed");
  
  // open the root directory
  if (!root.openRoot(&volume)) error("openRoot failed");

  // create a new file
  char name[] = "WRITE00.TXT";
  for (uint8_t i = 0; i < 100; i++) {
    name[5] = i/10 + '0';
    name[6] = i%10 + '0';
    if (file.open(&root, name, O_CREAT | O_EXCL | O_WRITE)) break;
  }
  if (!file.isOpen()) error ("file.create");
  Serial.print("Writing to: ");
  Serial.println(name);
  
  // write 100 line to file
  for (uint8_t i = 0; i < 100; i++) {
    writeString(file, "line ");
    writeNumber(file, i);
    writeString(file, " millis = ");
    writeNumber(file, millis());
    writeCRLF(file);
  }
  // close file and force write of all data to the SD card
  file.close();
  Serial.println("Done");
}

void loop(void) {}
