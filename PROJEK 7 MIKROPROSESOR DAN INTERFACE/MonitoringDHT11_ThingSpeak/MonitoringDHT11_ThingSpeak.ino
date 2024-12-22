#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN 2           // Pin data DHT terhubung ke GPIO2 (D4 pada NodeMCU)
#define DHTTYPE DHT11      // Menggunakan DHT11

const char* ssid = "daradella";            // Ganti dengan SSID WiFi Anda
const char* password = "12345678";    // Ganti dengan password WiFi Anda
const char* server = "api.thingspeak.com";      // Server ThingSpeak

String apiKey = "C2F2L6UBGE6WJG4X";        // Masukkan API Key ThingSpeak Anda

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Menghubungkan ke WiFi
  Serial.println("Menghubungkan ke WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTerhubung ke WiFi!");
}

void loop() {
  // Membaca suhu dan kelembaban
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Membaca nilai potensiometer
  int potValue = analogRead(A0); // Baca nilai analog dari potensiometer (0-1023)
  float potPercentage = (potValue / 1023.0) * 100.0; // Mengkonversi ke persentase (0-100%)

  // Cek apakah pembacaan DHT berhasil
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    return;
  }

  // Menampilkan data di Serial Monitor
  Serial.print("Kelembaban: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.println(" *C");
  Serial.print("Nilai Potensiometer: ");
  Serial.print(potValue);
  Serial.print(" (");
  Serial.print(potPercentage);
  Serial.println("%)");

  // Mengirim data ke ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {  // Pastikan WiFi terhubung
    WiFiClient client;

    if (client.connect(server, 80)) {   // Koneksi ke server ThingSpeak
      String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(temperature);
      postStr += "&field2=";
      postStr += String(humidity);
      postStr += "&field3=";
      postStr += String(potPercentage);
      postStr += "\r\n\r\n";

      client.print("POST /update HTTP/1.1\n");
      client.print("Host: " + String(server) + "\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);

      Serial.println("Data dikirim ke ThingSpeak");
    }
    client.stop();
  } else {
    Serial.println("Gagal terhubung ke ThingSpeak");
  }

  delay(20000);  // Mengirim data setiap 20 detik
}