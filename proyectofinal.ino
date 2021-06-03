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
//const  char * password = "sergio.0822.1118" ;
const  char * ssid = "SIN WIFI" ;
const  char * password = "luanabra2021" ;

//02222222254const  char * ssid = "MORAPIÑEROS" ;  
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
String Estadocasa;


// **
// * FUNCIONES **
// **

void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void setup_wifi();
void sensor();
void moditemp();
void modigas();
void Correobomberos();
void sensorgas();

void setup() {
  Serial.begin(115200);

  dht.begin();

  miServo.attach(servoPin); //Conecta la variable miServo a un pin(15).
  miServo.write(0);
  
 
  pinMode(v, OUTPUT); 
  digitalWrite(v, LOW);
  
  
  pinMode(led_1, OUTPUT); 
  //digitalWrite(led_1, LOW);

  setup_wifi();
   Firebase.begin(HOST, AUTH);
   Firebase.reconnectWiFi(true);
   client.setServer(mqtt_server, mqtt_port);
   client.setCallback(callback);
   ThingSpeak.begin(thing);
}

void loop() {
  
  if(!client.connected()) {
   reconnect();
  }

  if (client.connected()){
    sensor();
    moditemp();
    sensorgas();
    modigas();
 
    String General = "Temperatura = " + String(t) + "°C" + " Humedad = " + String(h)  + "sensorgas =" + String(lect);
    General.toCharArray(msgG,60);
    client.publish(root_topic_publish,msgG);
    //count++;
    delay(200);
    
    String strT = "La temperatura es -> " + String(t) + "°C";
    strT.toCharArray(msg,30);
    client.publish(root_topic_publish_Temp,msg);
    //count++;
    delay(200);

    String strH = "La humedad es -> " + String(h);
    strH.toCharArray(msgG ,30);
    client.publish(root_topic_publish_Hum,msg);
    //count++;
    delay(200);


   
  
    String strlect = "el gas es -> " + String(lect);
    strlect.toCharArray(msg,30);
    client.publish(root_topic_publish_gas,msg);
    //count++;
    delay(200);


   
  }
  client.loop();
}

//*
//*    CONEXION WIFI      *
//*

void setup_wifi(){
  delay(10);
  // Nos conectamos a nuestra red Wifi
  Serial.println();
  Serial.print("Conectando a ssid: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado a red WiFi!");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}



//*
//*    CONEXION MQTT      *
//*

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión Mqtt...");
    // Creamos un cliente ID
    String clientId = "IOTICOS_H_W_";
    clientId += String(random(0xffff), HEX);
    // Intentamos conectar
    if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {
      Serial.println("Conectado!");
      // Nos suscribimos
      if(client.subscribe(root_topic_subscribe)){
        Serial.println("Suscripcion General");
      }else{
        Serial.println("Fallo Suscripción General");
      }
      if(client.subscribe(root_topic_subscribe_Temp1)){
      Serial.println("Suscripcion a Temp");
      }else{
      Serial.println("Fallo Suscripción a Temp");
      }
      if(client.subscribe(root_topic_subscribe_gas1)){
      Serial.println("Suscripcion a gas");
      }else{
      Serial.println("Fallo Suscripción a gas");
      }
       
    } else {
      Serial.print("Falló :( con error -> ");
      Serial.print(client.state());
      Serial.println(" Intentamos de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

//*
//*       CALLBACK        *
//*
void callback(char* topic, byte* payload, unsigned int length){
  String incoming = "";
  Serial.print("Mensaje recibido desde -> ");
  Serial.print(topic);
  Serial.println("");
  if(String(topic) == "Uv15cdzHn4yzDcj/Temp1"){
    Serial.println("TEMP1");
    for (int i = 0; i < length; i++) {
      incoming += (char)payload[i];
      }
        incoming.trim();
        Serial.println("Mensaje -> " + incoming);
        String cad = incoming.substring(0,2);
        Serial.println(cad);
        ModificarTemp = cad.toInt();
        Serial.print("La temperatura se modificó a: ");
        Serial.println(ModificarTemp);
        Firebase.pushInt(firebaseData, sensores + "temperatura modificada", ModificarTemp);
        moditemp();
    }
     if(String(topic) == "Uv15cdzHn4yzDcj/gas1"){
    Serial.println("gas1");
    for (int i = 0; i < length; i++) {
      incoming += (char)payload[i];
      }
        incoming.trim();
        Serial.println("Mensaje -> " + incoming);
        String ca = incoming.substring(0,4);
        Serial.println(ca);
        Modificargas = ca.toInt();
        Serial.print("gas se modifico a: ");
        Serial.println(Modificargas);
        Firebase.pushInt(firebaseData, sensores + " elgas se modifico", Modificargas);
        modigas();
    }
  }  
void sensor(){
  //Serial.println(F("DHT11 test!"));
  //dht.begin();
  delay(2000); //Es un sensor lento, por lo que hay que darle tiempo.
  h = dht.readHumidity();
  t = dht.readTemperature();
  // Comprobamos si las lecturas pueden dar algún fallo mediante la función isnan()
  // Esta función devuelve un 1 en caso de que el valor no sea numérico
  // Los caracteres || son como un OR. Si se cumple una de las dos condiciones mostramos error
 if (isnan(h) || isnan(t)) {
    Serial.println(F("¡No se pudo leer el sensor DHT11!"));
  return;
  }
  else{
    Serial.print(F("Humedad: "));
    Serial.print(h);
    Serial.print("% Temperatura: ");
    Serial.print(t);
    Serial.println(F("°C "));

    //cargarlores que se envian
    ThingSpeak.setField(1, t);
    ThingSpeak.setField(2, h);

    //
    ThingSpeak.writeFields (ID, WriteAPIKey);
    Serial.println("datos enviados");

    
  }
  }

  void sensorgas (){

  lect = analogRead (pin_sensorgas);
  Serial.print("gas: ");
  Serial.println(lect);
  delay (50);
   ThingSpeak.setField(3, lect);
  

    //
   ThingSpeak.writeFields (ID, WriteAPIKey);
   Serial.println("datos enviados");
    
}
void modigas (){
  if( lect <= Modificargas){
    digitalWrite(led_1, LOW);
    Serial.println("alarma:off");
    Serial.println("Puerta off");
      miServo.write(0);
    
    }
    else{
     digitalWrite(led_1, HIGH);
     Estadocasa = "incendio,direccion : cll";
     Correobomberos();
     Serial.println("alarma_on");
     Serial.println("Puerta on");
     miServo.write(130);
     
      }    
}
void moditemp (){
  if(t <= ModificarTemp){
    digitalWrite(v, LOW);
    Serial.println("Ventilador apagado");
    }
    else{
      digitalWrite(v, HIGH);
      Serial.println("Ventilador encendido");
      }
}


void Correobomberos(){
      //Configuración del servidor de correo electrónico SMTP, host, puerto, cuenta y contraseña
  datosSMTP.setLogin("smtp.gmail.com", 465, "iotprueba05@gmail.com", "1118571597");
    // Establecer el nombre del remitente y el correo electrónico
  datosSMTP.setSender("ESP32", "iotprueba05@gmail.com");
  // Establezca la prioridad o importancia del correo electrónico High, Normal, Low o 1 a 5 (1 es el más alto)
  datosSMTP.setPriority("High");
  // Establecer el asunto
  datosSMTP.setSubject("ESTADO DE lA CASA");
  // Establece el mensaje de correo electrónico en formato de texto (sin formato)
  datosSMTP.setMessage(Estadocasa, false);
  // Agregar destinatarios, se puede agregar más de un destinatario
  datosSMTP.addRecipient("iotprueba05@gmail.com");
  //Comience a enviar correo electrónico.
  if (!MailClient.sendMail(datosSMTP))
  Serial.println("Error enviando el correo, " + MailClient.smtpErrorReason());
  //Borrar todos los datos del objeto datosSMTP para liberar memoria
  datosSMTP.empty();
  delay(100);
  Serial.println("¡Correo Enviado!");
  }
