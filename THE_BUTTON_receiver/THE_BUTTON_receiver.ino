#include <WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>

//Server object
WiFiServer server(80);

//UDP object 
WiFiUDP Udp;

unsigned short int LED_CONNECTED_PIN = 13;

//Wifi SSID and password
const char SSID[] = "WIFI-SSID";
const char PW[] = "WIFI-PASSWORD";

void setup() {
  // put your setup code here, to run once:

  //Start serial monitor
  Serial.begin(9600);

  //Disconnect from wifi connection if one was already established
  WiFi.disconnect();
  delay(3000);
  
  Serial.println("----starting program----");

  pinMode(LED_CONNECTED_PIN, OUTPUT);
  
  //Connect to wifi 
  WiFi.begin(SSID, PW);
  
  while ((!(WiFi.status() == WL_CONNECTED))){
    Serial.println("...");
    delay(300);
  }
  
  Serial.println("-------connected-------");
  Serial.println((WiFi.localIP()));

  server.begin();
  Udp.begin(60221);
}

void loop() {
  WiFiClient cli = server.available();

  //This is not used to communitate but more for debugging purposes to know
  //  whether transmitter is able too successfully connect to the receiver. 
  if (cli){
    if (cli.connected()){
      digitalWrite(LED_CONNECTED_PIN, HIGH);
      delay(1000);
      digitalWrite(LED_CONNECTED_PIN, LOW);
    }
  } 
  check_port();
}

void check_port(){
  char buffer[255];
  if (!(Udp.parsePacket() == 0)){
    //Udp.read();
    Udp.read(buffer, 255);
    digitalWrite(LED_CONNECTED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_CONNECTED_PIN, LOW);
  }
}
