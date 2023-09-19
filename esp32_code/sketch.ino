#include <WiFi.h>
#include <PubSubClient.h>
// WiFi bağlantı bilgilerinizi girin
const char* ssid = "Wokwi-GUEST";
const char* password = "";
// MQTT broker adresini ve bağlantı bilgilerini girin
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883; //dikkat edilcek nokta bi burasi bi de yukarisi
const char* mqttUser = "hasanfiratkeskin";
const char* mqttPassword = "abdulselam12";
WiFiClient espClient;// MQTT istemcisi oluşturun
PubSubClient client(espClient);
const char* topic = "ilk_mesaj";// MQTT iletişim için konu (topic)
const int potPin = 34; // pot pin numarası
const int ledPin = 26; // led pin numarası

void setup() {
  // Seri iletişimi başlatın
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); //pin output olarak ayarlanmıştır.
  analogReadResolution(12); //ADC'den analog sinyal 12 bit olarak seçilmiştir.
  // WiFi ağına bağlanın
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi bağlanıyor...");
  }
  Serial.println("WiFi bağlandı");
  // MQTT istemcisini ayarlayın
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  // MQTT sunucusuna bağlanın
  while (!client.connected()) {
    Serial.println("MQTT sunucusuna bağlanılıyor...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("MQTT sunucusuna bağlandı");
    } else {
      Serial.print("Hata, MQTT sunucusuna bağlanılamadı, rc=");
      Serial.print(client.state());
      Serial.println(" 5 saniye sonra tekrar denenecek...");
      delay(5000);
    }
  }
  client.subscribe(topic);// Abone olduğumuz MQTT konusunu belirtin
}

void loop() {
  // MQTT istemcisini sürekli olarak kontrol edin
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
    // MQTT konusuna bir mesaj gönderin

  int potValue = analogRead(potPin); // ADC değeri okunur ve potValue'ye atanır.
  Serial.print("Potansiyometre Değeri: "); //serial ekranına yazdırılır.
  Serial.println(potValue);
  digitalWrite(ledPin, HIGH); //ledi yakar
  delay(potValue/4); //  potdeğeri/4 saniye boyunca LED'i açık tutun
  digitalWrite(ledPin, LOW); //ledi söndürür
  delay(potValue/4); // potdeğeri/4 saniye boyunca LED'i kapalı tutun
  char message[50];
  //string veri = "string(potValue)";
  snprintf(message, 50, "ahmed mahsun, bu bir test mesajidir.", millis());
  client.publish(topic, message);
  Serial.print("mesaj başarı ile gönderildi");
  // Mesaj gönderdikten sonra bir süre bekleme yapın

  delay(5000);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Yeni bir mesaj alındı: [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("MQTT sunucusuna tekrar bağlanılıyor...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("MQTT sunucusuna bağlandı");
      client.subscribe(topic);
    } else {
      Serial.print("Hata, MQTT sunucusuna tekrar bağlanılamadı, rc=");
      Serial.print(client.state());
      Serial.println(" 5 saniye sonra tekrar denenecek...");
      delay(5000);
    }
  }
}