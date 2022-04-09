#include <LiquidCrystal.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;// piny wyswietlacz
const int sw = 2, buz = 3, sho = 4, led_r = A0, led_g = A1, led_b = A2; //pozostałe piny
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int dodaj = 0, a = 0, srednia = 0;
unsigned long aktualnyCzas = 0;
unsigned long czas_leku = 20000, czas_wody = 30000;
bool lek_wziety = false, nawodniony = false;

void setup()
{
  pinMode(sw, INPUT_PULLUP);
  pinMode(sho, INPUT_PULLUP);
  pinMode(led_r, OUTPUT);
  pinMode(led_g, OUTPUT);
  pinMode(led_b, OUTPUT);
  pinMode(buz, OUTPUT);

  lcd.begin(16, 2);

  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) //SPRAWDENIE AKCELEROMETRU
  {
    lcd.setCursor(0, 0);
    lcd.print("mpu error");
    delay(500);
  }
}

void loop()
{

  //przypomnienie o lekach
  aktualnyCzas = millis();
  if (aktualnyCzas > czas_leku && lek_wziety == false)
  {
    while (digitalRead(sw) == HIGH)
    {
      digitalWrite(led_r, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("WEZ PIGULKE: A1"); //motywacja
      lcd.setCursor(0, 1);
      lcd.print("WCISNIJ PRZYCISK");
      digitalWrite(buz, HIGH);
      delay(100);
      digitalWrite(buz, LOW);
      delay(100);
    }
    lcd.clear(); //czysc ekran
    lek_wziety = true;
    digitalWrite(led_r, LOW);
    delay(500);
  }
  else if (aktualnyCzas > czas_wody && nawodniony == false)
  {
    while (digitalRead(sw) == HIGH)
    {
      digitalWrite(led_r, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("NAWODNIJ SIE"); //motywacja
      lcd.setCursor(0, 1);
      lcd.print("WCISNIJ PRZYCISK");
      digitalWrite(buz, HIGH);
      delay(100);
      digitalWrite(buz, LOW);
      delay(100);
    }
    lcd.clear(); //czysc ekran
    nawodniony = true;
    digitalWrite(led_r, LOW);
    delay(500);

  } else
  {
    //akcelerometr, sprawdzenie upadku
    Vector normAccel = mpu.readNormalizeAccel();
    dodaj = dodaj + abs(normAccel.XAxis);
    a = a + 1;
    if ( a == 5 )
    {

      srednia = dodaj / a;
    //  lcd.setCursor(0, 0);
     // lcd.print(srednia); //motywacja
      if ( srednia < 1)
      {
        while (digitalRead(sw) == HIGH)
        {
          lcd.setCursor(0, 0);
          lcd.print("PADLES POWSTAN"); //motywacja
          lcd.setCursor(0, 1);
          lcd.print("WCISNIJ PRZYCISK");
          digitalWrite(led_r, HIGH);
          digitalWrite(buz, HIGH);
          delay(100);
          digitalWrite(buz, LOW);
          digitalWrite(led_r, LOW);
          delay(100);
        }
        lcd.clear(); //czysc ekran
        a = 0;
        dodaj = 0;
        srednia = 0;
      }
      else
      {
        a = 0;
        dodaj = 0;
        srednia = 0;
      }
    }
  }

  //    if (digitalRead(sw) == LOW) //
  //    {
  //      digitalWrite(buz, HIGH);
  //    } else
  //    {
  //      digitalWrite(buz, LOW);
  //    }
  
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
}
