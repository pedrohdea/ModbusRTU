#define DATA_PIN   4 // SER
#define CLOCK_PIN  6 // SRCLK
#define LATCH_PIN 5 // RCLK

#define BAUDRATE   9600

// Envia dois bytes para os shift registers (MSB primeiro)
void escreverShiftRegister(uint16_t valor) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, (valor >> 8) & 0xFF); // byte mais significativo
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, valor & 0xFF);        // byte menos significativo
  digitalWrite(LATCH_PIN, HIGH);
}

uint16_t gerarMapaUnitario(const char *comando) {
  if (!comando || strlen(comando) != 2)
    return 0;

  char col = toupper(comando[0]);
  char lin = comando[1];

  uint8_t coluna = 0;
  uint8_t linha  = 0;

  if (col >= 'A' && col <= 'H')
    coluna = (1 << (col - 'A'));

  if (lin >= '1' && lin <= '8')
    linha = (1 << (lin - '1'));

  coluna = ~coluna;  // inverte todos os bits da coluna

  return ((uint16_t)linha << 8) | coluna;
}

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  Serial.begin(BAUDRATE);

  // Teste inicial: piscar todos os 16 bits
  escreverShiftRegister(0xFFFF);
  delay(500);
  escreverShiftRegister(0x0000);
  delay(500);
}

void loop() {
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n'); // Lê até encontrar uma quebra de linha
    comando.trim(); // Remove espaços e quebras extras

    uint16_t valor = gerarMapaUnitario(comando.c_str());
    escreverShiftRegister(valor);
  }
}

