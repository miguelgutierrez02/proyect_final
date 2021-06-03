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
