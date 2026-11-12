#include "sensor.h"

// Instância utilizada pela interrupção
Sensor* Sensor::instancia = nullptr;

// Construtor
Sensor::Sensor(){
    instancia = this;
}

// Interrupção estática
void IRAM_ATTR Sensor::isrEcho(){
    if (instancia != nullptr)
    {
        instancia->tratarInterrupcao();
    }
}

// Tratamento da interrupção (lê o pino 35 do HC-SR501)
void IRAM_ATTR Sensor::tratarInterrupcao(){
    movimentoDetectado = digitalRead(sensorPresenca);
    estadoAlterado = true;
}

void Sensor::iniciar(){
    Serial.begin(115200);
    Serial.println("Inicializando sensor...");

    pinMode(sensorPresenca, INPUT);
    pinMode(LedSensorPresenca, OUTPUT);
    pinMode(LedTemperaturaBaixa, OUTPUT);
    pinMode(LedTemperaturaAlta, OUTPUT);
    pinMode(LedUmidadeAlta, OUTPUT);
    dht.setup(sensorUmidadeTemperatura, DHTesp::DHT11);

    // Registra a interrupção no pino 35
    attachInterrupt(digitalPinToInterrupt(sensorPresenca), isrEcho, CHANGE);
}

void Sensor::atualizarCaptura(){
    if (estadoAlterado)
    {
        estadoAlterado = false;

    }

    if (millis() - ultimaLeituraDht >= 2000) {
        ultimaLeituraDht = millis();

        TempAndHumidity leitura = dht.getTempAndHumidity();
        if (dht.getStatus() != DHTesp::ERROR_NONE) {
            Serial.println(dht.getStatusString());
            return;
        }

        temperatura = leitura.temperature;
        umidade = leitura.humidity;
        leituraDhtValida = true;

        Serial.print("Temperatura: ");
        Serial.print(leitura.temperature);
        Serial.print(" C | Umidade: ");
        Serial.print(leitura.humidity);
        Serial.println(" %");

    }
}

bool Sensor::possuiLeituraDht() const {
    return leituraDhtValida;
}

bool Sensor::possuiPresenca() const {
    return movimentoDetectado;
}

float Sensor::obterTemperatura() const {
    return temperatura;
}

float Sensor::obterUmidade() const {
    return umidade;
}

void Sensor::atualizarLedTemperatura(char* estadoLED, Mqtt& mqtt){
    if(estadoLED == "1" && mqtt.getTopicoRecebido() == mqtt.topicoSubTemperatura){
        digitalWrite(LedTemperaturaAlta, HIGH);
        digitalWrite(LedTemperaturaBaixa, LOW);
    }else if(estadoLED == "0" && mqtt.getTopicoRecebido() == mqtt.topicoSubTemperatura){
        digitalWrite(LedTemperaturaAlta, LOW);
        digitalWrite(LedTemperaturaBaixa, HIGH);
    }
}

void Sensor::atualizarLedPresenca(char* estadoLED, Mqtt& mqtt){
    if(estadoLED == "1" && mqtt.getTopicoRecebido() == mqtt.topicoSubPresenca){
        digitalWrite(LedSensorPresenca, HIGH);
    }else if(estadoLED == "0" && mqtt.getTopicoRecebido() == mqtt.topicoSubPresenca){
        digitalWrite(LedSensorPresenca, LOW);
    }
}
void Sensor::atualizarLedUmidade(char* estadoLED, Mqtt& mqtt){
    if(estadoLED == "1" && mqtt.getTopicoRecebido() == mqtt.topicoSubUmidade){
        digitalWrite(LedUmidadeAlta, HIGH);
    }else if(estadoLED == "0" && mqtt.getTopicoRecebido() == mqtt.topicoSubUmidade){
        digitalWrite(LedUmidadeAlta, LOW);
    }
}