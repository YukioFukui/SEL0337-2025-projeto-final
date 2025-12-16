#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int leitura;

void vTask1(void *pvParameters);


void setup()
{
  pinMode(14, OUTPUT); // pino correspondente ao LED onboard como saída
  pinMode(13, INPUT_PULLDOWN);
  Serial.begin(115200); // taxa de transferência serial (baud rate) = 115200 bits por segundo (bps)
  digitalWrite(14, LOW); // inicialmente desligado (LED apagado)
  Serial.println("ESP32 Touch Sensor Test"); // mensagem inicial impressa no monitor serial
  xTaskCreatePinnedToCore(vTask1, "toque", 2048, NULL, 5, NULL, 1);

} //end setup


void loop(){
  vTaskDelay(50);

} 

void vTask1(void *pvParameters){
 
  while(1){
    leitura = digitalRead(13); // armazena o valor lido do touch do pino13
    Serial.print("key:"); // imprimir a msg "key"...
    Serial.println(leitura); // e o valor lido no monitor serial
    if (leitura == HIGH) { // lógica para acender o LED onboard quando o pino13 for tocado
    digitalWrite(14, HIGH); // se sim, acende o LED (pino 2)...
    Serial.println("touched"); // msg p/ sinalizar quando houve toque
    }
    else {
    digitalWrite(14, LOW); // caso contrário, o LED permanece apagado
    }
  }
}