// Programação de um robô de limpeza
// Feito na aula de Programação para Engenharia Elétrica

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

void setup() {
  // LOW and HIGH -> forward
  // HIGH and LOW -> backward
  pinMode(R_MOTOR_PIN_A, OUTPUT);
  pinMode(R_MOTOR_PIN_B, OUTPUT);

  // LOW and HIGH -> forward
  // HIGH and LOW -> backward
  pinMode(L_MOTOR_PIN_A, OUTPUT);
  pinMode(L_MOTOR_PIN_B, OUTPUT);

  // BLUE and RED LED
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // sensor digital input
  // pinMode(INTERRUPT_PIN, INPUT);

  // interruption
  // pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), interrupt_function, LOW);
  // noInterrupts();
  // interrupts();

  Serial.begin(9600);
  // Serial.println();
}

void loop() {

  // Robot ON
  digitalWrite(RED_LED, HIGH);

}

int left_is_clear(void){
  int state;
  r_motor(FORWARD);
  l_motor(STOP);
  delay(250); 
  state = digitalRead(INTERRUPT_PIN);
  r_motor(BACKWARD);
  l_motor(STOP);
  delay(250);  
  return state;
}

int front_is_clear(void){
  return digitalRead(INTERRUPT_PIN);
}

void turn_left(void){
  r_motor(FORWARD);
  l_motor(BACKWARD);
  delay(1080);
  r_motor(STOP);
  l_motor(STOP); 
}

void move(void) {
  r_motor(FORWARD);
  l_motor(FORWARD);
  delay(25);
  r_motor(STOP);
  l_motor(STOP);  
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
  ;
}
