#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include <WiFi.h>

class Mqtt{
    private:
        // CREDENCIAIS DO WI-FI
        const char* ssid = "A15 de Juvam";
        const char* password = "juvam123";

        // ENDEREÇO DO BROKER MQTT
        const char* mqtt_server = "3.121.120.104";
        const int mqtt_port = 1883;
        const char* user = nullptr;
        const char* senha = nullptr;

        WiFiClient wifiClient;
        PubSubClient clienteMqtt;
        const char* id = "CaixaJuvam";

        //Variáveis de configuração de WIFI e Mqtt
        unsigned long ultimaTentativaWifi = 0;
        bool wifiIniciado = false;
        unsigned long ultimaTentativaMQTT = 0;

        char* mensagemRecebida;
        char* topicoRecebido;
        
    public:
        //Topicos publicacao
        const char* topicoPubPresenca = "sensor/entrada/presenca";
        const char* topicoPubUmidade = "sensor/entrada/umidade";
        const char* topicoPubTemperatura = "sensor/entrada/temperatura";

        //Topicos inscricao
        const char* topicoSubPresenca = "sensor/saida/presenca";
        const char* topicoSubUmidade = "sensor/saida/umidade";
        const char* topicoSubTemperatura = "sensor/saida/temperatura";

        char* getMensagemRecebida();
        char* getTopicoRecebido();

        Mqtt();
        void iniciar();
        void conectarWifi();
        void conectarBrokerMQTT();
        void atualizar();
        void publicar(const char* topico, const char* mensagem);
        void inscreverTopico(const char* topico);
        void receberMensagem(char* topico, byte* mensagem, unsigned int tamanho);
};
#endif