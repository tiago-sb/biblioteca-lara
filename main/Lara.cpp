#include "Lara.h"
#include "OlhosRobo.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>

LARA::LARA(): display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
  // Configura motores como saída
  pinMode(PINO_MOTOR_A1, OUTPUT);
  pinMode(PINO_MOTOR_A2, OUTPUT);
  pinMode(PINO_MOTOR_B1, OUTPUT);
  pinMode(PINO_MOTOR_B2, OUTPUT);
  pinMode(PINO_VELOCIDADE_MOTOR_A, OUTPUT);
  pinMode(PINO_VELOCIDADE_MOTOR_B, OUTPUT);

  // Configura sensores de linha
  pinMode(SENSOR_LINHA_ESQUERDO, INPUT);
  pinMode(SENSOR_LINHA_DIREITO, INPUT);

  // Inicializa display (instância global em globals.cpp)
  // falha ao iniciar display — trate conforme necessário
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {}
  
  display.clearDisplay();
  display.display();

  olhosNormais();
}

// ===================== AUXILIAR DE MOTORES =====================
void LARA::setMotores(int A1, int A2, int B1, int B2, int velA, int velB) {
  digitalWrite(PINO_MOTOR_A1, A1);
  digitalWrite(PINO_MOTOR_A2, A2);
  digitalWrite(PINO_MOTOR_B1, B1);
  digitalWrite(PINO_MOTOR_B2, B2);

  analogWrite(PINO_VELOCIDADE_MOTOR_A, constrain(velA, 0, 255));
  analogWrite(PINO_VELOCIDADE_MOTOR_B, constrain(velB, 0, 255));
}

// ===================== MOVIMENTOS =====================
void LARA::frente(int vel) {
  setMotores(HIGH, LOW, HIGH, LOW, vel, vel);
  olhosNormais();
}

void LARA::re(int vel) {
  setMotores(LOW, HIGH, LOW, HIGH, vel, vel);
  olhosAssustados();
}

void LARA::esquerda(int vel) {
  setMotores(LOW, HIGH, HIGH, LOW, vel, vel);
  olhosEsquerda();
}


void LARA::direita(int vel) {
  setMotores(HIGH, LOW, LOW, HIGH, vel, vel);
  olhosDireita();
}


void LARA::parar() {
setMotores(LOW, LOW, LOW, LOW, 0, 0);
olhosPiscar();
}

// ===================== MOVIMENTOS POR DISTÂNCIA =====================
unsigned long LARA::calcularTempo(float distancia_cm, int velocidade) {
  const float VELOCIDADE_BASE_CM_POR_S = 10.0f; // ajuste experimental
  
  // evita divisão por zero
  if (velocidade <= 0) return 0; 
  if (VELOCIDADE_MAXIMA <= 0) return 0;
  
  float cm_por_seg = VELOCIDADE_BASE_CM_POR_S * (velocidade / (float) VELOCIDADE_MAXIMA);
  if (cm_por_seg <= 1e-6f) return 0;
  
  return (unsigned long)((distancia_cm / cm_por_seg) * 1000.0f);
}

void LARA::andarParaTras(float distancia_cm, int velocidade) {
  re(velocidade);
  unsigned long t = calcularTempo(distancia_cm, velocidade);
  if (t > 0) delay(t);
  parar();
}


void LARA::virarParaDireita(float distancia_cm, int velocidade) {
  direita(velocidade);
  unsigned long t = calcularTempo(distancia_cm, velocidade);
  if (t > 0) delay(t);
  parar();
}


void LARA::virarParaEsquerda(float distancia_cm, int velocidade) {
  esquerda(velocidade);
  unsigned long t = calcularTempo(distancia_cm, velocidade);
  if (t > 0) delay(t);
  parar();
}

// ===================== FUNÇÕES DOS OLHOS =====================
void LARA::olhosNormais() { 
  display.clearDisplay(); 
  look_nuetral(display, 200); 
  display.display(); 
  delay(1); 
}

void LARA::olhosEsquerda() { 
  display.clearDisplay(); 
  look_left(display, 200); 
  display.display(); 
  delay(1); 
}

void LARA::olhosDireita() { 
  display.clearDisplay(); 
  look_right(display, 200); 
  display.display(); 
  delay(1); 
}

void LARA::olhosAssustados() { 
  display.clearDisplay(); 
  look_squint(display, 200); 
  display.display(); 
  delay(1); 
}

void LARA::olhosPiscar() { 
  display.clearDisplay(); 
  look_nuetral(display, 200); 
  display.display(); 
  delay(200); 
  olhosNormais(); 
}

// ===================== SENSORES DE LINHA =====================
bool LARA::linhaDetectadaEsquerda() { 
  return digitalRead(SENSOR_LINHA_ESQUERDO) == LOW; 
}

bool LARA::linhaDetectadaDireita() { 
  return digitalRead(SENSOR_LINHA_DIREITO) == LOW; 
}

bool LARA::linhaCentralizada() { 
  return linhaDetectadaEsquerda() && linhaDetectadaDireita(); 
}

bool LARA::linhaPerdida() { 
  return !linhaDetectadaEsquerda() && !linhaDetectadaDireita(); 
}

// seguir linha (simples)
void LARA::seguirLinha(int velocidade) {
  bool esq = linhaDetectadaEsquerda();
  bool dir = linhaDetectadaDireita();

  if (esq && dir) {
    frente(velocidade);
  } else if (esq && !dir) {
    // vira para esquerda
    setMotores(LOW, HIGH, HIGH, LOW, velocidade / 2, velocidade);
    olhosEsquerda();
  } else if (!esq && dir) {
    // vira para direita
    setMotores(HIGH, LOW, LOW, HIGH, velocidade, velocidade / 2);
    olhosDireita();
  } else {
    parar();
  }
}