#include <Wire.h>
//Estas son las variables del sensor de temperatura en esta caso(depende de sus sensores)
int sensorPin= 0;
int temp = 0;
int tempAnt = 0;

struct Datos{
    int dirOrigen;
  int idSensor;
    int valor;
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
}
 
void loop() {

  //Esto es cosa de nuestro sensor, cada uno maneja el suyo
  temp = analogRead(sensorPin); 
  temp = (((5 * temp * 100.0)/1024.0)-50);
  // La idea es que el resultado lo dejen en una variable numerica en lo posible entera

 
  //PREGUNTA SI CAMBIO EL VALOR DE SU SENSOR
  if (tempAnt != temp){
    
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
  tempAnt = temp;
        
   }
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
  }

  //Escribo el valor
  Serial.print(msjRecibido.datos.valor);  
}
