#include <ESP32SPISlave.h>

ESP32SPISlave slave;

#define SERIAL_USB_BAUD 1000000

static constexpr uint32_t BUFFER_SIZE {32};
uint8_t spi_slave_tx_buf[BUFFER_SIZE];
uint8_t spi_slave_rx_buf[BUFFER_SIZE];

// #define LED 2
int LED = LED_BUILTIN;

void setup() {
    Serial.begin(SERIAL_USB_BAUD);
    delay(2000);
    pinMode(LED, OUTPUT);
    // begin() after setting
    //SCLK = 5, MISO = 21, MOSI = 19, CS/SS = 33
    slave.setDataMode(SPI_MODE0);
    // slave.begin();
    slave.begin(VSPI);   // you can use VSPI like this

    // clear buffers
    memset(spi_slave_tx_buf, 0, BUFFER_SIZE);
    memset(spi_slave_rx_buf, 0, BUFFER_SIZE);
}

void loop() {
    // block until the transaction comes from master
    slave.wait(spi_slave_rx_buf, spi_slave_tx_buf, BUFFER_SIZE);

    // if transaction has completed from master,
    // available() returns size of results of transaction,
    // and buffer is automatically updated
    char data;
    while (slave.available()) {
        // show received data
         Serial.print("Command Received: ");
         Serial.println(spi_slave_rx_buf[0]);
         data = spi_slave_rx_buf[0];
         slave.pop();
    }
    
    if(data == 1)
    {
        Serial.println("Setting LED active HIGH ");
        digitalWrite(LED, HIGH);
    }
    else if(data == 0)
    {
        Serial.println("Setting LED active LOW ");
        digitalWrite(LED, LOW);
    }
     Serial.println("");
}