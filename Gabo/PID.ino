//Laboratorio de Microcontroladores
//Laboratorio #3 
//Realizado por:
//Gabriel Barahona Otoya B70896
//Yenner Gonzalez Araya B83375

//Librerias
#include <Adafruit_PCD8544.h>

//Pines
int LEDblue=13;
int LEDred=12;
int ComuPC=7;

//Valores
int Temperatura;
float R1 = 100000;              // resistencia fija del divisor de tension 
float logR2, R2, TemperaturaX;
float c1 = 0.6895404624e-03, c2 = 2.892045445e-04, c3 = 0.01028622639e-07;
// http://www.thinksrs.com/downloads/programs/Therm%20Calc/NTCCalibrator/NTCcalculator.htm


int Humedad;
int HumedadX;
int Referencia;
int ReferenciaX;
int SControl;
int Error;

//LCD PCD8544
// pin 4 - Serial clock out (SCLK)
// pin 5 - Serial data out (DIN)
// pin 6 - Data/Command select (D/C)
// pin X - LCD chip select (CS)
// pin X - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(4, 5, 6, 8, 9);

void setup() {
  Serial.begin(9600);
  Serial.println("PCD test");
  display.begin();
  // init done
  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(75);
  display.display(); // show splashscreen
  delay(500);
  display.clearDisplay();   // clears the screen and buffer
  // text display tests
  display.setTextSize(1);
  display.setTextColor(BLACK);

  pinMode(LEDblue, OUTPUT);
  pinMode(LEDred, OUTPUT);
  pinMode(ComuPC, INPUT);
}

void loop() {  
  display.setCursor(0,0);
  display.print("T.Ref: ");
  display.println(ReferenciaX);
  display.print("S.Ctrl: ");
  display.println(SControl);
  display.print("T.Term: ");
  display.println(TemperaturaX);
  display.print("Humedad: ");
  display.println(HumedadX);
  display.display();
  display.clearDisplay();


  Temperatura = analogRead(A1);
  R2 = R1 * (1023.0 / (float)Temperatura - 1.0);
  logR2 = log(R2);
  TemperaturaX = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  TemperaturaX = TemperaturaX - 273.15;

  Humedad = analogRead(A4);
  HumedadX = map(Humedad, 0, 1023, 0, 100);
  Referencia = analogRead(A0);
  ReferenciaX = map(Referencia, 0, 1023, 0, 100);

  if(TemperaturaX<30){
    digitalWrite(LEDblue, HIGH);
    digitalWrite(LEDred, LOW);
  }
  else if(TemperaturaX>45){
    digitalWrite(LEDblue, LOW);
    digitalWrite(LEDred, HIGH);
  }
  else if(30<TemperaturaX<45){
    digitalWrite(LEDblue, LOW);
    digitalWrite(LEDred, LOW);
  }


  //Controlador PID
  Error = ReferenciaX-TemperaturaX;
}
