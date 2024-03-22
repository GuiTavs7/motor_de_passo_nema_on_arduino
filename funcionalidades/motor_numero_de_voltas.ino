const int pino_pulso = 8; //CONFIGURA O PINO DO PULSO / PASSO (PRECISÃO)
const int pino_direcao = 9; // CONFIGURA O PINO DA DIREÇÃO (HORÁRIO / ANTI-HORÁRIO)
const int enable_pin = 13; // CONFIGURA O PINO ENA

// FUNÇÃO SETUP - CONFIGURAÇÕES INICIAIS

void setup() {
  pinMode(pino_pulso, OUTPUT); //DEFINE PINO PULSO COMO SAÍDA
  pinMode(pino_direcao, OUTPUT); // DEFINE PINO DIRECAO COMO SAÍDA
  pinMode(enable_pin, OUTPUT);

  // Inicializa a comunicação serial com a velocidade de 9600 baud
  Serial.begin(9600);
}

// -------------------------------------------------------------------

// FUNCÃO LOOP - LOOP PRINCIPAL DO PROGRAMA

void loop() {
  
  // Espera até que o usuário insira o número de voltas na porta serial
  Serial.println("Digite quantas voltas o motor dará nos dois sentidos: ");

  // O programa não continua até que o usuário digite o número de voltas
  while (!Serial.available()); 

  // Lê o número de voltas da porta serial (800 = 1 VOLTA)
  // 1 volta = 1 * 800 = 800 no Loop For -> 2 voltas = 2 * 800 = 1600 no Loop For 
  int num_voltas = Serial.parseInt();

  // GIRANDO O MOTOR NO SENTIDO HORÁRIO

  digitalWrite(pino_direcao, HIGH); // Atribui HIGH ao pino de direção -> HIGH = HORÁRIO
  delayMicroseconds(1000); //Atraso de 1 segundo
  Serial.println("Girando o motor no sentido horário"); // Printa no monitor serial em qual sentido o motor está girando

  for(int x = 0; x < num_voltas * 800; x++){ // Loop For para girar o motor no sentido horário

    digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
    delayMicroseconds(400); // Atraso para regular a velocidade do motor (QUANTO MENOR ESSE ATRASO MAIS RÁPIDO O MOTOR GIRA!!!)
    digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA
  }

  delay(1000); // INTERVALO ENTRE INVERSÃO DE SENTIDO

  // -------------------------------------------------------------------------------------------------------------------------------------

  // GIRANDO O MOTOR NO SENTIDO ANTI-HORÁRIO

  digitalWrite(pino_direcao, LOW); // Movimenta o motor no sentido inverso -> LOW = ANTI-HORÁRIO
  delayMicroseconds(1000); // Atraso de 1 segundo
  Serial.println("Girando o motor no sentido anti-horário"); // Printa no monitor serial em qual sentido o motor está girando

  for(int y = 0; y < num_voltas * 800; y++){ // DEFINA A PRECISÃO DE PASSOS NESSE LOOP - QUANTO MAIOR A CONDIÇÃO MAIS PRECISO

    digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
    delayMicroseconds(400); // Atraso para regular a velocidade do motor (QUANTO MENOR ESSE ATRASO MAIS RÁPIDO O MOTOR GIRA!!!)
    digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA
  }

  delay(1000); // Atraso de 1 segundo antes de receber outra entrada de dados da porta serial
}

// OBSERVAÇÕES

// USE O PINO ENA (ENABLE) PARA FORÇAR O MOTOR A TRAVAR

// EXEMPLO:   //digitalWrite(enable_pin, LOW); // PINO ENA - ATIVADO
              //digitalWrite(enable_pin, HIGH); //PINO ENA - DESATIVADO
