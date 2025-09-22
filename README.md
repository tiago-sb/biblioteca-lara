# Biblioteca de Controle para Robôs com ESP32 - Projeto LARA

A biblioteca **LARA** foi desenvolvida para controlar robôs baseados em **ESP32**, possui suporte a:

- Controle de motores 
- Leitura de sensores de linha infravermelhos
- Exibição de expressões visuais ("olhos") em display OLED SSD1306 128x64
- Movimentações básicas e por distância estimada 
- Função de seguir linha

---

## Funcionalidades

### Controle de Movimento

- `frente(int velocidade)`
- `re(int velocidade)`
- `esquerda(int velocidade)`
- `direita(int velocidade)`
- `parar()`

### Movimento por Distância

- `andarParaFrente(distancia_cm, velocidade)`
- `andarParaTras(distancia_cm, velocidade)`
- `virarParaDireita(distancia_cm, velocidade)`
- `virarParaEsquerda(distancia_cm, velocidade)`

### Sensores de Linha

- `linhaDetectadaEsquerda()`
- `linhaDetectadaDireita()`
- `linhaCentralizada()`
- `linhaPerdida()`

### Olhos no Display OLED

- `olhosNormais()`: olhos do robô com expressão neutra  
- `olhosEsquerda()`: olhos do robô olham para esquerda  
- `olhosDireita()`: olhos do robô olham para direita  
- `olhosAssustados()`: olhos do robô olham com expressão de surpresa/medo  
- `olhosPiscar()`: olhos do robô piscam rapidamente

---

## Requisitos

- **Placa:** ESP32 para receber e processar os comandos
- **Display:** OLED SSD1306 (I2C) 128x64 para processar os comandos relativos aos olhos do robô
- **Bibliotecas:**
  - `Adafruit_GFX`
  - `Adafruit_SSD1306`
  - `Wire.h`

---

## Exemplo de Uso Prático

```cpp
#include "LARA.h"

LARA robo;

void setup() {}

void loop() {
  // robô anda 10cm para frente
  robo.andarParaFrente(10, 200); 
  delay(1000);

  // robô para seu movimento
  robo.parar();
  delay(500);
}

