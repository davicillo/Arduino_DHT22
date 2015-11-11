#include <DHT.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

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

  if (client) {
    // read the command
    String command = client.readString();
    command.trim();        //kill whitespace
    Serial.println(command);
    // is "temperature" command?
    if (command == "temperature") {
      // get the time from the server:
      Process time;
      time.runShellCommand("date");
      String timeString = "";
      while(time.available()) {
        char c = time.read();
        timeString += c;
      }
      
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float h = dht.readHumidity();
      // Read temperature as Celsius
      float t = dht.readTemperature();
      // Read temperature as Fahrenheit
      float f = dht.readTemperature(true);
      
      client.print("Current time on the Yún: ");
      client.println(timeString);
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
  delay(500);

  
}
