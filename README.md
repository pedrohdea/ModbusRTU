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

# Passo-a-passo detalhado do projeto ModbusRTU com matriz de LEDs

1. **Usuário abre o programa MESTRE**  
   O software em C para Linux é executado no terminal pelo usuário.

2. **Navega no menu do terminal**  
   Um menu interativo com opções de controle da matriz de LEDs é exibido.

3. **Seleciona a opção 1**  
   O usuário escolhe a função de envio de comando para acender LEDs.

4. **Informa a posição de xadrez desejada (ex: D4)**  
   O programa converte essa posição em um padrão de bits correspondente aos LEDs que devem acender.

5. **Gera os bytes de dados dos coils (`resultadoA` e `resultadoB`)**  
   O padrão de 16 bits é dividido em dois bytes, representando os estados dos LEDs.

6. **Monta o quadro Modbus RTU (função 0x0F)**  
   O mestre constrói o quadro com endereço do escravo, função, quantidade de coils e dados.

7. **Calcula o CRC16 e adiciona ao quadro**  
   Um código de verificação (CRC) é gerado usando a biblioteca `crc16.h` e anexado ao final do quadro.

8. **Envia o quadro pela interface serial RS-485**  
   O mestre transmite o quadro pela porta `/dev/ttyUSB0` para o Arduino escravo.

9. **Inicia contagem de tempo (timeout)**  
   A função `millis_now()` é usada para garantir que a resposta do escravo chegue em tempo hábil.

10. **ESCRAVO (Arduino) recebe e valida o quadro**  
    O Arduino verifica o endereço, função e CRC para confirmar se o quadro é válido e para ele.

11. **Escravo interpreta os dados e atualiza os LEDs**  
    Os bytes recebidos (`resultadoA` e `resultadoB`) são usados para acender os LEDs correspondentes na matriz 8x8.

12. **Escravo monta e envia resposta Modbus**  
    O Arduino responde com um quadro de confirmação contendo os dados esperados.

13. **Mestre recebe a resposta e verifica o CRC**  
    O software mestre valida a integridade da resposta com novo cálculo de CRC.

14. **Mestre exibe mensagem de sucesso (“OK”)**  
    Se a resposta for válida, o terminal exibe que os LEDs foram atualizados com sucesso.

15. **(Opcional) Tratamento de falhas**  
    Se o tempo de resposta for excedido ou o CRC estiver errado, o mestre exibe erro e permite nova tentativa.
