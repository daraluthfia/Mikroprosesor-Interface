#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definisikan pin untuk sensor DHT
#define DHTPIN 2     
#define DHTTYPE DHT11   // Ganti dengan DHT22 jika menggunakan sensor DHT22

// Inisialisasi objek DHT
DHT dht(DHTPIN, DHTTYPE);

// Inisialisasi LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); // Alamat I2C bisa bervariasi, pastikan sesuai dengan modul Anda

void setup() {
  // Mulai serial monitor
  Serial.begin(9600);
  
  // Mulai sensor DHT
  dht.begin();
  
  // Inisialisasi LCD
  lcd.init(); // Ganti lcd.begin() dengan lcd.init()
  lcd.backlight();
}

void loop() {
  // Tunggu beberapa detik antara pembacaan
  delay(2000);
  
  // Membaca suhu dan kelembaban
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Baca dalam Celsius
  
  // Cek apakah pembacaan gagal dan keluar jika ya
  if (isnan(h) || isnan(t)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    return;
  }

  // Tampilkan hasil di serial monitor
  Serial.print("Kelembaban: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Suhu: ");
  Serial.print(t);
  Serial.println(" *C");

  // Tampilkan hasil di LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Kelembaban: ");
  lcd.print(h);
  lcd.print(" %");
  
  lcd.setCursor(0, 1);
  lcd.print("Suhu: ");
  lcd.print(t);
  lcd.print(" *C");
}
