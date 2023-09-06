#include <ESP32Servo.h>
#include "FirebaseESP32.h"
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define PIN_SG90 26 // Output pin for servo

Servo servo;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pl.pool.ntp.org", 3600);
FirebaseData timer, feed;
String stimer;
String Str[3] = {"00:00", "00:00", "00:00"};
int feednow = 0;

void setup()
{
    Serial.begin(9600);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    // Set time zone and NTP servers
    configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "tempus1.gum.gov.pl", "pl.pool.ntp.org");

    timeClient.begin();
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    servo.setPeriodHertz(50);          // PWM frequency for SG90
    servo.attach(PIN_SG90, 500, 2400); // Servo angle limits
}

void loop()
{
    timeClient.update();
    String currentTime = String(timeClient.getHours()) + ":" + (timeClient.getMinutes() < 10 ? "0" : "") + String(timeClient.getMinutes());
    Serial.println("Current time: " + currentTime);

    Firebase.getInt(feed, "feednow");
    feednow = feed.to<int>();
    Serial.println("Feednow value: " + String(feednow));

    if (feednow == 1)
    {                                  // Direct Feeding
        servo.writeMicroseconds(1000); // Rotate clockwise
        delay(3000);                   // Rotate for n microseconds (adjust as needed)
        servo.writeMicroseconds(1500); // Stop rotation
        feednow = 0;
        Firebase.setInt(feed, "feednow", feednow);
        Serial.println("Fed");
    }
    else
    { // Scheduled Feeding
        for (int i = 0; i < 3; i++)
        {
            String path = "/times/time" + String(i);
            Firebase.getString(timer, path);
            stimer = timer.stringData();
            Str[i] = stimer.substring(0, 5); // Extract HH:mm from the timer string
            Serial.print("Timer " + String(i) + " path: ");
            Serial.println(path);
            Serial.print("Timer " + String(i) + " value: ");
            Serial.println(stimer);
        }
        if (Str[0] == currentTime || Str[1] == currentTime || Str[2] == currentTime)
        {
            servo.writeMicroseconds(1000); // Rotate clockwise
            delay(3000);                   // Rotate for n microseconds (adjust as needed)
            servo.writeMicroseconds(1500); // Stop rotation
            Serial.println("Success");
            delay(60000); // Delay for one minute before checking again
        }
    }

    Str[0] = "00:00";
    Str[1] = "00:00";
    Str[2] = "00:00";
}
