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
int Humedad;
int Referencia;
int Error;

//LCD PCD8544
// pin 4 - Serial clock out (SCLK)
// pin 5 - Serial data out (DIN)
// pin 6 - Data/Command select (D/C)
// pin X - LCD chip select (CS)
// pin X - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(4, 5, 6, 8, 9);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


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
  display.print("T.Ref:");
  display.println(Referencia);
  display.print("T.Term:");
  display.println(Temperatura);
  display.print("Humedad:");
  display.println(Humedad);
  display.display();
  display.clearDisplay();


  Temperatura = analogRead(A1);
  Humedad = analogRead(A4);
  Referencia = analogRead(A0);

  //Controlador PID
  Error = Referencia-Temperatura;
}
