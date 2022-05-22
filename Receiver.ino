#include <M5StickC.h>
#include "BLEDevice.h"

//BLE Server name
#define bleServerName "M5"

/* UUID's of the service, characteristic that we want to read*/
// BLE Service
static BLEUUID bmeServiceUUID("91bad492-b950-4226-aa2b-4ede9fa42f59");

//Temperature Celsius Characteristic
static BLEUUID temperatureCharacteristicUUID("cba1d466-344c-4be3-ab3f-189f80dd7518");


//Flags stating if should begin connecting and if the connection is up
static boolean doConnect = false;
static boolean connected = false;

//Address of the peripheral device. Address will be found during scanning...
static BLEAddress *pServerAddress;

//Characteristicd that we want to read
static BLERemoteCharacteristic* temperatureCharacteristic;

//Activate notify
const uint8_t notificationOn[] = {0x1, 0x0};
const uint8_t notificationOff[] = {0x0, 0x0};


//Variables to store temperature and humidity
char* temperatureChar;

//Flags to check whether new temperature and humidity readings are available
boolean newTemperature = false;


//Connect to the BLE Server that has the name, Service, and Characteristics
bool connectToServer(BLEAddress pAddress) {
  BLEClient* pClient = BLEDevice::createClient();

  // Connect to the remove BLE Server.
  pClient->connect(pAddress);
  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(bmeServiceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(bmeServiceUUID.toString().c_str());
    return (false);
  }

  // Obtain a reference to the characteristics in the service of the remote BLE server.
  temperatureCharacteristic = pRemoteService->getCharacteristic(temperatureCharacteristicUUID);

  if (temperatureCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID");
    return false;
  }
  Serial.println(" - Found our characteristics");

  //Assign callback functions for the Characteristics
  temperatureCharacteristic->registerForNotify(temperatureNotifyCallback);
  return true;
}

//Callback function that gets called, when another device's advertisement has been received
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      if (advertisedDevice.getName() == bleServerName) { //Check if the name of the advertiser matches
        advertisedDevice.getScan()->stop(); //Scan can be stopped, we found what we are looking for
        pServerAddress = new BLEAddress(advertisedDevice.getAddress()); //Address of advertiser is the one we need
        doConnect = true; //Set indicator, stating that we are ready to connect
        Serial.println("Device found. Connecting!");
      }
    }
};

//When the BLE Server sends a new temperature reading with the notify property
static void temperatureNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
                                      uint8_t* pData, size_t length, bool isNotify) {
  //store temperature value
  temperatureChar = (char*)pData;
  newTemperature = true;
}


//function that prints the latest sensor readings in the OLED display
void printReadings() {
  M5.update();

  //Erase the previous contents in the display
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.setTextColor(RED);
  M5.Lcd.setCursor(20, 18);
  M5.Lcd.setTextSize(1.8);
  M5.Lcd.print("[ ");
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.print("BLE Smart Flask");
  M5.Lcd.setTextColor(RED);
  M5.Lcd.print(" ]");
  M5.Lcd.setCursor(18, 40);
  M5.Lcd.setTextColor(GREEN);
  Serial.print("Temperature:");
  M5.Lcd.print("Temperature: ");
  Serial.println(temperatureChar);
  M5.Lcd.print(temperatureChar);
  M5.Lcd.print(" C");

  M5.Lcd.setCursor(15, 55);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.print("---------------------");

  float x = atof(temperatureChar);
  if (x >= 35.0)
  {
    Serial.println("It may be a over heat");
    M5.Lcd.setCursor(15, 65);
    M5.Lcd.setTextColor(RED);

    M5.Lcd.print("It's may be over heat");
  }
  else {
    Serial.println("It's ok to drink");
    M5.Lcd.setCursor(30, 65);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.print("It's ok to drink");
  }

  M5.Lcd.setCursor(15, 75);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.print("---------------------");
}
void setup() {

  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Axp.ScreenBreath(10);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.setCursor(10, 35);
  M5.Lcd.setTextSize(1.8);

  //Start serial communication
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");

  //Init BLE device
  BLEDevice::init("");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);
}

void loop() {
   
  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) {
      Serial.println("We are now connected to the BLE Server.");
      M5.Lcd.print("Connected to BLE Server");
      //Activate the Notify property of each Characteristic
      temperatureCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
      connected = true;
    } else {
      Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
      M5.Lcd.print("Not Connected");
    }
    doConnect = false;
  }
  //if new temperature readings are available, print in the OLED
  if (newTemperature) {
    newTemperature = false;
    printReadings();
  }
}
