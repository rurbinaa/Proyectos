#include <DHT.h> // Librería para el sensor DHT

// --- Sensor de Temperatura y Humedad ---
#define DHTTYPE DHT11
#define DHTPIN 9       // Pin digital para el DHT11 en Arduino Uno
DHT dht(DHTPIN, DHTTYPE);

// --- Sensor de Nivel de Agua ---
#define alimentacionSensor 12
#define lectura A1     // Pin analógico para el sensor de nivel

int nivelLiquido = 0;
// Límites originales para la escala del Arduino Uno (0-1023)
int limiteInferior = 47;
int limiteSuperior = 130;

byte ledRojo     = 11;
byte ledAmarillo = 10;
byte ledVerde    = 8; // Cambiado al pin 8 para no usar el mismo que el DHT

// --- Sensor de pH ---
#define LECTURA_PH A0  // Pin analógico para el sensor de pH
int buffer_arr[10];
int temp;
long avgval;
float calibration_value = 21.34;

void setup() {
  Serial.begin(9600); // Se ajusta la velocidad a 9600, más común para Uno
  Serial.println("Inicializando sistemas...");

  // --- Configuración del sensor DHT11 ---
  dht.begin();

  // --- Configuración del sensor de nivel de agua ---
  pinMode(alimentacionSensor, OUTPUT);
  digitalWrite(alimentacionSensor, LOW);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledVerde, OUTPUT);

  Serial.println("Bienvenido a Medidor pH");
  delay(2000);
}

void loop() {
  // --- Lógica del Sensor de Nivel de Agua ---
  int nivel = leerSensorNivel();
  
  // Se ajusta la lógica a los valores originales para Arduino Uno
  if (nivel <= limiteInferior) {
    Serial.println("Nivel de agua: Bajo");
    digitalWrite(ledRojo, HIGH); 
    digitalWrite(ledAmarillo, LOW); 
    digitalWrite(ledVerde, LOW);
  } else if (nivel > limiteInferior && nivel <= limiteSuperior) {
    Serial.println("Nivel de agua: Medio");
    digitalWrite(ledRojo, LOW); 
    digitalWrite(ledAmarillo, HIGH); 
    digitalWrite(ledVerde, LOW);
  } else if (nivel > limiteSuperior) {
    Serial.println("Nivel de agua: Alto");
    digitalWrite(ledRojo, LOW); 
    digitalWrite(ledAmarillo, LOW); 
    digitalWrite(ledVerde, HIGH);
  }
  Serial.print("Valor Nivel: ");
  Serial.println(nivel);
  Serial.println("--------------------");
  delay(1000);

  // --- Lógica del Sensor de pH ---
  for (int i = 0; i < 10; i++) { 
    buffer_arr[i] = analogRead(LECTURA_PH);
    delay(30);
  }
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buffer_arr[i] > buffer_arr[j]) {
        temp = buffer_arr[i]; buffer_arr[i] = buffer_arr[j]; buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++) {
    avgval += buffer_arr[i];
  }
  
  // **Cálculo ajustado para el ADC de 10 bits del Arduino Uno (1023)**
  float volt = (float)avgval * 5.0 / 1023 / 6; 
  float ph_act = -5.70 * volt + calibration_value;
  
  Serial.print("pH Voltaje: ");
  Serial.print(volt, 3);
  Serial.print(" V  |  pH: ");
  Serial.println(ph_act, 2);
  Serial.println("--------------------");
  delay(1000);

  // --- Lógica del DHT11 ---
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("ERROR EN EL SENSOR DHT11");
  } else {
    Serial.print("Humedad: ");
    Serial.print(humidity);
    Serial.print("%  |  Temperatura: ");
    Serial.print(temperature);
    Serial.println(" C");
  }
  Serial.println("====================");
  delay(1000);
}

// --- Función para leer el sensor de nivel de agua ---
int leerSensorNivel() {
  digitalWrite(alimentacionSensor, HIGH);
  delay(50);
  nivelLiquido = analogRead(lectura);
  digitalWrite(alimentacionSensor, LOW);
  return nivelLiquido;
}