#include <Wire.h>
//Estas son las variables del sensor de temperatura en esta caso(depende de sus sensores)
int sensorPin= 0;
int temp = 0;
int tempAnt = 0;
 
void setup() {
  // Me asigno una direccion de 1 a 6 (vamos a designar un numero por cada grupo)
  Wire.begin(1);
 
  // Registramos el evento al recibir datos
  Wire.onReceive(receiveEvent);
 
  // Iniciamos el monitor serie para monitorear la comunicación
  Serial.begin(9600);
}
 
void loop() {

  //Esto es cosa de nuestro sensor, cada uno maneja el suyo
  temp = analogRead(sensorPin); 
  temp = (((5 * temp * 100.0)/1024.0)-50);
  // La idea es que el resultado lo dejen en una variable numerica en lo posible entera

 
  //PREGUNTA SI CAMBIO EL VALOR DE SU SENSOR
  if (tempAnt != temp){

      // Comenzamos la transmisión a los 8 arduinos
      for (int i=1; i <= 6; i++){
      Wire.beginTransmission(i);
 
      // Enviamos nuestra direccion, 
      Wire.write(1); 
         
      // Enviamos el id del sensor 
      Wire.write(4);
         
     // Enviamos el valor del sensor
     Wire.write(temp);
  
     // Paramos la transmisión
     Wire.endTransmission();
        
     //resguardamos el valor del sensor  
     tempAnt = temp;
     }     
   }
}

//Esto se activa cuando recibo algo (Es la parte del esclavo) 
void receiveEvent(int howMany) {
  int origen = 0;
  int idSensor = 0;
  int valor = 0;
  
  //Leo el primer byte que me dice el arduino que envio
  origen = Wire.read();
  Serial.println("");
  Serial.print("Arduino: ");
  Serial.println(origen);
  
  //Leo el segundo byte que me dice que sensor es (tenemos que armar un switch grande con un id por sensor)
  idSensor = Wire.read();
  switch(idSensor){
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

  //Leo el tercer byte que me dice el valor
    valor = Wire.read();
    Serial.print(valor);  

  //Pregunto si el sensor es Joystick (Xq este manda dos valores)
  if (idSensor == 1){
    int valor2 = 0;
    valor2 = Wire.read();
    Serial.print(", ");
    Serial.println(valor2);  
  }  
}
