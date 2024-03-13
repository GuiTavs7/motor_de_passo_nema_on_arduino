const int pino_pulso = 8; //CONFIGURA O PINO DO PULSO / PASSO (PRECISÃO)
const int pino_direcao = 9; // CONFIGURA O PINO DA DIREÇÃO (HORÁRIO / ANTI-HORÁRIO)
const int enable_pin = 13; // CONFIGURA O PINO ENA - ATIVA OU DESATIVA O MOTOR DE PASSO

// FUNÇÃO SETUP - CONFIGURAÇÕES INICIAIS

void setup() {
  pinMode(pino_pulso, OUTPUT); //DEFINE PINO PULSO COMO SAÍDA
  pinMode(pino_direcao, OUTPUT); // DEFINE PINO DIRECAO COMO SAÍDA
  pinMode(enable_pin, OUTPUT);
}

// -------------------------------------------------------------------

// FUNCÃO LOOP - LOOP PRINCIPAL DO PROGRAMA

void loop() {

  digitalWrite(pino_direcao, LOW); // GIRA O MOTOR EM UM SENTIDO -> HIGH E LOW = HORÁRIO E ANTI-HORÁRIO
  digitalWrite(enable_pin, LOW); // PINO ENA - ATIVADO
  delayMicroseconds(10); //ATRASO

  for(int x = 0; x < 400; x++){ // DEFINA A PRECISÃO DE PASSOS NESSE LOOP - QUANTO MAIOR A CONDIÇÃO MAIS PRECISO

    digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
    delayMicroseconds(1500); // ATRASO PARA OBSERVAR O MOVIMENTO
    digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA
  }

  digitalWrite(enable_pin, HIGH); //PINO ENA - DESATIVADO

  delay(1000); // INTERVALO ENTRE INVERSÃO DE SENTIDO

  digitalWrite(pino_direcao, HIGH); // GIRA O MOTOR NO SENTIDO INVERSO
  digitalWrite(enable_pin, LOW); // PINO ENA - ATIVADO
  delayMicroseconds(10); // ATRASO

  for(int y = 0; y < 800; y++){ // DEFINA A PRECISÃO DE PASSOS NESSE LOOP - QUANTO MAIOR A CONDIÇÃO MAIS PRECISO

    digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
    delayMicroseconds(3000); // ATRASO PARA OBSERVAR O MOVIMENTO
    digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA
  }

  digitalWrite(enable_pin, HIGH); //PINO ENA - DESATIVADO

  delay(1000); // ATRASO
}