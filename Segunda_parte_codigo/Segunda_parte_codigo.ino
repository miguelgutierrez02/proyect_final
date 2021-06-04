// **
// * FUNCIONES **
// **

void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void setup_wifi();
void sensor();
void moditemp();
void modigas();
void CorreoPuertaAbierta();
void CorreoVentiladorOn();
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
SEGUNDA PARTE
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
