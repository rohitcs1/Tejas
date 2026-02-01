#include <ESP8266WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

/************ WIFI ************/
#define WIFI_SSID     "1ST FLOOR"
#define WIFI_PASS     "Cg07la7336"

/************ SINRIC PRO CREDENTIALS ************/
#define APP_KEY       "ae952b5b-fc30-4d25-91bb-adac457d6cbe"
#define APP_SECRET    "25f43eba-596e-44e0-bc02-f49575cd2af3-ffad9d04-c3ae-484e-9d9f-fdc018095205"
#define DEVICE_ID     "694cc06d6ebb39d664d3eb37"

/************ PINS ************/
#define RELAY_PIN   D1
#define SWITCH_PIN  D2

/************ VARIABLES ************/
bool relayState = false;
bool lastSwitchState = HIGH;

/************ ALEXA / APP CALLBACK ************/
bool onPowerState(const String &deviceId, bool &state) {
  relayState = state;
  digitalWrite(RELAY_PIN, relayState ? LOW : HIGH);
  return true;
}

/************ WIFI CONNECT ************/
void setupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

/************ SINRIC SETUP ************/
void setupSinric() {
  SinricProSwitch &mySwitch = SinricPro[DEVICE_ID];
  mySwitch.onPowerState(onPowerState);

  SinricPro.begin(APP_KEY, APP_SECRET);
}

/************ MANUAL SWITCH ************/
void checkPhysicalSwitch() {
  bool currentState = digitalRead(SWITCH_PIN);

  if (lastSwitchState == HIGH && currentState == LOW) {
    relayState = !relayState;
    digitalWrite(RELAY_PIN, relayState ? LOW : HIGH);

    // 🔁 Sync back to Alexa + App
    SinricProSwitch &mySwitch = SinricPro[DEVICE_ID];
    mySwitch.sendPowerStateEvent(relayState);

    delay(300); // debounce
  }
  lastSwitchState = currentState;
}

/************ SETUP ************/
void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  digitalWrite(RELAY_PIN, HIGH); // OFF initially

  Serial.begin(9600);

  setupWiFi();
  setupSinric();
}

/************ LOOP ************/
void loop() {
  SinricPro.handle();
  checkPhysicalSwitch();
}
