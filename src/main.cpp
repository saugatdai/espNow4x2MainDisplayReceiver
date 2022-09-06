/*
 * "THE BEER-WARE LICENSE" (Revision 42):
 * regenbogencode@gmail.com wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return
 */
#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif ESP32
#include <WiFi.h>
#endif
#include "ESPNowW.h"

#include <DMD2.h>
#include <fonts/Arial_Black_16.h>

#define SECRET_DATA "PRABHU"

SPIDMD dmd(2, 4); // width, height
const uint8_t *FONT = Arial_Black_16;
String rows[4] = {"", "", "", ""};

struct TokenData
{
  char category[32];
  int tokenNumber;
  int counterNumber;
  char secretString[15];
} tokenData;

boolean light = false;

void displayTokenAtDMD()
{
    char line[15] = "";
    rows[3] = rows[2];
    rows[2] = rows[1];
    rows[1] = rows[0];
    if (strcmp(tokenData.category,"!") == 0)
    {
        Serial.println("No Category");
        sprintf(line, "%04d-%02d", tokenData.tokenNumber, tokenData.counterNumber);
    }
    else
    {
        Serial.println("With Category");
        sprintf(line, "%s%03d-%02d", tokenData.category, tokenData.tokenNumber, tokenData.counterNumber);
    }
    rows[0] = line;

    dmd.drawString(0, 1, "       ");
    dmd.drawString(0, 1, rows[0].substring(0, 4));
    dmd.drawString(40, 1, rows[0].substring(4));
    dmd.drawString(0, 17, "      ");
    dmd.drawString(0, 17, rows[1].substring(0, 4));
    dmd.drawString(40, 17, rows[1].substring(4));
    dmd.drawString(0, 33, "      ");
    dmd.drawString(0, 33, rows[2].substring(0, 4));
    dmd.drawString(40, 33, rows[2].substring(4));
    dmd.drawString(0, 49, "      ");
    dmd.drawString(0, 49, rows[3].substring(0, 4));
    dmd.drawString(40, 49, rows[3].substring(4));
}

void onRecv(unsigned char *mac_addr, unsigned char *data, unsigned char data_len)
{
    memcpy(&tokenData, data, sizeof(tokenData));

    Serial.println("================================");
    Serial.println(tokenData.category);
    Serial.println(tokenData.tokenNumber);
    Serial.println(tokenData.counterNumber);
    Serial.println(tokenData.secretString);
    light = !light;
    digitalWrite(2, light);
    Serial.println("================================");

    if(strcmp(tokenData.secretString, SECRET_DATA) == 0){
        displayTokenAtDMD();
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("ESPNow receiver Demo");
    pinMode(2, OUTPUT);
#ifdef ESP8266
    WiFi.mode(WIFI_STA); // MUST NOT BE WIFI_MODE_NULL
#elif ESP32
    WiFi.mode(WIFI_MODE_STA);
#endif
    WiFi.disconnect();
    ESPNow.init();
    ESPNow.reg_recv_cb(onRecv);
    dmd.begin();
    dmd.setBrightness(5);
    dmd.selectFont(FONT);
    dmd.drawString(3, 1, "Ready");
}

void loop() {}