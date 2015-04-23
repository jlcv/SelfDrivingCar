const int ultrasonico = 10;
const int qrdInput = 3;
const int enableH = 6;
const int direccion = 2;

double d;
double cm;
int qrd;

int sum_ds;
int s4;
int s5;
int s6;
int s7;
int EstadoAnteriorS7 = 0;
int cambioDir;

double dc;
double dcHold;

int dirCarro = 0;

const int switch4 = A3;
const int switch5 = A5;
const int switch6 = A4;
const int switch7 = A2;

void setup() {
  Serial.begin(9600);
  pinMode(qrdInput, INPUT);
  pinMode(switch4, INPUT);
  pinMode(switch5, INPUT);
  pinMode(switch6, INPUT);
  pinMode(switch7, INPUT);
  
  pinMode(direccion, OUTPUT);
  digitalWrite(direccion, dirCarro);
}

//poner led para declarar que esta en apagado este pdo
void loop() {
//  if(){}else{} para prendido y apagado
  leerUltrasonico();
  leerQRD();
  imprimir();
  //checarObjetivo() este para frenar o seguir moviendo el carro
  //calcularPD(); este metodo da el dc que ahorita es con switch
  //AccionDeControl() 
  moverCarro();// este va a dentro de checar objetivo
  //pararCarro()// 
  delay(300);
}

void imprimir(){
//  Serial.print("QRD: ");
//  Serial.println(qrd);
  Serial.print("  |  Distancia: ");
  Serial.println(cm);
}

void moverCarro(){
  leerSwitches();
  potenciaMotor();
  analogWrite(enableH, dc);
}

void leerUltrasonico(){
  d = analogRead(ultrasonico) / 2;
  cm = d * 2.54;
}

void leerQRD(){
  qrd = digitalRead(qrdInput);
}

void leerSwitches(){
  s4 = digitalRead(switch4) * 1;
  s5 = digitalRead(switch5) * 2;
  s6 = digitalRead(switch6) * 4;
  s7 = digitalRead(switch7);
  
  if(s7 == 1 && EstadoAnteriorS7 == 0){
    EstadoAnteriorS7 = s7;
    cambiarDireccion();
  }else if(s7 == 0 && EstadoAnteriorS7 == 1){
    EstadoAnteriorS7 = s7;
    cambiarDireccion();
  }
  
  sum_ds = s4 + s5 + s6;
}

void potenciaMotor(){
  dc = ((sum_ds * 75) / 7) + 75;
}

void cambiarDireccion(){
  dcHold = dc;
  dc = 0;
  analogWrite(enableH, dc);
  
  if(dirCarro == 0){
    dirCarro = 1;
    digitalWrite(direccion, dirCarro);
  }else{
    dirCarro = 0;
    digitalWrite(direccion, dirCarro);
  }
  
  dc = dcHold;
  analogWrite(enableH, dc);  
}
