#define DATA_PIN   8 //SER
#define CLOCK_PIN  9 //SRCLK
#define LATCH_PIN 10 //RCLK

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
}

// Envia os 2 bytes para os 74HC595 (coils 0-15)
void escreverShiftRegister(uint8_t leds[2]) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, leds[1]); // byte mais significativo
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, leds[0]); // byte menos significativo
  digitalWrite(LATCH_PIN, HIGH);
}

void loop() {
  uint8_t leds[2] = {0b11111111, 0b00000000}; // Liga coils 0–7
  escreverShiftRegister(leds);
  delay(100);

  leds[0] = 0b00000000;
  leds[1] = 0b11111111; // Liga coils 8–15
  escreverShiftRegister(leds);
  delay(1000);
}
