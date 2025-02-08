/*****************
 * Example: ATEM Tally basic
 * Uses digital outputs 4 and 5 for Tally LEDs Program & Preview
 */
/*****************
 * TO MAKE THIS EXAMPLE WORK:
 ) - You must install Arduino core for ESP8266 WiFi chip
		Boards manager link: https://arduino.esp8266.com/stable/package_esp8266com_index.json. Library version tested on 2.7.4 with arduino IDE 1.8.19
		Documentation: https://arduino-esp8266.readthedocs.io/en/
 * - You must have an Atem Switcher connected to the same network as the esp8266 - and you should have it working with the desktop software
 * - You must make specific set ups in the below lines where the comment "/
 */
#include <ESP8266WiFi.h>

const char* ssid = "YourWifiSSID";
const char* password = "YourWifiPassword";

// TallyNumber assignment:
int TallyNumber = 1; // change the number to your desired TallyNumber


// Include ATEM library and make an instance:
#include <ATEMWIFI.h>

// Connect to an ATEM switcher on this address and using this local port:
// The port number is chosen randomly among high numbers.
ATEMWIFI AtemSwitcher(IPAddress(192, 168, 10, 240), 56417);  // <= SETUP (the IP address of the ATEM switcher)



void setup() { 

  pinMode(4, OUTPUT);  // LED: 1 is on Program (Tally)
  pinMode(5, OUTPUT);  // LED: 2 is on Preview (Tally)
    
  // Start the Ethernet, Serial (debugging) and UDP:
  Serial.begin(115200);
  while (!Serial); // Wait for Serial monitor

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


  // Initialize a connection to the switcher:
  AtemSwitcher.serialOutput(true); // true for enable ATEM serial monitor , false to disable ATEM serial monitor
  AtemSwitcher.connect();
}


void loop() {
  
    // Check for packets, respond to them etc. Keeping the connection alive!
  AtemSwitcher.runLoop();


    // Write the Tally LEDS:
	//Method 1:	
  digitalWrite(4, !AtemSwitcher.getProgramTally(TallyNumber));
  digitalWrite(5, !AtemSwitcher.getPreviewTally(TallyNumber));
  
  //Method 2: 
  //digitalWrite(4, AtemSwitcher.getProgramTally(TallyNumber));
  //digitalWrite(5, AtemSwitcher.getPreviewTally(TallyNumber));

}
  
