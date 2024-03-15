// ATRIBUINDO O NÚMERO DOS PINOS A SUAS RESPECTIVAS FUNÇÕES

const int pino_pulso = 8;           //CONFIGURA O PINO DO PULSO / PASSO (PRECISÃO)
const int pino_direcao = 9;         // CONFIGURA O PINO DA DIREÇÃO (HORÁRIO / ANTI-HORÁRIO)
const int enable_pin = 13;          // CONFIGURA O PINO ENA

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// CRIAÇÃO DAS VARIÁVEIS - VETORES E ITERÁVEIS

long codigos[] = {15756719, 15756540, 15755617};          //Vetor de longs para os códigos de barras
float larguras[] = {4.0, 8.0, 16.0};                      //Vetor de larguras correspondentes às peças conforme os códigos de barras
float alturas[] = {3.0, 5.0, 15.0};                       //Vetor de alturas correspondentes às peças conforme os códigos de barras
int num_pedidos = sizeof(codigos) / sizeof(codigos[0]);   // Variável para ser utilizada no limite do loop "for", para fazer o programa iterar até a quantidade necessária de códigos de barras     
int posicao_atual;
int medida;

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// FUNÇÃO SETUP - CONFIGURAÇÕES INICIAIS - ENTRADA, SAÍDA E COMUNICAÇÃO SERIAL

void setup() {

  pinMode(pino_pulso, OUTPUT);               // DEFINE PINO PULSO COMO SAÍDA
  pinMode(pino_direcao, OUTPUT);             // DEFINE PINO DIRECAO COMO SAÍDA
  pinMode(enable_pin, OUTPUT);               // DEFINE PINO ENABLE COMO SAÍDA
  Serial.begin(9600);                        // INICIALIZA A COMUNICAÇÃO SERIAL COM UMA TAXA DE 9600 BAUD

} // FIM DA FUNÇÃO SETUP

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 0.1) FUNÇÃO ESCOLHE SENTIDO HORÁRIO OU ANTI-HORÁRIO DE ACORDO COM A COMPARAÇÃO ENTRE A POSIÇÃO ATUAL (MEDIDA ANTERIOR) E A NOVA ETIQUETA !!! 

void sentido_rotacao(int posicao_atual, int medida) {

  if (posicao_atual < medida){

    digitalWrite(pino_direcao, HIGH); // Atribui HIGH ao pino de direção -> HIGH = HORÁRIO
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("\n MOTOR GIRANDO NO SENTIDO HORÁRIO \n"); // Printa no monitor serial em qual sentido o motor está girando

  }
  else{

    digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direção -> LOW = ANTI-HORÁRIO
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("\n MOTOR GIRANDO NO SENTIDO ANTI-HORÁRIO \n"); // Printa no monitor serial em qual sentido o motor está girando

  }
  
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 0.2) FUNÇÃO DE GIRO DO MOTOR - MOVIMENTA O MOTOR 1 CICLO (1 CICLO = 800 PULSOS)

void gira_motor(int pino_pulso){

  for (int i = 0; i < 800; i++){     

    digitalWrite(pino_pulso, HIGH);
    delayMicroseconds(4000);
    digitalWrite(pino_pulso, LOW);
  }
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void loop() { // INÍCIO DA FUNÇÃO LOOP

    medida = 0; // ZERA O VALOR DA MEDIDA PARA NÃO CONTINUAR COM O VALOR ANTERIOR

    // Espera até que o usuário insira o número do código de barras
    Serial.println("\n Insira o número do código de barras: ");

    // O programa não continua até que o usuário digite o número de voltas
    while (!Serial.available()); 

    long medida = Serial.parseInt(); // Variável medida recebe o que o usuário inseriu na porta serial

    // Verifica se a leitura é um número de código de barras válido
    if (medida != 0) {
      
        sentido_rotacao(posicao_atual, medida);
        gira_motor(pino_pulso);

        posicao_atual = medida; // POSICAO ATUAL RECEBE O VALOR DA MEDIDA LIDA PARA PRÓXIMAS COMPARAÇÕES

    } else {

        // Caso contrário, limpa o buffer de entrada serial
        while (Serial.available() > 0) {
            Serial.read();
        }
        // E exibe uma mensagem de erro
        Serial.println("\n Código de barras inválido. Por favor, insira um número válido.");
    }

} // FIM DA FUNÇÃO LOOP