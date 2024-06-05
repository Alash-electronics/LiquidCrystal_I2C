// Простой пример подключения дисплея по шине I2C с добавлением своих символов через CGRAM

#include <Wire.h> // Импорт библиотеки для работы с шиной I2C
#include <LiquidCrystal_I2C.h> // Импорт библиотеки для работы с LCD по I2C

// Объявление пользовательских символов в формате байтов
uint8_t bell[8]  = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
uint8_t note[8]  = {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
uint8_t Clock[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
uint8_t duck[8]  = {0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0};
uint8_t check[8] = {0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0};
uint8_t cross[8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};
uint8_t retarrow[8] = {0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};

LiquidCrystal_I2C lcd(0x27, 16, 2); // Создание объекта lcd с адресом 0x27, размером 16x2

void setup() {
  lcd.init(); // Инициализация LCD
  // Для Arduino: A4 - SDA, A5 - SCL
  // Для ESP8266: 4(D2) - SDA, 5(D1) - SCL
  // Для ESP8266-01 I2C: 0 - SDA, 2 - SCL
  // Для ESP32: D21 - SDA, D22 - SCL

  lcd.backlight(); // Включение подсветки

  // Создание пользовательских символов
  lcd.createChar(0, bell); // Создание символа колокола
  lcd.createChar(1, note); // Создание символа ноты
  lcd.createChar(2, Clock); // Создание символа часов
  lcd.createChar(3, heart); // Создание символа сердца
  lcd.createChar(4, duck); // Создание символа утки
  lcd.createChar(5, check); // Создание символа галочки
  lcd.createChar(6, cross); // Создание символа креста
  lcd.createChar(7, retarrow); // Создание символа стрелки влево

  lcd.home(); // Установка курсора в начальную позицию

  // Вывод сообщения на LCD
  lcd.print("Hello world..."); // Вывод приветствия
  lcd.setCursor(0, 1); // Установка курсора на вторую строку
  lcd.print(" i "); // Вывод текста
  lcd.printByte(3); // Вывод пользовательского символа сердца
  lcd.print(" arduinos!"); // Вывод текста
  delay(5000); // Задержка 5 секунд
  displayKeyCodes(); // Вызов функции для отображения всех кодов клавиш
}

// Функция для отображения всех кодов клавиш
void displayKeyCodes(void) {
  uint8_t i = 0;
  while (1) {
    lcd.clear(); // Очистка дисплея
    lcd.print("Codes 0x"); lcd.print(i, HEX); // Вывод сообщения с кодами клавиш
    lcd.print("-0x"); lcd.print(i + 15, HEX); // Вывод сообщения с кодами клавиш
    lcd.setCursor(0, 1); // Установка курсора на вторую строку
    for (int j = 0; j < 16; j++) {
      lcd.printByte(i + j); // Вывод всех кодов клавиш
    }
    i += 16; // Увеличение счетчика кодов клавиш
    delay(4000); // Задержка 4 секунды
  }
}

void loop()
{

}
