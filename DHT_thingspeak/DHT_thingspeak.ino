#include <Bridge.h>
#include <Console.h>
#include <Process.h>
#include <DHT.h>


#define ARRAY_SIZE           3
#define DHTPIN               2
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Bridge.begin();
  Console.begin();  
};

void postToThingSpeak(String key, float value[]) {
  Process p;
  String cmd = "curl --data \"key="+key;
  for (int i=0;i<ARRAY_SIZE;i++) {
    cmd = cmd + "&field"+ (i+1) + "=" + value[i];
  }
  cmd = cmd + "\" http://api.thingspeak.com/update";
  p.runShellCommand(cmd);
  Console.println(cmd);
  p.close();
}
 
void loop() {
  float vol[ARRAY_SIZE];
  vol[0] = dht.readTemperature();
  vol[1] = dht.readTemperature(true);
  vol[2] = dht.readHumidity();
  postToThingSpeak("",vol);
  delay(30*60000);
}
