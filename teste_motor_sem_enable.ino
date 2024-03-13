const int pino_pulso = 8; //CONFIGURA O PINO DO PULSO / PASSO (PRECISÃO)
const int pino_direcao = 9; // CONFIGURA O PINO DA DIREÇÃO (HORÁRIO / ANTI-HORÁRIO)

// FUNÇÃO SETUP - CONFIGURAÇÕES INICIAIS

void setup() {
  pinMode(pino_pulso, OUTPUT); //DEFINE PINO PULSO COMO SAÍDA
  pinMode(pino_direcao, OUTPUT); // DEFINE PINO DIRECAO COMO SAÍDA
}

// -------------------------------------------------------------------

// FUNCÃO LOOP - LOOP PRINCIPAL DO PROGRAMA

void loop() {

  digitalWrite(pino_direcao, LOW); // GIRA O MOTOR EM UM SENTIDO -> HIGH E LOW = HORÁRIO E ANTI-HORÁRIO
  delayMicroseconds(1000); //ATRASO

  for(int x = 0; x < 800; x++){ // DEFINA A PRECISÃO DE PASSOS NESSE LOOP - QUANTO MAIOR A CONDIÇÃO MAIS PRECISO

    digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
    delayMicroseconds(400); // ATRASO PARA OBSERVAR O MOVIMENTO
    digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA
  }

  delay(1000); // INTERVALO ENTRE INVERSÃO DE SENTIDO

  digitalWrite(pino_direcao, HIGH); // GIRA O MOTOR NO SENTIDO INVERSO
  delayMicroseconds(100); // ATRASO

  for(int y = 0; y < 800; y++){ // DEFINA A PRECISÃO DE PASSOS NESSE LOOP - QUANTO MAIOR A CONDIÇÃO MAIS PRECISO

    digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
    delayMicroseconds(400); // ATRASO PARA OBSERVAR O MOVIMENTO
    digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA
  }

  delay(1000); // ATRASO
}