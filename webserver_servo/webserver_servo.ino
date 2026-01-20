#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const char* SSID = "WLANName";
const char* PASSWORD = "Password1234";

Servo base;
WebServer server(80);

const char* INDEX_HTML PROGMEM = R"rawlit(
<!DOCTYPE html><html lang="en">
<head><meta charset="utf-8"><title>ESP32 Servo Control</title>
<style>
  body{font-family:sans-serif;text-align:center;padding:2rem;background:#f4f6fa;}
  input[type=range]{width:80%%;}
  .angle{font-weight:bold;font-size:1.2rem;}
</style>
<script>
function updateAngle(val){
  document.getElementById('aval').innerText=val;
  fetch('/?angle='+val);
}
</script></head>
<body>
  <h2>ESP32 Servo on GPIO 15</h2>
  <p><span class="angle" id="aval">90</span>°</p>
  <input type="range" min="0" max="180" value="90" oninput="updateAngle(this.value)">
  <p>Drag the slider or send <code>/?angle=°</code></p>
</body></html>
)rawlit";

void handleRoot() {
  if (server.hasArg("angle")) {
    int angle = server.arg("angle").toInt();
    angle = constrain(angle, 0, 180);
    Serial.printf("%s\n", analogRead(15))
    base.write(angle);
  }
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}

void setup() {
  Serial.begin(115200);

  Serial.printf("Connecting to %s", SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.printf("\nConnected! IP address: %s\n", WiFi.localIP().toString().c_str());

  base.attach(15);
  base.write(0);

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();
}
