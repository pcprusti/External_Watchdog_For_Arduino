unsigned char pulsePin = 2;
unsigned char led = 4;
unsigned char button = 5;
unsigned long lastWatchDogPing = 0;
unsigned long lastUptimeReport = 0;
unsigned int watchdog_reset_time = 1000; /* time in milisecond*/
unsigned char led_state = LOW;

void setup() 
{
    Serial.begin(9600);
    Serial.println("Arduino Reset!");
    pinMode(led, OUTPUT);
    pinMode(pulsePin, OUTPUT);
    pinMode(button, INPUT);
    // Send an initial watchdog_ping.
    watchdog_ping("Initial watchdog ping!");
}

void loop() 
{
    unsigned long uptime = millis();
    // Every 10 seconds issue a watchdog ping.
    if ((uptime - lastUptimeReport) >= watchdog_reset_time) 
    {
        lastUptimeReport = (uptime - (uptime % watchdog_reset_time));
        watchdog_ping("Still alive!");
    }
  
    led_state ^= HIGH;
    digitalWrite(led, led_state); /* Blink LED at 500 mili second */
    
    /* If button pressed enten into infinite loop to break watchdog ping*/
    if(HIGH == digitalRead(button)) 
    {
      while(1)
      {
        //infinite loop
      }
    }
    delay(500);
}

/**
 * @brief function to handle watchdog ping
 */
void watchdog_ping(String message) 
{
    // Sink current to drain charge from C2
    digitalWrite(pulsePin, HIGH);
    // Give enough time for C2 to discharge (should discharge in 50 ms)
    delay(100);
    digitalWrite(pulsePin, LOW);
    lastWatchDogPing = millis();
    Serial.println(message);
}