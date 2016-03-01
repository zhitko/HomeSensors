#include <VirtualWire.h>
#define TXPIN 4

#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11     // DHT 11 
//#define DHTTYPE DHT22   // DHT 22 (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

#define NODEID 1

void setup()
{
  Serial.begin(9600);

  randomSeed(analogRead(A0));

  //vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);
  vw_set_tx_pin(TXPIN);
  Serial.println("Transmitter");

  dht.begin();
}

int ftoa(char *a, float f)
{
  int left=int(f);
  float decimal = f-left;
  int right = decimal *100;
  if (right > 10) {
    sprintf(a, "%d.%d",left,right);
  } else { 
    sprintf(a, "%d.0%d",left,right);
  }
}

int xmitMessage(char *msg){
    digitalWrite(13, true);
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx();
    digitalWrite(13, false);
}

void loop()
{
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float hic = dht.computeHeatIndex(t, h, false);
  
  if (isnan(h) || isnan(t)) {
    sprintf(message, "ID:%d:TS:%lu:ER:ERROR\0", NODEID, millis());
    Serial.println("Failed to read from DHT sensor!");
    xmitMessage(message);
  } else {
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
    Serial.print(" %\t");
    Serial.print("Heat index: "); 
    Serial.print(hic);
    Serial.println(" *C");

    char temperature[6];
    char humidity[6];
    char heat_index[6];

    ftoa(temperature, t);
    ftoa(humidity, h);
    ftoa(heat_index, hic);

    Serial.print("Sending Message: ");
    sprintf(message, "ID:%d:TS:%lu:TC:%s:HI:%s:RH:%s\0", MYID, millis(), temperature, heat_index, humidity);
    Serial.println(message);
    xmitMessage(message);
  }  
  unsigned long randNumber = random(60,120);
  unsigned long sleepTime=randNumber*1000;
  Serial.print("Sleeping ");
  Serial.print(sleepTime);
  Serial.println(" miliseconds");
  delay(sleepTime);  //Sleep randomly to avoid cross talk with another unit
}
