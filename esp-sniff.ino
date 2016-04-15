extern "C" {
#include "user_interface.h"
}

void callback(uint8 *buf, uint16 len) {
  Serial.println("callback called");
  Serial.println(len);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Serial.print(" -> Promisc mode setup ...");
  wifi_set_promiscuous_rx_cb(callback);
  wifi_promiscuous_enable(1);
  Serial.println(" done.");

  Serial.print(" -> Set opmode ...");
  wifi_set_opmode( 0x1 );
  Serial.println(" done.");  
}

void loop()
{
  uint8 new_channel = wifi_get_channel() % 12 + 1;
  wifi_set_channel(new_channel);
  Serial.print("Channel set to ");
  Serial.println(new_channel);

  // wait one second before changing the channel
  delay(1000);
}
