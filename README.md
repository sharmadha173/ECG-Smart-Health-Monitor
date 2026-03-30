"IoT-Based Smart Health Monitoring System: ECG Noise Simulator"

Overview :
This project is an IoT-enabled ECG monitoring and noise simulation system built using an ESP8266 microcontroller and an AD8232 ECG sensor. It generates a simulated ECG (PQRS) waveform and features a custom web interface that allows users to inject and suppress various types of clinical signal noise in real-time. This project demonstrates advanced hardware-software integration, real-time web serving, and digital signal processing concepts.
Note: This repository contains the simulation and web-server codebase supporting the research published in the 9th IEEE International Conference on CSITSS-2025.

Features :
      Real-Time Web Server: Hosts an interactive HTML/JS interface directly from the ESP8266.
      Clinical Noise Injection: Dynamically simulates 5 common ECG interference types using mathematical modeling:
                    Baseline Wander
                    Muscle Artifact
                    Powerline Interference (50Hz)
                    Motion Artifact
                    White Noise
                    Hardware Integration: Configured to read analog inputs from the AD8232 sensor and utilize its Lead-Off detection pins.
                    Real-Time Telemetry: Streams simulated, noisy, and clean ECG data via serial output for visualization.

Tech Stack: 
       Hardware: ESP8266 (NodeMCU/Wemos), AD8232 ECG Sensor Biometric Board
       Software/Languages: C/C++ (Arduino framework), HTML, JavaScript, CSS
       Libraries: ESP8266WiFi.h, ESP8266WebServer.h

Hardware Wiring (AD8232 to ESP8266) :
      Output -> A0 (Analog Pin)
      LO+ -> D1 (Lead-Off Positive)
      LO- -> D2 (Lead-Off Negative)

How to Use :
      Clone this repository and open the .ino file in the Arduino IDE.
      Update the ssid and password variables with your local WiFi credentials.
      Flash the code to your ESP8266 board.
      Open the Serial Monitor (115200 baud) to find the ESP8266's dynamically assigned IP address.
      Type that IP address into any web browser connected to the same network to access the control panel and toggle noise interference.
      <img width="580" height="487" alt="image" src="https://github.com/user-attachments/assets/0f04804b-da82-46a0-beeb-21cd4fc54bf4" />
      <img width="621" height="277" alt="image" src="https://github.com/user-attachments/assets/9aaaec8e-b2f7-4e9d-95dc-84b281909ac3" />
      <img width="388" height="286" alt="image" src="https://github.com/user-attachments/assets/b66f278b-fadf-4185-a7f5-ec4ecdfcc8ae" />
      <img width="416" height="284" alt="image" src="https://github.com/user-attachments/assets/b277983b-0526-48f6-a07c-22e05c696b2b" />
      <img width="410" height="250" alt="image" src="https://github.com/user-attachments/assets/7c211c73-4fa4-4263-894c-cf3aa3cb6960" />
      <img width="392" height="232" alt="image" src="https://github.com/user-attachments/assets/9822af4b-b970-4814-bc8e-3d3fedfa8899" />
      <img width="370" height="235" alt="image" src="https://github.com/user-attachments/assets/7a7218aa-50b3-4b03-ad88-c479726a5501" />







