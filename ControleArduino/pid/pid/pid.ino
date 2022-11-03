#include <Servo.h>
#define  Umax 66 //ângulo máximo do servo motor em radianos.
#define  Umin -66// ângulo mínimo
#define  Umax_rad 1.151 // ângulo máximo do servo motor em radianos.
#define  Umin_rad -1.151 // ângulo mínimo
#define  T 0.09 // tempo de amostra

const int echoPin2 = 1; 
const int trigPin2 = 2;
const int echoPin1 = 3;
const int trigPin1 = 4;




Servo servo;

double setpoint, setpoint_prec;
double y, y_prec;
double error;
double P, I, D, U;
double I_prec = 0, U_prec = 0, D_prec = 0;
boolean Saturacao = false; 


double Kp = 8.6;
double Ki = 1.1;
double Kd = 6.3;



float medicao_1(void);
float medicao_2(void);
void move_servo(int);



void setup() {
  Serial.begin(9600);




  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin2,INPUT);
  pinMode(echoPin1,OUTPUT);
  pinMode(trigPin1,INPUT);
  servo.attach(9);

  delay(1000);
  move_servo(90);
  delay(2000);
  setpoint_prec = medicao_2();
  delay(1000);
  y_prec = medicao_1();
  delay(1000);

}

void loop() {
  setpoint = medicao_2();
  setpoint = 0.53*setpoint + 0.47*setpoint_prec;

  delay(3);

  y = medicao_1();
  y = 0.53*y + 0.47*y_prec;
  
  delay(3);

  error = round( 100 * ( y - setpoint))*0.01;

  P = Kp * error;

  if(!Saturacao) I = I_prec + T*Ki*error;
  
  D = (Kd/T) * (y - y_prec);
  
  D = 0.56 * D + 0.44 * D_prec;

  U = P + I + round( 100 * D) * 0.01;

  if(U < Umin_rad){
                  U = Umin_rad;
                  Saturacao = true;
                  }

  else if (U > Umax_rad){
                        U = Umax_rad;
                        Saturacao = true;
                        }
  
  else Saturacao = false;

  U = round(U * 180 / M_PI);
  U = map(U, Umin, Umax, 24, 156);

  if( U < 83 || U > 95 || abs(error) > 0.02) move_servo(round(U));

  delay(24);
  

  I_prec = I;
  y_prec = y;
  D_prec = D;
  setpoint_prec = setpoint;
  

}
