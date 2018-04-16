# LoRaWanSensors
2 LoRaWAN Sensors and a central receiver

This is the code to run 2 sensors over LoRaWAN to a central receiver. Eash piece is based on the following LoRa Radio part: https://learn.adafruit.com/adafruit-rfm69hcw-and-rfm96-rfm95-rfm98-lora-packet-padio-breakouts connected to a Wemos D1 Mini. I used them only because they are cheap, and I do not use the WiFi portion. 


## LoRa_Rx

This is the central receiver. Both sensors send data to this device. It outputs the data in InfluxDB Line Protocol because I connected it to a serial port on a Pine-64 board and Telegraf picks it up there. 

## LoRa_Tx

This is a temperature sensor. It uses a Melexis MLX90614 non-contact sensor to read ambient and "object" temperature. See here: https://learn.adafruit.com/using-melexis-mlx90614-non-contact-sensors/wiring-and-test. These measurements are sent over LoRAWan to the central receiver mentioned above. 

## LoRa_Rad

This is a Radiation monitor/Geiger Counter sensor based on a part from SparkFun: https://www.sparkfun.com/products/14209
