#include "Lara.h"
#include "OlhosRobo.h"

// ===================== CONSTRUTOR =====================
LARA::LARA() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {

  // Configura motores como saída
  pinMode(PINO_MOTOR_A1, OUTPUT);
  pinMode(PINO_MOTOR_A2, OUTPUT);
  pinMode(PINO_MOTOR_B1, OUTPUT);
  pinMode(PINO_MOTOR_B2, OUTPUT);

  // Configura sensores de linha
  pinMode(SENSOR_LINHA_ESQUERDO, INPUT);
  pinMode(SENSOR_LINHA_DIREITO, INPUT);

  // Inicializa PWM para motores (ESP32)
  ledcSetup(0, 5000, 8); // canal 0, 5kHz, 8 bits
  ledcSetup(1, 5000, 8); // canal 1, 5kHz, 8 bits
  ledcAttachPin(PINO_VELOCIDADE_MOTOR_A, 0);
  ledcAttachPin(PINO_VELOCIDADE_MOTOR_B, 1);

  // Inicializa display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
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
  ledcWrite(0, velA);
  ledcWrite(1, velB);
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
  const float VELOCIDADE_BASE_CM_POR_S = 10.0;
  float cm_por_seg = VELOCIDADE_BASE_CM_POR_S * (velocidade / (float)VELOCIDADE_MAXIMA);
  return (unsigned long)((distancia_cm / cm_por_seg) * 1000);
}

void LARA::andarParaFrente(float distancia_cm, int velocidade) {
  frente(velocidade);
  delay(calcularTempo(distancia_cm, velocidade));
  parar();
}

void LARA::andarParaTras(float distancia_cm, int velocidade) {
  re(velocidade);
  delay(calcularTempo(distancia_cm, velocidade));
  parar();
}

void LARA::virarParaDireita(float distancia_cm, int velocidade) {
  direita(velocidade);
  delay(calcularTempo(distancia_cm, velocidade));
  parar();
}

void LARA::virarParaEsquerda(float distancia_cm, int velocidade) {
  esquerda(velocidade);
  delay(calcularTempo(distancia_cm, velocidade));
  parar();
}

// ===================== FUNÇÕES DOS OLHOS =====================
void LARA::olhosNormais()   { 
  display.clearDisplay(); 
  eye_nuetral(500); 
  display.display();
  vTaskDelay(1); 
}

void LARA::olhosEsquerda() { 
  display.clearDisplay(); 
  eye_lookleft(500); 
  display.display();
  vTaskDelay(1); 
}

void LARA::olhosDireita() { 
  display.clearDisplay(); 
  eye_lookright(500); 
  display.display();
  vTaskDelay(1); 
}

void LARA::olhosAssustados() { 
  display.clearDisplay(); 
  eye_squint(500); 
  display.display();
  vTaskDelay(1); 
}

void LARA::olhosPiscar() { 
  display.clearDisplay(); 
  eye_nuetral(500); 
  display.display(); 
  delay(200); 
  olhosNormais(); 
}

// ===================== SENSORES DE LINHA =====================
bool LARA::linhaDetectadaEsquerda() { 
  return digitalRead(SENSOR_LINHA_ESQUERDO) == LOW; 
}

bool LARA::linhaDetectadaDireita()  { 
  return digitalRead(SENSOR_LINHA_DIREITO) == LOW; 
}

bool LARA::linhaCentralizada() { 
  return linhaDetectadaEsquerda() && linhaDetectadaDireita(); 
}

bool LARA::linhaPerdida() { 
  return !linhaDetectadaEsquerda() && !linhaDetectadaDireita(); 
}

// ===================== SEGUIR LINHA =====================
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
    setMotores(HIGH, LOW, LOW, HIGH, velocidade, velocidade / 2); // direita devagar
    olhosDireita();
  } else {
    parar();
  }
}