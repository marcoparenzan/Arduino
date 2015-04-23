int power;
int resistor;
int out_digital_resistor = 2;
int out_digital_power = 3;
int in_digital_off = 4;
int in_digital_on = 5;
int in_digital_temp_lower = 6;
int in_digital_temp_higher = 7;
int in_analog_temp = 2;
int ref_temp = 70;
int temp_delta = 15;
float temp;

void setup()
{
  Serial.begin(9600); 

  pinMode(in_digital_off, INPUT);
  pinMode(in_digital_on, INPUT);
  pinMode(in_digital_temp_lower, INPUT);
  pinMode(in_digital_temp_higher, INPUT);

  pinMode(out_digital_resistor, OUTPUT);
  pinMode(out_digital_power, OUTPUT);
  power = LOW;
  resistor = LOW;
  digitalWrite(out_digital_resistor, resistor);
  digitalWrite(out_digital_power, power);
}

void loop()
{
  temp = getTemp();
  handleTemp(temp);
  
  if (Serial.available())
  {
    String command = Serial.readStringUntil('\n');
    if (command.startsWith("GETSTATE"))
    {
      Serial.println("STATE=" + getState());
    }
    else if (command.startsWith("POWERON"))
    {
      Serial.println("COMMANDRESULT=" + powerOn());
    }
    else if (command.startsWith("POWEROFF"))
    {
      Serial.println("COMMANDRESULT=" + powerOff());
    }
    else if (command.startsWith("LOWERTEMP"))
    {
      Serial.println("COMMANDRESULT=" + lowerTemp());
    }
    else if (command.startsWith("HIGHERTEMP"))
    {
      Serial.println("COMMANDRESULT=" + higherTemp());
    }
    else
    {
      Serial.println("COMMANDRESULT=NOTRECOGNIZED");
    }
  }
  
  if (digitalRead(in_digital_off) == HIGH)
  {
    powerOff();
  }
  else if (digitalRead(in_digital_on) == HIGH)
  {
    powerOn();
  }
  else if (digitalRead(in_digital_temp_lower) == HIGH)
  {
    lowerTemp();
  }
  else if (digitalRead(in_digital_temp_higher) == HIGH)
  {
    higherTemp();
  }
  
  Serial.println("STATE=" + getState());
  delay(100);
}
  
void handleTemp(float temp)
{  
  if ((temp < ref_temp-temp_delta))
  {
    if ((resistor == LOW) && (power == 1))
    {    
      resistor = HIGH;
      digitalWrite(out_digital_resistor, resistor);
    }
  }
  else if ((temp > ref_temp+temp_delta))
  {
    if (resistor == HIGH)
    {
      resistor = LOW;
      digitalWrite(out_digital_resistor, resistor);
    }
  }
}

float getTemp()
{
  int raw = analogRead(in_analog_temp);
  float Vout = raw * (5.0 / 1023.0);
  float temp = (Vout-1.25)/0.005;
  return temp;
}

String getState()
{
  return String(temp) + ";" + String(ref_temp) + ";" + String(power) + ";" + String(resistor);
}

String lowerTemp()
{
    ref_temp--;
    return "OK";
}

String higherTemp()
{
    ref_temp++;
    return "OK";
}

String powerOn()
{
  power = HIGH;
  digitalWrite(out_digital_power, power);
    return "OK";
}

String powerOff()
{
    power = LOW;
    digitalWrite(out_digital_power, power);
    resistor = LOW;
    digitalWrite(out_digital_resistor, resistor);
}
