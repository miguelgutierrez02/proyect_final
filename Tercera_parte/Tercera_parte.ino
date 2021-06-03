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
