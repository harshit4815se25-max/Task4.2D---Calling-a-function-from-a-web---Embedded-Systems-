#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>

const char* WIFI_SSID = "Go through 5g";
const char* WIFI_PASSWORD = "#Hj#562007";

const char* FIREBASE_API_KEY =
  "AIzaSyCgiG7FBOJCz3XgcXXr1_LnDwfMrRSLdjA";

const char* FIREBASE_HOST =
  "sit210-dtask-default-rtdb.asia-southeast1.firebasedatabase.app";

const char* FIREBASE_EMAIL =
  "harshitjindal348@gmail.com";

const char* FIREBASE_PASSWORD =
  "harshit";

const int LIVINGROOM_LED = 4;
const int BATHROOM_LED = 5;
const int CLOSET_LED = 6;

String firebaseToken = "";

void setup() {
  Serial.begin(115200);

  pinMode(LIVINGROOM_LED, OUTPUT);
  pinMode(BATHROOM_LED, OUTPUT);
  pinMode(CLOSET_LED, OUTPUT);

  digitalWrite(LIVINGROOM_LED, LOW);
  digitalWrite(BATHROOM_LED, LOW);
  digitalWrite(CLOSET_LED, LOW);

  connectWiFi();

  if (!firebaseLogin()) {
    Serial.println("Firebase login failed");

    while (true) {
      delay(1000);
    }
  }
}

void loop() {
  updateLED("livingroom", LIVINGROOM_LED);
  updateLED("bathroom", BATHROOM_LED);
  updateLED("closet", CLOSET_LED);

  delay(1000);
}

void connectWiFi() {
  Serial.print("Connecting to WiFi");

  while (WiFi.begin(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {
    Serial.print(".");
    delay(2000);
  }

  Serial.println();
  Serial.println("WiFi connected");

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

bool firebaseLogin() {
  WiFiSSLClient sslClient;

  HttpClient http(
    sslClient,
    "identitytoolkit.googleapis.com",
    443
  );

  String url =
    "/v1/accounts:signInWithPassword?key=" +
    String(FIREBASE_API_KEY);

  String body =
    "{\"email\":\"" +
    String(FIREBASE_EMAIL) +
    "\",\"password\":\"" +
    String(FIREBASE_PASSWORD) +
    "\",\"returnSecureToken\":true}";

  http.beginRequest();
  http.post(url);

  http.sendHeader(
    "Content-Type",
    "application/json"
  );

  http.sendHeader(
    "Content-Length",
    body.length()
  );

  http.beginBody();
  http.print(body);
  http.endRequest();

  int status = http.responseStatusCode();

  String response = http.responseBody();

  http.stop();

  Serial.print("Firebase login status: ");
  Serial.println(status);

  if (status != 200) {
    Serial.println(response);
    return false;
  }

  JSONVar data = JSON.parse(response);

  if (JSON.typeof(data) == "undefined") {
    Serial.println("Invalid Firebase response");
    return false;
  }

  firebaseToken =
    (const char*)data["idToken"];

  Serial.println("Firebase login successful");

  return true;
}

bool readRoom(String room) {
  WiFiSSLClient sslClient;

  HttpClient http(
    sslClient,
    FIREBASE_HOST,
    443
  );

  String url =
    "/" + room + ".json?auth=" +
    firebaseToken;

  http.get(url);

  int status =
    http.responseStatusCode();

  String response =
    http.responseBody();

  http.stop();

  response.trim();

  if (status != 200) {
    Serial.print("Database error: ");
    Serial.println(status);

    Serial.println(response);

    return false;
  }

  Serial.print(room);
  Serial.print(": ");
  Serial.println(response);

  return response == "true";
}

void updateLED(String room, int pin) {
  bool state = readRoom(room);

  if (state) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}