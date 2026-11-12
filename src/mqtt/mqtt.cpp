#include "mqtt.h"

Mqtt::Mqtt() : clienteMqtt(wifiClient){ //Construtor que já inicializa o client mqtt com o wifi client
}

void Mqtt::iniciar() {
    Serial.println("Inicializando MQTT...");
    clienteMqtt.setServer(mqtt_server, mqtt_port);
    clienteMqtt.setCallback(
    [this](char* topico, byte* mensagem, unsigned int tamanho) {
        receberMensagem(topico, mensagem, tamanho);
        }
    );
    conectarWifi();
}

//Conectar ao WIFI
void Mqtt::conectarWifi() {

     if (WiFi.status() == WL_CONNECTED) {
        return;
    }
    if (!wifiIniciado) {
        WiFi.begin(ssid, password);
        wifiIniciado = true;
        ultimaTentativaWifi = millis();
        Serial.println("Conectando ao Wi-Fi...");
    }

    if (millis() - ultimaTentativaWifi >= 500) {
        ultimaTentativaWifi = millis();
        Serial.print(".");
    }
}

// Conectar ao broker MQTT
void Mqtt::conectarBrokerMQTT() {
    if (millis() - ultimaTentativaMQTT >= 5000) {
        ultimaTentativaMQTT = millis();
        Serial.print("Conectando ao MQTT...");
        if (clienteMqtt.connect(id, user, senha)) {
            Serial.println("Conectado!");
            inscreverTopico(this->topicoSubUmidade);
            inscreverTopico(this->topicoSubTemperatura);
            inscreverTopico(this->topicoSubPresenca);
        } else {
            Serial.print("Falha: ");
            Serial.println(clienteMqtt.state());
        }
    }
}

void Mqtt::atualizar() {
    if (WiFi.status() != WL_CONNECTED) {
        conectarWifi();
        return;
    }
    if(!clienteMqtt.connected()){
        conectarBrokerMQTT();
    }else{
        clienteMqtt.loop();
    }
}

//Método de publicar
void Mqtt::publicar(const char* topico, const char* mensagem) {

    if (clienteMqtt.connected()) {

        if (clienteMqtt.publish(topico, mensagem)) {
            Serial.print("Publicado em ");
            Serial.print(topico);
            Serial.print(": ");
            Serial.println(mensagem);
        } else {
            Serial.println("Falha ao publicar!");
        }
    }
}

//Método de se inscrever num tópico
void Mqtt::inscreverTopico(const char* topico) {
    if (clienteMqtt.connected()) {
        clienteMqtt.subscribe(topico);
        Serial.print("Inscrito no tópico: ");
        Serial.println(topico);
    }
}

//Método de receber mensagem
void Mqtt::receberMensagem(char* topico, byte* mensagem, unsigned int tamanho) {

    topicoRecebido = topico;
    mensagemRecebida = "";
    for (int i = 0; i < tamanho; i++) {
        mensagemRecebida += (char)mensagem[i];
    }

    Serial.println("\nMensagem recebida!");
    Serial.print("Tópico: ");
    topicoRecebido = topico;
    Serial.println(topicoRecebido);
    Serial.print("Mensagem: ");
    Serial.println(mensagemRecebida);
    Serial.println("\n");
}

char* Mqtt::getMensagemRecebida() {
    return mensagemRecebida;
}

char* Mqtt::getTopicoRecebido() {
    return topicoRecebido;
}