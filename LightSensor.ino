const int analogInPin = A7;
const int ledPin = 3;
const int dayTreshold = 1000;
const int transitionDelay = 5 * 1000;

int sensorValue = 0;

bool nightMode = false;
bool initMode = false;

unsigned long transitionStart = 0; 

void DebugPrint(String message, String value1 = "", String value2 = "", String value3 = "");

void setup() 
{
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  initMode = true;
}

void loop() 
{
  sensorValue = analogRead(analogInPin);

  if (initMode)
  {
    DebugPrint("In init mode");
    SetLight(sensorValue < dayTreshold);
    initMode = false;
    return;
  }
  
  int nightDetected = sensorValue < dayTreshold;

  if (nightMode == nightDetected) 
  {
    transitionStart = 0;
    DebugPrint("transitionStart reset");
  }
  else
  {
    if (transitionStart <= 0)
    {
      transitionStart = millis();
      DebugPrint("transitionStart set to: ", String(transitionStart));
    }
    else
    {
      DebugPrint("expired: ", String(millis() - transitionStart));
      if ((millis() - transitionStart) > transitionDelay)
      {
        SetLight(!nightMode);
      }
    }
  }

  delay(500);
}


void DebugPrint(String message, String value1 = "", String value2 = "", String value3 = "")
{
    Serial.print(message);
    Serial.print(value1);
    Serial.print(value2);
    Serial.println(value3);
}

void SetLight(bool enable)
{
    digitalWrite(ledPin, enable);
    transitionStart = 0;
    nightMode = enable;

    DebugPrint("Light switched to: ", String(enable));
}
