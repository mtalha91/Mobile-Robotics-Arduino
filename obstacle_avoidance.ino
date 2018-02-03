

#define motor_left_forward_pin 5
#define motor_left_backword_pin 4
#define motor_right_forward_pin 2
#define motor_right_backword_pin 3

// sending and receiving from ultrasonic sensor at front
#define receive_front_pin 10 // Echo Pin
#define send_front_pin 11 // Trigger Pin

// sending and receiving from ultrasonic sensor at left
#define receive_left_pin 8 // Echo Pin
#define send_left_pin 9 // Trigger Pin

// sending and receiving from ultrasonic sensor at right
#define receive_right_pin 13 // Echo Pin
#define send_right_pin 12 // Trigger Pin

int robot_stop = 0, n=5; // n represents no. of readings for average distance calculation
int min_gap_from_obstacle = 15; // that is 15 cm

// move robot straight
void moveForward(){
  digitalWrite(motor_left_pin, HIGH);
  digitalWrite(motor_left_backword_pin, LOW);
  digitalWrite(motor_right_forward_pin, HIGH);
  digitalWrite(motor_right_backword_pin, LOW);
}

// stop robot
void stopRobot(){
  digitalWrite(motor_left_forward_pin, LOW);
  digitalWrite(motor_left_backword_pin, LOW);
  digitalWrite(motor_right_forward_pin, LOW);
  digitalWrite(motor_right_backword_pin, LOW);
}

// average distance reading for left sensor
float avgDistanceLeft(float sensorReadingLeft){
  float arrLeft[n], sumLeft = 0.0, distanceLeft = 0.0;
  for(int i=0; i<n; i++)
  {
    arrLeft[i]  = pulseIn(receive_left_pin, HIGH);
    sumLeft += arrLeft[i];
  }
  distanceLeft = sumLeft/n;  
  return distanceLeft;
}

// average distance reading for right sensor
float avgDistanceRight(float sensorReadingRight){
  float arrRight[n], sumRight = 0.0, distanceRight = 0.0;
  for(int i=0; i<n; i++)
  {
    arrRight[i] = pulseIn(receive_right_pin, HIGH);
    sumRight += arrRight[i];
  }
   distanceRight = sumRight/n;  
   return distanceRight;
}


// average distance reading for front sensor
float avgDistanceFront(float sensorReadingfront){
  float  arrFront[n], sumFront = 0.0, , distanceFront = 0.0;
  for(int i=0; i<n; i++)
  {
    arrFront[i] = pulseIn(receive_front_pin, HIGH);
    sumFront += arrFront[i];
  }
  distanceFront = sumFront/n;  
  return distanceFront;
}


void setup() {
  Serial.begin (9600);
  pinMode(motor_left_forward_pin, OUTPUT);
  pinMode(motor_left_backword_pin, OUTPUT);
  
  pinMode(motor_right_forward_pin, OUTPUT);
  pinMode(motor_right_backword_pin, OUTPUT);
  
  pinMode(receive_front_pin, INPUT);
  pinMode(receive_left_pin, INPUT);
  pinMode(receive_right_pin, INPUT);
  
  pinMode(send_front_pin, OUTPUT);
  pinMode(send_left_pin, OUTPUT);
  pinMode(send_right_pin, OUTPUT);
}

void loop(){
//main function

  if(robot_stop == 0)
  {
    moveForward();
  }
  delay(500);

  float frontDist = avgDistanceFront(),
        leftDist  = avgDistanceLeft(),
        rightDist = avgDistanceRight();

  // check if obstacle face
  if(frontDist <= min_gap_from_obstacle)
  {
    if(leftDist > rightDist){
      // take turn towards left and move until obstacle avoid(path clears)    
    }
    else{
      // take turn towards right and move until obstacle avoid(path clears) 
    }
  }
  
  else{
      // keep moving in straight line unless no obstacle face
  }

 //robot_stop =1;
}


