int power;
int resistor;
int out_digital_resistor = 5; // 2
int out_digital_power = 6; // 3
int out_digital_power_relays = 7;
int in_analog_temp = 0; // 2
int ref_temp = 40;
int temp_delta = 5;
float temp;

void setup()
{
  // give relays +5V power
  pinMode(out_digital_power_relays, OUTPUT);
  digitalWrite(out_digital_power_relays, HIGH);

  Serial.begin(9600); 

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
  float vin = 3.3;
  int raw = analogRead(in_analog_temp);
  float Vout = raw * (vin / 675.0);
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
