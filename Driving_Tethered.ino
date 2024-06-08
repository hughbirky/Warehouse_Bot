/*  Final Project: Ben, Jehan, and Hugh
 *   with Arduino and Display Shield
 *  2019
 *  This project is the WarehouseBot
*/
// ---------------------------------------------------------------------------------

#include <Wire.h>                         // The Display Shield needs Wire for its I2C communication.  It comes with Arduino
#include <Adafruit_MotorShield.h>         // Motor Shield functions.  From Adafruit
#include <Servo.h>                        // Servo library.   Standard with Arduino
#include <Pixy2.h>                        // Pixy Library
#include <math.h>                         // High math functionality Library


Adafruit_MotorShield  AFMS;                //creates Motor Shield object
Adafruit_DCMotor *leftWheel = AFMS.getMotor(1);   // Left Wheel DC Motor is on port 1 of Shield
Adafruit_DCMotor *rightWheel = AFMS.getMotor(2);   // Right Wheel DC Motor is on port 2 of Shield
Pixy2 pixy;                                // creates global instance of pixy
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int somenumber;        // adjusted by setValue                 
float level;           // used by readAnalog
int wheelSpeed = 100;  
int wheelDiff = 0;
int range;          // distance (in cm) to object sensed by rangefinder
int distance;
int distance1;
int distance2;
int distance3;
int farthest;
int error;
int speeds;
int closest;
int x_bot;
int y_bot;
int angle1;
float angle_diff;
float angle3;
int x_ball;
int y_ball;
int diff_x;
int diff_y;
int x1;
int x2;
int y1;
int y2;
int ang1;
int sum1;
int sum2;
int aveangle;
int ave_x1;
int ave_x2;
int ave_y1;
int ave_y2;
int ave_angle;
int diff_ang;
int angle2;
int distance_diff;
int finalspeed;
int squared;
int x;
int n;
int y = 20;
int sigbase;
int sigball;
int tester;
int z;
int directions;
int buttonPin = 2;
int buttonState;

// ----------------------------------------------------------------------------------
void setup() {
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object
  AFMS.begin();  // Initialize the motor shield
  Serial.begin(115200);    // Begins the serial port display
  Serial.print("Starting...\n");
  pixy.init();   // Initializes the pixy
}

// -----------------------------------------------------------------------------------
void loop() {
  myservo.write(10); // Initially opens the claw
  delay(100);



//-------------Bot to turn----------------- 



  y = 20; // Variable declared for while loop
  while(y == 20) {  // Does this for the bot going to the ball
    x = 10; // Variable declared for while loop
    while(x == 10){
      int i; 
    
      pixy.ccc.getBlocks();  // Tells the arduino to gather info on the object tracked by the pixy
    
    
    //    --------Serial Printing--------
      if (pixy.ccc.numBlocks){  // Prints the info onto the serial monitor

        for (i=0; i<pixy.ccc.numBlocks; i++){  // Does this for the number of blocks shown
          n = i; 
          pixy.ccc.blocks[i].print(); // Prints all objects serially
        }
        if(n >= 2){  // Checks if the camera is tracking the right amount of objects
          if(pixy.ccc.blocks[1].m_signature == 10){  // The pixy cam for some reason would randomly switch two of the objects and what they are designated as. The next four if statements take caere of that
            if(pixy.ccc.blocks[2].m_signature == 4){
              x_bot = pixy.ccc.blocks[1].m_x;  // Getting x and y coordinates for bot
              y_bot = pixy.ccc.blocks[1].m_y;
              angle1 = pixy.ccc.blocks[1].m_angle;  // Getting initial angle of the bot
              x_ball = pixy.ccc.blocks[2].m_x;  // Geting x and y coordinates for ball
              y_ball = pixy.ccc.blocks[2].m_y;
              x = 15;
            }
          }
          if(pixy.ccc.blocks[1].m_signature == 4){
            if(pixy.ccc.blocks[2].m_signature == 10){
              x_bot = pixy.ccc.blocks[2].m_x;  // Getting x and y coordinates for bot
              y_bot = pixy.ccc.blocks[2].m_y;
              angle1 = pixy.ccc.blocks[2].m_angle;  // Getting initial angle of the bot
              x_ball = pixy.ccc.blocks[1].m_x;  // Geting x and y coordinates for ball
              y_ball = pixy.ccc.blocks[1].m_y;
              x = 15;
            }
          }
        }
        else{
          goStop();  // If it isn't tracking the right amount of objects, it stops
          x = 10;
        }
      }
    }

     
    
    diff_x = x_ball-x_bot;  // Finds distance between ball and bot for x and y
    diff_y = -y_ball+y_bot;  // This is backwards because the pixy cam has y go from top to bottom for 0-208
    squared = max(abs(diff_x), abs(diff_y));
    distance_diff = squared; // Find distance between them
    distance_diff = distance_diff/1.2;
    distance_diff = constrain(distance_diff, 0, 255);  // Constrains it
    angle_diff = atan2(diff_y, diff_x);  // Angle between the bot and ball position
    angle_diff = angle_diff*57;  // Converts from radians to angles
    speeds = angle_diff - angle1;  // Makes speed proportional to difference in angles
    if(speeds < -180){speeds = speeds + 360;}  // Converts angle given to left (positive) or right (negative)
    if(speeds > 180){speeds = speeds - 360;}
    finalspeed = distance_diff + speeds;
    finalspeed = constrain(finalspeed, 0, 255);


    
    if(speeds > 0){
      leftWheel->setSpeed(speeds+20);  // Turns left
      rightWheel->setSpeed(speeds+20); 
      goLeft();  // Tells it to go forward
    }
    if(speeds < 0){
      leftWheel->setSpeed(abs(speeds)+20);  // Turns right
      rightWheel->setSpeed(abs(speeds)+20); 
      goRight();  // Tells it to go forward
    }
    if(abs(speeds)<5){  // When the angle is within a certain range, it goes onto the next function
      y = 30; 
      break;
    }
  }





//------Sends bot to the ball----------



// All code same as before with slight tweaks




  while(1) {  // Does this for the bot going to the ball
    x = 10;
    while(x == 10){
      int i; 
    
      pixy.ccc.getBlocks();  // Tells the arduino to gather info on the object tracked by the pixy
    
    
    //    --------Serial Printing--------
      if (pixy.ccc.numBlocks){  // Prints the info onto the serial monitor

        for (i=0; i<pixy.ccc.numBlocks; i++){  // Does this for the number of blocks shown
          n = i;
        }
        if(n >= 2){
          if(pixy.ccc.blocks[1].m_signature == 10){
            if(pixy.ccc.blocks[2].m_signature == 4){
              x_bot = pixy.ccc.blocks[1].m_x;  // Getting x and y coordinates for bot
              y_bot = pixy.ccc.blocks[1].m_y;
              angle1 = pixy.ccc.blocks[1].m_angle;  // Getting initial angle of the bot
              x_ball = pixy.ccc.blocks[2].m_x;  // Geting x and y coordinates for ball
              y_ball = pixy.ccc.blocks[2].m_y;
              x = 15;
            }
          }
          if(pixy.ccc.blocks[1].m_signature == 4){
            if(pixy.ccc.blocks[2].m_signature == 10){
              x_bot = pixy.ccc.blocks[2].m_x;  // Getting x and y coordinates for bot
              y_bot = pixy.ccc.blocks[2].m_y;
              angle1 = pixy.ccc.blocks[2].m_angle;  // Getting initial angle of the bot
              x_ball = pixy.ccc.blocks[1].m_x;  // Geting x and y coordinates for ball
              y_ball = pixy.ccc.blocks[1].m_y;
              x = 15;
            }
          }
        }
        else{
          goStop();
          x = 10;
        }
      }
    }

    
    angle1 = angle1 - 12;
    diff_x = x_ball-x_bot;  // Finds distance between ball and bot for x and y
    diff_y = -y_ball+y_bot;  // This is backwards because the pixy cam has y go from top to bottom for 0-208
    squared = max(abs(diff_x), abs(diff_y));
    distance_diff = squared; // Find distance between them
    distance_diff = distance_diff/1.2;
    distance_diff = constrain(distance_diff, 0, 255);
    angle_diff = atan2(diff_y, diff_x);  // Angle between the bot and ball position
    angle_diff = angle_diff*57;  // Converts from radians to angles
    speeds = angle_diff - angle1;  // Makes speed proportional to difference in angles
    if(speeds < -180){speeds = speeds + 400;}
    if(speeds > 180){speeds = speeds - 400;}
    finalspeed = distance_diff + speeds;
    finalspeed = constrain(finalspeed, 0, 255);
    Serial.print("angle_diff = ");
    Serial.println(angle_diff);
    Serial.print("angle1 = ");
    Serial.println(angle1);

    
    leftWheel->setSpeed(distance_diff);  // Turns right
    rightWheel->setSpeed(finalspeed); 
    goForward();  // Tells it to go forward


    if(abs(speeds)<5 and squared < 31){  // When it is close enough to the base and turned towards the center it stops and picks up the ball
      goStop();
      myservo.write(160);
      delay(1000);
      break;
    }
  }














//-------------Bot to turn to base----------------- 
// This is the same as the first chunk of code but now going for red base



 
  y = 20;
  while(y == 20) {  // Does this for the bot going to the ball
    x = 10;
    while(x == 10){
      int i; 
    
      pixy.ccc.getBlocks();  // Tells the arduino to gather info on the object tracked by the pixy
    
    
    //    --------Serial Printing--------
      if (pixy.ccc.numBlocks){  // Prints the info onto the serial monitor

        for (i=0; i<pixy.ccc.numBlocks; i++){  // Does this for the number of blocks shown
          n = i;
          pixy.ccc.blocks[i].print();
        }
        if(n >= 2){
          if(pixy.ccc.blocks[1].m_signature == 10){
            if(pixy.ccc.blocks[2].m_signature == 4){
              x_bot = pixy.ccc.blocks[1].m_x;  // Getting x and y coordinates for bot
              y_bot = pixy.ccc.blocks[1].m_y;
              angle1 = pixy.ccc.blocks[1].m_angle;  // Getting initial angle of the bot
              x_ball = pixy.ccc.blocks[0].m_x;  // Geting x and y coordinates for ball
              y_ball = pixy.ccc.blocks[0].m_y;
              x = 15;
            }
          }
          if(pixy.ccc.blocks[1].m_signature == 4){
            if(pixy.ccc.blocks[2].m_signature == 10){
              x_bot = pixy.ccc.blocks[2].m_x;  // Getting x and y coordinates for bot
              y_bot = pixy.ccc.blocks[2].m_y;
              angle1 = pixy.ccc.blocks[2].m_angle;  // Getting initial angle of the bot
              x_ball = pixy.ccc.blocks[0].m_x;  // Geting x and y coordinates for ball
              y_ball = pixy.ccc.blocks[0].m_y;
              x = 15;
            }
          }
        }
        else{
          goStop();
          x = 10;
        }
      }
    }
    
 
    
    diff_x = x_ball-x_bot;  // Finds distance between ball and bot for x and y
    diff_y = -y_ball+y_bot;  // This is backwards because the pixy cam has y go from top to bottom for 0-208
    squared = max(abs(diff_x), abs(diff_y));
    distance_diff = squared; // Find distance between them
    distance_diff = distance_diff/1.2;
    distance_diff = constrain(distance_diff, 0, 255);
    angle_diff = atan2(diff_y, diff_x);  // Angle between the bot and ball position
    angle_diff = angle_diff*57;  // Converts from radians to angles
    speeds = angle_diff - angle1;  // Makes speed proportional to difference in angles
    if(speeds < -180){speeds = speeds + 360;}
    if(speeds > 180){speeds = speeds - 360;}
    finalspeed = distance_diff + speeds;
    finalspeed = constrain(finalspeed, 0, 255);

    
    if(speeds > 0){
      leftWheel->setSpeed(speeds+20);  // Turns right
      rightWheel->setSpeed(speeds+20); 
      goLeft();  // Tells it to go forward
    }
    if(speeds < 0){
      leftWheel->setSpeed(abs(speeds)+20);  // Turns right
      rightWheel->setSpeed(abs(speeds)+20); 
      goRight();  // Tells it to go forward
    }
    if(abs(speeds)<5){
      y = 30;
      break;
    }
  }





//------Sends bot to the base----------
// Same as second chunck of code but for red base






  y = 20;
  while(y == 20) {  // Does this for the bot going to the ball
    x = 10;
    while(x == 10){
      int i; 
    
      pixy.ccc.getBlocks();  // Tells the arduino to gather info on the object tracked by the pixy
    
    
    //    --------Serial Printing--------
      if (pixy.ccc.numBlocks){  // Prints the info onto the serial monitor

        for (i=0; i<pixy.ccc.numBlocks; i++){  // Does this for the number of blocks shown
          n = i;
        }
        if(n >= 2){
          if(pixy.ccc.blocks[1].m_signature == 10){
            if(pixy.ccc.blocks[2].m_signature == 4){
              x_bot = pixy.ccc.blocks[1].m_x;  // Getting x and y coordinates for bot
              y_bot = pixy.ccc.blocks[1].m_y;
              angle1 = pixy.ccc.blocks[1].m_angle;  // Getting initial angle of the bot
              x_ball = pixy.ccc.blocks[0].m_x;  // Geting x and y coordinates for ball
              y_ball = pixy.ccc.blocks[0].m_y;
              x = 15;
            }
          }
          if(pixy.ccc.blocks[1].m_signature == 4){
            if(pixy.ccc.blocks[2].m_signature == 10){
              x_bot = pixy.ccc.blocks[2].m_x;  // Getting x and y coordinates for bot
              y_bot = pixy.ccc.blocks[2].m_y;
              angle1 = pixy.ccc.blocks[2].m_angle;  // Getting initial angle of the bot
              x_ball = pixy.ccc.blocks[0].m_x;  // Geting x and y coordinates for ball
              y_ball = pixy.ccc.blocks[0].m_y;
              x = 15;
            }
          }
        }
        else{
          goStop();
          x = 10;
        }
      }
    }



    angle1 = angle1 - 10;
    diff_x = x_ball-x_bot;  // Finds distance between ball and bot for x and y
    diff_y = -y_ball+y_bot;  // This is backwards because the pixy cam has y go from top to bottom for 0-208
    squared = max(abs(diff_x), abs(diff_y));
    distance_diff = squared; // Find distance between them
    distance_diff = distance_diff/1.2;
    distance_diff = constrain(distance_diff, 0, 255);
    angle_diff = atan2(diff_y, diff_x);  // Angle between the bot and ball position
    angle_diff = angle_diff*57;  // Converts from radians to angles
    speeds = angle_diff - angle1;  // Makes speed proportional to difference in angles
    if(speeds < -180){speeds = speeds + 390;}
    if(speeds > 180){speeds = speeds - 390;}
    finalspeed = distance_diff + speeds;
    finalspeed = constrain(finalspeed, 0, 255);

 
    leftWheel->setSpeed(distance_diff);  // Turns right
    rightWheel->setSpeed(finalspeed); 
    goForward();  // Tells it to go forward

    if(abs(speeds)<20 and squared < 29){ // When close enough to the base and turned the right direction, it drops the ball and backs up
      goStop();
      letgo();
      backup();
      y = 30;
      break;
    }
  }
}


















//------------------------  Non-menu Functions -------------------------------



// function to go forwards
void goForward(){
  leftWheel->run(FORWARD);
  rightWheel->run(FORWARD);
}


// function to go backwards
void goBackward(){
  leftWheel->run(BACKWARD);
  rightWheel->run(BACKWARD);
}


// function to go left
void goLeft(){
  leftWheel->run(BACKWARD);
  rightWheel->run(FORWARD);
}


// function to go right
void goRight(){
  leftWheel->run(FORWARD);
  rightWheel->run(BACKWARD);
}



// function to stop
void goStop(){
  leftWheel->run(RELEASE);
  rightWheel->run(RELEASE);
}


void letgo(){
  myservo.write(10);
  delay(1500);
}


void backup(){
  leftWheel->setSpeed(50);  // Turns right
  rightWheel->setSpeed(50); 
  goBackward();  // Tells it to go forward
  delay(1500);
}



// function to give initial wheel speeds
void goBegin() {    // Call this before any of the others
  leftWheel->setSpeed(wheelSpeed + wheelDiff);
  rightWheel->setSpeed(wheelSpeed - wheelDiff);}
