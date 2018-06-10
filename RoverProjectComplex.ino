#include "AFMotor.h"
#include "Ultrasonic.h"


AF_DCMotor M1(1); //motor esquerda frente
AF_DCMotor M2(2); //motor direita frente 
AF_DCMotor M3(3); //motor esquerda trás
AF_DCMotor M4(4); //motor direita trás

//Global Variables

const int VELOCIDADEMAX = 70; // Variável para a velocidade das rodas do carrinho

//Sensor Ports Constants
const int sensorLeft          = 22;
const int sensorMiddle        = 23;
const int sensorRight         = 24;
const int ultraLeftEcho       = 46;
const int ultraLeftTrigger    = 44;
const int ultraMiddleEcho     = 42;
const int ultraMiddleTrigger  = 40;
const int ultraRightEcho      = 50;
const int ultraRightTrigger   = 48;

const int VALORMAX = 512;
 
//Led Ports Constants
const int ledRight = 25;
const int ledLeft  = 26;

//Sensor States Variables
int sensorLeftState;
int sensorMiddleState;
int sensorRightState;

//Ultra Sensors Variables
double ultraLeftState;
double ultraMiddleState;
double ultraRightState;

//Ultra Read Variables
HC_SR04  ultraLeftData(ultraLeftTrigger, ultraLeftEcho); // Sensor da esquerda
HC_SR04  ultraMiddleData(ultraMiddleTrigger, ultraMiddleEcho); // Sensor do meio
HC_SR04  ultraRightData(ultraRightTrigger, ultraRightEcho); // Sensor da direita

void moveForward(int velocity){
  M1.setSpeed(velocity);
  M1.run(FORWARD);
  M2.setSpeed(velocity);
  M2.run(BACKWARD);
  M3.setSpeed(velocity);
  M3.run(FORWARD);
  M4.setSpeed(velocity);
  M4.run(BACKWARD);
}

void moveBackward(int velocity){
  M1.setSpeed(velocity);
  M1.run(BACKWARD);
  M2.setSpeed(velocity);
  M2.run(FORWARD);
  M3.setSpeed(velocity);
  M3.run(BACKWARD);
  M4.setSpeed(velocity);
  M4.run(FORWARD);
}

void extremeRight(int velocity){
  M1.setSpeed(velocity);
  M1.run(FORWARD);
  M2.setSpeed(velocity-30);
  M2.run(FORWARD); // Indo para trás
  M3.setSpeed(velocity);
  M3.run(FORWARD); // Indo para trás
  M4.setSpeed(velocity-30);
  M4.run(FORWARD);
}

void extremeLeft(int velocity){
  M1.setSpeed(velocity-30);
  M1.run(BACKWARD);
  M2.setSpeed(velocity);
  M2.run(BACKWARD);
  M3.setSpeed(velocity-30);
  M3.run(BACKWARD);
  M4.setSpeed(velocity);
  M4.run(BACKWARD);  
}

void moveToRight(int velocity){
  M1.setSpeed(velocity-20);
  M1.run(BACKWARD);
  M2.setSpeed(velocity+20);
  M2.run(FORWARD);
  M3.setSpeed(velocity-20);
  M3.run(BACKWARD);
  M4.setSpeed(velocity+20);
  M4.run(FORWARD);
}

void moveToLeft(int velocity){
  M1.setSpeed(velocity+20);
  M1.run(BACKWARD);
  M2.setSpeed(velocity-20);
  M2.run(FORWARD);
  M3.setSpeed(velocity+20);
  M3.run(BACKWARD);
  M4.setSpeed(velocity-20);
  M4.run(FORWARD);
}

void setup() {
  pinMode(sensorLeft,INPUT);
  pinMode(sensorMiddle, INPUT);
  pinMode(sensorRight, INPUT);
  pinMode(ledRight,OUTPUT);
  pinMode(ledLeft,OUTPUT);
  Serial.begin(9600);
  delay(1000);
}

void getUltraSensorsValue(){
     
    // Lê a distância do sensor q atribui para as variáveis
    ultraLeftState = ultraLeftData.distance();
    ultraMiddleState = ultraMiddleData.distance();
    ultraRightState = ultraRightData.distance();

    
    if(ultraLeftState>VALORMAX) ultraLeftState=VALORMAX;
    
    if(ultraMiddleState>VALORMAX) ultraMiddleState=VALORMAX;
    
    if(ultraRightState>VALORMAX) ultraRightState=VALORMAX;
    
    
    Serial.print("Esquerda = ");
    Serial.print(ultraLeftState);
    Serial.print(" CM");
    Serial.println(" ");
    
    Serial.print("Meio = ");
    Serial.print(ultraMiddleState);
    Serial.print(" CM");
    Serial.println(" ");     
    
    Serial.print("Direita = ");
    Serial.print(ultraRightState);
    Serial.print(" CM");
    Serial.println(" ");    
    
    Serial.println("\n================================\n\n");
}

void getInfraRedSensorsValue(){
  sensorLeftState     = digitalRead(sensorLeft);
  sensorMiddleState   = digitalRead(sensorMiddle);
  sensorRightState    = digitalRead(sensorRight); 
}

void loop() { 

  getUltraSensorsValue();
  getInfraRedSensorsValue();

    if( ultraMiddleState <= 30.0 ) {

        // Se estiver muito próximo do alvo, deverá dar uma ré, parar e verificar qual lado poderá ir
        //moveBackward(VELOCIDADEMAX);
        
        //delay(100);
         
        if(ultraLeftState < ultraRightState && ultraLeftState < 60.0 ){
            moveToRight(VELOCIDADEMAX);
        } else if( ultraRightState < ultraLeftState && ultraRightState < 60.0 ){
            moveToLeft(VELOCIDADEMAX);
        }

        delay(80);
              
    } else if(ultraLeftState <= 30.0) {
      
          // Verifica o lado esquerdo para poder desviar para a direita
          extremeRight(VELOCIDADEMAX);
          
    } else if(ultraRightState <= 30.0) {
        
           // Verifica o lado direito para poder desviar para a esquerdo
           extremeLeft(VELOCIDADEMAX);
           
    } else {
        // Se não precisar desviar, irá seguir em frente
        moveForward(VELOCIDADEMAX);
    }

    // Delay para diminuir a auto frequência no carrinho
    delay(150);
  
}
