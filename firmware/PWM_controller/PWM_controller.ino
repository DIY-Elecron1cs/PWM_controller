/*
  ШИМ контроллер

  автор - https://github.com/DIY-Elecron1cs

  УПРАВЛЕНИЕ
  - кнопками ВЛЕВО-ВПРАВО выбираем режим работы (ШИМ или АЦП)
  - нажатием кнопки ОК подтверждаем выбор
  В РЕЖИМЕ ШИМ
  - передвигаемся по меню кнопками ВВЕРХ-ВНИЗ
  - кнопкой ОК подтверждаем и переходим в режим настройки выбранного ШИМ-канала
  - кнопками ВПРАВО-ВЛЕВО настраиваем ШИМ (доступно от 10 до 255)
  - кнопкой ОК выходим в режим навигации по меню
  - ожновременным нажатием кнопки ОК и любой другой кнопки выходим в главное меню (выбора режима работы)
  В РЕЖИМЕ АЦП
  - нажатием кнопки ОК выходим в главное меню (выбора режима работы)

  ПРИМЕЧАНИЯ
  - лучше использовать дисплей OLED128x64 с SPI, т.к. доступно на 2 пина АЦП больше
  - кнопки подключаются к GND без резистора
*/

/*---настройки---*/
#define OLED_TYPE 0   //0 - если I2C, 1 - SPI. С SPI в режиме АЦП доступно на 2 входа больше

/*---пины ШИМ---*/
#define PWM1 3
#define PWM2 5
#define PWM3 6
#define PWM4 9
#define PWM5 10
#define PWM6 11

/*---аналоговые входы---*/
#define ADC1 A0
#define ADC2 A1
#define ADC3 A2
#define ADC4 A3
#define ADC5 A4
#define ADC6 A5
#define ADC7 A6
#define ADC8 A7

/*---пины кнопок---*/
#define LEFT 2
#define RIGHT 6
#define UP 4
#define DOWN 5
#define OK 3

/*---режимы---*/
#define SELECT 0
#define PWM 1
#define ADC 2
//#define EMPTY 3  //вспомогательный

#define ITEMS_PWM 6
#if OLED_TYPE == 0
#define ITEMS_ADC 8
#else
#define ITEMS_ADC 6
#endif

/*---библиотеки---*/
#include "bitmaps.h"
#include <EEPROM.h>
#include <GyverOLED.h>

/*---объекты---*/
#if OLED_TYPE == 0
GyverOLED <SSD1306_128x64, OLED_NO_BUFFER> oled;
#else
GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, 7, 8, 12> oled;
#endif

/*---переменные---*/
uint8_t global_mode = SELECT;
bool global_select = 0;
uint8_t screens = SELECT;
bool flag_PWM = true;
int param[ITEMS_PWM];
uint16_t values[ITEMS_ADC];

void setup() {
  pinMode(LEFT, INPUT_PULLUP);    //для кнопок
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(OK, INPUT_PULLUP);
  pinMode(PWM1, OUTPUT);    //для ШИМ
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);
  pinMode(PWM5, OUTPUT);
  pinMode(PWM6, OUTPUT);
  pinMode(ADC1, INPUT);    //для АЦП
  pinMode(ADC2, INPUT);
  pinMode(ADC3, INPUT);
#if OLED_TYPE == 1
  pinMode(ADC4, INPUT);
  pinMode(ADC5, INPUT);
#endif
  pinMode(ADC6, INPUT);
  pinMode(ADC7, INPUT);
  pinMode(ADC8, INPUT);

  oled.init(800);
  oled.clear();
  //Serial.begin(9600);
  /*---лого---*/
  oled.drawBitmap(0, 0, main_128x40, 128, 40);
  oled.update();
  oled.setCursor(20, 7);
  oled.print("DIY электроника");
  delay(1500);
  /*---начальный экран---*/
  oled.clear();
  oled.setCursor(45, 2);
  oled.setScale(2);
  oled.print("ШИМ");
  oled.drawBitmap(0, 34, pwm_128x30, 128, 30);
  oled.update();

  EEPROM_read();    //читаем сохранённые значения из EEPROM
}

void loop() {
  if (screens == SELECT) {    //если экран выбора режима (начальный, по умолчанию)
    select_mode();            //выполняем
  }
  else if (screens == PWM) {  //если экран настройки ШИМ
    PWM_work();               //выполняем
  }
  else if (screens == ADC) {  //если экран АЦП
    ADC_work();               //выполняем
  }
  PWM_write();      //даём ШИМ на пины
}

void EEPROM_read() {          //читаем значения из EEPROM
  param[0] = EEPROM.read(5);
  param[1] = EEPROM.read(6);
  param[2] = EEPROM.read(7);
  param[3] = EEPROM.read(8);
  param[4] = EEPROM.read(9);
  param[5] = EEPROM.read(10);
}

void EEPROM_write() {         //записывает в EEPROM
  EEPROM.write(5, param[0]);
  EEPROM.write(6, param[1]);
  EEPROM.write(7, param[2]);
  EEPROM.write(8, param[3]);
  EEPROM.write(9, param[4]);
  EEPROM.write(10, param[5]);
}

void PWM_write() {            //даёт ШИМ на пины
  analogWrite(PWM1, param[0]);
  analogWrite(PWM2, param[1]);
  analogWrite(PWM3, param[2]);
  analogWrite(PWM4, param[3]);
  analogWrite(PWM5, param[4]);
  analogWrite(PWM6, param[5]);
}
