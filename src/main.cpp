#include <Arduino.h>

#include <SPI.h>
#include "mcp_can.h"

long unsigned int rxId;
unsigned long rcvTime;

unsigned char len = 0;
unsigned char buf[8];

const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup()
{
    Serial.begin(9600);
    
     while (CAN_OK != CAN.begin(CAN_200KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Module Failed to Initialized");
        Serial.println("Retrying....");
        delay(200);
    }    
    Serial.println("CAN BUS Module Initialized!");
    Serial.println("Time\t\tPGN\t\tUS1\tUS2\tUS3\tUS4");    
}

/*
@description: convert two bytes into one word (16 bits)
@param: .......
*/
uint16_t combineUSData(uint8_t lb, uint8_t hb )  {
    uint16_t value = (hb << 8) | lb ;
     return value;
}

void loop()
{
    
    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        rcvTime = millis();
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        rxId= CAN.getCanId();
        Serial.print(rcvTime);
        Serial.print("\t\t");
        Serial.print("0x");
        Serial.print(rxId, HEX);
        Serial.print("\t\t");
        Serial.print(buf[0]);
        uint16_t us1 =  combineUSData(buf[0], buf[1]);
        Serial.print(us1, DEC);  
        Serial.print("\t");  
        uint16_t us2 =  combineUSData(buf[2], buf[3]);
        Serial.print(us2, DEC);  
        Serial.print("\t");  
        uint16_t us3 =  combineUSData(buf[4], buf[5]);
        Serial.print(us3, DEC);  
        Serial.print("\t");  
        uint16_t us4 =  combineUSData(buf[6], buf[7]);
        Serial.print(us4, DEC);  
        Serial.print("\t");  
 
        Serial.println();
    } 
}
