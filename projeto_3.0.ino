// PROJETO: PERSOL INC
// AUTOR: GUILHERME TAVARES PINHEIRO
// DATA: 14/03/2024
// OBJETIVO: MOVIMENTAR MOTOR DE PASSO NEMA COM DUAS MEDIDAS (LARGURA PARA SENTIDO HORÁRIO E ALTURA PARA SENTIDO ANTI-HORÁRIO) ATRAVÉS DE LEITOR DE CÓDIGO DE BARRAS

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ATRIBUINDO O NÚMERO DOS PINOS A SUAS RESPECTIVAS FUNÇÕES

const int pino_pulso = 8; //CONFIGURA O PINO DO PULSO / PASSO (PRECISÃO)
const int pino_direcao = 9; // CONFIGURA O PINO DA DIREÇÃO (HORÁRIO / ANTI-HORÁRIO)
const int enable_pin = 13; // CONFIGURA O PINO ENA
const int botao_altura = 3; // CONFIGURA O PINO DO BOTÃO
const int botao_fim_de_curso = 2;  //CONFIGURA O PINO DO BOTÃO DE FIM DE CURSO
const int botao_parada_total = 4; // CONFIGURA O PINO DO BOTÃO DE PARADA TOTAL

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// CRIAÇÃO DAS VARIÁVEIS - VETORES E ITERÁVEIS

long codigos[] = {15756719, 15756540, 15755617}; //Vetor de longs para os códigos de barras
float larguras[] = {4.0, 8.0, 16.0}; //Vetor de larguras correspondentes às peças conforme os códigos de barras
float alturas[] = {3.0, 5.0, 15.0}; //Vetor de alturas correspondentes às peças conforme os códigos de barras
int num_pedidos = sizeof(codigos) / sizeof(codigos[0]); // Variável para ser utilizada no limite do loop "for", para fazer o programa iterar até a quantidade necessária de códigos de barras
int qtd_voltas; // Variável para armazenar o número de voltas, que será de acordo com o valor do float larguras ou do float alturas
int segundo_ciclo = 0;
int contador = 0; // Inicializa contador com 0

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// FUNÇÃO SETUP - CONFIGURAÇÕES INICIAIS - ENTRADA, SAÍDA E COMUNICAÇÃO SERIAL

void setup() {
  pinMode(pino_pulso, OUTPUT);   //DEFINE PINO PULSO COMO SAÍDA
  pinMode(pino_direcao, OUTPUT); // DEFINE PINO DIRECAO COMO SAÍDA
  pinMode(enable_pin, OUTPUT);   // DEFINE PINO ENABLE COMO SAÍDA
  pinMode(botao_altura, INPUT_PULLUP); // DEFINE O PINO DO BOTÃO COMO ENTRADA E COM UM RESISTOR INTERNO PARA EVITAR LEITURAS ALEATÓRIAS
  pinMode(botao_fim_de_curso, INPUT_PULLUP); // DEFINE O BOTÃO FIM DE CURSO COMO ENTRADA E COM RESISTOR INTERNO
  pinMode(botao_parada_total, INPUT_PULLUP); // DEFINE O BOTÃO PARADA TOTAL COMO ENTRADA E COM RESISTOR INTERNO
  Serial.begin(9600);            // Inicializa a comunicação serial com a velocidade de 9600 baud
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// FUNCÃO LOOP - LOOP PRINCIPAL DO PROGRAMA

void loop() {

  digitalWrite(enable_pin, LOW); // HABILITA O MOTOR APÓS UMA POSSÍVEL PARADA TOTAL

  // PRIMEIRA VERIFICAÇÃO DE PARADA TOTAL - ANTES DO PRIMEIRO CICLO

  int estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

  if(estado_botao_parada_total == LOW){ // SE O BOTÃO DE PARADA TOTAL FOR PRESSIONADO:

    Serial.println("BOTÃO PARADA TOTAL PRESSIONADO!!!");

    digitalWrite(enable_pin, HIGH);

  }

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  //PRIMEIRO CICLO - LARGURAS

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  for(contador = 0; contador < num_pedidos * 2; contador++){ // REPETE A LEITURA DE ETIQUETAS ATÉ A QUANTIDADE DE PEDIDOS (*2 PARA BATER O CÁLCULO)

    // SEGUNDA VERIFICAÇÃO DE PARADA TOTAL - DURANTE AS TRÊS PRIMEIRAS LEITURAS

    estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

    if(estado_botao_parada_total == LOW){ // SE O BOTÃO DE PARADA TOTAL FOR PRESSIONADO:

      Serial.println("BOTÃO PARADA TOTAL PRESSIONADO!!!");

      digitalWrite(enable_pin, HIGH);

    }

    // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    Serial.println(contador); // EXIBE O VALOR DE CONTADOR NO SERIAL PRINT 

    // Espera até que o usuário insira o número do código de barras
    Serial.println("Insira o número do código de barras: ");

    // O programa não continua até que o usuário digite o número de voltas
    while (!Serial.available()); 

    long codigo = Serial.parseInt(); // Variável código recebe o que o usuário inseriu na porta serial

    // LOOP FOR PARA ITERAR ATÉ O NÚMERO DE PEDIDOS

    for (int i = 0; i < num_pedidos; i++) {

      // VERIFICAÇÃO DE PARADA TOTAL - 

      estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

      if(estado_botao_parada_total == LOW){ // SE O BOTÃO DE PARADA TOTAL FOR PRESSIONADO:

        Serial.println("BOTÃO PARADA TOTAL PRESSIONADO!!!");

        digitalWrite(enable_pin, HIGH);

      }

      // O PROGRAMA SÓ INICIA SE O CÓDIGO DE BARRAS DIGITADO BATER COM ALGUM DOS CÓDIGOS NO VETOR

      if (codigos[i] == codigo) { 

        // VERIFICAÇÃO DE PARADA TOTAL - 

        estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

        if(estado_botao_parada_total == LOW){ // SE O BOTÃO DE PARADA TOTAL FOR PRESSIONADO:

            Serial.println("BOTÃO PARADA TOTAL PRESSIONADO!!!");

            digitalWrite(enable_pin, HIGH);

        }

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

          // VERIFICAÇÃO DE PARADA TOTAL - 

          estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

          if(estado_botao_parada_total == LOW){ // SE O BOTÃO DE PARADA TOTAL FOR PRESSIONADO:

            Serial.println("BOTÃO PARADA TOTAL PRESSIONADO!!!");

            digitalWrite(enable_pin, HIGH);

            x = qtd_voltas * 800;

          }

          digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
          delayMicroseconds(400); // Atraso para regular a velocidade do motor (QUANTO MENOR ESSE ATRASO MAIS RÁPIDO O MOTOR GIRA!!!)
          digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

        }  //FIM DO LOOP FOR PARA SENTIHO HORÁRIO
      
      }    // FIM DO IF
    
    } // FIM DO LOOP FOR QUE ITERA ATÉ O NÚMERO DE  PEDIDOS (CÓDIGOS DE BARRAS)
  

   // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // TERCEIRA VERIFICAÇÃO DE PARADA TOTAL - ANTES DO SEGUNDO CICLO - ALTURA

    estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

    if(estado_botao_parada_total == LOW){ // SE O BOTÃO DE PARADA TOTAL FOR PRESSIONADO:

      Serial.println("BOTÃO PARADA TOTAL PRESSIONADO!!!");

      digitalWrite(enable_pin, HIGH);

    }

    // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


    // SEGUNDO CICLO - SÓ PROSSEGUE SE O BOTÃO FOR PRESSIONADO!


    while(segundo_ciclo == 0){

      // O programa só continua se o botão for pressionado
      if (digitalRead(botao_altura) == LOW) { 

        // QUARTA VERIFICAÇÃO DE PARADA TOTAL DURANTE SEGUNDO CICLO - ALTURA

        estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

        if(estado_botao_parada_total == LOW){ // SE O BOTÃO DE PARADA TOTAL FOR PRESSIONADO:

          Serial.println("BOTÃO PARADA TOTAL PRESSIONADO!!!");

          digitalWrite(enable_pin, HIGH);

        }

        // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



        Serial.println("BOTÃO PRESSIONADO: Iniciando segundo ciclo!");

        // LOOP FOR PARA ITERAR ATÉ O NÚMERO DE PEDIDOS

        for (int i = 0; i < num_pedidos; i++) {

           // VERIFICAÇÃO DE PARADA TOTAL - 

          estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

          if(estado_botao_parada_total == LOW){ // SE O BOTÃO DE PARADA TOTAL FOR PRESSIONADO:

            Serial.println("BOTÃO PARADA TOTAL PRESSIONADO!!!");

            digitalWrite(enable_pin, HIGH);

           }

          // O PROGRAMA SÓ INICIA SE O CÓDIGO DE BARRAS DIGITADO BATER COM ALGUM DOS CÓDIGOS NO VETOR

          if (codigos[i] == codigo) { 

            qtd_voltas = alturas[i]; // Na segunda volta armazenamos na quantidade de voltas o valor do vetor ALTURAS
            Serial.println(alturas[i]); // Printa o valor correspondente ao índice altura = quantidade de voltas do motor

            // GIRANDO O MOTOR NO SENTIDO ANTI-HORÁRIO
            digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direção -> LOW = ANTI-HORÁRIO
            delayMicroseconds(1000); //Atraso de 1 segundo
            Serial.println("Girando o motor no sentido anti-horário"); // Printa no monitor serial em qual sentido o motor está girando

            for(int x = 0; x < qtd_voltas * 800; x++){ // Loop For para girar o motor no sentido horário

              // QUINTA VERIFICAÇÃO DE PARADA TOTAL - DURANTE A VOLTA PARA POSIÇÃO INICIAL

              estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

              if(estado_botao_parada_total == LOW){ // SE O BOTÃO DE PARADA TOTAL FOR PRESSIONADO:

                Serial.println("BOTÃO PARADA TOTAL PRESSIONADO!!!");

                digitalWrite(enable_pin, HIGH);

                x = qtd_voltas * 800;

              }

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
  
  }  // FIM DO LOOP FOR QUE CONTA 3 LEITURAS

  


  // ------------------------ INÍCIO DA PARTE DO CONTADOR ---------------------------- //

  //QUANDO CONTADOR CHEGAR AO NÚMERO DE PEDIDOS, O MOTOR IRÁ PARA A POSIÇÃO INICIAL AUTOMATICAMENTE E LENTAMENTE

  if (contador >= num_pedidos * 2){ 

    // GIRANDO O MOTOR NO SENTIDO ANTI-HORÁRIO
    digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direção -> LOW = ANTI-HORÁRIO
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("GIRANDO O MOTOR NO SENTIDO ANTI-HORÁRIO ATÉ POSIÇÃO INICIAL"); // Printa no monitor serial em qual sentido o motor está girando

    for(int x = 0; x < num_pedidos * 800; x++){ // Loop For para girar o motor no sentido anti-horário

      // QUINTA VERIFICAÇÃO DE PARADA TOTAL - DURANTE A VOLTA PARA POSIÇÃO INICIAL

      estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

      if(estado_botao_parada_total == LOW){ // SE O BOTÃO DE PARADA TOTAL FOR PRESSIONADO:

        Serial.println("BOTÃO PARADA TOTAL PRESSIONADO!!!");

        digitalWrite(enable_pin, HIGH);

        x = num_pedidos * 800;

      }

      // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

      // BOTÃO FIM DE CURSO
      
      int estado_botao_fim_de_curso = digitalRead(botao_fim_de_curso); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

      if(estado_botao_fim_de_curso == LOW){ // SE O BOTÃO DE FIM DE CURSO FOR PRESSIONADO:

        // SEXTA VERIFICAÇÃO DE PARADA TOTAL - DURANTE A EXECUÇÃO DO FIM DE CURSO

        int estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

        if(estado_botao_parada_total == LOW){ // SE O BOTÃO DE PARADA TOTAL FOR PRESSIONADO:

          Serial.println("BOTÃO PARADA TOTAL PRESSIONADO!!!");

          digitalWrite(enable_pin, HIGH);

        }

        // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        Serial.println("O BOTÃO FIM DE CURSO FOI PRESSIONADO!!!");

        digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direção -> LOW = ANTI-HORÁRIO
        delayMicroseconds(1000); //Atraso de 1 segundo
        Serial.println("SE APROXIMANDO DO FIM DE CURSO LENTAMENTE!"); // Printa no monitor serial em qual sentido o motor está girando

        for(int x = 0; x < 800; x++){ // Loop For para girar o motor no sentido anti-horário

          // SÉTIMA VERIFICAÇÃO DE PARADA TOTAL - SEGURANÇA

          estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

          if(estado_botao_parada_total == LOW){ // SE O BOTÃO DE PARADA TOTAL FOR PRESSIONADO:

            Serial.println("BOTÃO PARADA TOTAL PRESSIONADO!!!");

            digitalWrite(enable_pin, HIGH);

            x = 800;

          }

          // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

          digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
          delayMicroseconds(6000); // GIRA MUITO LENTAMENTE ATÉ O FIM DE CURSO
          digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

        } //FIM DO LOOP 

        x = num_pedidos * 800; // SE O BOTÃO FOR PRESSIONADO, X JÁ RECEBE O VALOR DA CONDIÇÃO DO FOR PARA JÁ SAIR FORA DO LOOP E VOLTAR AO INÍCIO DO CÓDIGO!!!!

      } // FIM DO BOTÃO DE FIM DE CURSO

      digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
      delayMicroseconds(2000); // Atraso para regular a velocidade do motor (QUANTO MENOR ESSE ATRASO MAIS RÁPIDO O MOTOR GIRA!!!)
      digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

    } //FIM DO LOOP FOR PARA SENTIDO HORÁRIO

    contador = 0;

  }

  // ------------------------------------------------------ // ---------------------------------------------------------------------------------------


}  // FIM FA FUNÇÃO VOID LOOP

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------