#include <Wire.h>
#include <MPU6050.h>
 
MPU6050 mpu;

int dodaj, a=0,srednia;
const int buzzer = A3;

void setup()
{
  Serial.begin(115200);
  pinMode(8, INPUT);
  Serial.println("Inicjalizacja MPU6050");
 
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Nie mozna znalezc MPU6050 - sprawdz polaczenie!");
    delay(500);
  }
}
 
void loop()
{
  
  Vector normAccel = mpu.readNormalizeAccel();
 

 
  Serial.print(" Xnorm = ");
  Serial.print(abs(normAccel.XAxis));
  Serial.print(" Ynorm = ");
  Serial.print(abs(normAccel.YAxis));
  Serial.print(" Znorm = ");
  Serial.println(abs(normAccel.ZAxis));

  dodaj = dodaj + abs(normAccel.ZAxis);
  a = a + 1;

  if( a==5 ){
    srednia = dodaj/a;
    if( srednia<6){
   
      while (digitalRead(8) == LOW){
       tone(buzzer, 1000); // Send 1KHz sound signal...
        delay(1000);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
        delay(1000);        // ...for 1sec
         }
         a=0;
      dodaj=0;
      srednia=0;
    }
    else{
      a=0;
      dodaj=0;
      srednia=0;
      
    }
    
  }
  Serial.println(abs(dodaj));
  Serial.println(abs(a));
  delay(500);
}
