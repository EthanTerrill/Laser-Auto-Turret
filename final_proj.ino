#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


#include <AccelStepper.h>


#include <string.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "../lib/lcd.h"


// ================================== MACROS ====================================== //

// Stepper motor values
#define IN1 6
#define IN2 5
#define IN3 4
#define IN4 7


// Led And LASER Pins
#define LEDR_PIN 42
#define LASER_PIN 40 
#define BUTTON_PIN 38
#define TRIGGER_PIN 1
#define ECHO_PIN 2


#define MAX_DISTANCE 50

#define MAX_ANGLE 360
#define ANGLE_TO_STEPS(x) (x * 200) / 180






// ============================= GLOBAL VARIABLES ======================================//
SemaphoreHandle_t binarySemaphore;
float avg = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the lcd

TaskHandle_t xMotorHandle;
TaskHandle_t xSensorHandle;
TaskHandle_t xHandle;
bool warning = false; // used for detecting if an object is available
AccelStepper myStepper(AccelStepper::FULL4WIRE, IN1, IN3, IN2, IN4);

// ============================= Function prototypes ======================================//


/*
* @description  rotates motor from side to side
*/
void process_motor(void* arg);


/*
* @description  blinks led on and off, and also
*                fires laser when warning is set to on
*/
void Alarm_task(void *arg);


/*
* @description  displays current status
*/
void LCD_Task(void *arg);

/*
* @description  retrieves measurements from the
*               ultrasonic distance sensor
*/
void getDistanceTask(void* arg);


/*
* @description  pauses motor and sensor tasks when 
*               button pressed
*/
void Task_ButtonHandle(void *pvParameters);


void setup() {
  // set the maximum speed, acceleration factor,
  // and the target position

  binarySemaphore = xSemaphoreCreateBinary();
  if(binarySemaphore == NULL) {
    Serial.println("ERROR SEMAPHORE");
  } else {
    xSemaphoreGive(binarySemaphore);
  }

  Serial.begin(115200);
  myStepper.setMaxSpeed(1600.0);
  myStepper.setAcceleration(200.0);
  myStepper.setSpeed(100.0);


  pinMode(LEDR_PIN, OUTPUT);
  pinMode(LASER_PIN, OUTPUT);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);


  // setup lcd
  Wire.begin();
  lcd.init();



  xTaskCreatePinnedToCore(
      &LCD_Task,
      "Lcd Task",
      4096,
      NULL, 
      1,
      &xHandle,
      0
    );

    xTaskCreatePinnedToCore(
      &Alarm_task,
      "Traffic light",
      4096,
      NULL, 
      1,
      &xHandle,
      1
    );
    xTaskCreatePinnedToCore(
      &getDistanceTask,
      "distance task",
      4096,
      NULL, 
      1,
      &xSensorHandle,
      0
    );

  xTaskCreatePinnedToCore(
    process_motor, 
    "motor Task", 
    4096, 
    NULL, 
    1, 
    &xMotorHandle,
    0);
  
  xTaskCreatePinnedToCore(
    Task_ButtonHandle, 
    "btn Task", 
    2048, 
    NULL,
    1, 
    NULL,
    1);
}

void loop() {
}

void process_motor(void* arg) {

  // set the target position
  myStepper.moveTo(ANGLE_TO_STEPS(MAX_ANGLE));
 
 // Change direction once the motor reaches target position
 while(1) {
    if (!myStepper.distanceToGo())   // this form also works - pick your favorite!
    {
      myStepper.moveTo(-myStepper.currentPosition());
    }
    myStepper.run();
    vTaskDelay(pdMS_TO_TICKS(10));
 }
}


void Alarm_task(void *arg) {
  while(1) {

    static bool led_state = false;
    if(warning) {
      led_state = !led_state;

      if(led_state) {
        digitalWrite(LEDR_PIN, HIGH);
        digitalWrite(LASER_PIN, HIGH);
      } else {
        digitalWrite(LEDR_PIN, LOW);
        digitalWrite(LASER_PIN, LOW);
      }
    }

    else {
      digitalWrite(LEDR_PIN, LOW);

      digitalWrite(LASER_PIN, LOW);
    }
    vTaskDelay(pdMS_TO_TICKS(75));
  }
}


void LCD_Task(void *arg) {
  static float last = 0;
  while (1) {
    int curr;
    
    if (warning) {
      clear_lcd(0x27);
      Wire.beginTransmission(0x27);
      send_cmd(LCD_RESET_CURSOR);
      write_string("ALERT ALLERT");
      Wire.endTransmission(0x27);
      
    } else {
      clear_lcd(0x27);
      Wire.beginTransmission(0x27);
      send_cmd(LCD_RESET_CURSOR);
      write_string("Scanning");
      Wire.endTransmission(0x27);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}



void getDistanceTask(void* arg){
   // Clears the trigPin
  long distance;
  long duration;
  while(1) {
    digitalWrite(TRIGGER_PIN, LOW);

    //vTaskDelay(pdUS_TO_TICKS(50));
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(ECHO_PIN, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    //delay(100);
    warning = (distance <= MAX_DISTANCE);
    vTaskDelay(pdMS_TO_TICKS(50));
  }
  
}


void Task_ButtonHandle(void *pvParameters) {
  static bool is_on = false;
  while (1) {
    if(digitalRead(BUTTON_PIN) == LOW) {
      if(!is_on) {
        vTaskSuspend( xMotorHandle );
        vTaskSuspend( xSensorHandle );
      } else {
        vTaskResume( xSensorHandle );
        vTaskResume( xMotorHandle );
      }
      is_on = !is_on;
      
      vTaskDelay(500);
    }
    vTaskDelay(100);
  }
}


