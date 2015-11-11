#include <DHT.h>
#include <Process.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

Process date;
YunServer server;

void setup() {
  Bridge.begin();        // initialize Bridge
  Serial.begin(9600); 
  dht.begin();

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // Get clients coming from server
  YunClient client = server.accept();

  date.begin("/bin/date");
  date.addParameter("+%d/%m/%Y %T");
  date.run();

  //if there's a result from the date process, get it.
  while (date.available()>0) {
    String timeString = date.readString();    
    // print the results we got.
    Serial.print(timeString);
    Serial.print("-->");
  }

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(", ");
  Serial.print(f);
  Serial.println(" *F ");

  if (client) {
    // read the command
    String command = client.readString();
    command.trim();        //kill whitespace
    Serial.println(command);
    // is "temperature" command?
    if (command == "temperature") {
      client.print("Humidity: "); 
      client.print(h);
      client.print(" %\t");
      client.print("Temperature: "); 
      client.print(t);
      client.print(" *C ");
      client.print(", ");
      client.print(f);
      client.println(" *F ");
    }
    client.stop();
  }

  // Wait a few seconds between measurements.
  delay(5000);
}
