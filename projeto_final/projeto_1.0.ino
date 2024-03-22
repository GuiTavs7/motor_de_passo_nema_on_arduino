// PROJETO: PERSOL INC
// AUTOR: GUILHERME TAVARES PINHEIRO
// DATA: 14/03/2024
// OBJETIVO: MOVIMENTAR MOTOR DE PASSO NEMA COM DUAS MEDIDAS (LARGURA PARA SENTIDO HORÁRIO E ALTURA PARA SENTIDO ANTI-HORÁRIO) ATRAVÉS DE LEITOR DE CÓDIGO DE BARRAS

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ATRIBUINDO O NÚMERO DOS PINOS A SUAS RESPECTIVAS FUNÇÕES

const int pino_pulso = 8; //CONFIGURA O PINO DO PULSO / PASSO (PRECISÃO)
const int pino_direcao = 9; // CONFIGURA O PINO DA DIREÇÃO (HORÁRIO / ANTI-HORÁRIO)
const int enable_pin = 13; // CONFIGURA O PINO ENA
const int botao_pin = 2; // CONFIGURA O PINO DO BOTÃO

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// CRIAÇÃO DAS VARIÁVEIS

long codigos[] = {15756719, 15756540, 15755617, 15755656, 15755512}; //Vetor de longs para os códigos de barras
float larguras[] = {2.0, 4.0, 8.0, 16.0, 32.0}; //Vetor de larguras correspondentes às peças conforme os códigos de barras
float alturas[] = {1.0, 3.0, 7.0, 15.0, 35.0}; //Vetor de alturas correspondentes às peças conforme os códigos de barras
int num_pedidos = sizeof(codigos) / sizeof(codigos[0]); // Variável para ser utilizada no limite do loop "for", para fazer o programa iterar até a quantidade necessária de códigos de barras
int qtd_voltas; // Variável para armazenar o número de voltas, que será de acordo com o valor do float larguras ou do float alturas
int segundo_ciclo = 0;

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// FUNÇÃO SETUP - CONFIGURAÇÕES INICIAIS

void setup() {
  pinMode(pino_pulso, OUTPUT);   //DEFINE PINO PULSO COMO SAÍDA
  pinMode(pino_direcao, OUTPUT); // DEFINE PINO DIRECAO COMO SAÍDA
  pinMode(enable_pin, OUTPUT);   // DEFINE PINO ENABLE COMO SAÍDA
  pinMode(botao_pin, INPUT_PULLUP); // DEFINE O PINO DO BOTÃO COMO ENTRADA E COM UM RESISTOR INTERNO PARA EVITAR LEITURAS ALEATÓRIAS
  Serial.begin(9600);            // Inicializa a comunicação serial com a velocidade de 9600 baud
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// FUNCÃO LOOP - LOOP PRINCIPAL DO PROGRAMA

void loop() {

  //PRIMEIRO CICLO - LARGURAS

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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
      Serial.println(larguras[i]); // Printamos a medida que o motor irá movimentar para não nos perdermos

      // GIRANDO O MOTOR NO SENTIDO HORÁRIO
      digitalWrite(pino_direcao, HIGH); // Atribui HIGH ao pino de direção -> HIGH = HORÁRIO
      delayMicroseconds(1000); //Atraso de 1 segundo
      Serial.println("Girando o motor no sentido horário"); // Printa no monitor serial em qual sentido o motor está girando
      segundo_ciclo = 0; // REINICIA O ESTADO DO BOTÃO PARA 0 - IMPORTANTE PARA COMEÇAR O SEGUNDO LOOP (ALTURA)

      for(int x = 0; x < qtd_voltas * 800; x++){ // Loop For para girar o motor no sentido horário

        digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
        delayMicroseconds(400); // Atraso para regular a velocidade do motor (QUANTO MENOR ESSE ATRASO MAIS RÁPIDO O MOTOR GIRA!!!)
        digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

      }  //FIM DO LOOP FOR PARA SENTIHO HORÁRIO
      
    }    // FIM DO IF
    
  } // FIM DO LOOP FOR QUE ITERA ATÉ O NÚMERO DE  PEDIDOS (CÓDIGOS DE BARRAS)
  

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


  // SEGUNDO CICLO - SÓ PROSSEGUE SE O BOTÃO FOR PRESSIONADO!


  while(segundo_ciclo == 0){

    // O programa só continua se o botão for pressionado
    if (digitalRead(botao_pin) == LOW) { 

      Serial.println("BOTÃO PRESSIONADO: Iniciando segundo ciclo!");

      // LOOP FOR PARA ITERAR ATÉ O NÚMERO DE PEDIDOS

      for (int i = 0; i < num_pedidos; i++) {

        // O PROGRAMA SÓ INICIA SE O CÓDIGO DE BARRAS DIGITADO BATER COM ALGUM DOS CÓDIGOS NO VETOR

        if (codigos[i] == codigo) { 

          qtd_voltas = alturas[i]; // Na segunda volta armazenamos na quantidade de voltas o valor do vetor ALTURAS
          Serial.println(alturas[i]); // Printa o valor correspondente ao índice altura = quantidade de voltas do motor

          // GIRANDO O MOTOR NO SENTIDO ANTI-HORÁRIO
          digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direção -> LOW = ANTI-HORÁRIO
          delayMicroseconds(1000); //Atraso de 1 segundo
          Serial.println("Girando o motor no sentido anti-horário"); // Printa no monitor serial em qual sentido o motor está girando

          for(int x = 0; x < qtd_voltas * 800; x++){ // Loop For para girar o motor no sentido horário

            digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
            delayMicroseconds(400); // Atraso para regular a velocidade do motor (QUANTO MENOR ESSE ATRASO MAIS RÁPIDO O MOTOR GIRA!!!)
            digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

          } //FIM DO LOOP FOR PARA SENTIHO HORÁRIO
          
        } // FIM DO IF
        
      } // FIM DO LOOP FOR QUE ITERA ATÉ O NÚMERO DE  PEDIDOS (CÓDIGOS DE BARRAS)

      break;
      
    } // FIM DO IF DO BOTÃO

    delay(1000); // Pequeno atraso para evitar detecção falsa de pressionamento do botão
    
  } // FIM DO LOOP WHILE DO SEGUNDO CICLO

segundo_ciclo = 1; //ATRIBUI 1 AO SEGUNDO CICLO PRA NÃO FICAR PRESO NO BOTÃO - VOLTAR AO PRIMEIRO CICLO (LARGURA)

}  // FIM FA FUNÇÃO VOID LOOP

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
