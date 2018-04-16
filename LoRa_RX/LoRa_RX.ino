
// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX

#include <SPI.h>
#include <RH_RF95.h>
#include <ESP8266WiFi.h>

// Wemos D1 Mini ...
#define RFM95_CS      D1
#define RFM95_IRQ     D2
#define RFM95_RST     D3
//
// This is teh receiver, so it receives from anyone, others send to this address.
#define MY_ADDRESS     1

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 434.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_IRQ);

// Blinky on receipt
#define LED LED_BUILTIN

void setup()
{
  // Why use 9600 when 115200 is faster?
  Serial.begin(115200);
  while (!Serial); {
    delay(1);
  }
  delay(100);
  // we're not using the Wemos WiFi.
  WiFi.mode(WIFI_OFF);

  Serial.println("LoRa RXer!");

  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(100);
  digitalWrite(RFM95_RST, HIGH);
  delay(100);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

void loop()
{
  if (rf95.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    String msgBuff = "iot_sensor,recv_from=LoRa ";
    if (rf95.recv(buf, &len)) {
      digitalWrite(LED, HIGH);
      char *p = (char *)buf;
      char *str;
      char* strAr[3];
      int x = 0;
      // incoming message format: T|R,reading1,reading2
      while ((str = strtok_r(p, ",", &p)) != NULL) {// delimiter is the comma
        strAr[x++] = str;
      }

      String mType = String(strAr[0]);
      double reading1 = String(strAr[1]).toFloat();
      double reading2 = String(strAr[2]).toFloat();

      if (mType == "T") {
        msgBuff += "AmbTempC=";
        msgBuff += String(reading1);
        msgBuff += ",ObjTempC=";
        msgBuff += String(reading2);
        msgBuff += ",AmbTempF=";
        msgBuff += String((reading1 * 1.8) + 32);
        msgBuff += ",ObjTempF=";
        msgBuff += String((reading2 * 1.8) + 32);
      } else {
        msgBuff += "gamma_ray=";
        msgBuff += String(reading1);
        msgBuff += ",dose=";
        msgBuff += String(reading2);
      }
      msgBuff += ",RSSI=";
      msgBuff += String(rf95.lastRssi());
      msgBuff += ".0";
      Serial.println(msgBuff);
      // Send a simple reply
      uint8_t data[] = "Roger that!";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      digitalWrite(LED, LOW);
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
}
