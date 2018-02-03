/*
#define l_f 11
#define l_b 12 
#define r_f 9 
#define r_b 10
 Reference link
http://blog.miguelgrinberg.com/post/building-an-arduino-robot-part-iv-a-not-so-basic-robot-firmware
*/

#define motor_left_pin_f 11
#define motor_left_pin_b 12 
#define motor_right_pin_f 9 
#define motor_right_pin_b 10



#define trigPin_left 2 //trig pin left
#define echoPin_left 3 //echopin right 

#define trigPin_front 4  //trig pin front
#define echoPin_front 5  //trig pin front

#define trigPin_right 6 //trig pin right
#define echoPin_right 7 //trig pin right


int max_Range =22; // max range 
int min_Range = 0; // min range 
long duration_front, distanc_front; 
/*long duration_left, distanc_left; 
long duration_right, distanc_right;*/ 
int robot_stop = 0;
int n=5; // n represents no. of readings for average distance calculation
int min_gap_from_obstacle = 20;

//indicates different moving states of the robot
int going_left=0;
int going_right=0;
int going_straight=1; // robot is moving straight at the start

//distanc measure by robot in cm the distance measure by robot while running and stop the robot if complete circle.
float distance_cover_by_robot=0.0;

void movefarward(){
  analogWrite(motor_left_pin_f, 255);
  analogWrite(motor_right_pin_f, 255);
  
  analogWrite(motor_left_pin_b, 0);
  analogWrite(motor_right_pin_b, 0);
  distance_cover_by_robot++;
}

void stoprobot(){
  analogWrite(motor_left_pin_f, 0);
  analogWrite(motor_right_pin_f, 0);
  
  analogWrite(motor_left_pin_b, 0);
  analogWrite(motor_right_pin_b, 0);
  
  
}

// robot turning function
void leftturn(){
  analogWrite(motor_left_pin_f, 0);
  analogWrite(motor_right_pin_f, 150);
  
  analogWrite(motor_left_pin_b, 150);
  analogWrite(motor_right_pin_b, 0);
  // robot is turned
  delay(1000);
}

void rightturn(){
  analogWrite(motor_left_pin_f, 150);
  analogWrite(motor_right_pin_f, 0);
  
  analogWrite(motor_left_pin_b, 0);
  analogWrite(motor_right_pin_b, 150);
  // robot is turned
  delay(1000);
  
}


// average distance reading for left sensor
float avgDistanceLeft(float sensorReadingLeft){
  float arrLeft[n], sumLeft = 0.0, distanceLeft = 0.0;
  float distance_in_cm = 0.0;
  for(int i=0; i<n; i++)
  {
    arrLeft[i]  = pulseIn(receive_left_pin, HIGH);
    //Calculate the distance (in cm) based on the speed of sound.
    distance_in_cm = arrLeft[i]/58.2; 
    sumLeft += distance_in_cm;
  }
  distanceLeft = sumLeft/n; 
 
  return distanceLeft;
}

// average distance reading for right sensor
float avgDistanceRight(float sensorReadingRight){
  float arrRight[n], sumRight = 0.0, distanceRight = 0.0;
  float distance_in_cm = 0.0;
  for(int i=0; i<n; i++){
    arrRight[i] = pulseIn(receive_right_pin, HIGH);
     //Calculate the distance (in cm) based on the speed of sound
    distance_in_cm = arrLeft[i]/58.2; 
    sumRight += distance_in_cm;
  }
   distanceRight = sumRight/n;  
   return distanceRight;
}


// average distance reading for front sensor
float avgDistanceFront(float sensorReadingfront){
  float  arrFront[n], sumFront = 0.0, , distanceFront = 0.0;
  float distance_in_cm = 0.0;
  for(int i=0; i<n; i++)
  {
    arrFront[i] = pulseIn(receive_front_pin, HIGH);
    distance_in_cm = arrLeft[i]/58.2; 
    sumFront += distance_in_cm[i];
  }
  distanceFront = sumFront/n;  
  return distanceFront;
}


void setup() {

 Serial.begin (9400);

 pinMode(echoPin_front, INPUT);
 pinMode(echoPin_right, INPUT);
 pinMode(echoPin_left, INPUT);  
 pinMode(trigPin_front, OUTPUT);
 pinMode(trigPin_right, OUTPUT);
 pinMode(trigPin_left, OUTPUT); 

 //motor movement
 pinMode(motor_left_pin_f, OUTPUT);
 pinMode(motor_left_pin_b, OUTPUT);
 pinMode(motor_right_pin_f, OUTPUT);
 pinMode(motor_right_pin_b, OUTPUT);

//motor current pins and set there voltage to high
 pinMode(8, OUTPUT);
 pinMode(13, OUTPUT);
 digitalWrite(8, HIGH);
 digitalWrite(13, HIGH);
 
 if(robot_stop==0){
    movefarward();
  }
}

void loop() {

//different state of the robot for bug algorithm 
/*going_left=0;
going_right=0;
going_straight=0;
*/
if(going_left==1){
  rightDist = avgDistanceRight();
    if(rightDist < max_Range){
      movefarward();
      //changing the robot state
      going_left=1;
      going_right=0;
      going_straight=0;
      goto endofloop;
    }
    else{
      rightturn();
      movefarward();
      going_left=0;
      going_right=0;
      going_straight=1;
      goto endofloop;
    }

}
if(going_right==1){
   rightDist = avgDistanceRight();
    if(rightDist < max_Range){
      movefarward();
      //changing the robot state
      going_left=1;
      going_right=0;
      going_straight=0;
      goto endofloop;
    }
    else{
      leftturn();
      movefarward();
      going_left=0;
      going_right=0;
      going_straight=1;
      goto endofloop;
    }
}
  float frontDist = avgDistanceFront(),
        leftDist  = avgDistanceLeft(),
        rightDist = avgDistanceRight();
        
 if ((frontDist >= max_Range || frontDist <= min_Range) && going_straight==1){
  Serial.println("No obstacles ahead--- keep moving.");
      //changing the robot state
      going_left=0;
      going_right=0;
      going_straight=1;
       movefarward();
       //increment the distance while robot is moving straight
      distance_cover_by_robot++;
 }
 else {
   Serial.println("obstacle ahead-- turn the robot");
  Serial.println(distanc_front);
  //dicision to turn the robot left or right
  if(leftDist > rightDist){
    leftturn();
    //if robot is turned left then obstacle is in the right side of the robot
    //now check get the reading agains from right sensors and move until obstacle is not clear
    rightDist = avgDistanceRight();
    if(rightDist < max_Range){
      movefarward();
      //changing the robot state
      going_left=1;
      going_right=0;
      going_straight=0;
    }
    
  }
  else if(leftDist > rightDist){
    rightturn();
    //if robot is turned right  then obstacle is in the left side of the robot
    //now check get the reading agains from right sensors and move until obstacle is not clear
    leftDist = avgDistanceLeft();
    if(leftDist < max_Range){
      movefarward();
        //changing the robot state
      going_left=0;
      going_right=1;
      going_straight=0;
    }
  }
  else{
    leftturn();
    //if robot is turned left then obstacle is in the right side of the robot
    //now check get the reading agains from right sensors and move until obstacle is not clear
    rightDist = avgDistanceRight();
    if( < max_Range){
      movefarward();
      //changing the robot state
      going_left=1;
      going_right=0;
      going_straight=0;
    }
  }
 }

endofloop:

// if circle distance covered then stop the robot
if(distance_cover_by_robot >= 2000){
  stoprobot();
}
} 


