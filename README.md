# Parte 1 - Caracterização de Sistemas Embarcados
De inicio, escolhemos o produto Tomada Inteligente Wi-Fi (Smart Plug),
de modelo Sonoff S26, cujo segmento é o de automação residencial ou IoT.

# Levantamento das Características Técnicas

Este dispositivo destaca-se pela simplicidade de hardware e baixo custo,
sendo amplamente documentado pela comunidade de IoT.

  **Característica e Detalhamento Técnico**
  
  *Unidade de Processamento*    *SoC (System on Chip):* ESP8266. Integra MCU de 32-bit e rádio Wi-Fi. Clock: 80 MHz.
  
  *Memória*                     *Flash:* 1 MB ou 2 MB para firmware e sistema de arquivos. RAM: 50 KB (SRAM).
  
  *Sistema Operacional*        *RTOS / SDK:* Roda um firmware baseado no FreeRTOS para gerenciar o Wi-Fi e TCP/IP.
  
  *Interfaces de Comunicação*   *Sem Fio:* Wi-Fi 802.11 (2.4 GHz). *Com Fio:* Possui pinos internos de UART (TX/RX) acessíveis na placa para gravação de firmware, mas não expostos ao usuário final.
  
  *Sensores*                    *Botão Físico:* Um botão conectado a uma GPIO para acionamento manual ou modo de pareamento.
  
  *Atuadores*                   *Relé Eletromecânico:* 10A/250V acionado via GPIO (transistor) para controlar a carga da tomada. *LEDs:* Indicador de estado (Wi-Fi/Ligado).
  
  *Alimentação*                 Fonte interna AC/DC que converte 110/220V da rede para 5V e 3.3V DC para alimentar o SoC e o Relé.
  
<img width="479" height="492" alt="sonoff" src="https://github.com/user-attachments/assets/a934814e-7f29-4973-8bbd-7c3c3585e430" />


[Produto Escolhido - Sonoff S26]


# Validação Científica

## Artigo 1: Tecnologia (Medição e Monitoramento IoT)

A referência 1 apresenta o desenvolvimento de um medidor de energia
inteligente utilizando a integração entre Arduino e o módulo Wi-Fi
ESP8266 (similar ao utilizado na tomada inteligente deste projeto). O
estudo utiliza sensores de corrente e a plataforma IoT Blynk para
realizar a Leitura Automática de Medidores em tempo real.

**Crítica**

A questão principal deste artigo diz respeito a como automatizar a
leitura de consumo e detectar anomalias utilizando hardwares de custo
baixo. Embora focado no medidor de residência, o artigo valida o uso da
tecnologia ESP combinada com sensores de corrente para medição via
Wi-Fi, que é o mesmo princípio de funcionamento de uma Tomada
Inteligente. Por fim, a crítica a este artigo é que o sistema depende de
um Arduino e uma ESP8266. Uma sugestão seria a migração para um único
ESP32, pois reduziria a complexidade e o custo.

Além disso, os autores não discutem limitações de escalabilidade nem
como o sistema reage a ruído elétrico em medições contínuas. Minha
dúvida aos autores seria justamente como o método se comporta em
cenários reais com cargas variáveis e interferência eletromagnética
típica de ambientes domésticos.

## Artigo 2: Aplicação (Controle Residencial)

O trabalho da referência 2 foca na implementação prática de um Smart Plug
para controle remoto de eletrodomésticos via aplicativo Android. O
estudo aborda a integração do hardware (relé e módulo Wi-Fi) com
servidores em nuvem, demonstrando a eficiência do acionamento remoto
para reduzir o consumo de energia de dispositivos que ficam em
"stand-by".

**Crítica**

A questão principal desse artigo é de como implementar um sistema de
controle residencial de baixo custo e alta acessibilidade. É importante
pois demonstra a democratização da automação residencial, o que permite
que usuários comuns controlem cargas AC sem necessidade de uma
infraestrutura complexa. Para a critica e sugestão deste artigo,
percebe-se que os autores focam na funcionalidade básica
(ligar/desligar), mas pouco explorou-se a capacidade de programação de
horários (timers) no próprio firmware, o que evitaria dependência total
da internet.

Os autores também não analisam a latência da comunicação em nuvem, ponto
relevante porque o sistema depende da internet até para ações simples.
Minha pergunta aos autores seria como o smart plug se comporta em
situações de perda ou instabilidade de conexão, já que isso impacta
diretamente a usabilidade do dispositivo.

# Parte 2 - Abordagem do Desenvolvimento

Nesta etapa, analisamos um possível desenvolvimento para o produto
escolhido, a Tomada Inteligente Wi-Fi.

# Escolha e Justificativa

A abordagem escolhida foi a fabricação do zero. Pois, para um
dispositivo IOT de altíssimo volume e baixo custo como um Smart Plug
(preço de venda aprox. R\$50), a escolha da arquitetura é crucial.

## Comparação de Custos

Para fazer o projeto com uma Raspberry Pi, seria inviável técnica e
economicamente. Uma Raspberry Pi custa 3x mais que o preço final do
produto e é fisicamente maior que a própria tomada. Além disso, rodar um
Linux completo apenas para acionar um relé é um desperdício massivo de
recursos computacionais.

Quanto aos CoM, ou Módulos industriais, ainda seriam caros demais para o
mercado doméstico. Logo, projetar a placa do zero, dedicada com o SoC
ESP8266/ESP32 é a única opção viável. O chip custa menos de $1 USD.

## Risco

O uso de SoCs como a família ESP reduz o tempo de design, pois os
esquemáticos de referência são simples e públicos. A existência de RTOS
(FreeRTOS) para esses chips mitiga o risco de desenvolvimento de
software, não sendo necessário criar protocolos de rede Wi-Fi do zero.

Por isso, na prática, apenas uma placa dedicada mantém o produto dentro
do preço competitivo que ele tem hoje no mercado.

# Parte 3 - Implementação

# Descrição

Para a prática final, foi desenvolvido um firmware simulando o
funcionamento da Tomada Inteligente caracterizada na Parte 1. O objetivo
foi demonstrar o uso do sistema operacional de tempo real **FreeRTOS**
operando em modo *Dual-Core* da ESP32.

Ora, sendo assim, sistema divide-se em duas Tasks concorrentes:

-   **Task de Hardware (Atuador):** Controla a carga (LED/Relé) e simula
    o monitoramento de consumo.

-   **Task de Interface (Sensor):** Lê o botão físico de acionamento
    manual de forma assíncrona.

# Implementação e Configuração do FreeRTOS

A principal vantagem explorada foi a arquitetura assimétrica da ESP32.
As tarefas foram alocadas em núcleos físicos distintos para garantir que
o processamento da interface humana não bloqueie o controle crítico da
carga.

  **Nome da Task**    **Prioridade**   **Núcleo**  **Função**
  ------------------ ---------------- ------------ ----------------------------------
  `Task_Hardware`        Alta (2)        Core 1    Controle do Relé (LED) e Status.
  `Task_Botao`          Baixa (1)        Core 0    Leitura do botão do usuário.

  : Alocação de Recursos e Prioridades

Utilizou-se a função `xTaskCreatePinnedToCore` para fixar a
*Task_Hardware* no núcleo 1 (Application Core) e a *Task_Botao* no
núcleo 0 (Protocol Core). A comunicação entre elas ocorre através de uma
variável de estado compartilhada (*volatile*), sendo assim a resposta ao
comando de quem utiliza o equipamento é praticamente imediata.

# Diagrama Esquemático Simplificado

A montagem no simulador Wokwi utilizou a pinagem padrão da placa ESP32
DevKit V1:

-   **GPIO 2 (Saída):** Conectado ao LED Onboard (Simulando o
    acionamento do Relé).

-   **GPIO 13 (Entrada):** Conectado ao Pushbutton (Configurado em modo
    *Input Pull-up*).

# Resultados da Simulação

O código foi feito no Wokwi. A figura abaixo
demonstra o funcionamento:
-   O **Core 1** mantém o loop de monitoramento ativo, imprimindo o
    status \"Tomada LIGADA\" periodicamente.

-   O **Core 0** intercepta o pressionamento do botão simultaneamente,
    exibe a mensagem de interrupção e altera o estado do sistema.
    
<img width="538" height="595" alt="image" src="https://github.com/user-attachments/assets/10798766-a4a8-4d84-8837-70f0138e186f" />

![Log do Monitor Serial comprovando a execução Multicore: O Core 0
processa o botão enquanto o Core 1 mantém o status do sistema
ativo.]

# Conclusão da Prática

A implementação comprovou que o uso de RTOS permite desacoplar a lógica
de leitura de sensores da lógica de atuação. Diferente de uma abordagem
linear ou tradicional, onde um delay de *debounce* no botão travaria
todo o sistema, a abordagem com Tasks permitiu que o sistema continuasse
responsivo e operando em paralelo nos dois núcleos da ESP32.

# Código Fonte do Firmware (ESP32) {#apendice:codigo}

Abaixo apresenta-se o código completo desenvolvido em C++
(PlatformIO/Arduino) utilizado na simulação.

``` {#lst:codigo caption="Dual-Core para a tomada" label="lst:codigo"}
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define PINO_RELE 2     // LED da placa (Simula a Tomada)
#define PINO_BOTAO 13   // Botao (Simula o usuario)

volatile bool estadoTomada = false;


void TaskControlarHardware(void *pvParameters);
void TaskLerBotao(void *pvParameters);

void setup() {
  Serial.begin(115200);
  pinMode(PINO_RELE, OUTPUT);
  pinMode(PINO_BOTAO, INPUT_PULLUP);

  Serial.println("--- Smart Plug Simples Iniciado ---");


  // 1. aq é a task do hardware, core 1 pois alta prioridade
  xTaskCreatePinnedToCore(
    TaskControlarHardware, "Task_Hardware", 2048, NULL, 2, NULL, 1
  );

  // 2. Task do Usuario, baixa prioridade, core 0
  xTaskCreatePinnedToCore(
    TaskLerBotao, "Task_Botao", 2048, NULL, 1, NULL, 0
  );
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000)); 
}


// TASK 1 cuida do LED (Rele)
// roda no core 1
void TaskControlarHardware(void *pvParameters) {
  for(;;) {
    if (estadoTomada == true) {
      digitalWrite(PINO_RELE, HIGH); 
    } else {
      digitalWrite(PINO_RELE, LOW);  
    }

    if (estadoTomada) {
       Serial.println("[Core 1] Tomada LIGADA - Consumo: 100W");
    }

    vTaskDelay(pdMS_TO_TICKS(1000)); 
  }
}

// TASK 2 Le o botao
// Roda no CORE 0
void TaskLerBotao(void *pvParameters) {
  int ultimoEstado = HIGH; 

  for(;;) {
    int leitura = digitalRead(PINO_BOTAO);

    if (ultimoEstado == HIGH && leitura == LOW) {
      estadoTomada = !estadoTomada; 
      Serial.println("[Core 0] Botao pressionado! Trocando estado...");
      vTaskDelay(pdMS_TO_TICKS(200)); 
    }
    
    ultimoEstado = leitura;
    vTaskDelay(pdMS_TO_TICKS(50)); 
  }
}
```

::: thebibliography

M. Karpagam, S. S. Sahana, S. Sivadharini and S. Soundhariyasri, "Smart
Energy Meter and Monitoring System using Internet of Things (IoT),"
*2023 9th International Conference on Advanced Computing and
Communication Systems (ICACCS)*, Coimbatore, India, 2023, pp. 1827-1830.

S. F. Ahmed, M. S. Alam and S. A. Hoque, "Design and Implementation of
Smart Plug based on IoT," *2019 International Conference on Electrical,
Computer and Communication Engineering (ECCE)*, Cox's Bazar, Bangladesh,
2019, pp. 1-6.

Real Time Engineers Ltd, "FreeRTOS - Market Leading RTOS (Real Time
Operating System) for Embedded Systems with Internet of Things
Extensions," \[Online\]. Disponível em: <https://www.freertos.org/>.
\[Acesso em: 08 dez. 2025\].

Espressif Systems, "ESP32 Series Datasheet," Version 4.5, 2024.
\[Online\]. Disponível em:
<https://www.espressif.com/en/support/documents/technical-documents>.
:::
