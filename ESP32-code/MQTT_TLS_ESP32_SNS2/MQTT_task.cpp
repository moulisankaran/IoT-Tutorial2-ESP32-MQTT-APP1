//**********************************************************************************************************
// Author: Mouli Sankaran (mouli.sankaran@yahoo.com)
// Date: 7 Jul 2021
// This implementes accessing the MQTT TLS Broker running on HiveMQ to exchange MQTT messages 
// By Publishing and subscribing to the topic of interest
// This code demonstrates other MQTT Clients controlling the Built-in LED of this ESP32
// Either to switch it ON or OFF or to blink at 100 msecs or 1 sec rates
//**********************************************************************************************************

void MQTT_Task(void * parameter);         // The MQTT task that hadles the communication with the MQTT broker

// System and library headers
#include <string.h>
// Use WiFiClient Secure to establish TLS connection with the MQTT Broker running on Cloud (here, HiveMQ)
#include <WiFi.h>
#include <WiFiClientSecure.h>
// PubSubClient library Ver 2.8.0  is installed that supports MQTT 3.1.1 and TLS option too
#include <PubSubClient.h>

// Own headers having the WiFi credentials for ESP32 to connect to the MQTT Broker running on Cloud (HiveMQ)
#include "MyWiFiCredentials.h"
/* The above header file has the credentials of the WiFi network to which ESP32 is to be connected to */
// const char* wifi_ssid = "Your WiFi SSID";
// const char* wifi_password = "Your WiFi Password";

// HiveMQ: To create your own MQTT TLS Broker: https://www.hivemq.com/mqtt-cloud-broker/
#include "MyMQTTBrokerDetails.h"
#include "MyMQTTBrokerCA.h"

void MQTT_Task(void * MQTT_TaskParam);
void MQTTconnect(void);
void CallbackOnMsgReceived(char* topic, byte* payload, unsigned int length);
int process_msg(byte* payload, unsigned int len); // Process the message received and return the command to control the LED

// Default TCP port IDs on which MQTT Brokers wait on
#define MQTT_TLS_TCP_PORT_ID 8883  // To connect to TLS Broker running on HiveMQ
#define MQTT_TCP_PORT_ID     1883  // Not used here

// create an instance of WiFiClientSecure
WiFiClientSecure espClient;
PubSubClient client(espClient);

// Topics for the control of Sensor 2 (this program instance is running on Sensor 1)
#define SNS2_CTRL  "sns2_ctrl"
String clientId = "ESP32_SNS2";   // C++ String object 

#define CONNECT_RETRY_DELAY  5000 // If connection failed with HiveMQ, retry after 5 seconds

// Command or Message received that controls the built-in LED
#define LED_OFF_MSG         "off"
#define LED_BLINK_SLOW_MSG  "blinkSlow"
#define LED_BLINK_FAST_MSG  "blinkFast"
#define LED_ON_MSG          "on"
#define MAX_MSG_BUF_LEN     64

char rx_msg_buf[MAX_MSG_BUF_LEN];   // Any message received more than this length is ignored

#define LED_OFF            0
#define LED_BLINK_SLOW     1
#define LED_BLINK_FAST     2
#define LED_ON             3
#define LED_BLINK_DEFAULT  4

int new_command = LED_BLINK_DEFAULT; // Updated by the MQTT task on receiving new command message

void MQTT_Task(void * MQTT_TaskParam){

  Serial.print("MQTT task is created with the parameter: ");
  Serial.println(*(int*)MQTT_TaskParam);
  
  // First connect to the WiFi network
  Serial.print("MQTT_task: Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("MQTT_task: WiFi connected");
  Serial.print("MQTT_task: Local IP address of this board on WiFi network: ");
  Serial.println(WiFi.localIP());

  // set SSL/TLS certificate
  espClient.setCACert(ca_cert);
  // Configure the MQTT server with the domain address of MQTT TLS Broker running on HiveMQ
  // Also pass on the TCP port ID the server is waiting on
  client.setServer(domain, MQTT_TLS_TCP_PORT_ID);

  /* This is receivedCallback function will be invoked when client receives message on the subscribed topic */
  client.setCallback(CallbackOnMsgReceived);

  for(;;){   // Infinite loop of MQTT Task waiting for the messages from MQTT Broker
    
    /* if client was disconnected then try to reconnect again */
    if (!client.connected()) {
      MQTTconnect();
    }
    
    // This function listens for incomming messages on the subscribed topics
    // If any message is received the callback function CallbackOnMsgReceived() is invoked
    client.loop();

    vTaskDelay(10);   // This is added to give control back to the Scheduler and to avoid Watchdog Timer exception
    
  }  // End of infinite loop 
  
}  // End of MQTT_Task()

void MQTTconnect(void){
  
  /* Loop until reconnected */
  while(!client.connected()){    // If it not connected already connect with the MQTT broker
    
    Serial.print("MQTT connecting ...\n");
    
    /* connect now */
    // The MQTT Broker login username and password to be passed to the TLS broker on HiveMQ
    if (client.connect(clientId.c_str(), MQTTUsername, MQTTPassword)) {
      Serial.println("Connected with the MQTT Broker on HiveMQ");
      /* subscribe topic */
      client.subscribe(SNS2_CTRL);
    } else {
      Serial.print("ERROR!!! Failed to connect with HiveMQ MQTT Broker. The status code = ");
      Serial.print(client.state());
      Serial.println("Will try again in 5 seconds");
      
      // Wait for 5 seconds before retrying
      delay(CONNECT_RETRY_DELAY);
    }
    
  }  // End of while()
  
} // End of MQTTconnect()

// This function is called when a message is received on the subscribed topic from MQTT broker on receiving from a Publisher
void CallbackOnMsgReceived(char* topic, byte* payload, unsigned int length) {
  
  Serial.print("A message on MQTT topic: ");
  Serial.println(topic);
  Serial.print("The message received is: ");

  // Print the payload received on the Serial monitor (here it contains only printable chars)
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);      // payload contains the binary data of number of bytes received
  }
  Serial.println();

  // Process the message received and return the command
  new_command = process_msg(payload, length);
  
} // end of CallbackOnMsgReceived()

int process_msg(byte* payload, unsigned int len){

  int i;
  
  if(len >= MAX_MSG_BUF_LEN)
    len = MAX_MSG_BUF_LEN -1;    // One byte is reserved for NULL char as string terminator

  for(i = 0; i < len; i++)
    rx_msg_buf[i] = payload[i];

  rx_msg_buf[i] = NULL;   // Terminate the command received from the payload with a NULL

  // strcmp() returns 0 if the strings are equal
  // note that the second parameter to strcmp() is also a string
  
  if(!strcmp(rx_msg_buf, LED_OFF_MSG))
    return LED_OFF;
    
  if(!strcmp(rx_msg_buf, LED_ON_MSG))
    return LED_ON;

  if(!strcmp(rx_msg_buf, LED_BLINK_SLOW_MSG))
    return LED_BLINK_SLOW;
    
  if(!strcmp(rx_msg_buf, LED_BLINK_FAST_MSG))
    return LED_BLINK_FAST;

  return LED_BLINK_DEFAULT; // wrong command received
  
} // end of process_msg()
