IoT-Tutorial2-ESP32-MQTT-APP1

This folder contains two ESP32 projects on Arduino Genuino IDE, demonstrating control of built-in LED using MQTT protocol, with the Broker running on HiveMQ.
You need to perform the following steps to build and upload it into ESP32 development boards and test it.
Note: Even if you have only one ESP32 board you will be able to check this demo by yourself. I have used "DOIT ESP32 DEVKIT V1" for this demo, but you will be able to compile and upload this code for any other ESP32 family boards as well.

1. Login to your account on HiveMQ (if not done, follow Turtorial 1 video) and make sure that you have your IoT device credentials, CA certificate, host name of your broker instance running there.
2. If you are new to ESP32, refer the IoT-Tutorial0-ESP32-Installation-Guide-MouliSankaran.pdf to install Arduino Genuino IDE and how to bring up your ESP32 development board with the IDE.
3. Choose the ESP32 board that you want this code to uploaded on to, in the Arduino IDE. Tools-->Board:-->ESP32 Arduino-->DOIT ESP32 DEVKIT V1 (choose the last option according to your board)
4a.  MyWiFiCredentials.h: Update your WiFi username and password details in this header file.
4b. MyMQTTBrokerDetails.h: Update your IoT device details username and password that you have provided while creating your instance of MQTT Broker on HiveMQ. (mouli_iot and Test12345 are to be replaced according to your choices)
4c. MyMQTTBrokerCA.h: Update this file by adding double quotes and \n to the CA certificate file that you have downloaded, by replacing the current contents (which is meant for my instance of the broker) for your instance of broker running on HiveMQ.
Note: The .pem file name (trustid-x3-root.pem) that was shown in the tutorial may be different for your broker instance which you need to download from HiveMQ.
5. As metnioned in the Tutorial 2 video, you need to install the library EspMQTTClient before compiling the projects for ESP32.
6. After taking care of these changes in both the projects, the code should get compiled successfully.
7. Connect your ESP32 board through micro USB cable to the USB port of your machine.
8a. You can verfiy if the board is recognized by checking on your Arduino Genino IDE, Tools--> Port. You should see a COM x port getting displayed.
8b. If the board is not recognized, you can follow the steps given in this link: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/establish-serial-connection.html
9. If the COM port is coming up, choose it by clicking on it before trying to upload the code that you have built.

10. If all these changes are done correctly, then ESP32 should be able to connect to your WiFi and also establish TCP/TLS connection with the HiveMQ broker.
11. If it is not successful, it could be because any of these above steps were not done correctly. You may not get a detailed error message from the HiveMQ server, you need to debug it yourself, by adding Serial.print messages. 
Note: Since you may not be able to view the inital messages from the board when it is starting off from reset, you can download the code by keeping the Serial Monitor window already open prior to the uploading of code.
12. If you are not able to view any output on the Serial monitor, you might have missed the message because the serial monitor window was opened late, you can try also giving reset to your ESP32 board by keeping the serial monitor window open.
13. If you observe unrecognizable chars showing up on the Serial monitor, choose the Serial port baud rate as 115200 which is used in this project, in the setup() function in the .ino file.
Note: You can notice a drop-down menu at the bottom left corner of the Serial monitor window, which should be set to 115200 (matching what is used in Serial.begin() within setup() function)


