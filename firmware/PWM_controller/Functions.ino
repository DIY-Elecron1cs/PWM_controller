void select_mode() {                  //выбираем, какой режим включить
  if (press(LEFT) || press(RIGHT)) {  //если нажимаем вправо/влево
    global_select = !global_select;   //меняем режим (только для отображения, ещё ничего не запускаем!)

    if (global_select == 0) {         //если выбрали ШИМ
      oled.setCursor(45, 2);          //отображение экрана
      oled.setScale(2);
      oled.print("ШИМ");
      oled.drawBitmap(0, 34, pwm_128x30, 128, 30);
    }
    else if (global_select == 1) {    //если выбрали АЦП
      oled.setCursor(45, 2);          //отображение экрана
      oled.setScale(2);
      oled.print("АЦП");
      oled.drawBitmap(0, 34, adc_128x30, 128, 30);
    }
  }

  oled.setCursor(25, 0);
  oled.setScale(0);
  oled.print("выбор режима: ");

  if (press(OK)) {              //если нажали OK (подтверждение выбора)
    if (global_select == 0) {   //и до этого выбрали режим ШИМ
      screens = PWM;            //меняем значение, и всё
      oled.clear();
    }
    else if (global_select == 1) {   //а до этого выбрали режим АЦП
      screens = ADC;                 //тоже меняем значение
      oled.clear();
    }
  }
}

void PWM_work() {               //сам по себе режим ШИМ (главная функция)
  static uint8_t pointer = 0;   //переменная для указателя, хранит номер строки
  if (flag_PWM) {               //если режим настройки отключен
    if (press(UP)) {            //если нажали UP
      pointer = constrain(pointer - 1, 0, ITEMS_PWM - 1);   //двигаем переменную указателя вверх
    }
    if (press(DOWN)) {          //если нажали DOWN
      pointer = constrain(pointer + 1, 0, ITEMS_PWM - 1);   //двигаем переменную указателя вниз
    }
  }
  oled.setScale(1);       //рисуем (да, не лучшим образом. Но с F-строкой не работает.)
  oled.setCursor(10, 0);
  oled.print("OUT 1:");
  oled.setCursor(10, 1);
  oled.print("OUT 2:");
  oled.setCursor(10, 2);
  oled.print("OUT 3:");
  oled.setCursor(10, 3);
  oled.print("OUT 4:");
  oled.setCursor(10, 4);
  oled.print("OUT 5:");
  oled.setCursor(10, 5);
  oled.print("OUT 6:");

  printPointer(pointer);    //рисуем курсор
  Set_PWM(pointer);         //выставляет ШИМ
  oled.update();

  if (cansel()) {           //если хотим выйти
    screens = SELECT;       //меняем значение, выходим
    oled.clear();
    oled.setCursor(45, 2);  //рисуем
    oled.setScale(2);
    oled.print("ШИМ");
    oled.drawBitmap(0, 34, pwm_128x30, 128, 30);
    EEPROM_write();         //пишем значения в EEPROM
  }
}

void Set_PWM(uint8_t sel) {      //настройка ШИМ
  if (click(RIGHT)) {            //если нажали RIGHT
    if (flag_PWM) {
      sel = constrain(sel - 1, 0, ITEMS_PWM - 1);
    }
    else {
      param[sel]++;   //прибавляем
    }
    range(sel);
  }
  else if (click(LEFT)) {      //если нажали RIGHT
    if (flag_PWM) {
      sel = constrain(sel + 1, 0, ITEMS_PWM - 1);
    }
    else {
      param[sel]--;   //вычитаем
    }
    range(sel);       //функция, не даёт выйти за пределы 10-255
  }
  else if (press(OK)) {       //если нажали OK
    flag_PWM = !flag_PWM;     //меняем значение (выходим из настройки)
  }
  for (uint8_t i = 0; i < 6; i++) {
    oled.setCursor(60, i);
    oled.print(String(param[i]) + "    ");  //выводим значения
  }
}

void printPointer(uint8_t ptr) {    //рисуем указатель
  if (flag_PWM) {                   //если НЕ режим настройки
    oled.setCursor(123, ptr);       //рисуем слева
    oled.print(" ");
    oled.setCursor(1, ptr);
    oled.print(">");
    oled.setCursor(1, abs(ptr - 1));
    oled.print(" ");
    oled.setCursor(1, ptr + 1);
    oled.print(" ");
  }
  else {                            //если режим настройки
    oled.setCursor(1, ptr);         //рисуем справа
    oled.print(" ");
    oled.setCursor(123, ptr);
    oled.print("<");
    oled.setCursor(123, abs(ptr - 1));
    oled.print(" ");
    oled.setCursor(123, ptr + 1);
    oled.print(" ");
  }
}

void range(uint8_t sel) {     //не даёт выйти за пределы 10-255
  if (param[sel] <= 10) {
    param[sel] = 10;
  }
  else if (param[sel] >= 255) {
    param[sel] = 255;
  }
}

void ADC_work() {               //сам по себе режим АЦП (главная функция)
  values[0] = analogRead(ADC1); //читаем с пинов
  values[1] = analogRead(ADC2);
  values[2] = analogRead(ADC3);
#if OLED_TYPE == 1
  values[3] = analogRead(ADC4);
  values[4] = analogRead(ADC5);
#endif
  values[5] = analogRead(ADC6);
  values[6] = analogRead(ADC7);
  values[7] = analogRead(ADC8);

  oled.setScale(1);
#if OLED_TYPE == 1
  oled.setCursor(0, 0);
  oled.print("IN 1: " + String(values[0]) + "   ");
  oled.setCursor(0, 1);
  oled.print("IN 2: " + String(values[1]) + "   ");
  oled.setCursor(0, 2);
  oled.print("IN 3: " + String(values[2]) + "   ");
  oled.setCursor(0, 3);
  oled.print("IN 4: " + String(values[3]) + "   ");
  oled.setCursor(0, 4);
  oled.print("IN 5: " + String(values[4]) + "   ");
  oled.setCursor(0, 5);
  oled.print("IN 6: " + String(values[5]) + "   ");
  oled.setCursor(0, 6);
  oled.print("IN 7: " + String(values[6]) + "   ");
  oled.setCursor(0, 7);
  oled.print("IN 8: " + String(values[7]) + "   ");

  oled.setCursor(80, 0);
  oled.print(String(float(values[0]) / 204.8) + " В  ");
  oled.setCursor(80, 1);
  oled.print(String(float(values[1]) / 204.8) + " В  ");
  oled.setCursor(80, 2);
  oled.print(String(float(values[2]) / 204.8) + " В  ");
  oled.setCursor(80, 3);
  oled.print(String(float(values[3]) / 204.8) + " В  ");
  oled.setCursor(80, 4);
  oled.print(String(float(values[4]) / 204.8) + " В  ");
  oled.setCursor(80, 5);
  oled.print(String(float(values[5]) / 204.8) + " В  ");
  oled.setCursor(80, 6);
  oled.print(String(float(values[6]) / 204.8) + " В  ");
  oled.setCursor(80, 7);
  oled.print(String(float(values[7]) / 204.8) + " В  ");
#else
  oled.setCursor(0, 0);
  oled.print("IN 1: " + String(values[0]) + "   ");
  oled.setCursor(0, 1);
  oled.print("IN 2: " + String(values[1]) + "   ");
  oled.setCursor(0, 2);
  oled.print("IN 3: " + String(values[2]) + "   ");
  oled.setCursor(0, 3);
  oled.print("IN 4: " + String(values[3]) + "   ");
  oled.setCursor(0, 4);
  oled.print("IN 5: " + String(values[6]) + "   ");
  oled.setCursor(0, 5);
  oled.print("IN 6: " + String(values[7]) + "   ");
  oled.setCursor(80, 0);
  oled.print(String(float(values[0]) / 204.8) + " В  ");
  oled.setCursor(80, 1);
  oled.print(String(float(values[1]) / 204.8) + " В  ");
  oled.setCursor(80, 2);
  oled.print(String(float(values[2]) / 204.8) + " В  ");
  oled.setCursor(80, 3);
  oled.print(String(float(values[3]) / 204.8) + " В  ");
  oled.setCursor(80, 4);
  oled.print(String(float(values[6]) / 204.8) + " В  ");
  oled.setCursor(80, 5);
  oled.print(String(float(values[7]) / 204.8) + " В  ");
#endif
  oled.update();

  if (press(OK)) {          //если хотим выйти
    screens = SELECT;       //меняем значение, выходим
    oled.clear();
    oled.setCursor(45, 2);  //рисуем
    oled.setScale(2);
    oled.print("АЦП");
    oled.drawBitmap(0, 34, adc_128x30, 128, 30);
  }
}  
