#include <Arduino.h>
#include <ModbusMaster.h>
#include <HardwareSerial.h>

// Tentukan pin untuk RX dan TX
const int RX_PIN = 4; // Pin untuk menerima data
const int TX_PIN = 0; // Pin untuk mengirim data

HardwareSerial rs485Serial(1);  // Gunakan UART1

ModbusMaster node;

void setup() {
  Serial.begin(9600);      // Memulai komunikasi serial dengan komputer
  rs485Serial.begin(4800, SERIAL_8N1, RX_PIN, TX_PIN); // Baud rate sesuai sensor
  node.begin(1, rs485Serial); // Inisialisasi Modbus dengan alamat slave 1
  Serial.println("Modbus RS485 Reader is ready.");
}

void loop() {
  uint8_t result;
  float registerValues[9]; // Array untuk menyimpan nilai dari register 0x0001 sampai 0x0008

  // Membaca register dari alamat 0x0001 sampai 0x0008
  for (uint8_t i = 0; i < 9; i++) {
    result = node.readHoldingRegisters(0x0000 + i, 1); // Membaca 1 register

    if (result == node.ku8MBSuccess) {
      uint16_t rawValue = node.getResponseBuffer(0);
      registerValues[i] = rawValue / 10.0; // Bagi dengan 10 untuk mendapatkan nilai yang benar
    } else {
      Serial.print("Error reading Register 0x000");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(result); // Tampilkan error jika ada
      registerValues[i] = 0; // Simpan 0 jika ada error
    }
  }
  Serial.println("======================");
  // Menampilkan nilai register yang disimpan dalam array
  for (uint8_t i = 0; i < 9; i++) {
    // Membaca nilai berdasarkan indeks
    float reading = registerValues[i]; // Mengambil nilai dari array

    switch (i) {
      case 0:
        Serial.print("Moisture: ");
        Serial.print(reading);
        Serial.println(" %");
        break;
      case 1:
        Serial.print("Temperature: ");
        Serial.print(reading);
        Serial.println(" °C");
        break;
      case 2:
        Serial.print("Conductivity: ");
        Serial.print(reading);
        Serial.println(" us/cm");
        break;
      case 3:
        Serial.print("pH: ");
        Serial.print(reading);
        Serial.println(" PH");
        break;
      case 4:
        Serial.print("Nitrogen: ");
        Serial.print(reading);
        Serial.println(" mg/L");
        break;
      case 5:
        Serial.print("Phosphorus: ");
        Serial.print(reading);
        Serial.println(" mg/L");
        break;
      case 6:
        Serial.print("Potassium: ");
        Serial.print(reading);
        Serial.println(" mg/L");
        break;
      case 7:
        Serial.print("Salinity: ");
        Serial.print(reading);
        Serial.println(" ppt");
        break;
      case 8:
        Serial.print("TDS: ");
        Serial.print(reading);
        Serial.println(" mg/L");
        break;

    }
  }
  Serial.println("======================");

  delay(2000); // Tunggu 2 detik sebelum membaca lagi
}