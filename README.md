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
como o sistema reage a ruído elétrico em medições contínuas, pois uma das maiores questões com seria como o método se comporta em
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
Uma das principais dúvidas seria como o smart plug se comportaria em
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

Esta prática visa experimentar com a implementação do RTOS (Real Time Operational System), uma ferramenta muito útil para o processamento paralelo de tarefas.

Para a parte I foi desenvolvido um código para detecção de toques por meio do pino capacitivo da ESP32 para ligar um LED, o processo para tal foi feito como uma task atribuída ao núcleo 1 do processador.

Para a parte II foi desenvolvido um código para o controle da posição de um servo-motor por meio de uma entrada analógica comandada por um potênciometro, além disso, para a visualização dos comandos foi utilizado um display LCD. Em paralelo ao controle da posição do motor, haverá um outro processo que irá acender um LED vermelho para caso o motor esteja em movimento e, caso o contrário, um LED verde irá acender. Isso é feito por meio de uma flag no processo de controle que dita se o motor está ou não se movimentando, daí o segundo processo lê essa flag para decidir qual LED acender, para tal foi implementado a funcionalidade de Mutex. Esta parte junta conceitos como: bibliotecas para servo motores, comunicação serial com diplay LCD e RTOS.

O código da parte 1 se encontra em: /pratica6_proj_embark/Pratica6_parte1/Codigo_pt1
O diagrama do circuito da parte 1 se encontra em: /pratica6_proj_embark/Pratica6_parte1/diagrama_pt1.json

O código da parte 2 se encontra em: /pratica6_proj_embark/Pratica6_parte2/src/Codigo_pt2
O diagrama do circuito da parte 1 se encontra em: /pratica6_proj_embark/Pratica6_parte2/diagrama_pt2.json

Recomenda-se copiar e colar esses códigos deste projeto dentro dos arquivos equivalentes de um projeto de simulação no Wokwi.

# Resultados da Parte 1
Na parte 1 da pratica foi implementado um detector de toque por meio dos pinos capacitivos da ESP32, um LED é utilizado para avaliar se houve ou não um toque. Contudo, como o programa foi desenvolvido no ambiente de Simulação WOKWI, foi utilizado um botão para simular o toque. O processo para esta parte foi implementado como uma task para ser executada pelo núcleo 1. Abaixo há imagens do funcionamento do programa:

<img width="581" height="442" alt="image" src="https://github.com/user-attachments/assets/88c3546c-2c06-44cb-80eb-f837dd4aa666" />

*Não há toque, logo o LED está desligado*

<img width="602" height="432" alt="image" src="https://github.com/user-attachments/assets/0339bd96-ffca-4aa8-86ae-829d870050c3" />

*Foi detectado um toque*

# Resultados da Parte 2
Abaixo segue um esquema da montagem:

<img width="690" height="446" alt="image" src="https://github.com/user-attachments/assets/58ff7a45-749a-4165-b706-7402aa489847" />

Exemplo de funcionamento - Motor em movimento:
<img width="576" height="451" alt="image" src="https://github.com/user-attachments/assets/981ec6ac-ffc0-4b2f-b852-b733ded5a294" />

*Como o motor não está no angulo de referência, ele está movimento e então o LED vermelho está aceso*

Exemplo de funcionamento - Motor em posição:
<img width="662" height="445" alt="image" src="https://github.com/user-attachments/assets/b987558f-8372-4de8-ba65-3656c23ff18b" />

*Como o motor está no angulo de referência, ele está parado e então o LED verde está aceso*
# Conclusão da Prática

A implementação comprovou que o uso de RTOS permite desacoplar a lógica
de leitura de sensores da lógica de atuação. Diferente de uma abordagem
linear ou tradicional, onde um delay de *debounce* no botão travaria
todo o sistema, a abordagem com Tasks permitiu que o sistema continuasse
responsivo e operando em paralelo nos dois núcleos da ESP32.

## Apêndice
# Código Fonte do Firmware (ESP32)

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
