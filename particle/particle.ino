/**************************************************************************/
/*
   Particle Photon code for the Beyond Uptake Data Fellows Sensor Class

   Uptake 2017 
*/
/**************************************************************************/


// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_TSL2561_U.h>


//TMP36 Pin Variables
const int sensorPin = A0; 	//the analog pin the TMP36's Vout (sense) pin is connected to

//TSL2561 Pins
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);


//This setup function is called everytime the device powers on
void setup() {

  
  /* Initialise the light sensor */
  if(!tsl.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    while(1);
  }
  

  /* Setup the sensor gain and integration time */
  configureSensor();
  
}


//The device will loop through this function as long as its powered
void loop() {

    // get a temp reading from analog sensor
    String temp = getTemp();
  	
    // Send temp event to cloud										
  	Particle.publish("temp",temp,PRIVATE);
  	
  	
  	
  	
  /* Get a new light sensor event */ 
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Display the results (light is measured in lux) */
  if (event.light)
  {
    Particle.publish("lux",String(int(event.light)),PRIVATE);
  }
  else
  {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
  }
  
  //wait 2 seconds
  delay(2000);
  
  
  
}


/**************************************************************************/
/*
    Reads the voltage from the TMP36 Temp sensor and converts into Celsius 
*/
/**************************************************************************/
String getTemp(){
    
     //getting the voltage reading from the temperature sensor
    int reading = analogRead(sensorPin);
    // converting that reading to voltage, for 3.3v arduino use 3.3
    float voltage = reading * 3.3;
    voltage /= 4095.0;
 
    float tempC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree with 500 mV offset
  												//to degrees ((voltage - 500mV) times 100)
    return String(int(tempC));
    
}



/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
}




