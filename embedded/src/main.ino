#include <Arduino.h>
#include <DHT.h>
#include <EEPROM.h>
#include <LoRa.h>

#include "AsyncJson.h"
#include "constants.h"
#include "crypto_adapter.h"
#include "data_types.h"
/* POST ENDPOINT STUFF */
#ifdef RECEIVER
#include <ArduinoJson.h>

#include <ESPmDNS.h>
#include <HTTPClient.h>

// #include <WebServer.h>
#include <StreamString.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <ESPAsyncWebServer.h>

static AsyncWebServer server(80);

const char *const MDNS_NAME = "lora-gateway";

const char *const SSID = "raspberrypiG3-superSecureHotspot";
const char *const PASSWORD = "[PASSWORD HERE]
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠛⠛⠛⠋⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠙⠛⠛⠛⠿⠻⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⡀⠠⠤⠒⢂⣉⣉⣉⣑⣒⣒⠒⠒⠒⠒⠒⠒⠒⠀⠀⠐⠒⠚⠻⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⠏⠀⠀⠀⠀⡠⠔⠉⣀⠔⠒⠉⣀⣀⠀⠀⠀⣀⡀⠈⠉⠑⠒⠒⠒⠒⠒⠈⠉⠉⠉⠁⠂⠀⠈⠙⢿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠔⠁⠠⠖⠡⠔⠊⠀⠀⠀⠀⠀⠀⠀⠐⡄⠀⠀⠀⠀⠀⠀⡄⠀⠀⠀⠀⠉⠲⢄⠀⠀⠀⠈⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⠋⠀⠀⠀⠀⠀⠀⠀⠊⠀⢀⣀⣤⣤⣤⣤⣀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠜⠀⠀⠀⠀⣀⡀⠀⠈⠃⠀⠀⠀⠸⣿⣿⣿⣿
⣿⣿⣿⣿⡿⠥⠐⠂⠀⠀⠀⠀⡄⠀⠰⢺⣿⣿⣿⣿⣿⣟⠀⠈⠐⢤⠀⠀⠀⠀⠀⠀⢀⣠⣶⣾⣯⠀⠀⠉⠂⠀⠠⠤⢄⣀⠙⢿⣿⣿
⣿⡿⠋⠡⠐⠈⣉⠭⠤⠤⢄⡀⠈⠀⠈⠁⠉⠁⡠⠀⠀⠀⠉⠐⠠⠔⠀⠀⠀⠀⠀⠲⣿⠿⠛⠛⠓⠒⠂⠀⠀⠀⠀⠀⠀⠠⡉⢢⠙⣿
⣿⠀⢀⠁⠀⠊⠀⠀⠀⠀⠀⠈⠁⠒⠂⠀⠒⠊⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⢀⣀⡠⠔⠒⠒⠂⠀⠈⠀⡇⣿
⣿⠀⢸⠀⠀⠀⢀⣀⡠⠋⠓⠤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠄⠀⠀⠀⠀⠀⠀⠈⠢⠤⡀⠀⠀⠀⠀⠀⠀⢠⠀⠀⠀⡠⠀⡇⣿
⣿⡀⠘⠀⠀⠀⠀⠀⠘⡄⠀⠀⠀⠈⠑⡦⢄⣀⠀⠀⠐⠒⠁⢸⠀⠀⠠⠒⠄⠀⠀⠀⠀⠀⢀⠇⠀⣀⡀⠀⠀⢀⢾⡆⠀⠈⡀⠎⣸⣿
⣿⣿⣄⡈⠢⠀⠀⠀⠀⠘⣶⣄⡀⠀⠀⡇⠀⠀⠈⠉⠒⠢⡤⣀⡀⠀⠀⠀⠀⠀⠐⠦⠤⠒⠁⠀⠀⠀⠀⣀⢴⠁⠀⢷⠀⠀⠀⢰⣿⣿
⣿⣿⣿⣿⣇⠂⠀⠀⠀⠀⠈⢂⠀⠈⠹⡧⣀⠀⠀⠀⠀⠀⡇⠀⠀⠉⠉⠉⢱⠒⠒⠒⠒⢖⠒⠒⠂⠙⠏⠀⠘⡀⠀⢸⠀⠀⠀⣿⣿⣿
⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠑⠄⠰⠀⠀⠁⠐⠲⣤⣴⣄⡀⠀⠀⠀⠀⢸⠀⠀⠀⠀⢸⠀⠀⠀⠀⢠⠀⣠⣷⣶⣿⠀⠀⢰⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠁⢀⠀⠀⠀⠀⠀⡙⠋⠙⠓⠲⢤⣤⣷⣤⣤⣤⣤⣾⣦⣤⣤⣶⣿⣿⣿⣿⡟⢹⠀⠀⢸⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠑⠀⢄⠀⡰⠁⠀⠀⠀⠀⠀⠈⠉⠁⠈⠉⠻⠋⠉⠛⢛⠉⠉⢹⠁⢀⢇⠎⠀⠀⢸⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⣀⠈⠢⢄⡉⠂⠄⡀⠀⠈⠒⠢⠄⠀⢀⣀⣀⣰⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⢀⣎⠀⠼⠊⠀⠀⠀⠘⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄⡀⠉⠢⢄⡈⠑⠢⢄⡀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠁⠀⠀⢀⠀⠀⠀⠀⠀⢻⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣦⣀⡈⠑⠢⢄⡀⠈⠑⠒⠤⠄⣀⣀⠀⠉⠉⠉⠉⠀⠀⠀⣀⡀⠤⠂⠁⠀⢀⠆⠀⠀⢸⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣦⣄⡀⠁⠉⠒⠂⠤⠤⣀⣀⣉⡉⠉⠉⠉⠉⢀⣀⣀⡠⠤⠒⠈⠀⠀⠀⠀⣸⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣶⣶⣶⣤⣤⣤⣤⣀⣀⣤⣤⣤⣶⣾⣿⣿⣿⣿⣿
";

const char *const SERVER_ENDPOINT = "/api/sensor/";
;
char SERVER_ADDRESS[20] = "raspberrypiG3.local";
char JWT_REFRESH_TOKEN[1000] = "aGFoYSEgdGhpcyBpcyBub3QgYW55IHZhbGlkIGRhdGE=";
char JWT_AUTH_TOKEN[1000] = "eW91IGNhbid0IGhhY2sgb3VyIHN5c3RlbQ==";

AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler(
    "/api/token", [](AsyncWebServerRequest *request, JsonVariant &json) {
      if (request->method() != HTTP_POST ||
          request->contentType() != "application/json") {
        request->send(405, "text/plain", "Method Not Allowed");
        return;
      }

      if (!json["refresh"].isNull()) {
        String refreshToken = json["refresh"].as<String>();
        strcpy(JWT_REFRESH_TOKEN, refreshToken.c_str());
      } else {
        request->send(500, "text/plain", "No refresh token specified");
        return;
      }

      if (!json["token"].isNull()) {
        String token = json["token"].as<String>();
        strcpy(JWT_AUTH_TOKEN, token.c_str());
      } else {
        request->send(500, "text/plain", "No access token specified");
        return;
      }
      request->send(200, "application/json", "{\"status\":\"received\"}");
    });

#endif
/* POST ENDPOINT STUFF */

/* LORA ENCRYPTION STUFF */
uint8_t default_aes_key[AES_KEY_SIZE] = {"we", "cant", "tell", "you"};

uint8_t default_hmac_key[HMAC_KEY_SIZE] = {"we", "wont", "tell", "you"};

// FEATURE: this needs to be saveable in EEPROM so that the key changes are
// presistent
node_config *current_config =
    new node_config(default_aes_key, default_hmac_key);

CryptoAdapter *crypto = new CryptoAdapter(current_config);

size_t buffer_size = 2 * MAX_MESSAGE_SIZE_helpme;

send_message *in_message = (send_message *)malloc(buffer_size);
send_message *in_buff = (send_message *)malloc(buffer_size);
size_t in_msg_size;

send_message *out_message = (send_message *)malloc(buffer_size);
send_message *out_buff = (send_message *)malloc(buffer_size);
size_t out_msg_data_size;
/* COMMUNICATION STUFF */

/* SENSORS STUFF */
#ifdef SENDER
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
// DHT22 sensor
DHT dht22(5, DHT22);
// BMP280 sensor
Adafruit_BMP085 bmp;
#endif
/* SENSORS STUFF */

/* LORA STUFF */
#define SS 15
#define RST 16
#define DIO0 17
/* LORA STUFF */

void setup() {
  Serial.begin(115200);
  delay(2000);

  memset(in_message, 0, buffer_size);
  memset(in_buff, 0, buffer_size);

  memset(out_message, 0, buffer_size);
  memset(out_buff, 0, buffer_size);

#ifdef SENDER
  dht22.begin();
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    ESP.restart();
  }
#endif

  Serial.println("Initializing the LoRa Module...");
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa module failed to start!");
    Serial.println("RESTARTING.");
    ESP.restart();
  }
  Serial.println("LoRa module initialized successfully!");

#ifdef RECEIVER
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin(MDNS_NAME)) {
    Serial.println("MDNS responder started");
  }
  server.addHandler(handler);
  server.begin();
#endif
}

#ifdef SENDER
void loop() {
  delay(1000);
  static int temp, hum;
  // dht
  temp = dht22.readTemperature();
  hum = dht22.readHumidity();

  float pressure = bmp.readPressure() / 100.0;
  message_data *out_mess_data = (message_data *)out_message->data;
  out_mess_data->command = SUBDEVICES_MEASUREMENTS;
  subdevices_measurements *meas =
      (subdevices_measurements *)out_mess_data->data;

  meas->bmp.pressure = pressure;
  meas->dht.humidity = hum;
  meas->dht.temperature = temp;
  Serial.printf("temp: %d, humidity: %d, pressure: %f\n", meas->dht.temperature,
                meas->dht.humidity, pressure);
  out_msg_data_size = CryptoAdapter::toValidSize(
      sizeof(message_data) + sizeof(subdevices_measurements));
  encrypt_error err =
      crypto->encryptMessage(out_buff, out_message, out_msg_data_size);
  if (err != encrypt_error::OK) {
    Serial.println("Error encrypting message.");
    return;
  }

  LoRa.beginPacket();
  LoRa.write((const uint8_t *)out_buff, out_msg_data_size);
  LoRa.endPacket();
  return;
}
#elif RECEIVER
void loop() {
  int packetSize = LoRa.parsePacket();
  if (!packetSize)
    return;
  // received a packet
  Serial.println("Received a packet");
  transmission_quality quality = {.rssi = LoRa.packetRssi(),
                                  .snr = LoRa.packetSnr()};

  Serial.printf("rssi: %d snr: %f\n", quality.rssi, quality.snr);
  LoRa.readBytes((uint8_t *)in_buff, packetSize);

  decrypt_error err = crypto->decryptMessage(in_message, in_buff, packetSize);
  if (err != decrypt_error::OK) {
    Serial.printf("%s (errno: %d)", "Error decrypting message.", uint8_t(err));
    return;
  }
  message_data *data =
      (message_data *)in_message->data; // <- access decrypted message
  switch (data->command) {
  case DHT_MEASUREMENT: {
    // FEATURE: solo DHT endpoint
    break;
  }
  case BMP_MEASUREMENT: {
    // FEATURE: solo BMP endpoint
    break;
  }
  case SUBDEVICES_MEASUREMENTS: {
    // TODO: post request to a python endpoint -> DB entry
    subdevices_measurements *meas = (subdevices_measurements *)data->data;
    Serial.printf("temp: %d, humidity: %d, pressure: %f \n",
                  meas->dht.temperature, meas->dht.humidity,
                  meas->bmp.pressure);
    JsonDocument jsonDoc;
    jsonDoc["temperature"] = meas->dht.temperature;
    jsonDoc["humidity"] = meas->dht.humidity;
    jsonDoc["pressure"] = meas->bmp.pressure;
    jsonDoc["RSSI"] = quality.rssi;
    jsonDoc["SNR"] = quality.snr;

    String jsonPayload;
    serializeJson(jsonDoc, jsonPayload);
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected!");
      // FEATURE: cache measurements that were lost due the esp not being
      // connected to the WiFi
      return;
    }

    WiFiClientSecure client;
    HTTPClient https;

    client.setInsecure();

    https.begin(client, String("https://") + SERVER_ADDRESS + SERVER_ENDPOINT);
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Authorization", String("Bearer ") + JWT_AUTH_TOKEN);

    int httpResponseCode = https.POST(jsonPayload);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    switch (httpResponseCode) {
    case (201): {
      // all good
      break;
    }
    case (401): {
      // token expired
      // "code":"token_not_valid"
      JsonDocument rep;
      break;
    }
    }
    if (httpResponseCode > 0) {
      String response = https.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.println("POST failed.");
    }

    https.end();
    break;
  }
  case KEY_EXCHANGE_SEND: {
    // FEATURE: ignore for now
    break;
  }
  case KEY_EXCHANGE_ACCEPT: {
    // FEATURE: ignore for now
    break;
  }
  default: {
    Serial.printf("Unexpected message type: %s\n",
                  get_enum_name_helpme(data->command));
    break;
  }
  }
}
#endif
