// Improtación de Librerías
#include <Arduino_JSON.h>
#include <WiFiNINA.h>


// Pin del senesor
int sensorpin = A0;

// Variables para conexión WIFI
char ssid[] = "iPhone de valdeon";
char password[] = "fer30300";
int status = WL_IDLE_STATUS;
char server[] = "3b07-148-241-111-162.ngrok.io";  
WiFiClient client;


// SETUP
void setup(){
  Serial.begin(9600);

  // Conexión a red WIFI
  while (status != WL_CONNECTED) {
    Serial.println("Attempting to connect to Network: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid,password);
    delay (1000);
  }

  Serial.print("Connected to SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

}


// LOOP
void loop(){

   float medicion; // Medición hecha por sensor 
   JSONVar data; // Variable JSON que almacenará la data

   // Lectura del sensor
   medicion = analogRead(sensorpin);

   // Calculo del valor con medición
   float voltaje = medicion * (5.0 / 1023.0);
   float resistencia = 1000*( (5 - voltaje) / voltaje);
   double valorCO =-0.913 * log(resistencia) + 9.6282;

   // Indicar el id del Dispositivo
   int id = 14;

   // Agregar valores al JSON
   data["valorMedicion"] = valorCO;
   data["idDispositivo"] = id;

   // Volver string el JSON
   String dataString = JSON.stringify(data);

   // Conexión con API desplegada en ngrok
   if (client.connect(server, 80)) {
    client.println("POST /mediciones/nuevo HTTP/1.1");
    client.println("Host: 3b07-148-241-111-162.ngrok.io");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(dataString.length());
    client.println();
    client.print(dataString);
   }
   
   if (client.connected()) {
    client.stop();
  }

  // Imprimir el JSON
  Serial.println(dataString);

  // Ciclos de 5 segundos
  delay(5000);
  
}
