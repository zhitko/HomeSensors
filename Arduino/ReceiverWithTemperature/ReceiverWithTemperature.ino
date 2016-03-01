//Reciever

#include <VirtualWire.h>
#define RXPIN 7

void setup()
{
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  Serial.println("Reciever");
  //vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);
  vw_set_rx_pin(RXPIN);
  vw_rx_start();
}

void loop()
{
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  uint8_t buf[VW_MAX_MESSAGE_LEN];

  if(vw_get_message(buf, &buflen))
  {
    digitalWrite(13,true);
    for (int i = 0; i < buflen; i++)
    {
      Serial.print(char(buf[i]));
    }
    digitalWrite(13,false);
    Serial.println("");
  }
}
