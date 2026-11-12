#include "sensor/sensor.h"
#include "mqtt/mqtt.h"

Sensor sensor;
Mqtt mqtt;
unsigned long ultimaPublicacao = 0;

void setup()
{    
    Serial.begin(115200);
    Serial.println("ESP32 iniciado!");
    sensor.iniciar();
    mqtt.iniciar();

}

void loop()
{
    // Gerencia as conexões Wi-Fi e MQTT
    mqtt.atualizar();
    // Atualiza as medições do sensor
    sensor.atualizarCaptura();

    if (millis() - ultimaPublicacao >= 2000) {
        ultimaPublicacao = millis();
        mqtt.publicar(mqtt.topicoPubPresenca, sensor.possuiPresenca() ? "1" : "0");
        if(sensor.possuiLeituraDht()){
            mqtt.publicar(mqtt.topicoPubTemperatura, String(sensor.obterTemperatura()).c_str());
            mqtt.publicar(mqtt.topicoPubUmidade, String(sensor.obterUmidade()).c_str());
        }
    }
    
    sensor.atualizarLedPresenca(mqtt.getMensagemRecebida(), mqtt);
    sensor.atualizarLedTemperatura(mqtt.getMensagemRecebida(), mqtt);
    sensor.atualizarLedUmidade(mqtt.getMensagemRecebida(), mqtt);

}