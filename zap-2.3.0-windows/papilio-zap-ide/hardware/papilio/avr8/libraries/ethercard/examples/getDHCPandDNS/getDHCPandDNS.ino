// This demo does web requests via DHCP and DNS lookup.
// 2011-07-05 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <EtherCard.h>

#define REQUEST_RATE 5000 // milliseconds

// ethernet interface mac address
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
// remote website name
char website[] PROGMEM = "google.com";

byte Ethernet::buffer[700];
static long timer;

// called when the client request is complete
static void my_result_cb (byte status, word off, word len) {
  Serial.print("<<< reply ");
  Serial.print(millis() - timer);
  Serial.println(" ms");
  Serial.println((const char*) Ethernet::buffer + off);
}

void setup () {
  delay(3000);
  
  // set up pins to use Ethernet Wing on Wingslot AL
  SPI1_cs_LOC   = WAL6;
  SPI1_sck_LOC  = WAL5;
  SPI1_mosi_LOC = WAL4;
  SPI1_miso_LOC = WAL3;
  
  Serial.begin(57600);
  Serial.println("\n[getDHCPandDNS]");
  
  if (ether.begin(sizeof Ethernet::buffer, mymac, SPI1_cs_LOC, SPI1_sck_LOC, SPI1_mosi_LOC, SPI1_miso_LOC) == 0) 
    Serial.println( "Failed to access Ethernet controller");

  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
  
  ether.printIp("My IP: ", ether.myip);
  // ether.printIp("Netmask: ", ether.mymask);
  ether.printIp("GW IP: ", ether.gwip);
  ether.printIp("DNS IP: ", ether.dnsip);

  if (!ether.dnsLookup(website))
    Serial.println("DNS failed");
  ether.printIp("Server: ", ether.hisip);
  
  timer = - REQUEST_RATE; // start timing out right away
}

void loop () {
    
  ether.packetLoop(ether.packetReceive());
  
  if (millis() > timer + REQUEST_RATE) {
    timer = millis();
    Serial.println("\n>>> REQ");
    ether.browseUrl(PSTR("/foo/"), "bar", website, my_result_cb);
  }
}
