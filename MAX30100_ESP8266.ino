// Library yang diperlukan
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS 1000

#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>

#include <ESP8266WiFi.h>
#include <Wire.h>

char auth[] = "Isi dengan token dari blynk";
char ssid[] = "Nama Wifi";
char pass[] = "Password Wifi";
 
// Connections : SCL PIN - D1 , SDA PIN - D2 , INT PIN - D0

// Membuat objek dari PulseOximeter bernama "pox"
PulseOximeter pox;

// Mendeklarasikan variabel untuk pembacaan detak jantung dan jumlah oksigen dalam darah
float BPM, SpO2;

// Inisialisasi variabel sebagai awal pembacaan sensor --> 0
uint32_t tsLastReport = 0;

// Fungsi ketika detak terdeteksi
void onBeatDetected()
{
    Serial.println("Beat Detected!");
}
 
void setup()
{
    // Kecepatan pengiriman dan penerimaan data melalui port serial
    Serial.begin(115200);
    pinMode(16, OUTPUT);
    
    // Aplikasi blynk bersiap memulai
    Blynk.begin(auth, ssid, pass);
 
    Serial.print("Initializing Pulse Oximeter..");

    // Jika sensor tidak berhasil mendeteksi --> failed
    if (!pox.begin())
    {
         Serial.println("FAILED");
         for(;;);
    }
    // Jika sensor berhasil mendeteksi --> success
    else
    {
         Serial.println("SUCCESS");
         pox.setOnBeatDetectedCallback(onBeatDetected);
    }
}
 
void loop()
{
    // Sensor akan terus meng-update data
    pox.update();
    
    // Aplikasi blynk bekerja
    Blynk.run();
 
    // Variabel untuk mendapatkan pembacaan detak jantung dan jumlah oksigen dalam darah
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();

    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        // Serial monitor menampilkan hasil pembacaan detak jantung dan jumlah oksigen dalam darah
        Serial.print("Heart rate:");
        Serial.print(BPM);
        Serial.print(" bpm / SpO2:");
        Serial.print(SpO2);
        Serial.println(" %");
 
        // Aplikasi blynk menampilkan hasil pembacaan detak jantung dan jumlah oksigen dalam darah
        Blynk.virtualWrite(V7, BPM);
        Blynk.virtualWrite(V8, SpO2);
  
        tsLastReport = millis();
    }
}
