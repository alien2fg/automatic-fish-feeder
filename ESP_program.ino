#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ESP32Servo.h>
#include <time.h> // Dodaj bibliotekę do obsługi czasu
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define API_KEY ""
#define DATABASE_URL ""

#define PIN_SG90 26 // Output pin for servo

Servo servo;
FirebaseData fbdo;
bool signupOK = false;
FirebaseAuth auth;
FirebaseConfig config;
WiFiUDP wifiUDP;
FirebaseData feed, timer;
String Str[3] = {"00:00", "00:00", "00:00"};
int feednow = 0;
unsigned long sendDataPrevMillis = 0; // Variable to track time for sending data

void setup()
{
    Serial.begin(115200);
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

    configTime(0, 0, "pool.ntp.org"); // Initialize time with NTP servers

    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;

    // Firebase authentication and initialization
    if (Firebase.signUp(&config, &auth, "", "")){
        Serial.println("Firebase sign up successful");
        signupOK = true;
    }
    else{
        Serial.println("Firebase sign up failed");
        Serial.printf("Error message: %s\n", config.signer.signupError.message.c_str());
    }

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    // Servo initialization
    servo.setPeriodHertz(50);          // PWM frequency for SG90
    servo.attach(PIN_SG90, 500, 2400); // Servo angle limits
}

void loop()
{
    time_t now;
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);

    int currentHour = timeinfo.tm_hour;
    int currentMinute = timeinfo.tm_min;

    String currentTime = String(currentHour) + ":" + (currentMinute < 10 ? "0" : "") + String(currentMinute);
    Serial.println("Current time: " + currentTime);

    // Check if Firebase is ready and it's time to send data
    if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
    {
        sendDataPrevMillis = millis(); // Update the time tracker
         
        // Retrieve feednow value from Firebase
        Firebase.getInt(feed, "/feednow");
        feednow = feed.to<int>();
        Serial.println("Feednow value: " + String(feednow));

        if (feednow == 1)
        { // Direct Feeding
            servo.writeMicroseconds(1000); // Rotate clockwise
            delay(3000);                   // Rotate for n microseconds (adjust as needed)
            servo.writeMicroseconds(1500); // Stop rotation
            feednow = 0;
            Firebase.setInt(feed, "/feednow", feednow);
            Serial.println("Fed");
        }
        else
        { // Scheduled Feeding
            for (int i = 0; i < 3; i++)
            {
                String path = "times/time" + String(i);
                Firebase.getString(timer, path);
                String stimer = timer.stringData();
                Str[i] = stimer.substring(0, 5); // Extract HH:mm from the timer string
                Serial.print("Timer ");
                Serial.print(i);
                Serial.print(" path: ");
                Serial.println(path);
                Serial.print("Timer ");
                Serial.print(i);
                Serial.print(" value: ");
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

        // Reset Str array
        for (int i = 0; i < 3; i++)
        {
            Str[i] = "00:00";
        }
    }
}
