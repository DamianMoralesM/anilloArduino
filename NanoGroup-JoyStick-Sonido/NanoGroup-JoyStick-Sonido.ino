#include <Wire.h>
//Variables Sensor Sonido
int aSensor = A2;
int son = 0;
int antson = 0;



//Variables Joystick
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = A0; // analog pin connected to X output
const int Y_pin = A1; // analog pin connected to Y output
int ya = 0;
int xa = 0;

struct Datos{
    int dirOrigen;
    int idSensor;
    int valor;
    int valor2;
};

union Mensaje {
   Datos datos; // Estructura
   byte arreglo[sizeof(Datos)]; // Representacion array bytes
};

 
void setup() {
  // Me asigno una direccion de 1 a 6 (vamos a designar un numero por cada grupo)
  Wire.begin(1);
 
  // Registramos el evento al recibir datos
  Wire.onReceive(receiveEvent);
 
  // Iniciamos el monitor serie para monitorear la comunicación
  Serial.begin(9600);
  
  // Activa recepcion broadcast
  TWAR = (1 << 1) | 1;

  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  pinMode( aSensor, INPUT_PULLUP) ;

}
 
void loop() {

// <-------------------------------------Maestro-------------------------------------------------------------------------------------------->

  // <------------------------------------------------------JoyStick ------------------------------------------------------->
  int x = analogRead(X_pin) - 519;
  int y = analogRead(Y_pin) - 501;
  delay(1000);

  if ((xa != x) && (ya != y)){

  Serial.println("x: " + x);
  Serial.println("y: " + y);
      
  // Enviamos como broadcast
  Wire.beginTransmission(0x0);
    
  // Inicializamos la union
  Mensaje msj;
 
  // Copiamos los valores
  msj.datos.dirOrigen = 1;
  msj.datos.idSensor = 1;
  msj.datos.valor = x;
  msj.datos.valor2 = y;
  
  // Escribimos en el canal
  for(int i=0 ; i < sizeof(msj.datos) ; i++)
    Wire.write(msj.arreglo[i]);
  
  // Paramos la transmisión
  Wire.endTransmission();
        
  //resguardamos el valor del sensor  
   ya = y;
   xa = x;
   }


   // <---------------------------------------------------- Sonido ----------------------------------------------------------->

   

  son = (int) analogRead(aSensor);
  delay(1000);
  
  if (antson != son){
  
  Serial.println("");
  Serial.print("Sonido: ");
  Serial.println(son);
      
  // Enviamos como broadcast
  Wire.beginTransmission(0x0);
    
  // Inicializamos la union
  Mensaje msj;
 
  // Copiamos los valores
  msj.datos.dirOrigen = 1;
  msj.datos.idSensor = 2;
  msj.datos.valor = son;
  
  // Escribimos en el canal
  for(int i=0 ; i < sizeof(msj.datos) ; i++)
    Wire.write(msj.arreglo[i]);
  
  // Paramos la transmisión
  Wire.endTransmission();
        
  //resguardamos el valor del sensor  
   antson = son;
   }
}

// <-------------------------------------Esclavo-------------------------------------------------------------------------------------------->
//Esto se activa cuando recibo algo (Es la parte del esclavo) 
void receiveEvent(int howMany) {
  
  // Inicializamos la union
  Mensaje msjRecibido;
  
  // Resguardamos tamaño para leerlo despues
  int tamano = sizeof(msjRecibido);
  
  // Recibimos todos los bytes y escribimos en la estructura 
  // array de la estructura
  for (int k=0; k < tamano; k++){
    msjRecibido.arreglo[k] =  Wire.read();
  }
  
  //Escribo el id del arduino origen
  Serial.println("");
  Serial.print("Arduino: ");
  Serial.println(msjRecibido.datos.dirOrigen);
  
  //Escribo el nombre del sensor
  switch(msjRecibido.datos.idSensor){
    case 1:
      Serial.print("Posicion: ");
    break;
    case 2:
      Serial.print("Sonido: ");
    break;
    case 3:
      Serial.print("Distancia: ");
    break;
    case 4:
      Serial.print("Temperatura: ");
    break;
    case 5:
      Serial.print("Movimiento: ");
    break;
    case 6:
      Serial.print("Rotacion: ");
    break;
  }

  //Escribo el valor
  Serial.print(msjRecibido.datos.valor);

  if(msjRecibido.datos.idSensor == 1){
  Serial.print(", " + msjRecibido.datos.valor2);  
  }  
}
