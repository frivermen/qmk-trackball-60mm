#include <Arduino.h>
#include <USB.h>
#include <USBHIDMouse.h>

// Пины для подключения к ESP32-S2 Mini
#define SCLK_PIN 37
#define SDIO_PIN 36

USBHIDMouse Mouse;

// ------------------------------------------------------------------
// Отправка одного байта (контроллер -> датчик)
// ------------------------------------------------------------------
void sendByte(uint8_t byte) {
  for (int i = 7; i >= 0; i--) {
    digitalWrite(SDIO_PIN, (byte >> i) & 0x01);
    digitalWrite(SCLK_PIN, HIGH);
    delayMicroseconds(1);
    digitalWrite(SCLK_PIN, LOW);
    delayMicroseconds(1);
  }
}

// ------------------------------------------------------------------
// Чтение одного байта (датчик -> контроллер)
// ------------------------------------------------------------------
uint8_t readByte() {
  uint8_t data = 0;
  pinMode(SDIO_PIN, INPUT_PULLUP);
  delayMicroseconds(1);
  for (int i = 7; i >= 0; i--) {
    digitalWrite(SCLK_PIN, HIGH);
    delayMicroseconds(1);
    if (digitalRead(SDIO_PIN)) {
      data |= (1 << i);
    }
    digitalWrite(SCLK_PIN, LOW);
    delayMicroseconds(1);
  }
  pinMode(SDIO_PIN, OUTPUT);
  return data;
}

// ------------------------------------------------------------------
// Запись в регистр
// ------------------------------------------------------------------
void writeRegister(uint8_t reg, uint8_t data) {
  pinMode(SDIO_PIN, OUTPUT);
  digitalWrite(SCLK_PIN, LOW);
  sendByte(reg | 0x80);
  sendByte(data);
}

// ------------------------------------------------------------------
// Чтение регистра
// ------------------------------------------------------------------
uint8_t readRegister(uint8_t reg) {
  pinMode(SDIO_PIN, OUTPUT);
  digitalWrite(SCLK_PIN, LOW);
  sendByte(reg & 0x7F);
  return readByte();
}

// ------------------------------------------------------------------
// Инициализация
// ------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== PAW3204 USB HID Mouse ===");

  // Настройка пинов
  pinMode(SCLK_PIN, OUTPUT);
  digitalWrite(SCLK_PIN, LOW);
  pinMode(SDIO_PIN, OUTPUT);
  digitalWrite(SDIO_PIN, HIGH);

  delay(100);

  // Проверка связи – чтение Product_ID1
  uint8_t pid = readRegister(0x00);
  Serial.printf("Product_ID1 = 0x%02X %s\n", pid, (pid == 0x30) ? "OK" : "ERROR");

  // Настройка разрешения 1600 CPI (пример)
  // Configuration: биты 2..0 = CPI, 0b110 = 1600
  writeRegister(0x06, 0b00000110);
  // Настройка качества при котором можно работать, 0~255
//  writeRegister(0x09, 0x5A); // write protect disable
//  writeRegister(0x0D, 20); // def 10
//  writeRegister(0x09, 0x00); // write protect enable
  Serial.printf("Configuration set to 0x%02X\n", readRegister(0x06));

  // Инициализация USB HID
  USB.begin();
  Mouse.begin();
  Serial.println("USB HID Mouse ready, polling rate 250 Hz");
}

void loop() {
  static uint32_t nextTime = 0;
  uint32_t now = micros();

  if (now >= nextTime) {
    nextTime = now + 4000;   // 4 мс = 250 Гц

    // Читаем статус движения
    uint8_t motion = readRegister(0x02);

    int8_t dx = 0, dy = 0;
    if (motion & 0x80) {               // бит Motion = 1
      dx = (int8_t)readRegister(0x03);
      dy = (int8_t)readRegister(0x04);
      // Можно добавить фильтрацию или масштабирование, если нужно
    }

    // Отправляем отчёт мыши (относительное перемещение)
//    Mouse.move(0, 0, -dy/4,/ dx/4); // scroll
    Mouse.move(dx, dy, 0, 0);
  }
}
