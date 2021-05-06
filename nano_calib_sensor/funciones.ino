//-----------Led-RGB----------------------
void rgb(char color) {
  switch (color) {
    case 'r':
      digitalWrite(pinLedR, 0);
      digitalWrite(pinLedG, 255);
      digitalWrite(pinLedB, 255);
      break;
    case 'g':
      digitalWrite(pinLedR, 255);
      digitalWrite(pinLedG, 0);
      digitalWrite(pinLedB, 255);
      break;
    case 'y':
      digitalWrite(pinLedR, 0);
      digitalWrite(pinLedG, 0);
      digitalWrite(pinLedB, 255);
      break;
    case 'b':
      digitalWrite(pinLedR, 255);
      digitalWrite(pinLedG, 255);
      digitalWrite(pinLedB, 0);
      break;
    case 'a':
      digitalWrite(pinLedR, 255);
      digitalWrite(pinLedG, 255);
      digitalWrite(pinLedB, 255);
      break;
  }
}
//-----------------Alarma----------------------------
void alarma(int veces, int duracionNota, char color) {
  rgb(color);
  for(int i=0; i<veces; i++) {
    tone(pinBuzzer, NOTE_C7, duracionNota);   // Hago sonar el buzzer, la nota c7 es la que más fuerte suena
    delay(duracionNota);                      // Espero lo que dura la nota
    noTone(pinBuzzer);                        // Silencio el buzzer
    delay(duracionNota);                      // Delay entre alarmas
  }
}
//-----------------Alarma----------------------------
void alarmaCO2(int veces, int duracionNota) {
  for(int i=0; i<veces; i++) {
    if(i<veces-1) {
      rgb('r');
      tone(pinBuzzer, NOTE_C7, duracionNota);   // Hago sonar el buzzer, la nota c7 es la que más fuerte suena
      delay(duracionNota);                      // Espero lo que dura la nota
      noTone(pinBuzzer);                        // Silencio el buzzer
      rgb('a');
      delay(duracionNota);                      // Delay entre alarmas
    }
    else {
      rgb('r');
      tone(pinBuzzer, NOTE_C7, duracionNota);   // Hago sonar el buzzer, la nota c7 es la que más fuerte suena
      delay(duracionNota);                      // Espero lo que dura la nota
      noTone(pinBuzzer);                        // Silencio el buzzer
      delay(duracionNota);                      // Delay entre alarmas
    }
  }
}
// Creacion del simbolo de numeral para ser utilizado en el display i2c
byte numeral[] = {
  B11100,
  B10100,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
//-----------Print-por-display----------------------
void displayPrint(int posicion, int linea, String texto) {
  display.setCursor(posicion, linea);       // Ubicamos el cursor en la posicion y linea deseada
  display.print(texto);                     // Escribe primera linea del cartel
}
void imprimirCO2(int co2ppm) {
  // Print por serial  
  Serial.print("CO2: " + String(co2ppm) + "ppm \n");  // Escribe CO2
  // Print por display
  // Muestra medición de CO2  
  //display.clear();                                  // Borra pantalla
  displayPrint(0, 1, "          ");                   // Simula un clear solo del renglon inferior
  // Print CO2
  displayPrint(0, 1, "CO2: " + String(co2ppm) + "ppm");   
  logoUNAHUR();
}
void logoUNAHUR() {
  byte UNAHUR1[] = {
    B11100,
    B11110,
    B11111,
    B11111,
    B11111,
    B11111,
    B01111,
    B00111
  };
  byte UNAHUR2[] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
  };
  byte UNAHUR3[] = {
    B00111,
    B01111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11110,
    B11100
  };
  display.createChar(0, UNAHUR1);
  display.createChar(1, UNAHUR2);
  display.createChar(2, UNAHUR3);
  display.setCursor(13, 0);       // Ubicamos el cursor en la posicion y linea deseada
  display.write(0);
  display.setCursor(14, 0);       // Ubicamos el cursor en la posicion y linea deseada
  display.write(1);
  display.setCursor(15, 0);       // Ubicamos el cursor en la posicion y linea deseada
  display.write(2);
  display.setCursor(13, 1);       // Ubicamos el cursor en la posicion y linea deseada
  display.write(2);
  display.setCursor(14, 1);       // Ubicamos el cursor en la posicion y linea deseada
  display.write(1);
  display.setCursor(15, 1);       // Ubicamos el cursor en la posicion y linea deseada
  display.write(0);
}
//-----------Calibración----------------------
void calibrar()
{
  const long segundosEspera = 1800;      // Cantidad de segundos a esperar
  long segundosPasados = 0;              // Cuenta segundos
  // Print por serial
  Serial.print("COMIENZA CALIBRACION \n"); // Notificamos que empezó el proceso de calibración
  // Print por display
  display.clear();
  displayPrint(0, 0, "COMIENZA");
  displayPrint(0, 1, "CALIBRACION");
  delay(10000); // Espera 10 segundos
 
  while(segundosPasados <= segundosEspera) {                      // Espera media hora
    if (++segundosPasados % 60 == 0) {                            // Si los segundos pasados son múltiplo de 60
    // Print por serial
      Serial.print(String(segundosPasados / 60) + " minutos \n"); // Cada minuto muestra el valor 
    }
    // Print por pantalla
    display.clear();                                              // Borra pantalla  
    displayPrint(0, 0, "CALIBRANDO");
    displayPrint(0, 1, String(segundosPasados / 60) + " minutos");// Escribimos los minutos pasados
    delay(1000); // Espera 1 segundo
    }
  sensor.calibrateZero();               // Calibra
  // Print por serial
  Serial.print("PRIMERA CALIBRACION \n");
  // Print por display
  display.clear();                      // Limpio pantalla
  displayPrint(0, 0, "PRIMERA");       
  displayPrint(0, 1, "CALIBRACION");     
  alarma(1, 250, 'b');                       // Avisamos que terminó la primera calibración
  delay(60000);                         // Espera 1 minuto
  sensor.calibrateZero();               // Calibra por segunda vez por las dudas
  // Print por serial
  Serial.print("SEGUNDA CALIBRACION \n");
  // Print por display
  display.clear();                      // Limpio pantalla     
  displayPrint(0, 0, "SEGUNDA");        
  displayPrint(0, 1, "CALIBRACION");    
  alarma(1, 250, 'b');                       // Avisamos que terminó la segunda calibración
  delay(10000); // Espera 10 segundos
  // Print por serial
  Serial.print("CALIBRACION TERMINADA \n");
  // Print por display
  display.clear(); // borra pantalla  
  displayPrint(0, 0, "CALIBRACION");       
  displayPrint(0, 1, "TERMINADA");    
  alarma(5, 250, 'g');                       // Avisamos que finalizó el proceso de calibración entero
  delay(10000); // Espera 10 segundos 
}

//------------Scrolling-Text-----------------------
#define STR_LEN 12                                                      // Largo del texto
char str_to_print[STR_LEN]={'A','i','r','e',' ','N','u','e','v','o'};   // String separado en caracteres

void scrollingText(uint8_t scrolled_by) {
  for (uint8_t i=0;i<11;i++) {
    display.setCursor(i,0);
    if (scrolled_by>=11) scrolled_by=0;
    if (scrolled_by<10) display.print(str_to_print[scrolled_by]);
    else display.print(' ');
    scrolled_by++;
  }
}

void aireNuevo() {
  for (uint8_t i=0;i<STR_LEN;i++) {
    scrollingText(i);
    delay(500);
  }
}
