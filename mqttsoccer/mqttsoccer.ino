
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "neo_PubSubClient.h"
#include "string.h"
#include "stdio.h"

#define WLAN_SSID       "geeklab"
#define WLAN_PASS       "ai@geeklab"

#define AIO_SERVER      "onedawn.top"
#define AIO_SERVERPORT  8883                   // 8883 for MQTTS
#define AIO_USERNAME "c90TFlj3"  // robot-wuyue
#define AIO_KEY "Za2mM6xi54v7Dr9HRChskdyLeQgE8Ij0"

int leftSpeed = 0, rightSpeed = 0;
const int leftPinA = D1;
const int leftPinB = D2;
const int rightPinA = D5;
const int rightPinB = D6;

ESP8266WebServer server(80);

void __set_speed_one(int pinA, int pinB, int duty) {
  if (duty == 0) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
  } else if (duty > 0) {
    analogWrite(pinA, duty);
    digitalWrite(pinB, LOW);
  } else {
    analogWrite(pinB, duty);
    digitalWrite(pinA, LOW);
  }
}

void set_speed(int left_duty, int right_duty) {
  __set_speed_one(leftPinA, leftPinB, left_duty);
  __set_speed_one(rightPinA, rightPinB, right_duty);
}

WiFiClient client;
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient mqtt(AIO_SERVER, AIO_SERVERPORT, callback, client);

void setup() {
  // put your setup code here, to run once:
  pinMode(leftPinA, OUTPUT);  
  pinMode(leftPinB, OUTPUT);
  pinMode(rightPinA, OUTPUT);
  pinMode(rightPinB, OUTPUT);
  analogWriteFreq(10000);
  delay(1000);
  Serial.begin(115200);

  // 连接WIFI
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println(); Serial.println();
  Serial.print("Connecting to "); Serial.println(WLAN_SSID);
  delay(500); digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(500); digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  delay(500); digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(500); digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(500); digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(500); digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    Serial.print(".");
  }
  Serial.println();
  delay(500);
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  randomSeed(micros());
  Serial.print("Connecting to ");
  Serial.println(AIO_SERVER);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (!mqtt.connected()) {
    reconnect();
  }
  mqtt.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("topic received: ");
  Serial.println(topic);
  if (strcmp("iot/" AIO_USERNAME "/info", topic) == 0) {
    mqtt.publish("iot/" AIO_USERNAME "/info", "robot soccer, see https://github.com/wuyuepku/RobotSoccer\n"
      "usage:\n"
      "./set_speed <left_duty> <right_duty>: both duty from -1023~1023\n");
  } else if (strcmp("iot/" AIO_USERNAME "/set_speed", topic) == 0) {
    int left_duty, right_duty;
    payload[length] = 0;
    sscanf((const char*)payload, "%d %d", &left_duty, &right_duty);
    set_speed(left_duty, right_duty);
    mqtt.publish("iot/" AIO_USERNAME "/set_speed_ret", "OK");
    Serial.print("set_speed, left=");
    Serial.print(left_duty);
    Serial.print(", right=");
    Serial.println(right_duty);
  }
}

void reconnect() {
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqtt.connect(clientId.c_str(), AIO_USERNAME, AIO_KEY, "iot/" AIO_USERNAME "/offline", 0, 0, "robot-wuyue")) {
      Serial.println("connected");
      mqtt.subscribe("iot/" AIO_USERNAME "/set_speed");
      mqtt.publish("iot/" AIO_USERNAME "/online","robot-wuyue");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000); digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }
}
