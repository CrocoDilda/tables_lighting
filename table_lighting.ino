#include "FastLED.h" // подключаем библиотеку фастлед
#include "GyverEncoder.h" // подключаем библиотеку энкфодера

byte baza = 0;     // изменение оттенка LED
byte Li = 0;    // Левая половина подсветки
byte bright = 255;
int counter = 0;
bool flag = 0;
bool relay_flag = 0;
bool mosfet_flag = 0;
long color = 6250000;
unsigned long last_time = 5000;

#define LED_NUM 118
#define PIN 13 // Управление RGB лентой
#define powerRGB A3
#define relay 5 // Реле блока питания RGB посветки
#define monitor_backlight 6 // Кнопка включения подсветки на мониках
#define mosfet 7 // Мосфет питания подсветки на мониках


#define CLK 4
#define DT 2
#define SW 3

Encoder enc1(CLK, DT, SW);
CRGB leds[LED_NUM];

//------------------Функции-----------------------
//====Яркость===
  byte brightFunc () {
    if (enc1.isRight() && bright < 255) {
      bright += 5;
    }
    if (enc1.isLeft() && bright > 5) {
      bright -= 5;
    }
    return bright;
  }
//====Яркость===
//------------------Функции-----------------------

void setup() {
  Serial.begin(9600);
  // --------------Настройки библиотек--------------
  FastLED.addLeds <WS2812, PIN, GRB>(leds, LED_NUM).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(bright);
  
  enc1.setType(TYPE1);
  // --------------Настройки библиотек--------------
  
  // --------------Настройка портов--------------
  pinMode(powerRGB, OUTPUT);
  pinMode(monitor_backlight, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(mosfet, OUTPUT);
  digitalWrite(mosfet, HIGH);
  // --------------Настройка портов--------------
}


void loop() {
  bool buttonState = digitalRead(powerRGB);

//---------------------Выключение и активный режим ленты--------------------

  // if(digitalRead(powerRGB) == 1) {
  //  for(byte i = LED_NUM / 2; i < LED_NUM; i-- ){
  //    Li = LED_NUM / 2 + abs(LED_NUM / 2 - i) - 1;
  //    leds[i].setHue(counter + i * (255 / LED_NUM));
  //    leds[Li].setHue(counter + i * (255 / LED_NUM)); // Если добавить для i множитель pwm, то с помощью потенциометра я смогу менять длину шага радуги
  //    FastLED.setBrightness(bright);
  //    FastLED.show();
  //    enc1.tick();    // оставляем тут для работы "временных" функций и антидребезга
  //    enc1.setTickMode(AUTO);
  //    if (enc1.isRight() && bright < 255) {
  //      bright += 5;
      
  //    Serial.print("Right");
  //    Serial.println(bright);
  //    }
  //    if (enc1.isLeft() && bright > 5) {
  //            for(byte i = 5; i > 0 && bright > 5; i--) {
  //        bright--;
  //      }
  //    Serial.print("Left");Serial.println(bright);
  //  }
  // }  counter ++;
  // } else {
  //  Li = LED_NUM - 1;
  //  for(byte i = 0; i < LED_NUM / 2; i++ ){
  //    leds[i] = 000;
  //    leds[Li] = 000; 
  //    Li--;
  //    Serial.println(i);
  //    FastLED.show();
      
  //  }
  // }
//---------------------Выключение и активный режим ленты--------------------



  //-------------------Мой режим со сменой цвета всей ленты--------------
//  for(byte i = LED_NUM / 2; i < LED_NUM; i-- ){
//    Li = LED_NUM / 2 + abs(LED_NUM / 2 - i) - 1;
//     leds[i] = CHSV( pwm, 187, bright);  //оттенок/насыщенность/яркость 
//     leds[Li] = CHSV( pwm, 187, bright);  //оттенок/насыщенность/яркость 
//    } 
  //-------------------Мой режим со сменой цвета всей ленты--------------
  
  if (enc1.isHolded()) { // Устанавливаю флажок на двойное нажатие
    flag = !flag;
  }
 //---------------Питание БПшников-----------------
 //--------------RGB лента---------------------
    if (digitalRead(powerRGB) == 0 && relay_flag == 1){
      digitalWrite(relay, HIGH);
      relay_flag = 0;
      Serial.println("off");
    }    
    if (digitalRead(powerRGB) == 1 && relay_flag == 0){
       relay_flag = 1;
       digitalWrite(relay, LOW);
       delay(250);
       } 
 //--------------RGB лента---------------------
 //--------------Белая лента------------------
 if (digitalRead(monitor_backlight) == 1 && mosfet_flag == 0) {
   digitalWrite(mosfet, HIGH);
   mosfet_flag = 1;
  }
   if (digitalRead(monitor_backlight) == 0 && mosfet_flag == 1) {
   digitalWrite(mosfet, LOW);
   mosfet_flag = 0;
  }
 
 //---------------Питание БП-----------------
    
  if (digitalRead(powerRGB) == 1) {
    if (flag == 0) {
      for (byte i = LED_NUM / 2; i < LED_NUM; i-- ){
        Li = LED_NUM / 2 + abs(LED_NUM / 2 - i) - 1;
        leds[i].setHue(counter + i * (255 / LED_NUM));
        leds[Li].setHue(counter + i * (255 / LED_NUM)); // Если добавить для i множитель pwm, то с помощью потенциометра я смогу менять длину шага радуги
        FastLED.setBrightness(bright);
        FastLED.show();
        enc1.tick();    // оставляем тут для работы "временных" функций и антидребезга
        enc1.setTickMode(AUTO);
        brightFunc();
  } } 
      if (flag == 1) {
        for (byte i = LED_NUM / 2; i < LED_NUM; i-- ){
          Li = LED_NUM / 2 + abs(LED_NUM / 2 - i) - 1;
          leds[i] = color;
          leds[Li] = color;
          brightFunc();
          FastLED.setBrightness(bright);
          FastLED.show();
          if (enc1.isRightH()){
            color += 50000;
          }
          if (enc1.isLeftH() && color > 0){
            color -= 50000;
          }
    } 
  } 
    counter ++; 
    } else {
      Li = LED_NUM - 1;
      for (byte i = 0; i < LED_NUM / 2; i++ ){
        leds[i] = 000;
        leds[Li] = 000; 
        Li--;
        FastLED.show();
       digitalWrite(relay, HIGH);
      }
    }
}  

                
    
