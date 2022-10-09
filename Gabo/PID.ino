//Laboratorio de Microcontroladores
//Laboratorio #3 
//Realizado por:
//Gabriel Barahona Otoya B70896
//Yenner Gonzalez Araya B83375

//Librerias
#include <Adafruit_PCD8544.h>
#include <PID_v1.h>


// --- Decalracion Variables Globales ---

//Pines

/* Pinout:
D0-D1: Comunicacion Serial    DIGITAL - OUTPUT
D3: Switch Comunicacion PC    DIGITAL - INPUT
D4-D11: Salida PID            DIGITAL - OUTPUT
D12-D13: LED_R - LED_A        DIGITAL - OUTPUT
A0: Temp_Ref                  ANALOG  - INPUT
A1: Temp_Sense                ANALOG  - INPUT
A2: Humedad                   ANALOG  - INPUT 
A3-A5: Control Pantalla       DIGITAL - OUTPUT
*/

int ComuRX    = 0;
int ComuTx    = 1;
int EnableTx  = 2;
int LEDred    = 12;
int LEDblue   = 13;
int SCLK      = 17;
int DIN       = 18;
int DCS       = 19;
int CS        = 11;  // CS y RST no se usan por lo que se define como 2 pero no se conectaran luego.
int RST       = 11;


//Valores
int Temperatura;
float R1 = 100000;              // resistencia fija del divisor de tension 
double logR2, R2, TemperaturaX;
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
Adafruit_PCD8544 display = Adafruit_PCD8544(SCLK, DIN, DCS, CS, RST);


// PID

// Variables PID
double Ref_PID, SControl_PID, Temp_PID;
PID pid(&Temp_PID, &SControl_PID, &Ref_PID, 2,5,1, DIRECT);

// --- Set Up ---

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
  pinMode(EnableTx, INPUT);


  //Inicializa control PID
  Temp_PID = analogRead(A1)/4;
  Ref_PID = analogRead(A0)/4;
  pid.SetMode(AUTOMATIC); 
}

// --- Loop ---

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

  Humedad = analogRead(A2);
  HumedadX = map(Humedad, 0, 1023, 0, 100);
  Referencia = analogRead(A0);
  ReferenciaX = map(Referencia, 0, 1023, 0, 80);

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
  Temp_PID = analogRead(A1)/4;
  Ref_PID = analogRead(A0)/4;
  pid.Compute();
  
  Serial.begin(57600);
  // Se convierte de double a int y se mapea de int de 32 bits a 8 bits 
  SControl = (int)SControl_PID;
  //SControl = map((int)SControl_PID,0, 65445, 0,255);
  Serial.print(sizeof(SControl));
// Se ponen en los pines D4-D11 la salida del PID representada en 8 bits
  for (int i = 0; i < 8; i++)
  {
    if (bitRead(SControl, i) == 0)
    {
      digitalWrite(i+3, LOW);
    }
    else
    {
      digitalWrite(i+4, HIGH);
    }
  }  
}
