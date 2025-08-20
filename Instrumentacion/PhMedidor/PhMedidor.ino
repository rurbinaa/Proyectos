int buffer_arr[10];
int temp;
long avgval;
float calibration_value = 21.34; // Ajustar según tu calibración

void setup() {
  Serial.begin(9600);  // Inicializa comunicación serial
  Serial.println("Bienvenido a Medidor pH");
  delay(2000);
}

void loop() {
  // Lectura de 10 valores
  for (int i = 0; i < 10; i++) {
    buffer_arr[i] = analogRead(A0);
    delay(30);
  }

  // Ordenamiento de valores
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buffer_arr[i] > buffer_arr[j]) {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }

  // Promedio (descartando extremos)
  avgval = 0;
  for (int i = 2; i < 8; i++) {
    avgval += buffer_arr[i];
  }

  // Conversión a voltaje y pH
  float volt = (float)avgval * 5.0 / 1024 / 6;
  float ph_act = -5.70 * volt + calibration_value;

  // Mostrar en el monitor serie
  Serial.print("Voltaje: ");
  Serial.print(volt);
  Serial.print(" V  |  pH: ");
  Serial.println(ph_act);

  delay(200);
}