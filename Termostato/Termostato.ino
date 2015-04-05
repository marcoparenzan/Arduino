int power;
int out_digital_relay = 2;
int in_digital_min_lower = 3;
int in_digital_min_higher = 4;
int in_digital_max_lower = 5;
int in_digital_max_higher = 6;
int in_analog_temp = 2;
int min_temp = 50;
int max_temp = 80;

void setup()
{
  Serial.begin(9600); 
  pinMode(out_digital_relay, OUTPUT);
  pinMode(in_digital_min_lower, INPUT);
  pinMode(in_digital_min_higher, INPUT);
  pinMode(in_digital_max_lower, INPUT);
  pinMode(in_digital_max_higher, INPUT);
  power = LOW;
  digitalWrite(out_digital_relay, power);
}

void loop()
{
  int raw = analogRead(in_analog_temp);
  float Vout = raw * (5.0 / 1023.0);
  float temp = (Vout-1.25)/0.005;
  
  if ((temp < min_temp) && (power == LOW))
  {
    power = HIGH;
    digitalWrite(out_digital_relay, power);
  }
  else if ((temp > max_temp) && (power == HIGH))
  {
    power = LOW;
    digitalWrite(out_digital_relay, power);
  }
  bool cmd_min_lower = digitalRead(in_digital_min_lower);
  bool cmd_min_higher = digitalRead(in_digital_min_higher);
  bool cmd_max_lower = digitalRead(in_digital_max_lower);
  bool cmd_max_higher = digitalRead(in_digital_max_higher);
  
  if (digitalRead(in_digital_min_lower) == HIGH)
  {
    min_temp--;
  }
  else if (digitalRead(in_digital_min_higher) == HIGH)
  {
    min_temp++;
  }
  else if (digitalRead(in_digital_max_lower) == HIGH)
  {
    max_temp--;
  }
  else if (digitalRead(in_digital_max_higher) == HIGH)
  {
    max_temp++;
  }
  
  
  Serial.print("current temp=");
  Serial.print(temp);
  Serial.print("-min temp=");
  Serial.print(min_temp);
  Serial.print("-max temp=");
  Serial.print(max_temp);
  Serial.print("-resistor=");
  Serial.print(power);
  Serial.print("-minmax=");
  Serial.print(cmd_min_lower);
  Serial.print(cmd_min_higher);
  Serial.print(cmd_max_lower);
  Serial.print(cmd_max_higher);
  Serial.println();
  delay(500); 
}
