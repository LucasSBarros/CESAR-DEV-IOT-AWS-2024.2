# CESAR-DEV-IOT-AWS-2024.2
Repositório de Projetos IoT desenvolvidos pela turma 2024.2 da CESAR School, integrando dispositivos inteligentes com AWS para soluções inovadoras em IoT.


# Guia de Configuração para Dispositivos IoT com ESP32 e AWS IoT Core

Este guia detalha o processo de configuração de um dispositivo ESP32 para comunicação com o AWS IoT Core, utilizando o PlatformIO como ambiente de desenvolvimento integrado (IDE). O objetivo é fornecer um passo a passo completo para que você possa conectar seu ESP32 à AWS, permitindo a publicação e assinatura de mensagens via MQTT.

## Sumário

1. [Pré-requisitos](#pré-requisitos)
2. [Instalação e Configuração do Ambiente de Desenvolvimento](#instalação-e-configuração-do-ambiente-de-desenvolvimento)
   - [Instalação do Visual Studio Code](#instalação-do-visual-studio-code)
   - [Instalação do PlatformIO](#instalação-do-platformio)
3. [Configuração do AWS IoT Core](#configuração-do-aws-iot-core)
   - [Criação de um “Thing” no AWS IoT](#criação-de-um-thing-no-aws-iot)
   - [Criação e Anexação de Políticas](#criação-e-anexação-de-políticas)
   - [Geração e Download de Certificados](#geração-e-download-de-certificados)
4. [Desenvolvimento e Upload do Código para o ESP32](#desenvolvimento-e-upload-do-código-para-o-esp32)
   - [Configuração do Projeto no PlatformIO](#configuração-do-projeto-no-platformio)
   - [Implementação do Código](#implementação-do-código)
   - [Upload do Código para o ESP32](#upload-do-código-para-o-esp32)
5. [Monitoramento e Testes](#monitoramento-e-testes)
6. [Referências](#referências)

## Pré-requisitos

Antes de iniciar, certifique-se de ter os seguintes itens:

### Hardware
- Módulo ESP32 com suporte a Wi-Fi.
- Cabo USB para conexão com o computador.

### Software
- Conta ativa na AWS.
- Visual Studio Code instalado.
- PlatformIO instalado como extensão do Visual Studio Code.
- Drivers USB para o ESP32 instalados (por exemplo, CP210x USB to UART Bridge VCP Drivers).

## Instalação e Configuração do Ambiente de Desenvolvimento

### Instalação do Visual Studio Code

1. Acesse o site oficial do Visual Studio Code.
2. Baixe a versão correspondente ao seu sistema operacional (Windows, macOS ou Linux).
3. Siga as instruções de instalação fornecidas pelo instalador.

### Instalação do PlatformIO

1. Abra o Visual Studio Code.
2. Clique no ícone de extensões na barra lateral esquerda.
3. No campo de busca, digite “PlatformIO IDE”.
4. Clique em “Install” para instalar a extensão.
5. Após a instalação, reinicie o Visual Studio Code se solicitado.

## Configuração do AWS IoT Core

### Criação de um “Thing” no AWS IoT

1. Faça login no AWS Management Console.
2. Navegue até o serviço AWS IoT Core.
3. No menu lateral, selecione **Manage** e depois **Things**.
4. Clique em **Create** e escolha **Create a single thing**.
5. Dê um nome ao seu “Thing” (por exemplo, MyNewESP32) e clique em **Next**.

### Criação e Anexação de Políticas

1. No menu lateral do AWS IoT, selecione **Secure** e depois **Policies**.
2. Clique em **Create** para criar uma nova política.
3. Dê um nome à política (por exemplo, Esp32Policy).
4. No campo de definição da política, insira o seguinte JSON, substituindo `REGION` pela região da AWS que você está utilizando e `ACCOUNT_ID` pelo seu ID de conta AWS:

```json
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": "iot:Connect",
      "Resource": "arn:aws:iot:REGION:ACCOUNT_ID:client/MyNewESP32"
    },
    {
      "Effect": "Allow",
      "Action": "iot:Subscribe",
      "Resource": "arn:aws:iot:REGION:ACCOUNT_ID:topicfilter/esp32/sub"
    },
    {
      "Effect": "Allow",
      "Action": "iot:Receive",
      "Resource": "arn:aws:iot:REGION:ACCOUNT_ID:topic/esp32/sub"
    },
    {
      "Effect": "Allow",
      "Action": "iot:Publish",
      "Resource": "arn:aws:iot:REGION:ACCOUNT_ID:topic/esp32/pub"
    }
  ]
}
```

5. Clique em **Create** para criar a política.
6. Volte ao menu **Secure**, selecione **Certificates** e localize o certificado criado para o seu dispositivo.
7. Selecione o certificado, clique em **Actions** e depois em **Attach policy**.
8. Selecione a política `Esp32Policy` e clique em **Attach**.

### Geração e Download de Certificados

1. Durante a criação do “Thing”, você será solicitado a criar certificados.
2. Clique em **Create certificate** para gerar os certificados necessários.
3. Baixe os seguintes arquivos e armazene-os em um local seguro:
   - Certificado do dispositivo (`xxxxxxxxxx-certificate.pem.crt`)
   - Chave privada do dispositivo (`xxxxxxxxxx-private.pem.key`)
   - Amazon Root CA 1 (`AmazonRootCA1.pem`)
4. Clique em **Activate** para ativar o certificado.

## Desenvolvimento e Upload do Código para o ESP32

### Configuração do Projeto no PlatformIO

#### Criação do Projeto

1. Abra o Visual Studio Code.
2. Clique no ícone do PlatformIO na barra lateral esquerda.
3. Selecione **New Project**.
4. Defina um nome para o projeto (por exemplo, `ESP32_AWS_IoT`).
5. Em **Board**, selecione o modelo do seu ESP32 (por exemplo, `ESP32 Dev Module`).
6. Em **Framework**, escolha **Arduino**.
7. Clique em **Finish** para criar o projeto.

#### Instalação de Bibliotecas Necessárias

No terminal do PlatformIO, execute os seguintes comandos para instalar as bibliotecas necessárias:

```bash
pio lib install "PubSubClient"
pio lib install "ArduinoJson"
```

### Implementação do Código

#### Configuração de Certificados e Credenciais

No diretório `src` do seu projeto, crie um arquivo chamado `secrets.h`. Insira o seguinte conteúdo, substituindo os valores conforme necessário:

```cpp
#include <pgmspace.h>

#define SECRET
#define THINGNAME "MyNewESP32"

const char WIFI_SSID[] = "SEU_SSID";
const char WIFI_PASSWORD[] = "SUA_SENHA";
const char AWS_IOT_ENDPOINT[] = "xxxxxxxxxx-ats.iot.REGION.amazonaws.com";

// Certificado Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
SEU_CERTIFICADO_CA
-----END CERTIFICATE-----
)EOF";

// Certificado do Dispositivo
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
SEU_CERTIFICADO_DISPOSITIVO
-----END CERTIFICATE-----
)KEY";

// Chave Privada do Dispositivo
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
SUA_CHAVE_PRIVADA
-----END RSA PRIVATE KEY-----
)KEY";
```

#### Código Principal

No arquivo `main.cpp` (ou `src/main.cpp`), insira o seguinte código:

```cpp
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "secrets.h"
#include "WiFi.h"

// Tópicos MQTT
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

void messageHandler(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void connectAWS() {
  Serial.print("Conectando ao Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" conectado.");

  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  client.setServer(AWS_IOT_ENDPOINT, 8883);
  client.setCallback(messageHandler);

  Serial.print("Conectando ao AWS IoT...");
  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println(" conectado.");

  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
}

void publishMessage() {
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["sensor_a0"] = analogRead(0);
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void setup() {
  Serial.begin(115200);
  connectAWS();
}

void loop() {
  if (!client.connected()) {
    connectAWS();
  }
  client.loop();
  publishMessage();
  delay(1000);
}
```

Este código conecta o ESP32 à rede Wi-Fi, estabelece uma conexão segura com o AWS IoT Core e publica mensagens JSON no tópico `esp32/pub` a cada segundo.

### Upload do Código para o ESP32

1. Conexão do Dispositivo:
   - Conecte o ESP32 ao computador via cabo USB.

2. Seleção da Porta Serial:
   - No PlatformIO, clique no ícone de monitor na barra inferior e selecione a porta correspondente ao ESP32.

3. Upload do Código:
   - Clique no ícone de seta para cima (Upload) na barra inferior do PlatformIO para compilar e enviar o código para o ESP32.
   - Aguarde a conclusão do processo.

## Monitoramento e Testes

### Monitor Serial

1. Após o upload, abra o Monitor Serial clicando no ícone de plugue na barra inferior do PlatformIO.
2. Defina a taxa de baud para `115200` para visualizar as mensagens de depuração.

### Testes de Publicação e Assinatura

1. No console do AWS IoT, navegue até **Test** e insira o tópico `esp32/pub` para visualizar as mensagens publicadas pelo ESP32.
2. Para enviar mensagens ao ESP32, publique no tópico `esp32/sub` e observe a resposta.

### Visualizando as Mensagens Publicadas

- Com a inscrição no tópico `esp32/pub`, você poderá visualizar as mensagens que o ESP32 está enviando. As mensagens devem aparecer em tempo real na interface do cliente de teste MQTT.

### Publicando Mensagens para o ESP32

- Para enviar mensagens ao ESP32, utilize o campo “Publicar em um tópico”.
- Insira o tópico `esp32/sub` e a mensagem que deseja enviar.
- Clique em **Publicar**.
- No Monitor Serial do PlatformIO, você deverá ver a mensagem recebida pelo ESP32, indicando que o dispositivo está assinando corretamente o tópico e processando as mensagens recebidas.

## Referências

Para aprofundar seus conhecimentos e obter mais detalhes sobre os tópicos abordados neste guia, consulte as seguintes referências:
- **AWS IoT Core**:
  - Documentação Oficial do AWS IoT Core
  - Exibir mensagens MQTT com o cliente MQTT do AWS IoT
- **ESP32**:
  - Documentação Oficial do ESP32
- **PlatformIO**:
  - Documentação Oficial do PlatformIO
- **ArduinoJson**:
  - Documentação do ArduinoJson
- **PubSubClient**:
  - Repositório do PubSubClient no GitHub

