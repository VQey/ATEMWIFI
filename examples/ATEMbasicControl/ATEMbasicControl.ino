/*****************
 * Example: ATEM Basic Control for esp8266
 * Connects to the Atem Switcher and outputs changes to Preview and Program on the Serial monitor (at 115200 baud)
 * Uses digital inputs 5 and 4 (active Low) to select input 1 and 2 on Preview Bus
 * Uses digital input 2 (active Low) to "Cut" (Preview and Program swaps)
 * Uses digital outputs 12 and 14 for Tally LEDs for input 1 and 2 (Active HIGH)
 */
/*****************
 * TO MAKE THIS EXAMPLE WORK:
 ) - You must install Arduino core for ESP8266 WiFi chip
		Boards manager link: https://arduino.esp8266.com/stable/package_esp8266com_index.json
		Documentation: https://arduino-esp8266.readthedocs.io/en/3.1.2/
 * - You must have an Atem Switcher connected to the same network as the esp8266 - and you should have it working with the desktop software
 * - You must make specific set ups in the below lines where the comment "/
 */

#include <ESP8266WiFi.h>
const char* ssid = "SmartBridge";
const char* password = "tally12345";

// Include ATEM library and make an instance:
#include <ATEMWIFI.h>

// Connect to an ATEM switcher on this address and using this local port:
// The port number is chosen randomly among high numbers.
ATEMWIFI AtemSwitcher(IPAddress(192, 168, 10, 240), 56417);  // <= SETUP (the IP address of the ATEM switcher)



void setup() { 

    // Set up pins for 
  pinMode(5, INPUT_PULLUP);  // Cut
  pinMode(4, INPUT_PULLUP);  // Select 1 for Preview
  pinMode(2, INPUT_PULLUP);  // Select 2 for Preview
  pinMode(12, OUTPUT);  // LED: 1 is on Program (Tally)
  pinMode(14, OUTPUT);  // LED: 2 is on Program (Tally)
    

  Serial.begin(115200);  
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
  AtemSwitcher.serialOutput(true);
  AtemSwitcher.connect();
}

// For pushButtons:
int pushButton = 0;

void loop() {
  
    // Check for packets, respond to them etc. Keeping the connection alive!
  AtemSwitcher.runLoop();


    // Write the Tally LEDS:
  digitalWrite(12, !AtemSwitcher.getProgramTally(1));
  digitalWrite(14, !AtemSwitcher.getProgramTally(2));


  if (digitalRead(5))  {
    if (pushButton !=5)  {
      pushButton=5;
      Serial.println("Select 1");
      AtemSwitcher.changePreviewInput(1);
    }
  } else if (pushButton==5) {
    pushButton = 0; 
  }
  if (digitalRead(4))  {
    if (pushButton !=4)  {
      pushButton=4;
      Serial.println("Select 2");
      AtemSwitcher.changePreviewInput(4);
    }
  } else if (pushButton==4) {
    pushButton = 0; 
  }
  if (digitalRead(2))  {
    if (pushButton !=2)  {
      pushButton=2;
      Serial.println("Cut");
      AtemSwitcher.doCut();
    }
  } else if (pushButton==2) {
    pushButton = 0; 
  }
}
