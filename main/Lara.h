#ifndef LARA_H
#define LARA_H

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "OlhosRobo.h"

// ============================== CONFIGURAÇÕES DO OLED =============================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// ========================== CONFIGURAÇÕES DE MOTORES =============================
#define PINO_VELOCIDADE_MOTOR_A 27
#define PINO_MOTOR_A1 19
#define PINO_MOTOR_A2 22

#define PINO_VELOCIDADE_MOTOR_B 14
#define PINO_MOTOR_B1 21
#define PINO_MOTOR_B2 17

#define VELOCIDADE_MAXIMA 200
#define VELOCIDADE_MINIMA 0

// ===================== CONFIGURAÇÕES DE SENSORES INFRA VERMELHO ==================
#define SENSOR_LINHA_ESQUERDO 32
#define SENSOR_LINHA_DIREITO 25

// ===================== DIREÇÕES ==================
#define FRENTE   0
#define DIREITA  1
#define ESQUERDA 2
#define RE       3

class LARA {
  public:
    // Construtor
    LARA();

    // Movimentos por tempo
    void frente(int velocidade);
    void direita(int velocidade);
    void esquerda(int velocidade);
    void re(int velocidade);
    void parar(); // corrigido, sem parâmetro

    // Movimentos por distância estimada
    void andarParaFrente(float distancia_cm, int velocidade);
    void andarParaTras(float distancia_cm, int velocidade);
    void virarParaDireita(float distancia_cm, int velocidade);
    void virarParaEsquerda(float distancia_cm, int velocidade);

    // Sensores infravermelho
    bool linhaDetectadaEsquerda();
    bool linhaDetectadaDireita();
    bool linhaCentralizada();
    bool linhaPerdida();

    // Olhos no display
    void olhosNormais();
    void olhosEsquerda();
    void olhosDireita();
    void olhosAssustados();
    void olhosPiscar();

    // Função para seguir linha
    void seguirLinha(int velocidade);

  private:
    Adafruit_SSD1306 display;
    
    unsigned long calcularTempo(float distancia_cm, int velocidade);
    void setMotores(int A1, int A2, int B1, int B2, int velA, int velB);
};

#endif