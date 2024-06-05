//Alash education
#ifndef LiquidCrystal_I2C_h
#define LiquidCrystal_I2C_h

#include <inttypes.h>
#include "Print.h" 
#include <Wire.h>

// Команды для управления LCD дисплеем
#define LCD_CLEARDISPLAY 0x01 // Очистка дисплея
#define LCD_RETURNHOME 0x02 // Возврат курсора в начало
#define LCD_ENTRYMODESET 0x04 // Установка режима ввода
#define LCD_DISPLAYCONTROL 0x08 // Управление дисплеем
#define LCD_CURSORSHIFT 0x10 // Управление перемещением курсора или дисплея
#define LCD_FUNCTIONSET 0x20 // Установка функциональных режимов
#define LCD_SETCGRAMADDR 0x40 // Установка адреса в CGRAM
#define LCD_SETDDRAMADDR 0x80 // Установка адреса в DDRAM

// Флаги для установки режима ввода
#define LCD_ENTRYRIGHT 0x00 // Ввод справа налево
#define LCD_ENTRYLEFT 0x02 // Ввод слева направо
#define LCD_ENTRYSHIFTINCREMENT 0x01 // Смещение дисплея
#define LCD_ENTRYSHIFTDECREMENT 0x00 // Без смещения дисплея

// Флаги для управления дисплеем
#define LCD_DISPLAYON 0x04 // Включение дисплея
#define LCD_DISPLAYOFF 0x00 // Выключение дисплея
#define LCD_CURSORON 0x02 // Включение курсора
#define LCD_CURSOROFF 0x00 // Выключение курсора
#define LCD_BLINKON 0x01 // Включение мигания курсора
#define LCD_BLINKOFF 0x00 // Выключение мигания курсора

// Флаги для перемещения дисплея/курсора
#define LCD_DISPLAYMOVE 0x08 // Перемещение дисплея
#define LCD_CURSORMOVE 0x00 // Перемещение курсора
#define LCD_MOVERIGHT 0x04 // Перемещение вправо
#define LCD_MOVELEFT 0x00 // Перемещение влево

// Флаги для установки функциональных режимов
#define LCD_8BITMODE 0x10 // 8-битный режим
#define LCD_4BITMODE 0x00 // 4-битный режим
#define LCD_2LINE 0x08 // Двухстрочный режим
#define LCD_1LINE 0x00 // Однострочный режим
#define LCD_5x10DOTS 0x04 // Символы 5x10 точек
#define LCD_5x8DOTS 0x00 // Символы 5x8 точек

// Флаги для управления подсветкой
#define LCD_BACKLIGHT 0x08 // Включение подсветки
#define LCD_NOBACKLIGHT 0x00 // Выключение подсветки

#define En B00000100  // Бит включения
#define Rw B00000010  // Бит чтения/записи
#define Rs B00000001  // Бит выбора регистра

class LiquidCrystal_I2C : public Print {
public:
  LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
  void clear(); // Очистка дисплея
  void home(); // Возврат курсора в начало
  void noDisplay(); // Выключение дисплея
  void display(); // Включение дисплея
  void noBlink(); // Отключение мигания курсора
  void blink(); // Включение мигания курсора
  void noCursor(); // Отключение курсора
  void cursor(); // Включение курсора
  void scrollDisplayLeft(); // Прокрутка дисплея влево
  void scrollDisplayRight(); // Прокрутка дисплея вправо
  void printLeft(); // Печать с прокруткой влево
  void printRight(); // Печать с прокруткой вправо
  void leftToRight(); // Установка режима ввода слева направо
  void rightToLeft(); // Установка режима ввода справа налево
  void shiftIncrement(); // Увеличение смещения
  void shiftDecrement(); // Уменьшение смещения
  void noBacklight(); // Выключение подсветки
  void backlight(); // Включение подсветки
  void autoscroll(); // Включение автопрокрутки
  void noAutoscroll(); // Выключение автопрокрутки
  void createChar(uint8_t, uint8_t[]); // Создание пользовательского символа
  void createChar(uint8_t location, const char *charmap); // Создание пользовательского символа
  // Пример: const char bell[8] PROGMEM = {B00100,B01110,B01110,B01110,B11111,B00000,B00100,B00000};
  
  void setCursor(uint8_t, uint8_t); // Установка курсора
#if defined(ARDUINO) && ARDUINO >= 100
  virtual size_t write(uint8_t); // Запись символа в дисплей
#else
  virtual void write(uint8_t); // Запись символа в дисплей
#endif
  void command(uint8_t); // Отправка команды на дисплей
  void init(); // Инициализация дисплея
  void oled_init(); // Инициализация OLED дисплея

//// Совместимость с API
void blink_on(); // псевдоним для blink()
void blink_off(); // псевдоним для noBlink()
void cursor_on(); // псевдоним для cursor()
void cursor_off(); // псевдоним для noCursor()
void setBacklight(uint8_t new_val); // псевдоним для backlight() и nobacklight()
void load_custom_character(uint8_t char_num, uint8_t *rows); // псевдоним для createChar()
void printstr(const char[]); // Печать строки

//// Нереализованные функции API
uint8_t status(); // Получение статуса
void setContrast(uint8_t new_val); // Установка контрастности
uint8_t keypad(); // Чтение клавиатуры
void setDelay(int, int); // Установка задержек
void on(); // Включение дисплея
void off(); // Выключение дисплея
uint8_t init_bargraph(uint8_t graphtype); // Инициализация гистограммы
void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end); // Рисование горизонтальной гистограммы
void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end); // Рисование вертикальной гистограммы

private:
  void init_priv(); // Приватная инициализация
  void send(uint8_t, uint8_t); // Отправка данных или команд
  void write4bits(uint8_t); // Отправка 4-битных данных
  void expanderWrite(uint8_t); // Запись через экспандер
  void pulseEnable(uint8_t); // Импульс для включения
  uint8_t _Addr; // Адрес I2C
  uint8_t _displayfunction; // Функции дисплея
  uint8_t _displaycontrol; // Управление дисплеем
  uint8_t _displaymode; // Режим дисплея
  uint8_t _numlines; // Количество линий
  bool _oled = false; // Флаг OLED дисплея
  uint8_t _cols; // Количество столбцов
  uint8_t _rows; // Количество строк
  uint8_t _backlightval; // Значение подсветки
};

#endif