#include <WiFi.h>

const char* ssid  = "WIFIName";
const char* password = "Password1234";

WiFiServer server(80);

String header;
String outputState = "off";

const int output = 16;

unsigned long currentTime  = 0;
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    previousTime = millis();
    Serial.println("New client connected");
    String currentLine = "";

    while (client.connected() && (millis() - previousTime <= timeoutTime)) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {

            // ---------- HANDLE REQUEST ----------
            if (header.indexOf("GET /16/on")  >= 0) {
              Serial.println("GPIO 16 on");
              outputState = "on";
              digitalWrite(output, HIGH);
            } else if (header.indexOf("GET /16/off") >= 0) {
              Serial.println("GPIO 16 off");
              outputState = "off";
              digitalWrite(output, LOW);
            }

            // ---------- SEND RESPONSE ----------
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
 
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>");
            client.println("html,body{font-family:Helvetica,Arial,sans-serif;display:inline-block;margin:0 auto;text-align:center;background:#121212;color:#e0e0e0}");
            client.println(".button{background:#03dac5;border:none;color:#000;padding:16px 40px;font-size:30px;margin:2px;cursor:pointer;border-radius:8px}");
            client.println(".button:hover{filter:brightness(115%)}");
            client.println(".button2{background:#cf6679;color:#000}");
            client.println("a{text-decoration:none}");
            client.println("</style></head>");

            client.println("<body><h1>ESP32 Web Server</h1>");
            client.println("<p>GPIO 16 - State: <strong>" + outputState + "</strong></p>");
            if (outputState == "off") {
              client.println("<p><a href=\"/16/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/16/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    header = "";
    client.stop();
    Serial.println("Client disconnected\n");
  }
}
