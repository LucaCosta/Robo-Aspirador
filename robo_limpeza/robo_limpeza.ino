// Programação de um robô de limpeza
// Feito na aula de Programação para Engenharia Elétrica

#include<Wire.h>

#define INTERRUPT_PIN 2
#define BLUE_LED 3
#define RED_LED 4
#define R_MOTOR_PIN_A 6 
#define R_MOTOR_PIN_B 5
#define L_MOTOR_PIN_A 10 
#define L_MOTOR_PIN_B 11
#define FORWARD 0
#define BACKWARD 1
#define STOP -1

int interrupt_state = HIGH;

void setup() {
  // MPU 6050
  mpu6050_init();
  // MOTORS
  // LOW and HIGH -> forward
  // HIGH and LOW -> backward
  pinMode(R_MOTOR_PIN_A, OUTPUT);
  pinMode(R_MOTOR_PIN_B, OUTPUT);
  // LOW and HIGH -> forward
  // HIGH and LOW -> backward
  pinMode(L_MOTOR_PIN_A, OUTPUT);
  pinMode(L_MOTOR_PIN_B, OUTPUT);

  // BLUE and RED LEDs
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // SENSORS INTERRUPTION - LOW HIGH CHANGE RISING FALLING 
  pinMode(INTERRUPT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), interrupt_function, FALLING);

  // ROBOT ON STATE
  digitalWrite(RED_LED, HIGH);

  // SERIAL DEBUG  
  Serial.begin(115200);
}

void loop() {

  // SAFE State
  r_motor_velocity(0);
  l_motor_velocity(0);
  delay(5000);
}

int front_is_clear(void){
  int front_state = HIGH;
  if(!interrupt_state){
    front_state = LOW;
    interrupt_state = HIGH;
  }
  return front_state;
}

void turn_left(void){
  for(int counter = 0; counter <= 127; counter++){
    r_motor_velocity(counter);
    l_motor_velocity(-counter);
    delay(4);
  }
  //while(int(mpu6050_yaw() < 90));
  for(int counter = 127; counter >= 0; counter--){
    r_motor_velocity(counter);
    l_motor_velocity(-counter);
    delay(4);
  }
}

void move(int distance) {
  for(int counter = 0; counter <= 127; counter++){
    r_motor_velocity(counter);
    l_motor_velocity(counter);
    delay(4);
  }
  delay(50*(distance-5));
  for(int counter = 127; counter >= 0; counter--){
    r_motor_velocity(counter);
    l_motor_velocity(counter);
    delay(4);
  }
}

void r_motor(int direction){
  if(direction != STOP){  
    digitalWrite(R_MOTOR_PIN_A, direction);
    digitalWrite(R_MOTOR_PIN_B, !direction);
  }
  else {
    digitalWrite(R_MOTOR_PIN_A, LOW);
    digitalWrite(R_MOTOR_PIN_B, LOW); 
  }
}

void l_motor(int direction){
  if(direction != STOP){  
    digitalWrite(L_MOTOR_PIN_A, direction);
    digitalWrite(L_MOTOR_PIN_B, !direction);
  }
  else {
    digitalWrite(L_MOTOR_PIN_A, LOW);
    digitalWrite(L_MOTOR_PIN_B, LOW); 
  }
}

void r_motor_velocity(int velocity){
  // LOW and HIGH -> foward
  // HIGH and LOW -> reverse
  // R_MOTOR_PIN_A > R_MOTOR_PIN_B -> foward
  // R_MOTOR_PIN_B > R_MOTOR_PIN_A -> reverse
  if(velocity){
    analogWrite(R_MOTOR_PIN_A, 127-velocity); 
    analogWrite(R_MOTOR_PIN_B, 127+velocity);
  }
  else {
    digitalWrite(R_MOTOR_PIN_A, LOW);
    digitalWrite(R_MOTOR_PIN_B, LOW);
  }
}

void l_motor_velocity(int velocity){
  // LOW and HIGH -> foward
  // HIGH and LOW -> reverse
  // R_MOTOR_PIN_A > R_MOTOR_PIN_B -> foward
  // R_MOTOR_PIN_B > R_MOTOR_PIN_A -> reverse
  if(velocity){
    analogWrite(L_MOTOR_PIN_A, 127-velocity); 
    analogWrite(L_MOTOR_PIN_B, 127+velocity);
  }
  else {
    digitalWrite(L_MOTOR_PIN_A, LOW);
    digitalWrite(L_MOTOR_PIN_B, LOW);
  }
}

void interrupt_function() {
  interrupt_state = !interrupt_state;
}

void mpu6050_init(void){
  Wire.begin(); 
  Wire.beginTransmission(0x68); 
  Wire.write(0x6B); 
  Wire.write(0x00); 
  Wire.endTransmission();
}

float mpu6050_offset(void){
  float gyro_Z_offset=0;
  // Sum 1024 sensor reads
  for(int counter = 0; counter < 4096; counter++) {
    Wire.beginTransmission(0x68);
    Wire.write(0x47);
    Wire.endTransmission();
    Wire.requestFrom(0x68, 0x02);
    if (Wire.available() >= 0x02)
      gyro_Z_offset += (Wire.read() << 8) | Wire.read();
  }
  // Return average offset
  return gyro_Z_offset/4096.0;
}

float mpu6050_yaw(void){
  static int fuse = 1;
  static float yaw, gyro_Z, gyro_Z_offset;
  static float current_time, previous_time, elapsed_time;
  // Calculate offset
  if(fuse){
    gyro_Z_offset =  mpu6050_offset();
    fuse = 0;
  }
  // Average value filtering
  gyro_Z = 0;
  for(int counter = 0; counter < 16; counter++){
    // For a 250deg/s range divide the raw value by 131.0, according to the datasheet
    Wire.beginTransmission(0x68);
    Wire.write(0x47);
    Wire.endTransmission();
    Wire.requestFrom(0x68, 0x02);
    if (Wire.available() >= 0x02)
      gyro_Z += (((Wire.read() << 8) | Wire.read()) - gyro_Z_offset ) / 131.0;
  }
  gyro_Z = gyro_Z/16.0;

  /*Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 0x06);
  accel_X=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  accel_Y=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  accel_Z=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 0x02);
  temp=Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 0x06);
  // For a 250deg/s range divide the raw value by 131.0, according to the datasheet
  gyro_X = (Wire.read() << 8 | Wire.read()) / 131.0; // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  gyro_Y = (Wire.read() << 8 | Wire.read()) / 131.0; // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  gyro_Z = (Wire.read() << 8 | Wire.read()) / 131.0; // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)*/

  // Calculate yaw angle from angular velocity
  current_time = millis();
  elapsed_time = (current_time - previous_time) / 1000.0;
  previous_time = current_time;
  yaw += gyro_Z * elapsed_time;

  // Return YAW
  return yaw;
}
