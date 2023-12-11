# Plant Happy 🌿
#### An end to end IoT solution for automated analysis of banana plants based on leaf disease detection and other weather and soil parameters. The solution is made for farmers allowing them to know which parameters of the banana plant are safe or in danger along with a guide page to help them care for the plant.
### Home Page
![](UI_Image/Home.png)
### Guide Page
![](UI_Image/Guide.png)

## Software Tech Stack 
[![](https://skillicons.dev/icons?i=html,css,tailwind,js,py,flask)](https://skillicons.dev)
* **HTML/Tailwind-CSS/JS:** For the User Interface of the application
* **Python:** Programming Language used for the backend.
* **Flask:** A micro web framework for developing the backend in the application.
* **Sckit-Learn:** Used for the development of the Random Forest Model for leaf disease detection.
* **ThingSpeak:** The cloud database used for analytics, pushing and recieving data from the hardware.

## Hardware Components:
* **Arduino IDE:** To program the microcontroller.
* **FTDI-Programmer:** Acts as an interface between USB and our microcontroller to help program it.
* **ESP32-Cam (OV2640):** Microcontroller with an OV2640 camera to capture images and retrieve analog signals from other sensors.
* **Breakout Board:** Allows the FTDI programmer to transmit voltage via the board to the ESP32 Microcontroller.
* **DHT11 Temperature & Humidity Sensor:** Allows us to get temperature in celcius and relative humidity.
* **Capacitative Soil Moisture Sensor:** Allows us to retrieve soil moisture levels with a relative interval of 0 - 1000.

## Architecture Diagram of the System
