#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "dlb";
const char* password = "www.dlb.one";

String serverName = "https://dlb.com.pl/api.php";
//finderprint valid to 20 July 2023
const uint8_t fingerprint[20] = {0x13, 0xA2, 0x47, 0xA0, 0x3F, 0x5E, 0x3F, 0xFA, 0x97, 0x5D, 0xF4, 0x00, 0xAB, 0xC9, 0xC8, 0x50, 0x24, 0x12, 0x8D, 0x65};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("*");
  }

  Serial.print("Connected ! IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED){
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);
    HTTPClient https;

    Serial.print("[HTTPS] begin ... \n");

    String serverPath = "user_name=Test&port=3";

    https.begin(*client, serverName.c_str());
    https.setTimeout(2000);
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");

    Serial.println("[HTTPS] method POST ---> ");

    int httpsRespCode = https.POST(serverPath.c_str());

    if(httpsRespCode > 0) {
      Serial.printf("[HTTPS] response from method POST --> %d\n",httpsRespCode);

      if(httpsRespCode == HTTP_CODE_OK) {
          const String& buff = https.getString();
          Serial.println("recived buffor ---> ");
          Serial.print(buff);
          Serial.println("-->");
      }
    }
    else {
      Serial.printf("[HTTPS] POST .... failed, error: %s\n", https.errorToString(httpsRespCode).c_str());
    }
    https.end();
    }
    else{
      Serial.println("WiFi Disconnected"); 
    }
    delay(5000);
}
