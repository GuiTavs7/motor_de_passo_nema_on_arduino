// PROJETO: PERSOL INC
// AUTOR: GUILHERME TAVARES PINHEIRO
// DATA: 18/03/2024
// OBJETIVO: MOVIMENTAR MOTOR DE PASSO NEMA COM DUAS MEDIDAS - LARGURA E ALTURA 

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ATRIBUINDO O NÚMERO DOS PINOS A SUAS RESPECTIVAS FUNÇÕES

const int pino_pulso = 8;           // CONFIGURA O PINO DO PULSO / PASSO (PRECISÃO)
const int pino_direcao = 9;         // CONFIGURA O PINO DA DIREÇÃO (HORÁRIO / ANTI-HORÁRIO)
const int enable_pin = 13;          // CONFIGURA O PINO ENA
const int botao_altura = 3;         // CONFIGURA O PINO DO BOTÃO
const int botao_fim_de_curso = 2;   // CONFIGURA O PINO DO BOTÃO DE FIM DE CURSO
const int botao_parada_total = 4;   // CONFIGURA O PINO DO BOTÃO DE PARADA TOTAL

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// CRIAÇÃO DAS VARIÁVEIS - VETORES E ITERÁVEIS

long codigos[] = {15756719, 15756540, 15755617, 15755656, 15755512};          // Vetor de longs para os códigos de barras
float larguras[] = {4.5, 4.2, 4.3, 4.2, 4.1};                            // Vetor de larguras correspondentes às peças conforme os códigos de barras
float alturas[] = {4.6, 4.3, 4.3, 4.3, 4.4};                             // Vetor de alturas correspondentes às peças conforme os códigos de barras

int num_pedidos = sizeof(codigos) / sizeof(codigos[0]);   // Variável para ser utilizada no limite do loop "for", para fazer o programa iterar até a quantidade necessária de códigos de barras                                        // Variável para armazenar o número de voltas, que será de acordo com o valor do float larguras ou do float alturas
int segundo_ciclo = 0;                                    // Inicializa segundo ciclo com 0
int contador = 0;                                         // Inicializa contador com 0
int estado_botao_parada_total = HIGH;                     // Estado do botão de parada total

float posicao_atual;                                       // Variável para armazenar a posição atual do motor                         // Variável para armazenar a posição atual do motor - ALTURA
float medida;                                               // Variável para armazenar a posição de destino 
                               

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// FUNÇÃO SETUP - CONFIGURAÇÕES INICIAIS - ENTRADA, SAÍDA E COMUNICAÇÃO SERIAL

void setup() {

  pinMode(pino_pulso, OUTPUT);               // DEFINE PINO PULSO COMO SAÍDA
  pinMode(pino_direcao, OUTPUT);             // DEFINE PINO DIRECAO COMO SAÍDA
  pinMode(enable_pin, OUTPUT);               // DEFINE PINO ENABLE COMO SAÍDA
  pinMode(botao_altura, INPUT_PULLUP);       // DEFINE O PINO DO BOTÃO COMO ENTRADA E COM UM RESISTOR INTERNO PARA EVITAR LEITURAS ALEATÓRIAS
  pinMode(botao_fim_de_curso, INPUT_PULLUP); // DEFINE O BOTÃO FIM DE CURSO COMO ENTRADA E COM RESISTOR INTERNO
  pinMode(botao_parada_total, INPUT_PULLUP); // DEFINE O BOTÃO PARADA TOTAL COMO ENTRADA E COM RESISTOR INTERNO

  Serial.begin(9600);                        // INICIALIZA A COMUNICAÇÃO SERIAL COM UMA TAXA DE 9600 BAUD

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // 1) CALIBRAÇÃO DO MOTOR 

  if (estado_botao_parada_total == HIGH){ // Continua a calibração enquanto o botão de parada total não for pressionado

    digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direção -> LOW = ANTI-HORÁRIO - Sentido de calibração
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("\n GIRANDO O MOTOR NO SENTIDO HORÁRIO ATÉ POSIÇÃO INICIAL \n"); // Mensagem inicial de calibração do motor

    for(int x = 0; x < 9000; x++){ // Loop For para girar o motor no sentido de calibração

      while (estado_botao_parada_total == HIGH){ // Permanece calibrando até que o botão de parada total seja acionado

        parada_total(); // Chama a função parada total

        digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
        delayMicroseconds(3000); // VELOCIDADE DE CALIBRAÇÃO
        digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

        int estado_botao_fim_de_curso = digitalRead(botao_fim_de_curso);

        if (estado_botao_fim_de_curso == LOW){ // Reduz a velocidade de calibração se o botão de fim de curso for pressionado!
 
          Serial.println("BOTÃO FIM DE CURSO PRESSIONADO - VELOCIDADE REDUZIDA!!!");

          digitalWrite(pino_pulso, HIGH);
          delayMicroseconds(4000); // Velocidade de calibração reduzida 10 vezes (400 -> 4000); 
          digitalWrite(pino_pulso, LOW);
        }

      }

    } 

  }

  // FIM DA CALIBRAÇÃO DO MOTOR!!!

  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

} // FIM DA FUNÇÃO SETUP

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 2) FUNÇÃO PARADA TOTAL !!! 

void parada_total() {

  estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE PARADA TOTAL

  if(estado_botao_parada_total == LOW){ // Se o botão de parada total for pressionado:

    Serial.println("\n BOTÃO PARADA TOTAL PRESSIONADO!!! \n");

    digitalWrite(enable_pin, HIGH); // Ativa o pino ENA - Desabilita o movimento do motor

    delay(1000); // Atraso de 1 segundo por segurança

  }
  
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 3) FUNÇÃO ESCOLHE SENTIDO HORÁRIO OU ANTI-HORÁRIO DE ACORDO COM A COMPARAÇÃO ENTRE A POSIÇÃO ATUAL (MEDIDA ANTERIOR) E A NOVA ETIQUETA !!! 

void sentido_rotacao(float posicao_atual, float medida) { // Função recebe a posição atual e a medida bipada como parâmetros

  if (posicao_atual > medida){ // Se a medida for maior do que a posição atual do motor:

    digitalWrite(pino_direcao, HIGH); // Atribui HIGH ao pino de direção -> HIGH = HORÁRIO
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("\n MOTOR GIRANDO NO SENTIDO HORÁRIO \n"); // Printa no monitor serial em qual sentido o motor está girando

  }
  else{ // Se a medida for menor que a posição atual do motor

    digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direção -> LOW = ANTI-HORÁRIO
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("\n MOTOR GIRANDO NO SENTIDO ANTI-HORÁRIO \n"); // Printa no monitor serial em qual sentido o motor está girando

  }
  
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 4) FUNÇÃO DE GIRO DO MOTOR - MOVIMENTA O MOTOR 1 CICLO (1 CICLO = 800 PULSOS = 175mm = 17,5cm) -> (4632,3 Pulsos = 1000mm = 1m)

void gira_motor(int pino_pulso, float medida, float posicao_atual){ // Função recebe o pino de pulso, medida bipada e a posição atual como parâmetros

  float qtd_passos; // Variável que define quantas vezes o motor irá girar. POSIÇÃO ATUAL - MEDIDA DE DESTINO (CÓDIGO DE BARRAS)

  qtd_passos = abs((posicao_atual - medida)); // Usamos a função abs() para a subtração sempre retornar um valor positivo, isto é, para não correr o risco de termos um valor negativo e o motor travar!

  for (int i = 0; i < (qtd_passos * 4553.215); i++){ // O motor gira x vezes de acordo com a expressão anterior. Altere essa condição de acordo com seu referencial de medidas

    if(estado_botao_parada_total == HIGH){ // Verificação de parada total durante as voltas do motor
      parada_total(); 
    }

    digitalWrite(pino_pulso, HIGH);
    delayMicroseconds(1500);           // Velocidade de giro do motor durante as leituras de largura e altura (400 = Valor mais adequado para o motor girar razoavelmente rápido e sem ruídos)
    digitalWrite(pino_pulso, LOW);
  }
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// 5) FUNCÃO LOOP - LOOP PRINCIPAL DO PROGRAMA

void loop() { 

  digitalWrite(enable_pin, LOW); // HABILITA O MOTOR APÓS UMA POSSÍVEL PARADA TOTAL

  // 5.1) MOTOR GIRA UMA VOLTA LENTAMENTE PARA NÃO MANTER O BOTÃO DE PARADA TOTAL DA CALIBRAÇÃO PRESSIONADO!!!

  digitalWrite(pino_direcao, HIGH); // Atribui HIGH ao pino de direção -> HIGH = HORÁRIO
  delayMicroseconds(500); //Atraso de meio segundo
  Serial.println("\n INÍCIO DO PROGRAMA PRINCIPAL!!! \n");
  Serial.println("\n RETIRANDO O MOTOR DO BOTÃO \n"); // Printa no monitor serial em qual sentido o motor está girando

  for(int x = 0; x < 800; x++){ //DÁ UMA VOLTA PARA SAIR DO BOTÃO
     
    digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
    delayMicroseconds(2000); // Atraso para regular a velocidade do motor (QUANTO MENOR ESSE ATRASO MAIS RÁPIDO O MOTOR GIRA!!!)
    digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

  } // FIM DO IF

  posicao_atual = 4.734;

  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // !!! PRIMEIRA VERIFICAÇÃO DE PARADA TOTAL - ANTES DO PRIMEIRO CICLO !!!

  parada_total();

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // 5.2) PRIMEIRO CICLO DO VOID LOOP - LARGURAS

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  for(contador = 0; contador < num_pedidos * 2; contador++){ // REPETE A LEITURA DE ETIQUETAS ATÉ A QUANTIDADE DE PEDIDOS (*2 PARA BATER O CÁLCULO)

    // SEGUNDA VERIFICAÇÃO DE PARADA TOTAL - DURANTE AS TRÊS PRIMEIRAS LEITURAS

    parada_total();

    // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    Serial.println("CONTAGEM ATUAL DO NÚMERO DE PEDIDOS ATÉ A POSIÇÃO INICIAL: \n");

    Serial.println(contador); // EXIBE O VALOR DE CONTADOR NO SERIAL PRINT 

    // Espera até que o usuário insira o número do código de barras
    Serial.println("\n Insira o número do código de barras: ");

    // O programa não continua até que o usuário digite o número de voltas
   
    while (!Serial.available()); 

    long codigo = Serial.parseInt(); // Variável código recebe o que o usuário inseriu na porta serial


    // LOOP FOR PARA ITERAR ATÉ O NÚMERO DE PEDIDOS

    for (int i = 0; i < num_pedidos; i++) {

      // !!! VERIFICAÇÃO DE PARADA TOTAL !!!

      parada_total();

      // O PROGRAMA SÓ INICIA SE O CÓDIGO DE BARRAS DIGITADO BATER COM ALGUM DOS CÓDIGOS NO VETOR

      if (codigos[i] == codigo) { // Verifica se o código de barras digitado ou bipado corresponde à alguns dos códigos no vetor códigos[]

        // VERIFICAÇÃO DE PARADA TOTAL  

        parada_total(); 

        medida = larguras[i]; // Aqui vamos armazenar o valor correspondente a largura do respectivo índice

        Serial.println(larguras[i]); // Printamos a medida que o motor irá movimentar para não nos perdermos

        // GIRANDO O MOTOR NO PRIMEIRO SENTIDO

        sentido_rotacao(posicao_atual, medida); // Define o sentido de rotação do motor de acordo com os parâmetros da medida da largura
        
        segundo_ciclo = 0; // REINICIA O ESTADO DO BOTÃO PARA 0 - IMPORTANTE PARA COMEÇAR O SEGUNDO LOOP (ALTURA)

        gira_motor(pino_pulso, posicao_atual, medida); // Define a velocidade e quantidade de voltas do motor de acordo com os parâmetros atuais

        posicao_atual = medida;  // POSICAO ATUAL RECEBE O VALOR DA MEDIDA LIDA PARA PRÓXIMAS COMPARAÇÕES
      
      }    // FIM DO IF
    
    } // FIM DO LOOP FOR QUE ITERA ATÉ O NÚMERO DE  PEDIDOS (CÓDIGOS DE BARRAS)
  

   // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // VERIFICAÇÃO DE PARADA TOTAL - ENTRE CICLOS DE LARGURA E ALTURA

    parada_total();

    // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


    // 5.3) SEGUNDO CICLO - ALTURAS - SÓ PROSSEGUE SE O BOTÃO PARA LER A MEDIDA DA ALTURA FOR PRESSIONADO


    while(segundo_ciclo == 0){

      // O programa só continua se o botão for pressionado
      if (digitalRead(botao_altura) == LOW) { 

        // !!! VERIFICAÇÃO DE PARADA TOTAL DURANTE SEGUNDO CICLO - ALTURA !!!

        parada_total();

        // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        Serial.println("BOTÃO DO SEGUNDO CICLO PRESSIONADO!!! - Iniciando segundo ciclo - ALTURA! \n");

        // LOOP FOR PARA ITERAR ATÉ O NÚMERO DE PEDIDOS

        for (int i = 0; i < num_pedidos; i++) {

          // !!! VERIFICAÇÃO DE PARADA TOTAL !!!  

          parada_total();

          // O PROGRAMA SÓ INICIA SE O CÓDIGO DE BARRAS DIGITADO BATER COM ALGUM DOS CÓDIGOS NO VETOR

          if (codigos[i] == codigo) {  // Verifica se o código de barras digitado ou bipado corresponde à alguns dos códigos no vetor códigos[]

            medida = alturas[i]; // Aqui vamos armazenar o valor correspondente a altura do respectivo índice

            Serial.println(alturas[i]); // Printa o valor correspondente ao índice altura = quantidade de voltas do motor

            // GIRANDO O MOTOR NO SEGUNDO SENTIDO
         
            sentido_rotacao(posicao_atual, medida);  // Define o sentido de rotação do motor de acordo com os parâmetros da medida da altura

            gira_motor(pino_pulso, posicao_atual, medida);  // Define a velocidade e quantidade de voltas do motor de acordo com os parâmetros atuais

            posicao_atual = medida;  // POSICAO ATUAL RECEBE O VALOR DA MEDIDA LIDA PARA PRÓXIMAS COMPARAÇÕES
          
          } // FIM DO IF
        
        } // FIM DO LOOP FOR QUE ITERA ATÉ O NÚMERO DE  PEDIDOS (CÓDIGOS DE BARRAS)

        break; // Força a saída do loop
      
      } // FIM DO IF DO BOTÃO

      delay(1000); // Pequeno atraso para evitar detecção falsa de pressionamento do botão
    
    } // FIM DO LOOP WHILE DO SEGUNDO CICLO

    segundo_ciclo = 1; //ATRIBUI 1 AO SEGUNDO CICLO PRA NÃO FICAR PRESO NO BOTÃO - VOLTAR AO PRIMEIRO CICLO (LARGURA)
  
  }  // FIM DO LOOP FOR QUE CONTA 3 LEITURAS

  


  // ------------------------ INÍCIO DA PARTE DO CONTADOR ---------------------------- //

  // 6) MOTOR VOLTA LENTAMENTE PARA A POSIÇÃO INICIAL 

  // QUANDO CONTADOR CHEGAR AO NÚMERO DE PEDIDOS, O MOTOR IRÁ PARA A POSIÇÃO INICIAL AUTOMATICAMENTE E LENTAMENTE

  if (contador >= num_pedidos * 2){ 

    // GIRANDO O MOTOR NO SENTIDO ANTI-HORÁRIO
    digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direção -> LOW = ANTI-HORÁRIO
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("GIRANDO O MOTOR NO SENTIDO ANTI-HORÁRIO ATÉ POSIÇÃO INICIAL \n"); // Printa no monitor serial em qual sentido o motor está girando

    for(int x = 0; x < num_pedidos * 800; x++){ // Loop For para girar o motor no sentido anti-horário

      // !!! VERIFICAÇÃO DE PARADA TOTAL - DURANTE A VOLTA PARA POSIÇÃO INICIAL !!!

      estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

      if(estado_botao_parada_total == LOW){ // SE O BOTÃO DE PARADA TOTAL FOR PRESSIONADO:

        Serial.println("\n BOTÃO PARADA TOTAL PRESSIONADO!!! \n");

        digitalWrite(enable_pin, HIGH);

        x = num_pedidos * 800; // x recebe 800 vezes o número de pedidos para que seja satisfeita a condição de saída do Loop For e garanta a parada total

      }

      // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

      // 6.1) BOTÃO FIM DE CURSO - CASO SEJA NECESSÁRIO, REDUZ AINDA MAIS A VELOCIDADE DE RETORNO PARA O FIM DE CURSO
      
      int estado_botao_fim_de_curso = digitalRead(botao_fim_de_curso); // LÊ O ESTADO DO BOTÃO DE FIM DE CURSO

      if(estado_botao_fim_de_curso == LOW){ // SE O BOTÃO DE FIM DE CURSO FOR PRESSIONADO:

        // !!! VERIFICAÇÃO DE PARADA TOTAL - DURANTE A EXECUÇÃO DO FIM DE CURSO !!!

        parada_total();

        // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        Serial.println("O BOTÃO FIM DE CURSO FOI PRESSIONADO!!! \n");

        digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direção -> LOW = ANTI-HORÁRIO
        delayMicroseconds(1000); //Atraso de 1 segundo
        Serial.println("SE APROXIMANDO DO FIM DE CURSO LENTAMENTE! \n"); // Printa no monitor serial em qual sentido o motor está girando

        for(int x = 0; x < 800; x++){ // Loop For para girar o motor no sentido anti-horário

          // !!! VERIFICAÇÃO DE PARADA TOTAL !!! 

          if(estado_botao_parada_total == HIGH){

            parada_total();

            digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
            delayMicroseconds(6200); // GIRA MUITO LENTAMENTE ATÉ O FIM DE CURSO
            digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

          }

        } //FIM DO LOOP 

        x = num_pedidos * 800; // SE O BOTÃO FOR PRESSIONADO, X JÁ RECEBE O VALOR DA CONDIÇÃO DO FOR PARA JÁ SAIR FORA DO LOOP E VOLTAR AO INÍCIO DO CÓDIGO!!!!

      } // FIM DO BOTÃO DE FIM DE CURSO

      digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
      delayMicroseconds(3000); // Atraso para regular a velocidade do motor (QUANTO MENOR ESSE ATRASO MAIS RÁPIDO O MOTOR GIRA!!!)
      digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

    } //FIM DO LOOP FOR PARA SENTIDO HORÁRIO

    contador = 0;

  }

  // ------------------------------------------------------ // ---------------------------------------------------------------------------------------


}  // FIM DA FUNÇÃO VOID LOOP

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
