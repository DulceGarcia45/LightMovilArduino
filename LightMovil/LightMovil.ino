#include <analogWrite.h>
#include "Servo.h"
#include <DHT11.h>
#define Pecho 6
#define Ptrig 7
#define buzzer 10  //define el puerto de salida del buzzer
#define minima 5  //define la distancia minima a la que el buzzer emite sonido
#define maxima 10 //define la distancia maxima a la que el buzzer emite sonido
long duracion, distancia; 
int valorSensor = 0;


// Definimos la librerias par la conexion a nuestro servidor

#include "FirebaseESP32.h"

// Definimos la direccion host de nuestro servidor
#define FIREBASE_HOST "ligthmovil-4bb1c.firebaseio.com"
// Definimos el token de nuestro servidor
#define FIREBASE_AUTH "MSXwlatTeekdyQe9IRkRIYthnVIUK9MmjrLVwoIy"

// Definimos la conexion a punto de acceso wifi
#define WIFI_SSID "ATT_Internet_En_Casa_0929"
#define WIFI_PASSWORD "8QAGR2NGBD4"

WiFiClient client;
FirebaseData firebaseData;


#define LED 4
#define LED1 2
#define LED2 5

Servo myservo;  

void setup() {
  Serial.begin(9600);

  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println ("");
  Serial.println ("Se conectó al wifi!");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //Servomotor
    myservo.attach(13);  // attaches the servo on pin 13 to the servo object

  pinMode(LED, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  Serial.begin (9600);       // inicializa el puerto seria a 9600 baudios
  pinMode(Pecho, INPUT) ;     // define el pin 6 como entrada (echo)
  pinMode(Ptrig, OUTPUT) ;    // define el pin 7 como salida  (triger)
  pinMode(buzzer, OUTPUT) ;  // Define el pin 10 como salida

   
}

void leds() {

  Firebase.getInt(firebaseData,"/led");
  int dato=firebaseData.intData();
  
  analogWrite(LED, dato);

  Serial.println(dato);

  Firebase.getInt(firebaseData,"/led1");
  int dato1=firebaseData.intData();
  
  analogWrite(LED1, dato1);

  Serial.println(dato1);


  Firebase.getInt(firebaseData,"/led2");
  int dato2=firebaseData.intData();
  
  analogWrite(LED2, dato2);

  Serial.println(dato2);

}

void servo(){
  
   Firebase.getInt(firebaseData,"/puerta");
  int dato=firebaseData.intData();
  
  analogWrite(myservo, dato);

  Serial.println(dato);
  int pos = 0;    // variable to store the servo position
  //Servomotor
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
    
}


int pin=2;
DHT11 dht11(19);

void temperatura(){
  
  
  int err;
    valorSensor = analogRead(19);
    Serial.println(valorSensor);
       float temp, hum;
       if((err = dht11.read(hum, temp)) == 0)    // Si devuelve 0 es que ha leido bien
          {
            Firebase.getInt(firebaseData,"/temperatura");
           
             Serial.print("Temperatura: ");
             Serial.print(temp);
             Serial.print(" Humedad: ");
             Serial.print(hum);
             Serial.println();
              

          }
       else
          {
             Serial.println();
             Serial.print("Error Num :");
             Serial.print(err);
             Serial.println();
          }
       delay(1000);            //Recordad que solo lee una vez por segundo  


}

void Alarma(){
  
  digitalWrite(Ptrig, LOW) ;
  delayMicroseconds(2) ;
  digitalWrite(Ptrig, HIGH) ;   // genera el pulso de triger por 10ms
  delayMicroseconds(10) ;
  digitalWrite(Ptrig, LOW) ;
 
  duracion = pulseIn(Pecho, HIGH) ;
  distancia = (duracion/2) / 29;            // calcula la distancia en centimetros
 

  if (distancia <= 0){                      // si la distancia es menor o igual a cero
    Serial.println("---" ) ;                  // no mide nada
  }
  else {
    Serial.print(distancia);           // envia el valor de la distancia por el puerto serial
   Serial.println("cm" ) ;              // le coloca a la distancia la unidad "cm"
  } delay (500);                      // espera 1/2 segundo y mide de nuevo


   if (distancia >= minima && distancia <= maxima){ 
 //si el objeto esta en el rango entre la minima y maxima distancia emite sonido
    analogWrite(buzzer,128) ; //emite sonido
    delay(500); //espera medio segundo
    analogWrite(buzzer,LOW) ; // no emite sonido

   }
  
}
