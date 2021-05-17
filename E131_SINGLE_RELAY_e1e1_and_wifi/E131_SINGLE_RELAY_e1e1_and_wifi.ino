// Wemos D1 Mini E1.31 and Smart Home Support via Blync

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <E131.h>

#define VPIN_BUTTON_1 V1 // the virtual pin for blync
#define channel_1_green 1 // the channel number to link to output

// this sets the pin numbers to use as outputs.
#define output_1_green D1 // the pin to use as output 1 green

// ***** USER SETUP STUFF *****
#define AUTH "" // your blync auth
const char WIFI_SSID[] = "";  // replace with your SSID.
const char WIFI_PASS[] = ""; // replace with your PASSWORD.

BlynkTimer timer;
const int universe = 5; // this sets the universe number you are using.

// this sets the channel number used by the output.
E131 e131;

BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_BUTTON_1);
}
BLYNK_WRITE(VPIN_BUTTON_1) {
  digitalWrite(output_1_green, param.asInt());
  Serial.println("Button_1:");
}
void checkBlynkStatus() { // called every 3 seconds by SimpleTimer
}

void setup() {
  Serial.begin(115200);
 
  pinMode(output_1_green, OUTPUT); // set the pins chosen above as outputs.
  digitalWrite(output_1_green, LOW); // set the pins chosen above to low / off.
  
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(AUTH);

  /* Choose one to begin listening for E1.31 data */
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  e131.begin(WIFI_SSID, WIFI_PASS);               /* via Unicast on the default port */
  //   e131.beginMulticast(ssid, passphrase, universe); /* via Multicast for Universe 1 */
}

void loop() {
  /* Parse a packet */
  uint16_t num_channels = e131.parsePacket();
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi Not Connected");
  }
  else
  {
    
  
    /* Process channel data if we have it */
    if (num_channels) {
      Serial.println("we have data");
      Serial.println(e131.data[channel_1_green]);
  
      if (e131.data[channel_1_green] >= 200) //if channel value is greater then 127
      {
        digitalWrite(output_1_green, HIGH); //turn relay on
      }
      else
      {
        digitalWrite(output_1_green, LOW); //else turn it off
      }
    }

    Blynk.run();
  
  }

}
