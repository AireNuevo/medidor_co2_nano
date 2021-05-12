//-----------Led-RGB----------------------
void rgb(char color) {
  // Switch funciona solo con int o char
  // Como el led es anodo comun, pasamos los valos de manera inversa
  switch (color) {
    case 'r': //red
      digitalWrite(pinLedR, LOW);
      digitalWrite(pinLedG, HIGH);
      digitalWrite(pinLedB, HIGH);
      break;
    case 'g': //green
      digitalWrite(pinLedR, HIGH);
      digitalWrite(pinLedG, LOW);
      digitalWrite(pinLedB, HIGH);
      break;
    case 'y': //yellow
      digitalWrite(pinLedR, LOW);
      digitalWrite(pinLedG, LOW);
      digitalWrite(pinLedB, HIGH);
      break;
    case 'b': //blue
      digitalWrite(pinLedR, HIGH);
      digitalWrite(pinLedG, HIGH);
      digitalWrite(pinLedB, LOW);
      break;
    case 'a': //apagado
      digitalWrite(pinLedR, HIGH);
      digitalWrite(pinLedG, HIGH);
      digitalWrite(pinLedB, HIGH);
      break;
  }
}
//-----------------Alarma----------------------------
/* La idea de esta alarma es solo hacer sonar el buzzer la cantidad de veces deseada
 * y que el led solo cambie de color, sin titilar. 
 */
void alarma(int veces, int duracionNota, char color) {
  rgb(color);                                 // Pongo el rgb en el color deseado
  for(int i=0; i<veces; i++) {
    tone(pinBuzzer, NOTE_C7, duracionNota);   // Hago sonar el buzzer, la nota c7 es la que más fuerte suena
    delay(duracionNota);                      // Espero lo que dura la nota
    noTone(pinBuzzer);                        // Silencio el buzzer
    delay(duracionNota);                      // Delay entre alarmas
  }
}
//-----------------AlarmaCO2----------------------------
/* Esta alarma se utiliza para las mediciones de CO2, en vez de prender el led al inicio
 * Lo prende al hacer sonar el buzzer y lo apaga después de cada pítido.
 * En el último pítido, deja el led prendido de color rojo.
 */
void alarmaCO2(int veces, int duracionNota) {
  for(int i=0; i<veces; i++) {
    if(i<veces-1) {
      rgb('r');                                 // Prendo el led
      tone(pinBuzzer, NOTE_C7, duracionNota);   // Hago sonar el buzzer, la nota c7 es la que más fuerte suena
      delay(duracionNota);                      // Espero lo que dura la nota
      noTone(pinBuzzer);                        // Silencio el buzzer
      rgb('a');                                 // Apago el led
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

//-----------Print-por-display----------------------
/* Esta función nos reduce en la mitad la cantidad de líneas que necesitamos para 
 *  escribir en el display.
*/
void displayPrint(int posicion, int linea, String texto) {
  display.setCursor(posicion, linea);       // Ubicamos el cursor en la posicion y linea deseada
  display.print(texto);                     // Escribe primera linea del cartel
}
void imprimirCO2(int co2ppm) {
  // Print por serial  
  Serial.print("CO2: " + String(co2ppm) + "ppm \n");      // Escribe CO2
  // Print por display
  displayPrint(0, 1, "          ");                       // Simula un clear solo del renglon inferior
  // Print CO2
  displayPrint(0, 1, "CO2: " + String(co2ppm) + "ppm");   // Escribe CO2
  logoUNAHUR();                                           // Dibujamos el logo de la UNAHUR
} 
void logoUNAHUR() {
  // Primero creamos todos los caracteres necesarios para dibujar el logo
  byte UNAHUR1[] = { // Este caracter es el utilizado en la esquina superior izquierda y en la esquina inferior derecha
    B11100,
    B11110,
    B11111,
    B11111,
    B11111,
    B11111,
    B01111,
    B00111
  };
  byte UNAHUR2[] = { // Este caracter es utilizado en el medio arriba y abajo
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
  };
  byte UNAHUR3[] = { // Este caracter es el utilizado en la esquina superior derecha y en la esquina inferior izquierda
    B00111,
    B01111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11110,
    B11100
  };
  display.createChar(0, UNAHUR1); // Agregamos los caracteres al display
  display.createChar(1, UNAHUR2); // Solo podemos agregar 8 caracteres, del 0 al 7
  display.createChar(2, UNAHUR3); // Con estos numeros llamamos al caracter
  display.setCursor(13, 0);       // Ubicamos el cursor en la posicion y linea deseada
  display.write(0);               // Dibujamos el caracter deseado
  display.setCursor(14, 0);      
  display.write(1);
  display.setCursor(15, 0);       
  display.write(2);
  display.setCursor(13, 1);       // Bajamos de linea y repetimos
  display.write(2);
  display.setCursor(14, 1);      
  display.write(1);
  display.setCursor(15, 1);       
  display.write(0);
}
//-----------Calibración----------------------
void calibrar()
{
  const long segundosEspera = 1800;         // Cantidad de segundos a esperar, son 30 minutos pero en segundos para mayor comodidad
  long segundosPasados = 0;                 // Cuenta segundos pasados
  // Print por serial
  Serial.print("COMIENZA CALIBRACION \n");  // Notificamos que empezó el proceso de calibración
  // Print por display
  display.clear();                          // Limpiamos pantalla e imprimimos el mensaje por display
  displayPrint(0, 0, "COMIENZA");
  displayPrint(0, 1, "CALIBRACION");
  delay(10000);                             // Espera 10 segundos
 
  while(segundosPasados <= segundosEspera) {                      // Espera media hora
    if (++segundosPasados % 60 == 0) {                            // Si los segundos pasados son múltiplo de 60
    // Print por serial                                           // Imprimimos por serial para no llegar el monitor de mensajes innecesarios
      Serial.print(String(segundosPasados / 60) + " minutos \n");  
    }
    // Print por pantalla
    display.clear();                                              // Borra pantalla  
    displayPrint(0, 0, "CALIBRANDO");                             // Notificamos que se sigue calibrando
    displayPrint(0, 1, String(segundosPasados / 60) + " minutos");// Escribimos los minutos pasados
    delay(1000); // Espera 1 segundo
    }
  sensor.calibrateZero();                     // Ejecuta la primer calibración
  // Print por serial
  Serial.print("PRIMERA CALIBRACION \n");     // Avisamos por serial que ya se ejecuto la primer calibración
  // Print por display
  display.clear();                            // Limpio pantalla
  displayPrint(0, 0, "PRIMERA");              // Avisamos por display que ya se ejecutó la primer calibración
  displayPrint(0, 1, "CALIBRACION");     
  alarma(1, 250, 'b');                        // Avisamos que terminó la primera calibración y mantenemos el led en azul
  delay(60000);                               // Espera 1 minuto
  sensor.calibrateZero();                     // Calibra por segunda vez por las dudas
  // Print por serial
  Serial.print("SEGUNDA CALIBRACION \n");
  // Print por display
  display.clear();                            // Limpio pantalla     
  displayPrint(0, 0, "SEGUNDA");        
  displayPrint(0, 1, "CALIBRACION");    
  alarma(1, 250, 'b');                        // Avisamos que terminó la segunda calibración y mantenemos el led en azul
  delay(10000); // Espera 10 segundos
  // Print por serial
  Serial.print("CALIBRACION TERMINADA \n");
  // Print por display
  display.clear();                            // Borra pantalla  
  displayPrint(0, 0, "CALIBRACION");       
  displayPrint(0, 1, "TERMINADA");    
  alarma(5, 250, 'g');                        // Avisamos que finalizó el proceso de calibración entero y ponemos el led en verde para mostrar que vuelve a su funcionamiento normal
  delay(10000); // Espera 10 segundos 
}

//------------Scrolling-Text-----------------------
#define STR_LEN 12                                                      // Largo del texto
char str_to_print[STR_LEN]={'A','i','r','e',' ','N','u','e','v','o'};   // String separado en caracteres

void scrollingText(uint8_t scrolled_by) {
  for (uint8_t i=0;i<11;i++) {
    display.setCursor(i,0);
    if (scrolled_by>=11) scrolled_by=0;                                 // Si el indice es mayor que el array de caracteres y ya imprimió el espacio vacío, vuelve al inicio
    if (scrolled_by<10) display.print(str_to_print[scrolled_by]);       // Si el indice está dentro del array de caracteres, imprime el caracter
    else display.print(' ');                                            // Si el indice es mayor que el array de caracteres, imprime un espacio vacio para que el mensaje no se vea como "Aire NuevoAire Nuevo"
    scrolled_by++;
  }
}

void aireNuevo() {
  for (uint8_t i=0;i<STR_LEN;i++) {
    scrollingText(i);
    delay(500);
  }
}
