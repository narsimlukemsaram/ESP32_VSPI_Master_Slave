// using VSPI SPI
#include <SPI.h>

#define SERIAL_USB_BAUD 1000000
#define VSPI_MISO   MISO
#define VSPI_MOSI   MOSI
#define VSPI_SCLK   SCK
#define VSPI_SS     SS

static const int spiClk = 1000000; // 1 MHz

//uninitalised pointers to SPI objects
SPIClass * vspi = NULL;

// #define LED 2
int LED = LED_BUILTIN;

void setup() {
  //initialise instance of the SPIClass attached to VSPI
  // delay(2000);
  Serial.begin(SERIAL_USB_BAUD);
  vspi = new SPIClass(VSPI);
  
  //clock miso mosi ss
  //initialise vspi with default pins
  //SCLK = 5, MISO = 21, MOSI = 19, CS/SS = 33
  // vspi->begin();
  // vspi->begin(VSPI);
  vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS);
  //set up slave select pins as outputs as the Arduino API
  //doesn't handle automatically pulling SS low
  pinMode(VSPI_SS, OUTPUT); //VSPI SS
  // pinMode(vspi->pinSS(), OUTPUT);
    pinMode(LED, OUTPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {
  vspi_send_command();
  delay(100);
}

void vspi_send_command() {
  byte data_on = 0b00000001; // data 1 to turn on LED of slave
  byte data_off = 0b0000000; // data 0 to turn off LED of slave
  
  Serial.println("Sending command to activate LED HIGH ");
  digitalWrite(LED, HIGH);
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(VSPI_SS, LOW);
  vspi->transfer(data_on);
  digitalWrite(VSPI_SS, HIGH);
  vspi->endTransaction();

  delay(1000);
  
  Serial.println("Sending command to activate LED LOW ");
  digitalWrite(LED, LOW);
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(VSPI_SS, LOW);
  vspi->transfer(data_off);
  digitalWrite(VSPI_SS, HIGH);
  vspi->endTransaction();

  delay(1000);
}