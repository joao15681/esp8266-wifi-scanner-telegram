//Bibliotecas são códigos prontos que alguém escreveu, para não reescrever a gente "importa"

// Aspas "" = Arquivo; <> = Biblioteca Instalada

#include <ESP8266WiFi.h>           // Ensina o ESP8266 a usar o WI-FI (conectar, escanear redes)
#include <WiFiClientSecure.h>      // Permite conexão segura (HTTPS). O Telegram exige
#include <UniversalTelegramBot.h>  //Facilita conversar com o Telegram (enviar mensagem etc...)
#include "credentials.h"           // Traz os dados de outro arquivo

//-- Objetos Globais --
// Ficam fora das funções pra usar em qualquer lugar do código

WiFiClientSecure client;                      // A segurança que o ESP usa para falar com a Internet
UniversalTelegramBot bot(BOT_TOKEN, client);  //O bot recebe o token para provar quem é, cliente para saber por onde mandar
bool escaneado = false;                       // false = desligado, true = ligado

// -- Setup --
//Roda UMA vez quando o ESP liga ou reinicia, prepara tudo

void setup() {
  Serial.begin(115200);  //Liga a comunicação com Serial Monitor
                         //115200 é a velocidade: necessita ser igual ao do Serial Monitor

  client.setInsecure();  //Diz "não confira o certificado do Telegram", menos seguro, ótimo pra projetos

  WiFi.mode(WIFI_STA);  //STA = "estação", age como dispositivo assim se conectando a um roteador

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  //Começa a tentar conectar com o nome e senha
                                         // Só da a ordem, não espera terminar

  // ENQUANTO o WI-FI NÃO estiver conectado, fique repetindo o que está dentro das chaves
  //⬇️status atual ⬇️diferente de ⬇️"conectado"
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);  //Espere meio segundo antes de checar de novo, sem isso chegaria milhares de vezes por segundo

    Serial.print(".");  //Imprime um ponto para resposta de funcionamento
  }
  // Quando se conectar, a condição fica falsa e ele sai do while e segue em frente

  Serial.println("\nWi-Fi conectado");  // \n = pula linha
}

//-- LOOP --
// Roda Infinitamente apos setup: termina e volta pro comeco

unsigned long ultimaChecagem = 0;
unsigned long ultimoScan = 0;

void loop() {

  if (millis() - ultimaChecagem > 1000) {

    int novas = bot.getUpdates(bot.last_message_received + 1);  // Devolve quantas mensagens novas chegaram, o +1 evita ler duas vezes a mesma mensagem


    for (int i = 0; i < novas; i++) {
      String texto = bot.messages[i].text;
      texto.toLowerCase();
      texto.trim();
      String quem = bot.messages[i].chat_id;

      if (quem == CHAT_ID && texto == "iniciar") {
        escaneado = true;
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, LOW);
      }
      if (quem == CHAT_ID && texto == "parar") {
        escaneado = false;
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
    ultimaChecagem = millis();
  }

  if (escaneado && millis() - ultimoScan > 10000) {

    int n = WiFi.scanNetworks();

    String msg = "Redes encontradas:\n\n";

    // PARA i começando em 0, ENQUANTO i for menor que n E menor que 15, some 1 em i em cada volta
    for (int i = 0; i < n && i < 15; i++) {
      // int i = 0  -> é um contador, começa em 0
      // i <    -> só vai até o número de redes que achou (senão pegaria rede que não existe)
      // &&  i < 15 -> "e tambem" para em 15, pra mensagem não ficar gigante e travar o ESP
      // i++   -> depois de cada volta, soma 1 (i = i + 1)

      msg += String(i + 1) + ": " + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + "dBm)\n";
      // msg +=   -> "pega o que ja tem em msg e ACRESCENTA no final"
      // String( i + 1)   -> número da rede (+1 porque i começa em 0 e a lista humana começa em 1)
      // WiFi.SSID(i)    -> nome da rede número i
      // WiFi.RSSI(i)  -> força do sinal em dBm (mais perto de 0 = mais forte; -40 = otimo, -90 é fraco )
      // \n          -> cada rede fica em uma linha
    }

    bot.sendMessage(CHAT_ID, msg, "");  //Manda o texto pro Telegram
                                        //CHAT_ID = pra quem; msg = o que
                                        //"" = sem formatação especial (Markdown/HTML)

WiFi.scanDelete();  //Limpa da memoria o resultado do escaneamento, pois o ESP tem pouca memória

ultimoScan = millis();  //anota: acabei de escaner
  }
}