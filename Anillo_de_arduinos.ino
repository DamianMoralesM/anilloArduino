#include <Wire.h>
//Estas son las variables del sensor de temperatura en esta caso(depende de sus sensores)

//Distancia
const int EchoPin = 5;
const int TriggerPin = 6;
int cmant = 0;
int cm = 0;

//Temperatura
int sensorPin=A0;
int temp = 0;
int tempant = 0;

struct Datos{
    byte dirOrigen;
    byte idSensor;
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

  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT); 
}
 
void loop() {

///////////////////////////////////////////////////Parte Distancia//////////////////////////////////////////////////////////  
  cm = ping(TriggerPin, EchoPin);
  if ((cmant != cm) && (cm < 200)){
  
  Serial.println("");
  Serial.print("Distancia: ");
  Serial.println(cm);
      
  // Enviamos como broadcast
  Wire.beginTransmission(0x0);
    
  // Inicializamos la union
  Mensaje msj;
 
  // Copiamos los valores
  msj.datos.dirOrigen = 1;
  msj.datos.idSensor = 3;
  msj.datos.valor = cm;
  
  // Escribimos en el canal
  for(int i=0 ; i < sizeof(msj.datos) ; i++)
    Wire.write(msj.arreglo[i]);
  
  // Paramos la transmisión
  Wire.endTransmission();
        
  //resguardamos el valor del sensor  
   cmant = cm;
  }
  
//////////////////////////////////////////Parte temperatura///////////////////////////////////////////

  int readVal=analogRead(sensorPin);
  temp = Sensor(readVal);
  if (temp != tempant){
  
  Serial.println("");
  Serial.print("Temperatura: ");
  Serial.println(temp);
      
  // Enviamos como broadcast
  Wire.beginTransmission(0x0);
    
  // Inicializamos la union
  Mensaje msj;
 
  // Copiamos los valores
  msj.datos.dirOrigen = 1;
  msj.datos.idSensor = 4;
  msj.datos.valor = temp;
  
  // Escribimos en el canal
  for(int i=0 ; i < sizeof(msj.datos) ; i++)
    Wire.write(msj.arreglo[i]);
  
  // Paramos la transmisión
  Wire.endTransmission();
        
  //resguardamos el valor del sensor  
   tempant = temp;
  }
}

//FUNCION DE TEMPERATURA
double Sensor(int analog) {
 double Temp;
 Temp = log(10000.0*((1024.0/analog-1)));
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;
 return (int) Temp;
}

//FUNCION DE DISTANCIA
int ping(int TriggerPin, int EchoPin) {
   long duration, distanceCm;
   
   digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
   delayMicroseconds(4);
   digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
   delayMicroseconds(10);
   digitalWrite(TriggerPin, LOW);
   
   duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
   
   distanceCm = duration * 10 / 292/ 2;   //convertimos a distancia, en cm

   delay(1000);
   return distanceCm;
}

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
    case 7:
      Serial.print("Humedad: ");
    break;
    case 8:
      Serial.print("Potencia: ");
    break;
  }

  //Escribo el valor
  Serial.print(msjRecibido.datos.valor);

  if(msjRecibido.datos.idSensor == 1){
  Serial.print(", ");  
  Serial.println(msjRecibido.datos.valor); 
  }  
}
