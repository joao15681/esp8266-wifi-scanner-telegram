# ESP8266 Wi-Fi Scanner + Telegram

Projeto para ESP8266 que escaneia as redes Wi-Fi próximas e envia a lista (nome e força do sinal) para um bot do Telegram. O escaneamento é controlado por comandos enviados pelo próprio Telegram.

## Comandos

| Comando | O que faz |
|---|---|
| `iniciar` | Começa a escanear e enviar a lista a cada 10 segundos (o LED azul da placa acende) |
| `parar` | Para o escaneamento (o LED apaga) |
| `/ajuda` | Mostra os comandos disponíveis (o LED pisca) |

Os comandos aceitam maiúsculas e minúsculas. O bot só obedece mensagens vindas do `CHAT_ID` configurado.

## O que você precisa

- Uma placa com ESP8266 (NodeMCU, Wemos D1 Mini, etc.)
- Arduino IDE com o pacote de placas **ESP8266** instalado
- Uma rede Wi-Fi de **2,4 GHz** (o ESP8266 não conecta em 5 GHz)
- Um bot do Telegram

## Bibliotecas

Instale pelo Gerenciador de Bibliotecas da Arduino IDE:

- `UniversalTelegramBot`
- `ArduinoJson`

As bibliotecas `ESP8266WiFi` e `WiFiClientSecure` já vêm com o pacote de placas do ESP8266.

## Como usar

1. **Crie o bot:** no Telegram, converse com o [@BotFather](https://t.me/BotFather), envie `/newbot` e siga os passos. Ele vai te dar o **token** do bot.
2. **Descubra seu Chat ID:** converse com o [@userinfobot](https://t.me/userinfobot) e anote o número do seu `Id`.
3. **Inicie uma conversa com o seu bot:** procure o bot que você criou e envie `/start`. Sem isso, ele não consegue te mandar mensagens.
4. **Configure as credenciais:** copie o arquivo `credentials.example.h`, renomeie a cópia para `credentials.h` e preencha com seus dados:
   ```cpp
   #define BOT_TOKEN     "SEU_TOKEN_AQUI"
   #define CHAT_ID       "SEU_CHAT_ID_AQUI"
   #define WIFI_SSID     "NOME_DO_WIFI"
   #define WIFI_PASSWORD "SENHA_DO_WIFI"
   ```
5. **Envie o código** para o ESP8266 pela Arduino IDE.
6. Abra o Serial Monitor (115200) e espere aparecer `Wi-Fi conectado`.
7. No Telegram, mande `iniciar` para o seu bot.

## Segurança

- O arquivo `credentials.h` **não vai para o GitHub** (está no `.gitignore`). Nunca publique seu token nem a senha do Wi-Fi.
- Se o token vazar, gere um novo no @BotFather com `/revoke`.
- O código usa `setInsecure()`, que não valida o certificado do Telegram. É suficiente para estudo, mas menos seguro.

## Problemas comuns

- **Erro de compilação `UniversalTelegramBot.h: No such file or directory`:** instale as bibliotecas listadas acima.
- **Não conecta no Wi-Fi:** confira se a rede é 2,4 GHz e se o nome e a senha estão exatamente iguais (cuidado com `l`, `1`, `I`, `O` e `0`).
- **Não chega mensagem no Telegram:** confira o token e o Chat ID, e se você enviou `/start` para o bot.
- **O LED parece invertido:** na maioria das placas ESP8266 o LED da placa acende com `LOW` e apaga com `HIGH`.

## Como funciona

O ESP pergunta ao Telegram a cada 1 segundo se chegou algum comando novo e, quando o escaneamento está ligado, escaneia as redes e envia a lista a cada 10 segundos. O controle de tempo usa `millis()` em vez de `delay()`, para o ESP continuar ouvindo comandos enquanto espera.
