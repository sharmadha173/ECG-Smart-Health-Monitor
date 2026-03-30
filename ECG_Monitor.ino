#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi Credentials
const char* ssid = "ECG";
const char* password = "123456789";

// Web server on port 80
ESP8266WebServer server(80);

// Pin Definitions
int ecgPin = A0;       // Analog input from AD8232 Output pin
int loPlusPin = D1;    // Lead-Off Detection Positive
int loMinusPin = D2;   // Lead-Off Detection Negative

// Variables
int loPlusStatus = 0;
int loMinusStatus = 0;
bool leadsConnected = false;

// ECG waveform simulation
const int pqrsWaveform[] = {
  0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
  80, 60, 40, 20, -100, 300, -150, 0,
  10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
  90, 80, 70, 60, 50, 40, 30, 20, 10, 0
};
const int waveformLength = sizeof(pqrsWaveform) / sizeof(pqrsWaveform[0]);
int waveformIndex = 0;

enum NoiseType { NONE, BASELINE_WANDER, MUSCLE_ARTIFACT, POWERLINE_INTERFERENCE, MOTION_ARTIFACT, WHITE_NOISE };  //defines various types of noise
NoiseType currentNoise = NONE;  //Initially, there is no noise applied to the signal.

float generateNoise(NoiseType type) {
  switch (type) {
    case BASELINE_WANDER:
      return 5.0 * sin(millis() / 1000.0);
    case MUSCLE_ARTIFACT:
      return random(-500, 500) / 100.0;
    case POWERLINE_INTERFERENCE:
      return 6.0 * sin(2 * 3.14159 * 50 * millis() / 1000.0);
    case MOTION_ARTIFACT:
      return (millis() % 2000 < 1000) ? 6.0 : -6.0;
    case WHITE_NOISE:
      return random(-800, 800) / 100.0;
    default:
      return 0.0;
  }
}

void handleAddNoise() {
  String type = server.arg("type");
  if (type == "baselineWander") currentNoise = BASELINE_WANDER;
  else if (type == "muscleArtifact") currentNoise = MUSCLE_ARTIFACT;
  else if (type == "powerlineInterference") currentNoise = POWERLINE_INTERFERENCE;
  else if (type == "motionArtifact") currentNoise = MOTION_ARTIFACT;
  else if (type == "whiteNoise") currentNoise = WHITE_NOISE;

  server.send(200, "text/plain", "Noise set: " + type);
}

void handleSuppressNoise() {
  currentNoise = NONE;
  server.send(200, "text/plain", "Noise suppressed");
}

void handleWebpage() {
  String webpage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ECG Noise Control</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 20px; }
    h1 { color: #2c3e50; }
    button, select { margin: 10px; padding: 10px 20px; font-size: 16px; }
  </style>
</head>
<body>
  <h1>ECG Noise Control</h1>
  <p>Select a noise type and add it to the ECG signal, or suppress noise.</p>

  <label for="noiseType">Select Noise Type:</label>
  <select id="noiseType">
    <option value="baselineWander">Baseline Wander</option>
    <option value="muscleArtifact">Muscle Artifact</option>
    <option value="powerlineInterference">Powerline Interference</option>
    <option value="motionArtifact">Motion Artifact</option>
    <option value="whiteNoise">White Noise</option>
  </select>
  <button onclick="addNoise()">Add Noise</button>
  <button onclick="suppressNoise()">Suppress Noise</button>

  <script>
    function addNoise() {
      const noiseType = document.getElementById("noiseType").value;
      fetch('/addNoise?type=' + noiseType)
        .then(response => response.text())
        .then(data => alert(data))
        .catch(error => console.error('Error:', error));
    }

    function suppressNoise() {
      fetch('/suppressNoise')
        .then(response => response.text())
        .then(data => alert(data))
        .catch(error => console.error('Error:', error));
    }
  </script>
</body>
</html>
  )rawliteral";
  server.send(200, "text/html", webpage);
}

void setup() {
  Serial.begin(115200);  //Initializes serial communication at a baud rate of 115200
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) //This loop continuously checks if the WiFi is connected.
   {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");  //once connected, this message is printed to indicate success.

  server.on("/", handleWebpage);
  server.on("/addNoise", handleAddNoise);
  server.on("/suppressNoise", handleSuppressNoise);

  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
  loPlusStatus = digitalRead(loPlusPin);
  loMinusStatus = digitalRead(loMinusPin);
  leadsConnected = (loPlusStatus == 0 && loMinusStatus == 0);

  if (!leadsConnected) {
    Serial.println("Leads are not properly connected!");
  } else {
    int cleanECG = pqrsWaveform[waveformIndex];
    float noise = generateNoise(currentNoise);
    int simulatedECG = cleanECG + noise;

    Serial.println(simulatedECG);
    
    waveformIndex = (waveformIndex + 1) % waveformLength;
  }

  delay(50);
}