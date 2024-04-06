

////192.168. 1.184 that works in the gateway 192.168. 1.1. Important: you need to use an available IP address in your local network and the corresponding gateway.
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <AsyncTCP.h>  // Include AsyncTCP library
#include <ArduinoJson.h>
#include <Keypad.h>
const int ROW_NUM = 4;
const int COLUMN_NUM = 4;
char keys[ROW_NUM][COLUMN_NUM] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte pin_rows[ROW_NUM] = {26, 25, 33, 32};
byte pin_column[COLUMN_NUM] = {13,12,14,27};
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);
//#include <AsyncWebServer.h>  // Include AsyncWebServer library
const char* serverUrl = "http://192.168.137.93/PROJECT_1/welcome.php";
int fingerprintId = 0;

const char* ssid = "TOM 9210"; // Replace with your WiFi SSID
const char* password = "1234567890"; // Replace with your WiFi password

//AsyncWebServer server(80);  // Create AsyncWebServer instance

LiquidCrystal_I2C lcd(0x3F, 16, 2); // Set the LCD address and dimensions

// Initialize Serial2 using HardwareSerial
HardwareSerial mySerial(2); // Use Serial2
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

// Function prototypes
void enroll();
void verify();
uint8_t readnumber(void);
bool fingerprintExists(uint8_t id);
uint8_t getFingerprintEnroll();


void setup() {
  lcd.begin();
  Serial.begin(115200);
  lcd.backlight();

  // Connect to WiFi
  lcd.print("Connecting to ");
  lcd.setCursor(0, 1);
 lcd.print(String(ssid) + "wifi net");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
lcd.print ("connected ");
  // Initialize AsyncWebServer
  //server.on("/", HTTP_POST, handlePostRequest);   // Define POST request handler
  //server.begin();
display();
  // Set the data rate for the sensor serial port
  mySerial.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    lcd.clear();
    lcd.print("Fingerprint");
    lcd.setCursor(0, 1);
    lcd.print("sensor found!");
  } else {
    lcd.clear();
    lcd.print("Fingerprint");
    lcd.setCursor(0, 1);
    lcd.print("sensor not found!");
    while (1) { delay(1); }
  }

  delay(500); // Add delay after sensor found message

  lcd.setCursor(0, 1);
  lcd.print("Reading sensor");
  lcd.setCursor(0, 1);
  lcd.print("parameters");

  // Read sensor parameters
  finger.getParameters();
  lcd.clear();
  lcd.print("Capacity: "); lcd.print(finger.capacity);
  lcd.setCursor(0, 1);
  lcd.print("Security level: "); lcd.print(finger.security_level);
  delay(2000);
  lcd.clear();
}


void display() {
  String message = "ATTENDANCE MANAGEMENT SYSTEM";

  // Print the message on the LCD
  for (int i = 0; i < message.length() + 16; i++) {
    lcd.clear(); // Clear the LCD
    lcd.setCursor(max(0, -i), 0); // Set the cursor position
    lcd.print(message.substring(max(0, i), (size_t) min((unsigned int)(i + 16), (unsigned int) message.length())).c_str()); // Print the substring of the message
    delay(450); // Adjust the delay as needed for desired scrolling speed
  }
}

//uint8_t id; // Declare a global variable to store the fingerprint ID
void sendPostRequest(int id) {
  HTTPClient http;

  // Specify server address and endpoint
  String serverAddress = serverUrl;
  Serial.print("Sending POST request to: ");
  Serial.println(serverAddress);

  http.begin(serverAddress);

  // Set headers
  http.addHeader("Content-Type", "application/json");

  // Create JSON payload with the user ID
  String payload = "{\"id\": \"" + String(id) + "\"}";

  // Print the payload for debugging
  Serial.print("Payload: ");
  Serial.println(payload);

  // Send the POST request with the payload
  int httpResponseCode = http.POST(payload);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.println("Error sending POST request");
  }

  http.end();
}



uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p == FINGERPRINT_OK) {
    // Image taken successfully, convert it to a template
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK) {
      lcd.clear();
      lcd.print("Image conversion failed!");
      return p;
    }
  } else if (p != FINGERPRINT_NOFINGER) {
    // Handle other cases like communication errors or imaging failures
    lcd.clear();
    lcd.print("Unknown error");
    return p;
  }
  return p;
}
void verify() {
  Serial.println("Entering verify() function"); // Debugging statement
  lcd.clear();
  lcd.print("Place finger...");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");
 //finger.setFingerprintPassword(12345); // Change the password if needed
  //finger.setImageValidThreshold(50); 
  uint8_t p = FINGERPRINT_NOFINGER; // Initialize p to FINGERPRINT_NOFINGER

  // Keep looping until a fingerprint is detected
  while (p != FINGERPRINT_OK) {
    p = getFingerprintID();
    if (p == FINGERPRINT_OK) {
      // Fingerprint detected, continue with verification
      p = finger.fingerSearch();
      if (p == FINGERPRINT_OK) {
        // Update the global variable 'fingerprintId' with the fingerprint ID
        fingerprintId = finger.fingerID;

        // Print matched, send POST request to server
        Serial.print("Sending ID: ");
        Serial.println(fingerprintId); // Print the ID before sending
        // Check if fingerprintId is not 0 before sending POST request
        if (fingerprintId != 0) {
          sendPostRequest(fingerprintId);
          // Wait for response from server
          delay(100); // Adjust delay as needed for server response
          String serverResponse = ""; // Variable to store server response

          // Check for response from server
          while (Serial.available()) {
            char c = Serial.read();
            serverResponse += c;
            delay(10);
          }

          // Print the server response on the serial monitor
          Serial.print("Server Response: ");
          Serial.println(serverResponse);

          // Extract first name from the server response
          String firstName = "";
          if (serverResponse.length() > 0) {
            firstName = serverResponse.substring(1); // Exclude the first character which indicates response type
          } else {
            firstName = "Error: No response from server";
          }

          // Display welcome message with first name on LCD
          lcd.clear();
          lcd.print("RECORD UPDATED, ");
        lcd.setCursor(0,1);
        lcd.print("successfully");
        delay(2000);
lcd.clear();
lcd.print("thank you");
          delay(2000);
        } else {
          // Print error if fingerprintId is 0
          Serial.println("Invalid fingerprint ID");
          lcd.clear();
          lcd.print("Invalid print ");
          lcd.setCursor(0,1);
          lcd.print("ID");
          delay(2000);
        }
      } else if (p == FINGERPRINT_NOTFOUND) {
        lcd.clear();
        lcd.print("NOT REGISTERED");
        lcd.setCursor(0,1);
        lcd.print("visit website");
        delay(2000);
      } else {
        lcd.clear();
        lcd.print("Unknown error");
        delay(2000);
      }
    } else if (p == FINGERPRINT_NOFINGER) {
      lcd.clear();
      lcd.print("DETECTING FINGER PRINT...");
      lcd.setCursor(0,1);
      lcd.print("please wait");
      delay(500); // Add a small delay before checking again
    } else {
      lcd.clear();
      lcd.print("Unknown error");
      delay(2000);
    }
  }
}



void loop() {
  // Print initial instructions on the LCD
  lcd.clear();
  lcd.print("A=REGISTRATION");
  lcd.setCursor(0, 1);
  lcd.print("B=ATTENDANCE");
delay(100);
  // Check for keypress
  char key = keypad.getKey();
  
  if (key != NO_KEY) {
    lcd.clear();
    lcd.print("Key pressed:");
    lcd.setCursor(0, 1);
    lcd.print(key);

    if (key == 'A') {
      enroll();
    } else if (key == 'B') {
      verify();
    } else if (key == 'D') { // Enter key
      lcd.clear();
      lcd.print("Press A' to enroll");
      lcd.setCursor(0, 1);
      lcd.print("      B' to verify");
    } else {
      lcd.clear();
      lcd.print("Invalid choice!");
      lcd.setCursor(0,1);
      lcd.print("Try again");
      delay(2000);
    }
  }
}

void enroll() {
  lcd.clear();
lcd.print("Enter ID # (1-127):");
lcd.setCursor(0, 1);
lcd.print("ID: ");

String idString = ""; // String to store the entered ID

while (true) {
  char key = keypad.getKey();
  
  if (key != NO_KEY) {
    if (key == 'D') { // Enter key
      // Convert the entered ID from String to int
      id = idString.toInt();

      // Check if the entered ID is valid (1-127)
      if (id < 1 || id > 127) {
        lcd.clear();
        lcd.print("Invalid ID!");
        delay(2000);
        return;
      } else {
        break; // Exit the loop if ID is valid
      }
    } else if (key == '#') { // Delete key
      if (idString.length() > 0) {
        idString.remove(idString.length() - 1); // Remove the last digit
        lcd.setCursor(3, 1); // Move cursor back
        lcd.print(" "); // Print space to clear the deleted digit
        lcd.setCursor(3, 1); // Move cursor back
      }
    } else { // Numeric keys
      if (idString.length() < 3) { // Maximum 3 digits for ID
        idString += key; // Append the pressed key to the ID string
        lcd.print(key); // Print the pressed key
      }
    }
  }
}

// Display the entered ID on the LCD
lcd.setCursor(3, 1);
lcd.print(idString);


  if (fingerprintExists(id)) {
    lcd.clear();
    lcd.print("Fingerprint");
    lcd.setCursor(0, 1);
    lcd.print("exists!");
    delay(2000);
    return;
  }

  lcd.clear();
  lcd.print("Enrolling ID #");
  lcd.print(id);
  delay(2000);

  uint8_t p = getFingerprintEnroll();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("  REGISTRATION ");
    lcd.setCursor(0, 1);
    lcd.print(" SUCCESSFULL");
    delay(2000);
  } else {
    lcd.clear();
    lcd.print("Error enrolling");
    lcd.setCursor(0, 1);
    lcd.print("fingerprint!");
    delay(2000);
  }
}


uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (!Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

bool fingerprintExists(uint8_t id) {
  return finger.getImage() == FINGERPRINT_OK;
}

uint8_t getFingerprintEnroll() {
  int p = -1;
  lcd.clear();
  lcd.print("Place finger...");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    delay(50); // Add delay for stability
  }

  // Image taken
  lcd.clear();
  lcd.print("Image taken!");
  delay(2000);

  // Convert image to template
  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Error converting");
    lcd.setCursor(0, 1);
    lcd.print("image!");
    delay(2000);
    return p;
  }

  // Remove finger
  lcd.clear();
  lcd.print("Remove finger...");
  delay(2000);

  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
    delay(50); // Add delay for stability
  }

  // Ask for same finger again
  lcd.clear();
  lcd.print("Place same ");
  lcd.setCursor(0, 1);
  lcd.print("finger again...");
  delay(2000);

  // Wait for finger
  p = -1;
  lcd.clear();
  lcd.print("Waiting...");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    delay(50);
    // Add delay for stability
  }

  // Image taken
  lcd.clear();
  lcd.print("Image taken!");
  delay(2000);

  // Convert image to template
  // Convert image to template
  p = finger.image2Tz(2);
  if (p != FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Error converting");
    lcd.setCursor(0, 1);
    lcd.print("image!");
    delay(2000);
    return p;
  }

  // Create fingerprint model
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Fingerprint");
    lcd.setCursor(0, 1);
    lcd.print("enrolled!");
    delay(2000);
  } else {
    lcd.clear();
    lcd.print("Error creating");
    lcd.setCursor(0, 1);
    lcd.print("model!");
    delay(2000);
    return p;
  }

  // Store fingerprint model
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Fingerprint");
    lcd.setCursor(0, 1);
    lcd.print("stored!");
    delay(2000);
  } else {
    lcd.clear();
    lcd.print("Error storing");
    lcd.setCursor(0, 1);
    lcd.print("fingerprint!");
    delay(2000);
    return p;
  }


  




  return true;
}
