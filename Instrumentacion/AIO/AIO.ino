#include <DHT.h> // Librería para el sensor DHT

#define DHTTYPE DHT11
#define DHTPIN 15      // Cambia a un pin digital disponible en ESP32
DHT dht(DHTPIN, DHTTYPE);

// --- Sensor de Nivel de Agua ---
#define alimentacionSensor 2
#define lectura 34 // Pin analógico para ESP32 (GPIO34 es entrada ADC)

int nivelLiquido = 0;
int limiteInferior = 47;
int limiteSuperior = 130;

byte ledRojo     = 27;
byte ledAmarillo = 26;
byte ledVerde    = 25;

// --- Sensor de pH ---
#define LECTURA_PH 35 // Pin analógico ADC para pH
int buffer_arr[10];
int temp;
long avgval;
float calibration_value = 21.34;

void setup() {
  Serial.begin(115200);
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

  if (nivel <= 100) { // Ajustar según la nueva escala ADC (0-4095)
    Serial.println("Nivel de agua: Vacío");
    digitalWrite(ledRojo, LOW); digitalWrite(ledAmarillo, LOW); digitalWrite(ledVerde, LOW);
  } else if (nivel > 100 && nivel <= limiteInferior*4) {
    Serial.println("Nivel de agua: Bajo");
    digitalWrite(ledRojo, HIGH); digitalWrite(ledAmarillo, LOW); digitalWrite(ledVerde, LOW);
  } else if (nivel > limiteInferior*4 && nivel <= limiteSuperior*4) {
    Serial.println("Nivel de agua: Medio");
    digitalWrite(ledRojo, LOW); digitalWrite(ledAmarillo, HIGH); digitalWrite(ledVerde, LOW);
  } else if (nivel > limiteSuperior*4) {
    Serial.println("Nivel de agua: Alto");
    digitalWrite(ledRojo, LOW); digitalWrite(ledAmarillo, LOW); digitalWrite(ledVerde, HIGH);
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
  float volt = (float)avgval * 5.0 / 4095 / 6; // ADC 12 bits
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
