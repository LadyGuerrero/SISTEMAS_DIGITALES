// ==========================================================
// PROYECTO: Sistema Inteligente de Monitoreo Ambiental
// ASIGNATURA: Sistemas Digitales - UEA
// Integrantes: Muñoz Shiguango Flor Divina
//              Erminia Natividad Galeas Rodríguez
//              Lady Melida Guerrero Quiñonez
//              Jesenia Marisol Montalvan Ochoa
//              Lady Andrea Alejandro Alejandro
// Docente: Ing. Linda Aguilar Moncayo, Mgs
// Período lectivo: 2026-2026
// ==========================================================
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define LED_VERDE 4
#define LED_AMARILLO 5
#define LED_ROJO 6
#define BUZZER 7
#define LDR_PIN A0

#define TEMP_ADVERTENCIA 33.0
#define TEMP_ALARMA 36.0
#define HUM_ADVERTENCIA 90.0
#define HUM_ALARMA 95.0
#define LUZ_BAJA 300

#define ESTADO_INICIAL 0
#define ESTADO_MONITOREO 1
#define ESTADO_ADVERTENCIA 2
#define ESTADO_ALARMA 3

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int estadoActual = ESTADO_INICIAL;

void setup() {
  Serial.begin(9600);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sistema Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  delay(2000);
  lcd.clear();
  estadoActual = ESTADO_MONITOREO;
  Serial.println("Sistema iniciado correctamente");
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int luz = analogRead(LDR_PIN);

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Error leyendo DHT11");
    return;
  }

  if (temp >= TEMP_ALARMA || hum >= HUM_ALARMA) {
    estadoActual = ESTADO_ALARMA;
  } else if (temp >= TEMP_ADVERTENCIA || hum >= HUM_ADVERTENCIA) {
    estadoActual = ESTADO_ADVERTENCIA;
  } else {
    estadoActual = ESTADO_MONITOREO;
  }

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARILLO, LOW);
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(BUZZER, LOW);

  switch (estadoActual) {
    case ESTADO_MONITOREO:
      digitalWrite(LED_VERDE, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("T:"); lcd.print(temp, 1); lcd.print("C H:"); lcd.print(hum, 0); lcd.print("%  ");
      lcd.setCursor(0, 1);
      lcd.print("Luz:"); lcd.print(luz); lcd.print(" NORMAL  ");
      Serial.print("Temperatura: "); Serial.print(temp); Serial.println(" C");
      Serial.print("Humedad: "); Serial.print(hum); Serial.println(" %");
      Serial.print("Luminosidad: "); Serial.println(luz);
      Serial.println("Estado: Monitoreo");
      break;

    case ESTADO_ADVERTENCIA:
      digitalWrite(LED_AMARILLO, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("T:"); lcd.print(temp, 1); lcd.print("C H:"); lcd.print(hum, 0); lcd.print("%  ");
      lcd.setCursor(0, 1);
      lcd.print("!! ADVERTENCIA !!");
      Serial.print("Temperatura: "); Serial.print(temp); Serial.println(" C");
      Serial.print("Humedad: "); Serial.print(hum); Serial.println(" %");
      Serial.print("Luminosidad: "); Serial.println(luz);
      Serial.println("Estado: Advertencia");
      break;

    case ESTADO_ALARMA:
      digitalWrite(LED_ROJO, HIGH);
      digitalWrite(BUZZER, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("T:"); lcd.print(temp, 1); lcd.print("C H:"); lcd.print(hum, 0); lcd.print("%  ");
      lcd.setCursor(0, 1);
      lcd.print("!!! ALARMA !!!  ");
      Serial.print("Temperatura: "); Serial.print(temp); Serial.println(" C");
      Serial.print("Humedad: "); Serial.print(hum); Serial.println(" %");
      Serial.print("Luminosidad: "); Serial.println(luz);
      Serial.println("Estado: ALARMA CRITICA");
      break;
  }
  delay(2000);
}