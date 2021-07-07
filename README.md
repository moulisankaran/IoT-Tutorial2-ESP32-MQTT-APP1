# IoT-Tutorial2-ESP32-MQTT-APP1

# This folder contains two ESP32 projects on Arduino Genuino IDE demonstrating control of built-in LED using MQTT protocol, with Broker running on HiveMQ.
# You need to update the following on each of the projects indvidually before building and uploading it into ESP32 development boards.
# 1. Choose the ESP32 board that you want this code to uploaded on to, in the Arduino IDE.
# 2a. Update your WiFi username and password details in the header file: MyWiFiCredentials.h
# 2b. MyMQTTBrokerDetails.h: Update your IoT device details username and password that you have provided while creating your instance of MQTT Broker on HiveMQ. (mouli_iot and Test12345 to be replaced)
# 2c. MyMQTTBrokerCA.h: Update this file by adding double quotes and \n as shown in the existing file, by just replacing the CA certificate file that you have downloade for your instance of broker running on HiveMQ.
# Note: The .pem file name (trustid-x3-root.pem) which was shown in the tutorial may be different for your broker instance which you need to download from HiveMQ.
# 3. As metnioned in the Tutorial 2 video, you need to install the library EspMQTTClient before compiling the projects for ESP32.

# 4. After taking care of these changes in both the projects, the code should get compiled successfully.
# 5. Connect your ESP32 board through micro USB cable to the USB port of your machine.
# 6. You can verfiy if the board is recognized by verifying on your Arduino Genino IDE, Tools--> Port, you should see a COM port getting displayed.
# 7. If it is not recognized, you can follow the steps given in this link: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/establish-serial-connection.html
# 8. If the COM port is coming up, choose it by clicking on it before trying to upload the code that you have built from your machine.

# 9. If all these changes are done correctly, then ESP32 should be able to connect to your WiFi and also establish TCP/TLS connection with the HiveMQ broker.
# 10. If it is not successful, it could be because any of these above steps are incorrect, you may not get detailed error message from the HiveMQ server, you need to debug it yourself.
# 11. If you are not able to view any output on the Serial monitor, you might have missed the message because the serial monitor window was opened late, you can try giving reset to your ESP32 board by keeping the serial monitor window open.
# 12. If you observe junk chars showing up on the Serial monitor, choose the Serial port baud rates as 115200 which is shown in this project, in the setup() function in the .ino file.
