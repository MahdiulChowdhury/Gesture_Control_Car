//collects data from an analog sensor

int sensorpin = 0;                 // analog pin used to connect the sharp sensor
int val = 0;                 // variable to store the values from sensor(initially zero)

void setup()
{
 Serial.begin(9600);               // starts the serial monitor
 pinMode(2,OUTPUT); 
}

void loop()
{
 val = analogRead(sensorpin);       // reads the value of the sharp sensor
 Serial.println(val);            // prints the value of the sensor to the serial monitor
 if (val > 150) 
 {
  digitalWrite(2,HIGH); 
 }
 else 
    digitalWrite(2,LOW);
 delay(500);                    // wait for this much time before printing next value
}
