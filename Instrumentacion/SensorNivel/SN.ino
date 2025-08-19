// Prof. Maffucci Michele
// Fecha: 09/03/2022
// Detección de nivel de líquido - v02

// Pin del sensor
#define alimentacionSensor 12
#define lectura A0

// Variable para almacenar el nivel del agua
int nivelLiquido = 0;

// Límite inferior y superior obtenidos en función de la calibración
int limiteInferior = 47;
int limiteSuperior = 130;

// Declaración de pines donde conectaremos los LEDs
byte ledRojo   = 11;
byte ledAmarillo = 10;
byte ledVerde  = 9;

void setup() {
  // Configuramos el pin digital 12 como SALIDA
  pinMode(alimentacionSensor, OUTPUT);
  
  // Establecemos en LOW el +Vcc del sensor
  digitalWrite(alimentacionSensor, LOW);

  // Configuramos los pines a los que conectamos los LEDs como SALIDA
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledVerde, OUTPUT);

  // Configuramos la velocidad del puerto serial
  Serial.begin(9600);
}

void loop() {
  // Leemos el valor del sensor
  int nivel = leerSensor();

  if (nivel == 0) {
    
    Serial.println("Nivel de agua: Vacío");
    Serial.print("Valor leído: ");
    Serial.println(nivel);
    Serial.println("--------------------");
    
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledAmarillo, LOW);
    digitalWrite(ledVerde, LOW);
  }
  else if (nivel > 0 && nivel <= limiteInferior) {
    Serial.println("Nivel de agua: Bajo");
    Serial.print("Valor leído: ");
    Serial.println(nivel);
    Serial.println("--------------------");
    
    digitalWrite(ledRojo, HIGH);
    digitalWrite(ledAmarillo, LOW);
    digitalWrite(ledVerde, LOW);
  }
  else if (nivel > limiteInferior && nivel <= limiteSuperior) {
    Serial.println("Nivel de agua: Medio");
    Serial.print("Valor leído: ");
    Serial.println(nivel);
    Serial.println("--------------------");
    
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledAmarillo, HIGH);
    digitalWrite(ledVerde, LOW);
  }
  else if (nivel > limiteSuperior) {
    Serial.println("Nivel de agua: Alto");
    Serial.print("Valor leído: ");
    Serial.println(nivel);
    Serial.println("--------------------");
    
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledAmarillo, LOW);
    digitalWrite(ledVerde, HIGH);
  }
  delay(1000);
}

// Función que permite la lectura del nivel de agua
int leerSensor() {
  digitalWrite(alimentacionSensor, HIGH);  // Activa el sensor - se suministra alimentación
  delay(50);                               // Espera breve de 50 ms antes de realizar la lectura
  nivelLiquido = analogRead(lectura);      // Lee el valor y lo almacena en la variable nivelLiquido
  digitalWrite(alimentacionSensor, LOW);   // Quitamos alimentación al sensor
  return nivelLiquido;                     // Envía el valor leído a la función que lo llamó
}
