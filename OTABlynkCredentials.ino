#include "OTABlynkCredentials.h"
//#include <DHT.h>

#ifdef ESP8266
#include <BlynkSimpleEsp8266.h>
#elif defined(ESP32)
#include <BlynkSimpleEsp32.h>
#else
#error "Board not found"
#endif



//#define DHTPIN 12          // What digital pin the DHT is connected to
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321

#define BellPressPIN 13   // What digital pin the BellPress is connected to
#define RelayPIN D1   // Specify the digital output to use for the relay;
//do not use D3 as it is the flash pin and connecting a relay to it shorts the flash pin on boot;
//do not use D4 as it tends to boot up with interference
//cannot use relay direct need an amplification circuit

credentials Credentials;
//DHT dht(DHTPIN, DHTTYPE);
//BlynkTimer timer;
//unsigned long last_trigger;
unsigned long last_bell;

int val; // Variable to read the Bell Press Pin

//Variables
char auth_token[33];
bool connected_to_internet = 0;
const int Erasing_button = 0;

// the timer object
BlynkTimer timer;
int newTimer = 1;

//Provide credentials for your ESP server
char* esp_ssid = "Garage Door";
char* esp_pass = "";

//
//
// .     Add your variables 
//
// .             HERE
// .
//
//


void setup()

{

  Serial.begin(115200);
  pinMode(Erasing_button, INPUT_PULLUP);
  pinMode(BellPressPIN, INPUT);    // declare Bell Press Pin as input
  pinMode(RelayPIN, OUTPUT);    // declare D4 as output to light up the LED

  for (uint8_t t = 4; t > 0; t--) {
    // Serial.println(t);
    delay(1000);
  }

  // Press and hold the button to erase all the credentials
  if (digitalRead(Erasing_button) == LOW)
  {
    Credentials.Erase_eeprom();

  }

  String auth_string = Credentials.EEPROM_Config();
  auth_string.toCharArray(auth_token, 33);

  if (Credentials.credentials_get())
  {

    Blynk.config(auth_token);
    connected_to_internet = 1;

  }
  else
  {
    Credentials.setupAP(esp_ssid, esp_pass);
    connected_to_internet = 0;
  }


  if (connected_to_internet)
  {

    //
    //
    // .  Write the setup part of your code
    //
    // .             HERE
    // .
    //
    //

    // dht.begin();

    // Setup a function to be called every second
    // timer.setInterval(1000L, sendSensor);

  }


}


BLYNK_WRITE(V0){
  if(param.asInt()){
    // button pressed for 2000ms
    newTimer = timer.setTimeout(2000,newTimerCallback);
  } else {
    timer.disable(newTimer);
  }
}


void newTimerCallback(){
  // perform your "held" button action. 
  digitalWrite(RelayPIN, HIGH);
  delay(1000);        // pauses for 1000 ms
  digitalWrite(RelayPIN, LOW);


}


void loop()
{
  Credentials.server_loops();

  if (connected_to_internet)
  {


    //
    //
    // .  Write the loop part of your code
    //
    // .             HERE
    // .
    //
    //

    /*
    val = digitalRead(BellPressPIN);  // read whether bell has been pressed value
    if (val == HIGH) {         // check if the input is HIGH
                               // Do nothing, because the pin is normally high when not pressed
    } else {
      //run this only if bell value read is not high, and more than 5.5seconds has elapsed since the last not high
      if ((millis() > last_bell + 5500) or (last_bell == 0)) {
      Blynk.notify("Hey, there's someone at the door! Go answer it!");  // Notify user that bell has been pressed
      last_bell = millis();
      }    

    }
    */

    Blynk.run();
    timer.run(); // Initiates SimpleTimer
  }
}

/*
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  Serial.println(h);
  Serial.println(t);


  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);

  //run this only if temperature is higher than 29 and more than 30mins has elapsed since the last trigger
  if ((t > 29) and ((millis() > last_trigger + 1800000) or (last_trigger == 0))) {
  Blynk.notify("Hey, the temperature is above 29 degrees celsius!");
  last_trigger = millis();
  }
}
*/
