// Основано для работы Alash Education

#include "LiquidCrystal_I2C.h"
#include <inttypes.h>
#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"

#define printIIC(args)	Wire.write(args) // Макрос для I2C связи
inline size_t LiquidCrystal_I2C::write(uint8_t value) {
	send(value, Rs); // Отправка данных с битом регистра выбора
	return 1; // Возврат количества записанных байтов
}

#else
#include "WProgram.h"

#define printIIC(args)	Wire.send(args) // Макрос для I2C связи в старых версиях Arduino
inline void LiquidCrystal_I2C::write(uint8_t value) {
	send(value, Rs); // Отправка данных с битом регистра выбора
}

#endif
#include "Wire.h"

// При включении дисплей настраивается следующим образом:
//
// 1. Очистка дисплея
// 2. Установка функций: 
//    DL = 1; 8-битный интерфейс данных 
//    N = 0; 1-линейный дисплей 
//    F = 0; шрифт 5x8 точек 
// 3. Управление включением/выключением дисплея: 
//    D = 0; Дисплей выключен 
//    C = 0; Курсор выключен 
//    B = 0; Мерцание выключено 
// 4. Установка режима ввода: 
//    I/D = 1; Инкремент на 1
//    S = 0; Без сдвига 
//
// Однако, сброс Arduino не сбрасывает LCD, поэтому мы
// не можем предположить, что он находится в этом состоянии при запуске скетча (и вызове конструктора LiquidCrystal).

LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows)
{
  _Addr = lcd_Addr; // Адрес дисплея
  _cols = lcd_cols; // Количество столбцов дисплея
  _rows = lcd_rows; // Количество строк дисплея
  _backlightval = LCD_NOBACKLIGHT; // Значение подсветки (выключено)
}

void LiquidCrystal_I2C::oled_init(){
  _oled = true; // Установка флага OLED
	init_priv(); // Вызов приватной функции инициализации
}

void LiquidCrystal_I2C::init(){
	init_priv(); // Вызов приватной функции инициализации
}

void LiquidCrystal_I2C::init_priv()
{
	Wire.begin(); // Инициализация I2C
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS; // Установка функций дисплея
	begin(_cols, _rows); // Начальная настройка дисплея
}

void LiquidCrystal_I2C::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
	if (lines > 1) {
		_displayfunction |= LCD_2LINE; // Установка режима двух линий, если их больше одной
	}
	_numlines = lines; // Количество линий

	// Для некоторых однострочных дисплеев можно выбрать шрифт высотой 10 пикселей
	if ((dotsize != 0) && (lines == 1)) {
		_displayfunction |= LCD_5x10DOTS; // Установка шрифта 5x10 точек
	}

	// СМОТРИТЕ СТРАНИЦЫ 45/46 ДЛЯ СПЕЦИФИКАЦИИ ИНИЦИАЛИЗАЦИИ!
	// Согласно datasheet, необходимо как минимум 40мс после того, как напряжение поднимется выше 2,7В,
	// прежде чем отправлять команды. Arduino может включиться намного раньше 4,5В, поэтому подождем 50мс
	delay(50); 
  
	// Теперь мы устанавливаем низкий уровень для RS и R/W, чтобы начать отправку команд
	expanderWrite(_backlightval);	// Сброс расширителя и выключение подсветки (бит 8 = 1)
	delay(1000);

  	// Установка LCD в 4-битный режим
	// Это согласно datasheet Hitachi HD44780
	// Рисунок 24, стр. 46
	
	  // Начинаем в 8-битном режиме, пытаемся установить 4-битный режим
   write4bits(0x03 << 4);
   delayMicroseconds(4500); // ожидание минимум 4,1мс
   
   // Вторая попытка
   write4bits(0x03 << 4);
   delayMicroseconds(4500); // ожидание минимум 4,1мс
   
   // Третья попытка
   write4bits(0x03 << 4); 
   delayMicroseconds(150);
   
   // Наконец, установка 4-битного интерфейса
   write4bits(0x02 << 4); 


	// Установка количества линий, размера шрифта и т.д.
	command(LCD_FUNCTIONSET | _displayfunction);  
	
	// Включение дисплея без курсора и мерцания по умолчанию
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display();
	
	// Очистка дисплея
	clear();
	
	// Инициализация направления текста по умолчанию (для римских языков)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	
	// Установка режима ввода
	command(LCD_ENTRYMODESET | _displaymode);
	
	home(); // Возврат курсора домой
}

/********** команды высокого уровня для пользователя! */
void LiquidCrystal_I2C::clear(){
	command(LCD_CLEARDISPLAY); // Очистка дисплея, установка курсора в нулевую позицию
	delayMicroseconds(2000);  // Эта команда занимает много времени!
  if (_oled) setCursor(0,0); // Для OLED дисплеев, установка курсора в нулевую позицию
}

void LiquidCrystal_I2C::home(){
	command(LCD_RETURNHOME);  // Установка курсора в нулевую позицию
	delayMicroseconds(2000);  // Эта команда занимает много времени!
}

void LiquidCrystal_I2C::setCursor(uint8_t col, uint8_t row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 }; // Смещения строк
	if ( row > _numlines ) {
		row = _numlines-1;    // Строки считаются с нуля
	}
	command(LCD_SETDDRAMADDR | (col + row_offsets[row])); // Установка адреса DDRAM
}

// Быстрое включение/выключение дисплея
void LiquidCrystal_I2C::noDisplay() {
	_displaycontrol &= ~LCD_DISPLAYON; // Выключение дисплея
	command(LCD_DISPLAYCONTROL | _displaycontrol); // Отправка команды
}
void LiquidCrystal_I2C::display() {
	_displaycontrol |= LCD_DISPLAYON; // Включение дисплея
	command(LCD_DISPLAYCONTROL | _displaycontrol); // Отправка команды
}

// Включение/выключение подчеркивания курсора
void LiquidCrystal_I2C::noCursor() {
	_displaycontrol &= ~LCD_CURSORON; // Выключение курсора
	command(LCD_DISPLAYCONTROL | _displaycontrol); // Отправка команды
}
void LiquidCrystal_I2C::cursor() {
	_displaycontrol |= LCD_CURSORON; // Включение курсора
	command(LCD_DISPLAYCONTROL | _displaycontrol); // Отправка команды
}

// Включение/выключение мигающего курсора
void LiquidCrystal_I2C::noBlink() {
	_displaycontrol &= ~LCD_BLINKON; // Выключение мигания
	command(LCD_DISPLAYCONTROL | _displaycontrol); // Отправка команды
}
void LiquidCrystal_I2C::blink() {
	_displaycontrol |= LCD_BLINKON; // Включение мигания
	command(LCD_DISPLAYCONTROL | _displaycontrol); // Отправка команды
}

// Команды для прокрутки дисплея без изменения содержимого RAM
void LiquidCrystal_I2C::scrollDisplayLeft(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT); // Прокрутка влево
}
void LiquidCrystal_I2C::scrollDisplayRight(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT); // Прокрутка вправо
}

// Команды для текста, который идет слева направо
void LiquidCrystal_I2C::leftToRight(void) {
	_displaymode |= LCD_ENTRYLEFT; // Установка режима ввода текста слева направо
	command(LCD_ENTRYMODESET | _displaymode); // Отправка команды
}

// Команды для текста, который идет справа налево
void LiquidCrystal_I2C::rightToLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT; // Установка режима ввода текста справа налево
	command(LCD_ENTRYMODESET | _displaymode); // Отправка команды
}

// Команда для автопрокрутки текста вправо


void LiquidCrystal_I2C::autoscroll(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT; // Установка режима автопрокрутки
	command(LCD_ENTRYMODESET | _displaymode); // Отправка команды
}

// Команда для отмены автопрокрутки текста
void LiquidCrystal_I2C::noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT; // Отмена автопрокрутки
	command(LCD_ENTRYMODESET | _displaymode); // Отправка команды
}

// Позволяет заполнять первые 8 мест CGRAM
// пользовательскими символами
void LiquidCrystal_I2C::createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // У нас есть только 8 мест (0-7)
	command(LCD_SETCGRAMADDR | (location << 3)); // Установка адреса CGRAM
	for (int i=0; i<8; i++) {
		write(charmap[i]); // Запись символа
	}
}

// createChar с входными данными PROGMEM
void LiquidCrystal_I2C::createChar(uint8_t location, const char *charmap) {
	location &= 0x7; // У нас есть только 8 мест (0-7)
	command(LCD_SETCGRAMADDR | (location << 3)); // Установка адреса CGRAM
	for (int i=0; i<8; i++) {
	    	write(pgm_read_byte_near(charmap++)); // Запись символа из памяти PROGMEM
	}
}

// Включение/выключение (опциональной) подсветки
void LiquidCrystal_I2C::noBacklight(void) {
	_backlightval=LCD_NOBACKLIGHT; // Установка значения для отключения подсветки
	expanderWrite(0); // Отправка значения в расширитель
}

void LiquidCrystal_I2C::backlight(void) {
	_backlightval=LCD_BACKLIGHT; // Установка значения для включения подсветки
	expanderWrite(0); // Отправка значения в расширитель
}

/*********** команды среднего уровня для отправки данных/команд */

inline void LiquidCrystal_I2C::command(uint8_t value) {
	send(value, 0); // Отправка команды
}

/************ команды низкого уровня для отправки данных/команд */

// Отправка либо команды, либо данных
void LiquidCrystal_I2C::send(uint8_t value, uint8_t mode) {
	uint8_t highnib = value & 0xf0; // Выделение старшего полубайта
	uint8_t lownib = (value << 4) & 0xf0; // Выделение младшего полубайта
	write4bits((highnib) | mode); // Отправка старшего полубайта
	write4bits((lownib) | mode); // Отправка младшего полубайта
}

void LiquidCrystal_I2C::write4bits(uint8_t value) {
	expanderWrite(value); // Запись значения в расширитель
	pulseEnable(value); // Создание импульса enable
}

void LiquidCrystal_I2C::expanderWrite(uint8_t _data){                                        
	Wire.beginTransmission(_Addr); // Начало передачи по I2C
	printIIC((int)(_data) | _backlightval); // Отправка данных с учетом подсветки
	Wire.endTransmission();   // Завершение передачи
}

void LiquidCrystal_I2C::pulseEnable(uint8_t _data){
	expanderWrite(_data | En);	// En высокий уровень
	delayMicroseconds(1);		// импульс enable должен быть >450нс
	
	expanderWrite(_data & ~En);	// En низкий уровень
	delayMicroseconds(50);		// команды должны > 37мкс для установки
} 


// Функции-алиасы

void LiquidCrystal_I2C::cursor_on(){
	cursor(); // Включение курсора
}

void LiquidCrystal_I2C::cursor_off(){
	noCursor(); // Выключение курсора
}

void LiquidCrystal_I2C::blink_on(){
	blink(); // Включение мигания курсора
}

void LiquidCrystal_I2C::blink_off(){
	noBlink(); // Выключение мигания курсора
}

void LiquidCrystal_I2C::load_custom_character(uint8_t char_num, uint8_t *rows){
	createChar(char_num, rows); // Загрузка пользовательского символа
}

void LiquidCrystal_I2C::setBacklight(uint8_t new_val){
	if(new_val){
		backlight();		// включение подсветки
	}else{
		noBacklight();		// выключение подсветки
	}
}

void LiquidCrystal_I2C::printstr(const char c[]){
	// Эта функция не идентична функции, используемой для "настоящих" I2C дисплеев
	// она здесь, чтобы пользовательский скетч не пришлось менять
	print(c); // Печать строки
}


// Неподдерживаемые функции API
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void LiquidCrystal_I2C::off(){}
void LiquidCrystal_I2C::on(){}
void LiquidCrystal_I2C::setDelay (int cmdDelay, int charDelay) {}
uint8_t LiquidCrystal_I2C::status(){return 0;}
uint8_t LiquidCrystal_I2C::keypad (){return 0;}
uint8_t LiquidCrystal_I2C::init_bargraph(uint8_t graphtype){return 0;}
void LiquidCrystal_I2C::draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end){}
void LiquidCrystal_I2C::draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_row_end){}
void LiquidCrystal_I2C::setContrast(uint8_t new_val){}
#pragma GCC diagnostic pop