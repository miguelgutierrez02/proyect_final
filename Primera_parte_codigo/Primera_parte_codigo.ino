# include <WiFi.h>
# include <PubSubClient.h>
# include <ESP32_MailClient.h>
# include <ESP32Servo.h>
# include <DHT.h>
# include <FirebaseESP32.h>
# include "ThingSpeak.h"



# define DHTPIN 4
# define DHTTYPE DHT11
# define servoPin 15





// *
// * CONFIG MQTT **
// *
const  char * mqtt_server = "ioticos.org" ;
const  int    mqtt_port = 1883;
const  char * mqtt_user = "jXbGbtv9uRczDJV" ;
const  char * mqtt_pass = "tE3WSWBm65fzYc4" ;

const  char * root_topic_subscribe = "Uv15cdzHn4yzDcj";
const  char * root_topic_subscribe_Temp1 = "Uv15cdzHn4yzDcj/Temp1";
const  char * root_topic_subscribe_gas1 = "Uv15cdzHn4yzDcj/gas1";

const  char * root_topic_publish = "Uv15cdzHn4yzDcj/Raiz";
const  char * root_topic_publish_Temp = "Uv15cdzHn4yzDcj/Temp";
const  char * root_topic_publish_Hum = "Uv15cdzHn4yzDcj/Hum";
const  char * root_topic_publish_gas = "Uv15cdzHn4yzDcj/gas";


//thing speak conectar
unsigned long ID = 1404256 ;
const char * WriteAPIKey = "3G69UW97X15G8AKY";

//
const char * HOST = "https://proyectomateriaiot-default-rtdb.firebaseio.com/";
const char * AUTH ="ZDIamrI1xbCeLY2PuP8UkenbKyemmRz59W4sjnZY";

//
FirebaseData firebaseData;


// *
// * WIFICONFIG *
// *
//const  char * ssid = "FAMILIA_RS" ;
//const  char * ssid = "FAMILIA ORTIZ" ;
//const  char * password = "Paquemeroba2018" ;
//const  char * password = "sergio.0822.1118" ;
//const  char * ssid = "SIN WIFI" ;
//const  char * password = "luanabra2021" ;
const  char * ssid = "FAMILIA GARCIA" ;
const  char * password = "6373067930" ;
//const  char * ssid = "MORAPIÑEROS" ;
//const char * password = "269051487" ;

//const char* ssid     = "LAURA CAMILA";
//const char* password = "0591laura";

// *
// * GLOBALES *
// *
WiFiClient espClient;
WiFiClient thing;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
SMTPData datosSMTP;
Servo miServo
;
char msg [ 30 ];
char msgG [ 60 ];
int ModificarTemp = 32;
int Modificargas = 2000;
float h;
float t;
String sensores = "/sensores";
const  int pin_sensorgas = 32;
int lect = 0;
const int m = 23;
const int led_1 = 21;
const int v = 22;
String EstadoVentilador;
