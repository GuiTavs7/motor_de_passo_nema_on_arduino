//ATRIBUINDO O NÚMERO DOS PINOS A SUAS RESPECTIVAS FUNÇÕES

const int pino_pulso = 8; //CONFIGURA O PINO DO PULSO / PASSO (PRECISÃO)
const int pino_direcao = 9; // CONFIGURA O PINO DA DIREÇÃO (HORÁRIO / ANTI-HORÁRIO)
const int enable_pin = 13; // CONFIGURA O PINO ENA
const int botao_pin = 2;  //CONFIGURA O PINO DO BOTÃO DE FIM DE CURSO

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//CRIAÇÃO DAS VARIÁVEIS

long codigos[] = {15756719, 15756540, 15755617}; //Vetor de longs para os códigos de barras
float larguras[] = {2.0, 4.0, 8.0}; //Vetor de larguras correspondentes às peças conforme os códigos de barras
float alturas[] = {3.0, 5.0, 9.0}; //Vetor de alturas correspondentes às peças conforme os códigos de barras
int num_pedidos = sizeof(codigos) / sizeof(codigos[0]); // Variável para ser utilizada no limite do loop "for", para fazer o programa iterar até a quantidade necessária de códigos de barras
int qtd_voltas; // Variável para armazenar o número de voltas, que será de acordo com o valor do float largura
int contador = 0; // Inicializa contador com 0

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// FUNÇÃO SETUP - CONFIGURAÇÕES INICIAIS

void setup() {
  pinMode(pino_pulso, OUTPUT);   //DEFINE PINO PULSO COMO SAÍDA
  pinMode(pino_direcao, OUTPUT); // DEFINE PINO DIRECAO COMO SAÍDA
  pinMode(enable_pin, OUTPUT);   // DEFINE PINO ENABLE COMO SAÍDA
  pinMode(botao_pin, INPUT_PULLUP);
  Serial.begin(9600);            // Inicializa a comunicação serial com a velocidade de 9600 baud
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// FUNCÃO LOOP - LOOP PRINCIPAL DO PROGRAMA

void loop() {

  digitalWrite(enable_pin, LOW);

  for(contador = 0; contador < num_pedidos * 2; contador++){ // REPETE A LEITURA DE ETIQUETAS ATÉ A QUANTIDADE DE PEDIDOS (*2 PARA BATER O CÁLCULO)

    Serial.println(contador); // EXIBE O VALOR DE CONTADOR NO SERIAL PRINT 

    // Espera até que o usuário insira o número do código de barras
    Serial.println("Insira o número do código de barras: ");

    // O programa não continua até que o usuário digite o número de voltas
    while (!Serial.available()); 

    long codigo = Serial.parseInt(); // Variável código recebe o que o usuário inseriu na porta serial

    // LOOP FOR PARA ITERAR ATÉ O NÚMERO DE PEDIDOS

    for (int i = 0; i < num_pedidos; i++) {

      // O PROGRAMA SÓ INICIA SE O CÓDIGO DE BARRAS DIGITADO BATER COM ALGUM DOS CÓDIGOS NO VETOR

      if (codigos[i] == codigo) { 

        // Atribui a quantidade de voltas correspondente ao que foi lido no vetor de largura
        // 1 volta = 1 * 800 = 800 no Loop For -> 2 voltas = 2 * 800 = 1600 no Loop For 
        qtd_voltas = larguras[i];

        // GIRANDO O MOTOR NO SENTIDO HORÁRIO
        digitalWrite(pino_direcao, HIGH); // Atribui HIGH ao pino de direção -> HIGH = HORÁRIO
        delayMicroseconds(1000); //Atraso de 1 segundo
        Serial.println("Girando o motor no sentido horário"); // Printa no monitor serial em qual sentido o motor está girando

        for(int x = 0; x < qtd_voltas * 800; x++){ // Loop For para girar o motor no sentido horário

         digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
          delayMicroseconds(400); // Atraso para regular a velocidade do motor (QUANTO MENOR ESSE ATRASO MAIS RÁPIDO O MOTOR GIRA!!!)
          digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

        } //FIM DO LOOP FOR PARA SENTIDO HORÁRIO

      } // FIM DO IF
    
    } // FIM DO LOOP FOR QUE ITERA ATÉ O NÚMERO DE  PEDIDOS (CÓDIGOS DE BARRAS)

  } // FIM DO LOOP FOR QUE CONTA 3 LEITURAS

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  //QUANDO CONTADOR CHEGAR AO NÚMERO DE PEDIDOS, O MOTOR IRÁ PARA A POSIÇÃO INICIAL AUTOMATICAMENTE E LENTAMENTE

  if (contador >= num_pedidos * 2){ 

    // GIRANDO O MOTOR NO SENTIDO ANTI-HORÁRIO
    digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direção -> LOW = ANTI-HORÁRIO
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("Girando o motor no sentido anti-horário"); // Printa no monitor serial em qual sentido o motor está girando

    for(int x = 0; x < num_pedidos * 800; x++){ // Loop For para girar o motor no sentido anti-horário

      
      int estado_botao = digitalRead(botao_pin); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

      if(estado_botao == LOW){ // SE O BOTÃO DE FIM DE CURSO FOR PRESSIONADO:

        Serial.println("BOTÃO PARADA TOTAL PRESSIONADO!!!");

        digitalWrite(enable_pin, HIGH);

        x = num_pedidos * 800; // SE O BOTÃO FOR PRESSIONADO, X JÁ RECEBE O VALOR DA CONDIÇÃO DO FOR PARA JÁ SAIR FORA DO LOOP E VOLTAR AO INÍCIO DO CÓDIGO!!!!

      }

      digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
      delayMicroseconds(2000); // Atraso para regular a velocidade do motor (QUANTO MENOR ESSE ATRASO MAIS RÁPIDO O MOTOR GIRA!!!)
      digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

    } //FIM DO LOOP FOR PARA SENTIDO HORÁRIO

    contador = 0;

  }

}  

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------