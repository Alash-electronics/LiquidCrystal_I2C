// Простой пример подключения дисплея по шине I2C для печати на экране с Монитора порта

#include <Wire.h> // Импортируем библиотеку для работы библиотеки дисплея
#include <LiquidCrystal_I2C.h>// Импортируем библиотеку

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Создание объекта lcd с адресом 0x27, размером 16x2

void setup() {
  lcd.init(); // Инициализация LCD
  // Для Arduino: A4 - SDA, A5 - SCL
  // Для ESP8266: 4(D2) - SDA, 5(D1) - SCL
  // Для ESP8266-01 I2C: 0 - SDA, 2 - SCL
  // Для ESP32: D21 - SDA, D22 - SCL

  // Печать сообщения на LCD
  lcd.backlight(); // Включение подсветки
  lcd.setCursor(2, 0); // Установка курсора в позицию (2, 0)
  lcd.print("Serial port"); // Печать текста
  lcd.setCursor(5, 1); // Установка курсора в позицию (5, 1)
  lcd.print("input"); // Печать текста

  // Инициализация последовательного порта
  Serial.begin(9600);
}
void loop() {
  // При поступлении символов через последовательный порт...
  if (Serial.available()) {
    // Подождем немного, чтобы полностью принять сообщение
    delay(100);
    // Очистим экран
    lcd.clear();
    lcd.setCursor(0, 0);
    // Прочитаем все доступные символы
    while (Serial.available() > 0) {
      // Выводим каждый символ на LCD
      lcd.write(Serial.read());
    }
    Serial.println();
  }
}
