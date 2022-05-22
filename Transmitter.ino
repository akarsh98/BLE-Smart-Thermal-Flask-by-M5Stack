#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <FastLED.h>

// LED Count
#define NUM_LEDS 1
#define DATA_PIN 27

// Define the array of leds
CRGB leds[NUM_LEDS];

#define Button 39
//BLE server name
#define bleServerName "M5"

//LM35
#define ADC_VREF_mV    3300.0 // in millivolt
#define ADC_RESOLUTION 4096.0
#define PIN_LM35       36 // ESP32 pin GIOP36 (ADC0) connected to LM35

float temp;
float tempF;
float hum;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

bool deviceConnected = false;

#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"

// Temperature Characteristic and Descriptor
BLECharacteristic bmeTemperatureCelsiusCharacteristics("cba1d466-344c-4be3-ab3f-189f80dd7518", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor bmeTemperatureCelsiusDescriptor(BLEUUID((uint16_t)0x2902));

//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


void setup() {
  // Start serial communication
  Serial.begin(115200);

  FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);

  pinMode(Button, INPUT);

  // Create the BLE Device
  BLEDevice::init(bleServerName);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *bmeService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristics and Create a BLE Descriptor
  bmeService->addCharacteristic(&bmeTemperatureCelsiusCharacteristics);
  bmeTemperatureCelsiusDescriptor.setValue("BME temperature Celsius");
  bmeTemperatureCelsiusCharacteristics.addDescriptor(&bmeTemperatureCelsiusDescriptor);


  // Start the service
  bmeService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  static bool State;
  State = digitalRead(Button);
  if (State == 0)
  {
    ESP.restart();
  }
  if (deviceConnected) {
    if ((millis() - lastTime) > timerDelay) {

      // Turn the LED on, then pause
      leds[0] = 0xf00000;
      FastLED.show();
      delay(2000);

      // read the ADC value from the temperature sensor
      int adcVal = analogRead(PIN_LM35);
      // convert the ADC value to voltage in millivolt
      float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
      // convert the voltage to the temperature in °C
      temp = milliVolt / 10;

      static char temperatureCTemp[6];
      dtostrf(temp, 6, 2, temperatureCTemp);
      //Set temperature Characteristic value and notify connected client
      bmeTemperatureCelsiusCharacteristics.setValue(temperatureCTemp);
      bmeTemperatureCelsiusCharacteristics.notify();

      // Now turn the LED off, then pause
      leds[0] = 0x00f000;
      FastLED.show();
      
      Serial.print("Temperature Celsius: ");
      Serial.print(temp);
      Serial.println(" ºC");

      lastTime = millis();
    }
  }

}
