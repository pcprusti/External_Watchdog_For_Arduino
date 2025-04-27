/**
 * @brief 		Application to Test 555 Timer as External Watchdog
 * @author 		P C Prusti
 * @copyright Copyright(c) 2025
 */

#define PULSE_IN  2   // External Watchdog Pulse Input
#define LED       4   // LED Pin
#define BUTTON    5   // Button Pin

unsigned long lastWatchDogPing = 0;
unsigned long lastUptimeReport = 0;

unsigned int watchdog_reset_time = 1000; /* time in milisecond*/
bool led_state = LOW;

/*Function Declaration*/
void watchdog_ping(String message) ;
void ResetCause(void);

/**
 * @brief function to handle Initialization
 */
void setup() 
{
    Serial.begin(9600);
    ResetCause();
    pinMode(LED, OUTPUT);
    pinMode(PULSE_IN, OUTPUT);
    pinMode(BUTTON, INPUT);
    // Send an initial watchdog_ping.
    watchdog_ping("Watchdog Ping Success");
    delay(1000);
    
}


/**
 * @brief Application runs in super loop.
 */
void loop() 
{
    unsigned long uptime = millis();
    // Every 10 seconds issue a watchdog ping.
    if ((uptime - lastUptimeReport) >= watchdog_reset_time) 
    {
        lastUptimeReport = (uptime - (uptime % watchdog_reset_time));
        watchdog_ping("Watchdog Ping Success");
    }
  
    led_state ^= HIGH;
    digitalWrite(LED, led_state); /* Blink LED at 500 mili second */
    
    /* If button pressed enten into infinite loop to break watchdog ping*/
    if(HIGH == digitalRead(BUTTON)) 
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
    digitalWrite(PULSE_IN, HIGH);
    // Give enough time for C2 to discharge (should discharge in 50 ms)
    delay(100);
    digitalWrite(PULSE_IN, LOW);
    lastWatchDogPing = millis();
    Serial.println(message);
}


/**
 * @brief function to print cause of Reset.
 */
void ResetCause()
{
  if (MCUSR & _BV(EXTRF)){
     // Reset button is low
     Serial.println("External Watchdog Reset");
 }
 if (MCUSR & (_BV(BORF) | _BV(PORF))){
      // Brownout or Power On
      Serial.println("Power loss occured!");
 }
 if (MCUSR & _BV(WDRF)){
      //Internal Watchdog Reset
      Serial.println("Internal Watchdog Reset");
 }
 // Clear all MCUSR registers immediately for 'next use'
 MCUSR = 0;
}