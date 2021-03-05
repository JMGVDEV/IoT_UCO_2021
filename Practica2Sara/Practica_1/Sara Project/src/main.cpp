#include <ESP8266WiFi.h>
 
const char* ssid = "UNE_HFC_4739";
const char* password = "4373172089";
 
int ledPin = 13; // GPIO13
int ledPin2=5;
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin2, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request one and two
 
  int value = LOW;   
  if (request.indexOf("/LED1=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  else if (request.indexOf("/LED1=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
  else if (request.indexOf("/LED2=ON") != -1)  {
    digitalWrite(ledPin2, HIGH);
    value = HIGH;
  }
  else if (request.indexOf("/LED2=OFF") != -1)  {
    digitalWrite(ledPin2, LOW);
    value = LOW;
  }
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led pin is now: ");
 
  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED1=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED1=OFF\"\"><button>Turn Off </button></a><br />");  
  client.println("<a href=\"/LED2=ON\"\"><button>Turn On Number2 </button></a>");
  client.println("<a href=\"/LED2=OFF\"\"><button>Turn Off Number2</button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
  