#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#define triac_gate   8
#define pot         A0
#define inAumentar 7
#define inDiminuir 6

bool ZC = 0, mudouA, mudouD, botA, botD;
uint16_t alpha, val = 0;
 
void setup(void) {
  Serial.begin(9600);
  pinMode(triac_gate, OUTPUT);
  digitalWrite(triac_gate, LOW);
  attachInterrupt(0, ZC_detect, CHANGE);       // Enable external interrupt (INT0)

  pinMode(pot, INPUT);
  
  pinMode(inAumentar, INPUT);
  pinMode(inDiminuir, INPUT);
  
  lcd.init();
  lcd.backlight();
}

void ZC_detect() {
  ZC = 1;
}

void aumentar() {
  val += 103;
  if(val > 1023) val = 1023;
}

void diminuir() {
  if(val < 103) val = 1;
  else val -= 102;
}
 
void loop() {

  /* 
   * Para usar potênciometro
   * val = analogRead(pot);
   */
  
  botA = digitalRead(inAumentar);
  botD = digitalRead(inDiminuir);
  
  if(!botA && !mudouA){
    aumentar();
    mudouA = 1;
  }
  if(botA && mudouA) mudouA = 0;

  if(!botD && !mudouD){
    diminuir();
    mudouD = 1;
  }
  if(botD && mudouD) mudouD = 0;
  
  Serial.print(map(val, 0, 1023, 0, 100));
  Serial.println("%");
  lcd.setCursor(1,1);
  lcd.print(map(val, 0, 1023, 0, 100));
  lcd.setCursor(1,4);
  lcd.print("%");
  if(ZC){
    if(alpha < 9500) {
      delayMicroseconds(alpha);
      digitalWrite(triac_gate, HIGH);
      delayMicroseconds(200);
      digitalWrite(triac_gate, LOW);
    }
    ZC = 0;
    
    alpha = ( 1023 - val ) * 10;
    if(alpha > 9500)
      alpha = 9500;
  }
}
