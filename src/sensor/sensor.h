#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <DHTesp.h>
#include "mqtt/mqtt.h"

class Sensor{
    private:
        const int sensorPresenca = 35;
        const int sensorUmidadeTemperatura = 4;
        const int LedSensorPresenca = 16;
        const int LedTemperaturaBaixa = 17;
        const int LedTemperaturaAlta = 18;
        const int LedUmidadeAlta = 19;
        DHTesp dht;

        // Flags de controle da interrupção
        volatile bool movimentoDetectado = false;
        volatile bool estadoAlterado = false;
        float temperatura = 0;
        float umidade = 0;
        bool leituraDhtValida = false;
        unsigned long ultimaLeituraDht = 0;

        // Para permitir interrupções mesmo estando dentro de uma classe
        static Sensor* instancia;
        static void IRAM_ATTR isrEcho();
        void IRAM_ATTR tratarInterrupcao();

    public:
        Sensor();
        void iniciar();
        void atualizarCaptura();
        void atualizarLedTemperatura(char* estadoLED, Mqtt& mqtt);
        void atualizarLedPresenca(char* estadoLED, Mqtt& mqtt);
        void atualizarLedUmidade(char* estadoLED, Mqtt& mqtt);

        bool possuiLeituraDht() const;
        bool possuiPresenca() const;
        float obterTemperatura() const;
        float obterUmidade() const;
};
#endif
