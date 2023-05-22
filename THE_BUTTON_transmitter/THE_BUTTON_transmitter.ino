#include <WiFi.h>
#include <SPI.h>
#include <WiFiUdp.h> 

//Client object
WiFiClient cli;

//UDP object 
WiFiUDP Udp;

//Define if connection to receiver was successful
bool connected;

//SSID and password of the internet
const char SSID[] = "WIFI-SSID";
const char PW[] = "WIFI-PASSWORD";

//LED pin
const int LED_PIN_ERROR = 5;
const int LED_PIN_GOOD = 18;

//Port to connect to
unsigned short int port = 60221;

//IP address of the receiver. Run the receiver code and it will give you 
//  its IP addresss. Put it in here.
IPAddress receiver_ip(192, 168, 0, 1);

void setup() {
  // put your setup code here, to run once:

  //Start serial monitor
  Serial.begin(9600);

  //Set pins to output mode
  pinMode(LED_PIN_ERROR, OUTPUT);
  pinMode(LED_PIN_GOOD, OUTPUT);

  //Disconnect if device was already connected to wifi
  WiFi.disconnect();
  delay(3000);

  //Connect to wifi network and wait to establish connection
  Serial.println("----starting program----");
  WiFi.begin(SSID, PW);
  
  while ((!(WiFi.status() == WL_CONNECTED))){
    Serial.println("...");
    delay(300);
  }
  
  //Notify of connection and print local IP address
  Serial.println("-------connected-------");
  Serial.println((WiFi.localIP()));

  //Check if connection to receiver was successful
  connected = cli.connect(receiver_ip, port);

  if (connected){
    Serial.println("Connected to receiver");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  //detect press of a button to send signal to receiver
  int inp = (touchRead(4));
  if (inp < 5){
    int attempt_counter = 0;
    Serial.println("Sending request");

    //If packet was successfully sent then turn on LED, if
    //  error then turn on other LED. 
    if (send_packet() < 0){
      digitalWrite(LED_PIN_ERROR, HIGH);
      delay(4000);
      digitalWrite(LED_PIN_ERROR, LOW);
    } else {
      digitalWrite(LED_PIN_GOOD, HIGH);
      delay(1000);
      digitalWrite(LED_PIN_GOOD, LOW);
    }
  }
}

//Send single UDP packet. Return 0 if success, else return -1.
signed short int send_packet(){
  if (Udp.beginPacket(receiver_ip, port) == 1){
    Udp.write(1);
    Udp.endPacket();
    return 0;
  } else {
    return -1;
  }
}
