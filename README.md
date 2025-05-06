# Barramentos Industriais  
**IFRS – Campus Farroupilha**  
**Engenharia de Controle e Automação**  
**Professor Gustavo Künzel**
**Aluno Pedro Henrique de Assumpção**

# Projeto 01 – Dispositivo Modbus RTU EIA-485

Utilizar conceitos de Modbus RTU e comunicação serial UART para desenvolver uma aplicação compatível com o protocolo Modbus RTU.  
O objetivo é compreender como deve ser feita a programação do protocolo nos dispositivos.  
Nas I/Os do Arduino serão conectados potenciômetros e LEDs, de acordo com a aplicação.

---

## Material necessário
- Microcontrolador com porta Serial (Arduino Uno, Mega, NodeMCU, ESP32);
- Componentes diversos para os I/Os;
- Módulo conversor UART – EIA-485;
- Módulo conversor USB – EIA-485;
- PC, DEV-C++, IDE, bibliotecas para envio e recebimento de comunicação serial;
- Códigos desenvolvidos em outras disciplinas e atividades.

---

## Projetos e sugestões
- Trabalho individual, não podendo haver dois trabalhos similares.

### Projetos com montagem física (Requisito 1)
- **Módulo de saídas digitais (8 a 16 saídas)**:  
  - Representadas por LEDs;  
  - Programa do PC deve permitir acionamento de uma ou mais saídas no mesmo comando;  
  - **Função Modbus**: `0x0F Write Multiple Coils`.

- **Módulo de entradas digitais (8 a 16 entradas)**:  
  - Representadas por chaves ou DIP switches;  
  - Programa do PC deve permitir leitura de uma ou mais entradas;  
  - **Função Modbus**: `0x02 Read Discrete Inputs`.

- **Módulo de entradas analógicas (4 a 8 entradas)**:  
  - Representadas por potenciômetros;  
  - Programa do PC deve permitir leitura de uma ou mais entradas;  
  - **Função Modbus**: `0x04 Read Input Registers`.

- **Módulo de saídas analógicas (4 a 8 saídas PWM)**:  
  - Representadas por LEDs;  
  - Programa do PC deve permitir mudança do duty cycle de uma ou mais saídas;  
  - **Função Modbus**: `0x10 Write Multiple Registers`.

---

### Projetos sem montagem física (troca de Requisito 1 para mais uma função implementada)

- **Controle de Temperatura de Estufa com DHT-11**:
  - Programa do PC deve alterar o setpoint e o modo (manual/automático);
  - **Funções Modbus**: `0x05 Write Single Coil` + `0x10 Write Multiple Registers`;
  - Código base: Bancada Estufa Arduino.

- **Controle de Velocidade de Motor DC**:
  - Programa do PC deve alterar o setpoint de velocidade e modo (manual/automático);
  - **Funções Modbus**: `0x05 Write Single Coil` + `0x06 Write Single Register`;
  - Código base: Bancada Motor DC com Encoder.

- **Balança com célula de carga e módulo HX711**:
  - Leitura do peso atual e setar tara;
  - **Funções Modbus**:  
    - `0x03 Read Multiple Coils` (ler peso);  
    - `0x06 Write Single Register` (setar tara);
  - Código base: Bancada Balança HX711.

- **Outros projetos**:  
  - Permitidos desde que incluam um comando complexo (`0x0F`, `0x10`, `0x04`, `0x02`, `0x03`) e um comando simples (`0x05`, `0x06`).

---

## Requisitos para o Servidor / Escravo (Arduino, ESP ou outro microcontrolador)

### 1. Montagem física
- Desenhar esquema elétrico em ferramenta gráfica;
- Montagem em protoboard;
- Implementar usando a serial USB (RS485 opcional inicialmente);
- Duas chaves para ajuste de endereço (1 a 4).

### 2. Funcionamento Modbus RTU
- Aceitar comandos apenas para o endereço configurado ou broadcast;
- Implementar funções conforme a especificação Modbus;
- Atualizar entradas/saídas conforme requisição;
- Responder adequadamente às requisições e exceções (função inválida, registrador inválido, dado inválido);
- Implementar verificação e cálculo de CRC.

---

## Requisitos para o Cliente / Mestre (PC com projeto no DEV-C++)

### 3. Funcionamento do Programa Mestre
- Ter menu de opções para interação com o escravo;
- Validar funções implementadas (ler entradas/escrever saídas);
- Testar e exibir validade do CRC nos quadros recebidos;
- Identificar e exibir erros:
  - **Timeout**: sem resposta do escravo;
  - **Exceções**: função inválida, registrador inválido, dado inválido;
  - **CRC inválido**.

- **Observação**: Pode-se utilizar Modbus Poll ou Modbus Simulator para testar compatibilidade.

---

### 4. Entrega e Apresentação
- **Data da apresentação**: 06/05/2025
- **Nota**: 7 pontos de N1
- **Apresentação**:
  - Aproximadamente 15 minutos + 5-10 minutos para comentários;
  - Apresentação do projeto no TinkerCAD;
  - Explicação da função implementada (quadro RTU e bytes/campos);
  - Explicação de escolhas de projeto;
  - Apresentação das seções principais do código (escravo e mestre);
  - Enviar ZIP no Moodle contendo:
    - Código do mestre;
    - Código do escravo;
    - PDF da apresentação.

---

## Tabela de Requisitos
| Req 1 | Req 2 | Req 3 | Req 4 | Nota |
|:-----:|:-----:|:-----:|:-----:|:----:|
|   1   |   3   |   2   |   1   |  7   |

# APRESENTAÇÃO

**Curso:** Engenharia de Controle e Automação  
**Instituição:** IFRS – Campus Farroupilha  
**Disciplina:** Barramentos Industriais  
**Aluno:** Pedro Henrique de Assumpção  
**Professor:** Gustavo Künzel  
**Data:** 06/05/2025

---

## Índice

- Introdução  
- Objetivos  
- Justificativa  
- Fundamentação Teórica  
- Proposta  
- Testes  
- Desafios  
- Referências  

---

## Introdução

Utilizar conceitos de Modbus RTU e comunicação serial UART para desenvolver uma aplicação compatível com o protocolo Modbus RTU.  
O objetivo é compreender como deve ser feita a programação do protocolo nos dispositivos.  
Nas I/Os do Arduino serão conectados potenciômetros e LEDs, de acordo com a aplicação.

---

## Objetivos

- Módulo de saídas digitais (8 a 16 saídas);  
- Representadas por LEDs;  
- Programa do PC deve permitir acionamento de uma ou mais saídas no mesmo comando;  
- Função Modbus: 0x0F Write Multiple Coils.  

---

## Justificativa

Desenvolver um sistema que permita o controle de uma matriz de 64 LEDs via protocolo Modbus RTU utilizando Arduino.  
Além disso, busca-se compreender a implementação prática do protocolo e sua aplicabilidade em sistemas embarcados.

---

## Fundamentação Teórica

### Protocolo Modbus RTU

Modbus RTU é um protocolo de comunicação serial mestre-escravo que utiliza RS-485 para transmissão de dados.  
Ele permite a comunicação entre dispositivos de forma eficiente e é padrão em sistemas industriais.

### Comunicação Serial e RS-485

A comunicação serial RS-485 permite a transmissão de dados em longas distâncias com alta imunidade a ruídos.  
É ideal para aplicações industriais e é compatível com o protocolo Modbus RTU.

### Write Multiple Coils (0x0F)

[Slide reservado para estrutura da função 0x0F]

---

## Proposta

### Componentes do Sistema

- Arduino Uno  
- Módulo RS-485 (MAX485)  
- Matriz de LEDs 8x8  
- Comunicação via serial com protocolo Modbus RTU  

### Matriz de LEDs

A matriz 8x8 possui 64 LEDs dispostos em linhas e colunas.  
É controlada por registros que determinam quais LEDs devem ser acesos, permitindo diversos padrões visuais.

---

## Fluxograma explicado

A seguir está descrito o passo-a-passo detalhado da comunicação entre o software Mestre e o dispositivo Escravo, usando o protocolo Modbus RTU para acionar uma matriz de LEDs:

1. **Usuário abre o programa MESTRE**  
   O software escrito em C é executado no terminal Linux. Ele simula o papel de mestre Modbus RTU.

2. **Navega no menu do terminal**  
   O menu exibe opções interativas. O usuário pode selecionar funções específicas, como enviar comandos ou testar comunicação.

3. **Seleciona a opção 1**  
   Escolha do comando para envio de um novo padrão de LEDs (função Modbus 0x0F - Write Multiple Coils).

4. **Informa a posição de xadrez desejada (ex: D4)**  
   A entrada do usuário é convertida para um padrão de 16 bits, refletindo o estado desejado dos LEDs.

5. **Gera os bytes de dados (`resultadoA` e `resultadoB`)**  
   O padrão binário é dividido em dois bytes. Cada bit representa o estado de um LED (aceso ou apagado).

6. **Monta o quadro Modbus RTU**  
   O quadro contém: endereço do escravo, código da função, endereço inicial dos coils, quantidade, dados e CRC.

7. **Calcula o CRC16 e adiciona ao quadro**  
   O sistema usa uma função CRC (implementada em `crc16.h`) para garantir a integridade da mensagem.

8. **Envia via RS-485**  
   O quadro completo é transmitido pela interface serial `/dev/ttyUSB0`, utilizando padrão RS-485.

9. **Inicia timeout com `millis_now()`**  
   Um contador é ativado para aguardar resposta dentro de tempo limite, evitando travamentos.

10. **ESCRAVO (Arduino) valida o quadro**  
    O Arduino confere o endereço, função e CRC. Apenas responde se o quadro for válido e destinado a ele.

11. **Escravo interpreta os dados e atualiza LEDs**  
    Com base nos bytes recebidos, os LEDs da matriz 8x8 são ligados conforme o padrão solicitado.

12. **Escravo monta e envia resposta Modbus**  
    Um novo quadro é enviado ao mestre confirmando que os dados foram processados corretamente.

13. **Mestre recebe a resposta e verifica o CRC**  
    A resposta é validada novamente, garantindo que não houve corrupção na transmissão.

14. **Mestre exibe mensagem de sucesso ("OK")**  
    Caso o CRC seja válido, o terminal informa que a operação foi bem-sucedida.

15. **Tratamento de falhas**  
    Em caso de erro no CRC ou ausência de resposta dentro do timeout, o sistema exibe erro e permite nova tentativa sem encerrar o programa.

---

## Testes

### Procedimentos de Teste

Foram realizados testes para verificar a correta comunicação entre mestre e escravo, bem como o funcionamento da matriz de LEDs.  
Os testes confirmaram a eficácia da implementação.

### Rotinas de Falhas

O menu do Mestre trata erros como CRC inválido e ausência de resposta do escravo.  
Em caso de falha, o sistema exibe mensagens de erro e permite nova tentativa sem travar a execução.

---

## Desafios

### Troca do ESP32 pelo Arduino

O ESP32 apresentou instabilidade com Modbus RTU e acionamento de LEDs (sinais de 3,3V).  
Foi substituído pelo Arduino (5V), garantindo confiabilidade na comunicação e controle dos LEDs.

### Melhorias no Programa Mestre

O programa poderia ter variáveis dinâmicas como endereço do escravo e porta serial.  
Tornar essas variáveis configuráveis aumentaria a flexibilidade e reutilização do código.

### Endereçamento e Broadcast

Implementado suporte a múltiplos escravos com endereços de 1 a 4.  
Broadcast também foi testado com cuidado para evitar respostas no barramento, prevenindo colisões.

---

## Referências

- MODBUS ORGANIZATION. *Modbus Application Protocol Specification V1.1b3*. [S.l.]: 2012. Disponível em: https://modbus.org. Acesso em: 5 maio 2025.  
- DE ASSUMPÇÃO, Pedro Henrique. *ModbusRTU com Arduino*. GitHub, 2024. Disponível em: https://github.com/pedrohdea/ModbusRTU. Acesso em: 5 maio 2025.  
- ARDUINO. *Arduino Uno Rev3 – Datasheet*. [S.l.]: 2020. Disponível em: https://store.arduino.cc/products/arduino-uno-rev3. Acesso em: 5 maio 2025.  

