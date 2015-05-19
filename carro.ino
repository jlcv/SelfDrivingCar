#include <EEPROM.h>
#include <TimerOne.h>

//Hbridge Motor Control
const int qrd= 2;   //Half Bridge 1 Enable
int contador;

const int EN=9;   //Half Bridge 1 Enable
const int MC1=3;  //Motor Control 1
const int MC2=7;  //Motor Control 2
const int POT=0;  //POT on Analog Pin 0
const int pb=4;

const int anPin = A0;
double velocidadActual;
double errorActual;
double errorAnterior;
double estadoAnterior;
double control;
int x;
int potencia;
double distancia;
double ts=.68;
 int numberOfRevs;
long anVolt, inches, cm;
boolean avanzando;
boolean refresco;



void setup()
{
  Serial.begin(9600);
  contador=0;
  
  pinMode(pb,INPUT);
  pinMode(EN, OUTPUT);
  pinMode(MC1, OUTPUT);
  pinMode(MC2, OUTPUT);
  
  avanzando = false;
  refresco = false;

  velocidadActual=0;
 
  errorActual=0;
  errorAnterior=0;
  estadoAnterior=0;
  control=0;

  x=0;
  potencia = 0;
  numberOfRevs=0;
  
}

void loop()
{

  x=digitalRead(qrd);
    Serial.println(x);
  
  potencia = pid();
  
  if(!refresco) {
    if(digitalRead(pb) == HIGH && avanzando == true) {
      brake(potencia);
      refresco = true;
      delay(1000);
    }
    else if (digitalRead(pb) == HIGH && avanzando == false) { //parado y se quiere que avance  
      attachInterrupt(0, qrdInterrupt, RISING);
      Timer1.initialize(680000);                       
      Timer
      refresco = true;
    }
  } else {
    
       distancia = leerUltrasonico();
       Serial.println(distancia);
       int inputQrd = digitalRead(qrd);
       Serial.println(inputQrd);
  if (avanzando)
  {
    if (distancia <= 100) 
    {
      brake(potencia);
    } 
  } else
  {
     if (distancia <= 100)
     {
       
     } else
     {
        forward(128); 
     }
  } 
  }
  
  
  
  
 
  
}

void pid()
{
   detachInterrupt(qrd);
    double tau=1.7;
    double  kp=2;
    double to= 0.0316;
    double ti=0.117144;
    //double velocidadDeseada=.500794726;
    //velocidadActual=checkSpeed();
   
    errorActual=128-estadoAnterior; 
    control = estadoAnterior + ((ts*kp)/(ti) + kp) * errorActual - kp * errorAnterior;
    estadoAnterior=control;
    errorAnterior=errorActual;
    control = map(control, 0, 1, 0, 128);
}

void forward (double rate)
{
  digitalWrite(MC1, HIGH);
  digitalWrite(MC2, LOW);
  analogWrite(EN, rate);
}

double checkSpeed()
{
   double speed = numberOfRevs/ts;
   numberOfRevs = 0; 
   speed = speed * 16.1;
   return speed;
}

void qrdInterrupt()
{
  numberOfRevs++;
}

void brake (int potencia)
{
  
  for(int i=potencia;i>0;i/2)
  {
  //digitalWrite(EN, LOW);
  digitalWrite(MC1, HIGH);
  digitalWrite(MC2, LOW);
  analogWrite(EN, i);
  delay(10);
  }
  
}

double leerUltrasonico()
{
    anVolt = analogRead(anPin)/2;
    delay(10);
    
 
  cm = anVolt * 2.54;
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  //reset sample total
  delay(50);
  return cm;
}

