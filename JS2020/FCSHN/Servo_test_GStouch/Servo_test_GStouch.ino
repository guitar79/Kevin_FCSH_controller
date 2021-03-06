//https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#a608b2395b64ac15451d16d0371fe13ce

short subm = 0;
short menu = 1;
short power = 1;
//
short rm = 0;
//
short stepmode = 1;

bool PCMODE = false;

#define DHT22_ 1

#include <AccelStepper.h>
  #ifdef DHT22_
   //#include <dht.h>
   #include <DHT.h>
  #endif

// motor pins
#define motorInterfaceType 1
#define DIR 4
#define STEP 3
#define MS0 15
#define MS1 12
#define MS2 13
#define MOTOR_STEPS 200

// Declaration needed for the AccelStepper Library
  AccelStepper stepper(motorInterfaceType, STEP, DIR);

// for the temperature and hubmidity sensor
  #ifdef DHT22_
   #define DHT22_PIN 2
   #define DHTTYPE DHT22
   DHT dht(DHT22_PIN,DHTTYPE);
  #endif


String inputString = "";

// temperature and humidity sensor
  #ifdef DHT22
   //dht DHT;
   int chkSensor;
   String Temperature;
   String Humidity;
  #endif
  
#include <Servo.h>
Servo servo;
bool BMask = false;

void setup() {
  
  
  Serial.begin(115200);
  Serial.println("GStouch#");
  U8G_start();
  Servo_start();
  
  stepper.setMaxSpeed(2000.0);
  stepper.setAcceleration(300.0);
  stepper.setSpeed(100);

  inputString.reserve(200);

  pinset();

}


void loop() {

  #ifdef DHT22_
  if(stepper.distanceToGo() == 0 || subm!=2)
  {
    Temperature = String(dht.readTemperature(),1);
    Humidity = String(dht.readHumidity(),1);
    humidityTemperatureReport();
    buttonRead();
    draw();
  }
  #endif
  
}

void reportPosition() {
  Serial.print("POSITION:");
  Serial.print(stepper.currentPosition());
  Serial.println("#");
}


/**
* process the command we recieved from the client
* command format is <Letter><Space><Integer>
* i.e. A 500 ---- Fast Rewind with 500 steps
*/
void serialCommand(String commandString) {
  char _command = commandString.charAt(0);
  int _value = commandString.substring(2).toInt();
  String _answer = "";
  int _currentPosition = stepper.currentPosition();
  int _newPosition = _currentPosition;


  
  switch (_command) {

  case 'A':  // Mask On
  case 'a': _newPosition = _currentPosition; // non move command
  BMask = !BMask;
  
  servo.attach(6); // 5도 동시제어
  if(BMask) servo.write(_value);
    else if(!BMask) servo.write(0);
  delay(500);
  servo.detach();
  delay(500);
  
  case 'B':  // REVERSE "<"
  case 'b': _newPosition = _currentPosition - _value;
    break;
    
  case 'C':  // FORWARD ">"
  case 'c': _newPosition = _currentPosition + _value;
    break;
    
  case 'E':  // MOVE TO POSITION
  case 'e': _newPosition = _value;
    break;
    
  case 'F':  // GET CURRENT POSITION
  case 'f': _answer += _currentPosition;
    break;
  case 'G':  // SET CURRENT POSITION
  case 'g': _newPosition = _value;
    _currentPosition = _value;
    stepper.setCurrentPosition(_value);
    break;
  case 'H':  // SET ACCELERATION
  case 'h': _newPosition = _currentPosition; // non move command
    stepper.setAcceleration(_value);
    _answer += "SET-ACCELERATION:";
    _answer += _value;
    break;
  case 'I':  // SET SPEED
    _newPosition = _currentPosition; // non move command
    stepper.setSpeed(_value);
    _answer += "SET-SPEED:";
    _answer += _value;
    break;
  case 'i':  // GET SPEED
    _newPosition = _currentPosition; // non move command
    _answer += "GET-SPEED:";
    _answer += stepper.speed();
    break;

      #ifdef DHT22
        case 'k': // GET TEMPERATURE & HUMIDITY
          _newPosition = _currentPosition; // non move command
          humidityTemperatureReport();
          break;
      #endif

  case 'X':  // GET STATUS - may not be needed
  case 'x':
    stepper.stop();
    break;
  case 'Z':  // IDENTIFY
  case 'z':  _answer += "GStouch";
  PCMODE = true;
    break;
  default:
    _answer += "GStouch";
    break;
  }

  if (_newPosition != _currentPosition) {
        // a move command was issued
    Serial.print("MOVING:");
    Serial.print(_newPosition);
    Serial.println("#");
    //
    stepper.moveTo(_newPosition);
    stepper.runToPosition();
    _answer += "POSITION:";
    _answer += stepper.currentPosition();
  }


  Serial.print(_answer);
  Serial.println("#");
}

/**
* handler for the serial communicationes
* calls the SerialCommand whenever a new command is received
*/
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      serialCommand(inputString);
      inputString = "";
    }
  }
}

/**
* for DHT routine
*/
#ifdef DHT22_
  void humidityTemperatureReport() {
    chkSensor = digitalRead(DHT22_PIN);
    Temperature = String(dht.readTemperature(),1);
    Humidity = String(dht.readHumidity(),1);
    switch (chkSensor) {
    case 1:
      Serial.print("TEMPERATURE:");
      Serial.print(Temperature);
      Serial.println("#");
      delay(50);
      Serial.print("HUMIDITY:");
      Serial.print(Humidity);
      Serial.println("#");
      delay(50);
      break;
    case 0:
      Serial.print("TEMPERATURE:");
      Serial.print("CHECKSUMERROR");
      Serial.println("#");
      Serial.print("HUMIDITY:");
      Serial.print("CHECKSUMERROR");
      Serial.println("#");
      break;
    default:
      Serial.print("TEMPERATURE:");
      Serial.print("UNKNOWNERROR");
      Serial.println("#");
      Serial.print("HUMIDITY:");
      Serial.print("UNKNOWNERROR");
      Serial.println("#");
      break;
    }
  }
#endif

void Servo_start()
{
  servo.attach(6); // 5도 동시제어
  servo.write(0);
  delay(500);
  servo.detach();
}

