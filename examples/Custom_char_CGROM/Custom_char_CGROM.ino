// Простой пример подключения дисплея по шине I2C с добавлением своих символов через CGROM

#include <LiquidCrystal_I2C.h>// Импортируем библиотеку

LiquidCrystal_I2C lcd(0x27, 16, 2); // Создание объекта lcd с адресом 0x27, размером 16x2

// Создание пользовательских символов:

// Символ сердца:
byte Heart[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};

// Символ колокола:
byte Bell[] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000
};

// Символ инопланетянина:
byte Alien[] = {
  B11111,
  B10101,
  B11111,
  B11111,
  B01110,
  B01010,
  B11011,
  B00000
};

// Символ галочки:
byte Check[] = {
  B00000,
  B00001,
  B00011,
  B10110,
  B11100,
  B01000,
  B00000,
  B00000
};

// Символ динамика:
byte Speaker[] = {
  B00001,
  B00011,
  B01111,
  B01111,
  B01111,
  B00011,
  B00001,
  B00000
};

// Символ звука:
byte Sound[] = {
  B00001,
  B00011,
  B00101,
  B01001,
  B01001,
  B01011,
  B11011,
  B11000
};

// Символ черепа:
byte Skull[] = {
  B00000,
  B01110,
  B10101,
  B11011,
  B01110,
  B01110,
  B00000,
  B00000
};

// Символ замка:
byte Lock[] = {
  B01110,
  B10001,
  B10001,
  B11111,
  B11011,
  B11011,
  B11111,
  B00000
};

void setup() {
  lcd.init(); // Инициализация LCD
  // Для Arduino: A4 - SDA, A5 - SCL
  // Для ESP8266: 4(D2) - SDA, 5(D1) - SCL
  // Для ESP8266-01 I2C: 0 - SDA, 2 - SCL
  // Для ESP32: D21 - SDA, D22 - SCL
  
  lcd.backlight(); // Включение подсветки

  // Создание новых символов:
  lcd.createChar(0, Heart);   // Создание символа сердца
  lcd.createChar(1, Bell);    // Создание символа колокола
  lcd.createChar(2, Alien);   // Создание символа инопланетянина
  lcd.createChar(3, Check);   // Создание символа галочки
  lcd.createChar(4, Speaker); // Создание символа динамика
  lcd.createChar(5, Sound);   // Создание символа звука
  lcd.createChar(6, Skull);   // Создание символа черепа
  lcd.createChar(7, Lock);    // Создание символа замка

  // Очистка экрана LCD:
  lcd.clear();

  // Вывод сообщения на LCD:
  lcd.print("Custom Character");
}

// Вывод всех пользовательских символов:
void loop() {
  // Установка курсора на первую строку, второй символ
  lcd.setCursor(0, 1);
  // Вывод символа сердца
  lcd.write(0);

  // Установка курсора на третий символ
  lcd.setCursor(2, 1);
  // Вывод символа колокола
  lcd.write(1);

  // Установка курсора на пятый символ
  lcd.setCursor(4, 1);
  // Вывод символа инопланетянина
  lcd.write(2);

  // Установка курсора на седьмой символ
  lcd.setCursor(6, 1);
  // Вывод символа галочки
  lcd.write(3);

  // Установка курсора на девятый символ
  lcd.setCursor(8, 1);
  // Вывод символа динамика
  lcd.write(4);

  // Установка курсора на одиннадцатый символ
  lcd.setCursor(10, 1);
  // Вывод символа звука
  lcd.write(5);

  // Установка курсора на тринадцатый символ
  lcd.setCursor(12, 1);
  // Вывод символа черепа
  lcd.write(6);

  // Установка курсора на пятнадцатый символ
  lcd.setCursor(14, 1);
  // Вывод символа замка
  lcd.write(7);
}
