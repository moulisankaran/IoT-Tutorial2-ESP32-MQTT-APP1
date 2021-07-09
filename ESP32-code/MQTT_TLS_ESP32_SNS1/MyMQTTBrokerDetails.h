//**********************************************************************************************************
// Author: Mouli Sankaran (mouli.sankaran@yahoo.com)
// Date: 7 Jul 2021
// The link of HiveMQ where login was created to run my instance of MQTT Broker
// This MQTT Broker is running on Azure cloud (it has option to run it on AWS too) for free
// This allows IoT devices running on private network having access to Internet to access this Broker
// To either Publish and Subscribe to control and to be controlled by other devices and controllers
//**********************************************************************************************************

#ifndef MYMQTTBROKERDETAILS_H
#define MYMQTTBROKERDETAILS_H

// This is the Domain name of the MQTT TLS Broker running on HiveMQ
const char* domain = "0f04209940b547c6b783ccfba9b7f1a1.s2.eu.hivemq.cloud";

// My IoT device credentials created on HiveMQ to log into the MQTT Broker running there.
const char* MQTTUsername = "mouli_iot";
const char* MQTTPassword = "Test12345";

#endif //MYMQTTBROKERDETAILS_H
