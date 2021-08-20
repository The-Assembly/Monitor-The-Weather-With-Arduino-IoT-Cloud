#include <WiFiNINA.h>
#include "arduino_secrets.h"
#include "DHT.h"
///please enter your sensitive data in the Secret tab/arduino_secrets.h
//temporary status assigned when WiFi.begin()
int status = WL_IDLE_STATUS;     // the Wifi radio's status
WiFiClient client;
const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
#define DHTPIN 3    // what pin we're connected to, pin1 is 5th pin from end
const String devid = ""; //device ID on Pushingbox for our Scenario

#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN,DHTTYPE);
void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  dht.begin();
  while (!Serial);
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(SECRET_SSID);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(SECRET_SSID , SECRET_PASS );

    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");

  Serial.println("----------------------------------------");

  // if you get a connection, report back via serial:

  if (client.connect(WEBSITE, 80)) {

    Serial.println("connected to server");

    // Make a HTTP request:

    client.print("GET /pushingbox?devid=vFC3B02D2B4C0FD1&allowed_members=1");
    client.print("&Member_ID=5555");
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: api.pushingbox.com");
    client.println("Connection: close");
    client.println();
    
  }
}

void loop() {

  delay(5000);
  float humidityData=dht.readHumidity();
  float celData=dht.readTemperature();
  float fehrData=dht.readTemperature(true);
if (isnan(humidityData)||isnan(celData)||isnan(fehrData))
{
  Serial.println("Failed to read from DHT sensor!");
  return ;
}
Serial.print("Humidity");
Serial.print(humidityData);
Serial.print("%\t");
Serial.print("Temperature");
Serial.print(celData);
Serial.print("*C");
Serial.print("Temperature");
Serial.print(fehrData);
Serial.print("*F\t");
  // if you get a connection, report back via serial:
WiFiClient client;  //Instantiate WiFi object, can scope from here or Globally

    //API service using WiFi Client through PushingBox then relayed to Google
    if (client.connect(WEBSITE, 80))
      { 
         client.print("GET /pushingbox?devid=" + devid
       + "&humidityData=" + (String) humidityData
       + "&celData="      + (String) celData
       + "&fehrData="     + (String) fehrData
     
         );

      // HTTP 1.1 provides a persistent connection, allowing batched requests
      // or pipelined to an output buffer
      client.println(" HTTP/1.1"); 
      client.print("Host: ");
      client.println(WEBSITE);
      client.println("User-Agent: MKR1010/1.0");
      client.println();
      Serial.println("\nData Sent"); 
      }
}
