#include <EEPROM.h>
 //programa que va adelante se para y avanza

//Hbridge Motor Control
const int EN=9;   //Half Bridge 1 Enable
const int MC1=3;  //Motor Control 1
const int MC2=2;  //Motor Control 2
const int POT=0;  //POT on Analog Pin 0
const int pb=4;
const int qrd= 7;  
const int ledAlto = 12;
const int ledOn = 13;
const int anPin = A0;

int vuelta=0;
  long inicio;
  long milisegundos;
  double seg;
  
  int root;
int errorActual;

int senalQrd=0;

float velDeseada=.5;
double tau=1.7;
double  kp=2;
double to= 0.0316;
double ti=0.117144;
double ts=.68;
int estadoAnterior;
boolean obstaculo =false;


int val = 0;      //for storing the reading from the POT
int velocity = 115; //For storing the desired velocity (from 0-255)
boolean pb1=LOW;
const int sus=A0; // Sensor Ultra Sonico SUS


int potencia=255;
double velocidad =0;
long anVolt, inches, cm;
double   distancia=0.0;
boolean avanzando;
boolean refresco;
int counter = 0;

void setup()
{
  Serial.begin(9600);
  inicio = millis();
  pinMode(pb,INPUT);
  pinMode(EN, OUTPUT);
  pinMode(MC1, OUTPUT);
  pinMode(MC2, OUTPUT);
     pinMode(qrd,INPUT);
     pinMode(ledOn,OUTPUT);
     pinMode(ledAlto,OUTPUT);
  //forward(velocity);//Initialize with motor on
  avanzando = false;
  refresco = false;
  root=0;
  velocidad=0;
}

void loop() {
  
        

  
  milisegundos = millis();
  contarDistancia();

 
   
   
  
  if(!refresco) { //reset false no se le a picado
  
  
    if(digitalRead(pb) == HIGH && avanzando == true) { // esta avanzando y que se pare
      brake();
      refresco = true;
      delay(1000);
    }
    
    
    else if (digitalRead(pb) == HIGH && avanzando == false) {  //esta parado que avance    
      forward(potencia);
      refresco = true;
      delay(1000);
    }
  } //refresco false 
  
    if(!digitalRead(pb) == HIGH) {
      refresco = false;
    }
  
  


   if(avanzando) 
   {
      distancia = leerUltrasonico();
      
      if (distancia <= 100 && obstaculo ==false) {
//      Serial.println(distancia);
      brakeAvanzando();
      delay(20);
      reverse(255);
      delay(250);
      brakeAvanzando();
      obstaculo=true;
      }
      
      else 
      
      if(distancia > 100 && obstaculo ==true){
        
      forward(potencia);
      obstaculo=false;
    
    }
    
    if (distancia <= 100){
    digitalWrite(ledAlto,HIGH);
    }
    
    if (distancia > 100){
    digitalWrite(ledAlto,LOW);
    }
    

    digitalWrite(ledOn,HIGH);

  
    
   }


senalQrd=digitalRead(qrd);
Serial.println("qrd");
Serial.println(senalQrd);
 delay(10);
 guardar();


 

 
 
 
}

//Motor goes forward at given rate (from 0-255)
void forward (int rate)
{
  avanzando = true;
  digitalWrite(EN, LOW);
  digitalWrite(MC1, HIGH);
  digitalWrite(MC2, LOW);
  analogWrite(EN, 255);
}
//Motor goes backward at given rate (from 0-255)
void reverse (int rate)
{
  digitalWrite(EN, LOW);
  digitalWrite(MC1, LOW);
  digitalWrite(MC2, HIGH);
  analogWrite(EN, 255);
}
//Stops motor
void brake ()
{
  avanzando = false;
  digitalWrite(EN, LOW);
  digitalWrite(MC1, LOW);
//  digitalWrite(MC2, LOW);
  digitalWrite(EN, HIGH);
//  Serial.println("Brake!!!");
}

double leerUltrasonico(){
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

//Stops motor
void brakeAvanzando ()
{
  digitalWrite(EN, LOW);
  digitalWrite(MC1, LOW);
  digitalWrite(MC2, LOW);
  digitalWrite(EN, HIGH);
  Serial.println("Brake!!!");
}

void contarDistancia(){
  
if( senalQrd == 0 ){
   vuelta=vuelta+1;
   distancia = vuelta * .161;
   Serial.println("distancia recorrida=");
  Serial.println(distancia);

   //Serial.println("tiempo=");
   //Serial.println(milisegundos);

   seg=milisegundos/1000;
   Serial.println("segundos");
   Serial.println(seg);
   velocidad = distancia / seg ;

   Serial.println("velocidad actual=");
   Serial.println(velocidad);
//   Serial.println(root);
//  EEPROM.write(root, velocidad);
//  root = root + 1;
//  
//  if(root == EEPROM.length())
//  root = 0;
   
   }
}


void guardar()
{
  double dist=distancia;
  double s=seg;
  double vel= velocidad;
  
  Serial.println(root);
  EEPROM.write(root, dist);
  root = root + 1;
  
  Serial.println(root);
  EEPROM.write(root, seg);
  root = root + 1;
  
  Serial.println(root);
  EEPROM.write(root, vel);
  root = root + 1;
  
  if(root == EEPROM.length())
  root = 0;
  

}


