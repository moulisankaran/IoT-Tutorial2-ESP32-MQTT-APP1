//**********************************************************************************************************
// Author: Mouli Sankaran (mouli.sankaran@yahoo.com)
// Date: 7 Jul 2021
// This implementes accessing the MQTT TLS Broker running on HiveMQ to exchange MQTT messages 
// By Publishing and subscribing to the topic of interest
// This code demonstrates other MQTT Clients controlling the Built-in LED of this ESP32
// Either to switch it ON or OFF or to blink at 100 msecs or 1 sec rates
//**********************************************************************************************************
// PubSubClient library Ver 2.8.0  is installed that supports MQTT 3.1.1 and TLS option too
#include <PubSubClient.h>

// MQTT Task related parameters
// If you are adding more functionality to the MQTT_Task you may have to increase this stack size if the program crashes
// Please note that the crash could also be because of some logic error in the code or pointer corruption, etc.
#define MQTT_TASK_STACK_SIZE       6000   // Stack size in bytes for the MQTT task processing MQTT messages through WiFi
void MQTT_Task(void * parameter);         // The MQTT task that hadles the communication with the MQTT broker
int MQTT_TaskParam = 1234;               // Dummy parameter passed to MQTT task for verifying the task creation.
TaskHandle_t MQTT_TaskHandle;

// System and library headers
#include <string.h>
extern PubSubClient client;

#define BUILT_IN_LED   2    // On ESP32

// Topics for the control of Sensor 2 (this program instance is running on Sensor 1)
#define SNS_STATUS "sns_status"

#define MAX_PUB_MSG_BUF_LEN   128         // It should be lower than the length of message below
#define SNS2_PUBLISH_MSG  "Sensor 2 is alive ...\n"
char pub_msg_buf[MAX_PUB_MSG_BUF_LEN];   // Any message received more than this length is ignored

#define STAT_CNT_MAX 10
int stat_counter = 1;

#define LED_OFF            0
#define LED_BLINK_SLOW     1
#define LED_BLINK_FAST     2
#define LED_ON             3
#define LED_BLINK_DEFAULT  4

// LED blinking related constants and variables
#define SLEEP_DELAY_DEFAULT  1000   // LED needs to blink at 1 second rate by default
#define SLEEP_DELAY_SLOW     3000   // LED needs to blink at 3 seconds rate
#define SLEEP_DELAY_FAST     200    // LED needs to blink at 200 milliseconds rate
int sleep_delay = SLEEP_DELAY_DEFAULT;   // Default the LED blinks at defualt rate (at 1 second rate)

void control_LED(int cmd);

#define PUBLISH_DELAY 5000    // Publish once in 5 seconds I am alive message to MQTT Broker
int publish_delay = PUBLISH_DELAY;

int command = LED_BLINK_DEFAULT;     // Used by the application task taking care of blinking the LED
extern int new_command;              // Updated by the MQTT task on receiving new command message

bool led_stat = true;


void setup() {
  Serial.begin(115200);

  // initialize digital pin BUILT_IN_LED as an output.
  pinMode(BUILT_IN_LED, OUTPUT);
  // set it to HIGH which is the default state of LED
  digitalWrite(BUILT_IN_LED, HIGH);

  // This WiFi and MQTT task is created from setup() task running on Core 1 (APP core
  // The created MQTT task is pinned to Core 0 (PRO core) meant for running protcols and WiFi
  xTaskCreatePinnedToCore(
            MQTT_Task,                /* Task function which runs on Core 0*/
            "MQTT_Task_name",         /* String with the name of task. */
            MQTT_TASK_STACK_SIZE,     /* Stack size in words. */
            (void *)&MQTT_TaskParam,  /* Parameter passed as input of the task */
            2,                        /* Priority of the task. */
            &MQTT_TaskHandle,         /* Task handle. */
            0);                       /* Should always run on Core 0, which is a Protocol Core */
  
} // End of setup()

void loop() {

  delay(sleep_delay);

  publish_delay -= sleep_delay;   // Measure the time elapsed since last publishing of message on MQTT

  if(publish_delay < 0){
    // Construct the message to be published along with the status counter value at the begining
    snprintf (pub_msg_buf, MAX_PUB_MSG_BUF_LEN, "%d.%s", stat_counter, SNS2_PUBLISH_MSG);

    stat_counter++;
    if(stat_counter > STAT_CNT_MAX)
      stat_counter = 1;

    client.publish(SNS_STATUS, pub_msg_buf, strlen(pub_msg_buf)); 
    publish_delay = PUBLISH_DELAY;   // Set back to the MAX delay value after publishing a message
  }

  command = new_command;  // This can be protected by a semaphore since it is accessed by tasks running on both cores.
  control_LED(command);
 
}  // End of loop()


// Control the Built-in LED based on the command
void control_LED(int cmd){
  
  if(cmd == LED_OFF){
     digitalWrite(BUILT_IN_LED, LOW);  
     command = LED_OFF;
     led_stat = false;
  } 
  else if(cmd == LED_ON){
     digitalWrite(BUILT_IN_LED, HIGH);  
     command = LED_ON;
     led_stat = true;
  } 
  else if(cmd == LED_BLINK_FAST){
     command = LED_BLINK_FAST;
     sleep_delay = SLEEP_DELAY_FAST;
  } 
  else if(cmd == LED_BLINK_SLOW){
     command = LED_BLINK_SLOW;
     sleep_delay = SLEEP_DELAY_SLOW;      
  }
  else { // Illegal command received, blink it at default speed 
     command = LED_BLINK_DEFAULT;
     sleep_delay = SLEEP_DELAY_DEFAULT;   
  }

  // Need to process only if the command is one of the blinking cases, when it is not in ON or OFF condition
  if ( !(command == LED_ON || command == LED_OFF)){
    if(led_stat)
      digitalWrite(BUILT_IN_LED, LOW);  // If it is already ON, switch it OFF
    else 
      digitalWrite(BUILT_IN_LED, HIGH); // If it is already OFF, switch it ON

    led_stat = !led_stat;  // Then, toggle the LED stat
  }
  
} // end of control_LED()
