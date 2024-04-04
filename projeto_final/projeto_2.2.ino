// PROJETO: PERSOL INC
// AUTOR: GUILHERME TAVARES PINHEIRO
// DATA: 04/04/2024
// OBJETIVO: AUTOMAÃ‡ÃƒO DA MESA DE CORTE DE TECIDO - MOVIMENTAR MOTOR DE PASSO NEMA - LARGURA

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ATRIBUINDO O NÃšMERO DOS PINOS A SUAS RESPECTIVAS FUNÃ‡Ã•ES

const int pino_pulso = 8;           // CONFIGURA O PINO DO PULSO / PASSO (PRECISÃƒO)
const int pino_direcao = 9;         // CONFIGURA O PINO DA DIREÃ‡ÃƒO (HORÃRIO / ANTI-HORÃRIO)
const int enable_pin = 13;          // CONFIGURA O PINO ENA
const int pino_rele = 7;            // CONFIGURA O PINO DO RELE

const int botao_fim_de_curso = 2;   // CONFIGURA O PINO DO BOTÃƒO DE FIM DE CURSO
const int botao_parada_total = 4;   // CONFIGURA O PINO DO BOTÃƒO DE PARADA TOTAL
const int botao_afasta_motor = 1;   // CONFIGURA O PINO DO BOTÃƒO QUE AFASTA MOTOR

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// CRIAÃ‡ÃƒO DAS VARIÃVEIS GLOBAIS - VETORES E ITERÃVEIS

long codigos[] = {15756719, 15756540, 15755617, 15755656, 15755512};         // Vetor de longs para os cÃ³digos de barras
float larguras[] = {4.5, 4.6, 4.7, 4.2, 4.15};                            // Vetor de larguras correspondentes Ã s peÃ§as conforme os cÃ³digos de barras (em metros)

int num_pedidos = 5;                                      // NÃºmero de pedidos (etiquetas) - altere conforme necessÃ¡rio                                                        
int contador = 0;                                         // Inicializa contador com 0
int estado_botao_parada_total = HIGH;                     // Estado do botÃ£o de parada total
int estado_botao_afasta_motor = HIGH;

float posicao_atual;                                      // VariÃ¡vel para armazenar a posiÃ§Ã£o atual do motor                       
float medida_lida;                                        // VariÃ¡vel para armazenar a posiÃ§Ã£o de destino 
                               
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// FUNÃ‡ÃƒO SETUP - CONFIGURAÃ‡Ã•ES INICIAIS - ENTRADA, SAÃDA E COMUNICAÃ‡ÃƒO SERIAL

void setup() {

  pinMode(pino_pulso, OUTPUT);               // DEFINE PINO PULSO COMO SAÃDA
  pinMode(pino_direcao, OUTPUT);             // DEFINE PINO DIRECAO COMO SAÃDA
  pinMode(enable_pin, OUTPUT);               // DEFINE PINO ENABLE COMO SAÃDA
  pinMode(pino_rele, OUTPUT);                // DEFINE PINO DO RELÃ‰ COMO SAÃDA

  pinMode(botao_fim_de_curso, INPUT_PULLUP); // DEFINE O BOTÃƒO FIM DE CURSO COMO ENTRADA E COM RESISTOR INTERNO
  pinMode(botao_parada_total, INPUT_PULLUP); // DEFINE O BOTÃƒO PARADA TOTAL COMO ENTRADA E COM RESISTOR INTERNO
  pinMode(botao_afasta_motor, INPUT_PULLUP); // DEFINE O BOTÃƒO PARADA TOTAL COMO ENTRADA E COM RESISTOR INTERNO

  Serial.begin(9600);                        // INICIALIZA A COMUNICAÃ‡ÃƒO SERIAL COM UMA TAXA DE 9600 BAUD

} // FIM DA FUNÃ‡ÃƒO SETUP

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // 1) CALIBRAÃ‡ÃƒO DO MOTOR 

void calibra_motor(){

  if (estado_botao_parada_total == HIGH){ // Continua a calibraÃ§Ã£o enquanto o botÃ£o de parada total nÃ£o for pressionado

    delay(2000); // Aguarda 2 segundos antes de subir a prensa

    digitalWrite(pino_rele, LOW); // PINO DO RELÃ‰ LOW = PRENSA SOBE!

    delay(500); //PEQUENO ATRASO PARA NÃƒO MOVIMENTAR PRENSA E MOTOR SIMULTANEAMENTE

    digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direÃ§Ã£o -> LOW = ANTI-HORÃRIO - Sentido de calibraÃ§Ã£o
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("\n CALIBRANDO O MOTOR ATÃ‰ POSIÃ‡ÃƒO INICIAL \n"); // Mensagem inicial de calibraÃ§Ã£o do motor

    for(int x = 0; x < 24000; x++){ // Loop For para girar o motor no sentido de calibraÃ§Ã£o - 24000 = 30 Voltas em torno de si ~= 17,5cm * 30 = 5.25m - Calibra em qualquer posiÃ§Ã£o da mesa

      while (estado_botao_parada_total == HIGH){ // Permanece calibrando atÃ© que o botÃ£o de parada total seja acionado

        parada_total(); // Chama a funÃ§Ã£o parada total

        digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
        delayMicroseconds(2000); // VELOCIDADE DE CALIBRAÃ‡ÃƒO
        digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

        int estado_botao_fim_de_curso = digitalRead(botao_fim_de_curso); // LÃª o estado do botÃ£o de fim de curso

        if (estado_botao_fim_de_curso == LOW){ // Reduz a velocidade de calibraÃ§Ã£o se o botÃ£o de fim de curso for pressionado!
 
          Serial.println("BOTÃƒO FIM DE CURSO PRESSIONADO - VELOCIDADE REDUZIDA!!!");

          digitalWrite(pino_pulso, HIGH);
          delayMicroseconds(4000); // Velocidade de calibraÃ§Ã£o reduzida (2000 -> 4000); 
          digitalWrite(pino_pulso, LOW);
        }

      }

    } 

  }

}

// FIM DA CALIBRAÃ‡ÃƒO DO MOTOR!!!

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 2) FUNÃ‡ÃƒO PARADA TOTAL !!! 

void parada_total() {

  estado_botao_parada_total = digitalRead(botao_parada_total); // LÃŠ O ESTADO DO BOTÃƒO DE PARADA TOTAL

  if(estado_botao_parada_total == LOW){ // Se o botÃ£o de parada total for pressionado:

    Serial.println("\n BOTÃƒO PARADA TOTAL PRESSIONADO!!! \n");

    digitalWrite(enable_pin, HIGH); // Ativa o pino ENA - Desabilita o movimento do motor

    delay(1000); // Atraso de 1 segundo por seguranÃ§a

  }
  
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 3) FUNÃ‡ÃƒO ESCOLHE SENTIDO HORÃRIO OU ANTI-HORÃRIO DE ACORDO COM A COMPARAÃ‡ÃƒO ENTRE A POSIÃ‡ÃƒO ATUAL (MEDIDA ANTERIOR) E A NOVA ETIQUETA !!! 

void sentido_rotacao(float posicao_atual, float medida_lida) { // FunÃ§Ã£o recebe a posiÃ§Ã£o atual e a medida bipada como parÃ¢metros

  if (posicao_atual > medida_lida){ // Se a medida for menor do que a posiÃ§Ã£o atual do motor:

    digitalWrite(pino_direcao, HIGH); // Atribui HIGH ao pino de direÃ§Ã£o -> HIGH = HORÃRIO
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("\n MOTOR GIRANDO NO SENTIDO HORÃRIO \n"); // Printa no monitor serial em qual sentido o motor estÃ¡ girando

  }
  else{ // Se a medida for maior que a posiÃ§Ã£o atual do motor

    digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direÃ§Ã£o -> LOW = ANTI-HORÃRIO
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("\n MOTOR GIRANDO NO SENTIDO ANTI-HORÃRIO \n"); // Printa no monitor serial em qual sentido o motor estÃ¡ girando

  }
  
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 4) FUNÃ‡ÃƒO DE GIRO DO MOTOR - MOVIMENTA O MOTOR 1 CICLO (1 CICLO = 800 PULSOS = 175,9mm = 17,59cm) -> (4548.038658328596 Pulsos = 1000mm = 1m) - USANDO TODAS CASAS DECIMAIS PARA AUMENTAR A PRECISÃƒO!

void gira_motor(int pino_pulso, float medida_lida, float posicao_atual){ // FunÃ§Ã£o recebe o pino de pulso, medida bipada e a posiÃ§Ã£o atual como parÃ¢metros

  const int velocidade_inicial = 2400;   // Velocidade inicial - Um pouco maior que a velocidade de calibraÃ§Ã£o
  const int velocidade_final = 600;      // Velocidade final - Mais rÃ¡pida e adequada para o motor
  const int passos_aceleracao = 600;     // NÃºmero de passos para alcanÃ§ar a velocidade final - de 600 em 600 - (600 -> 2400 = 4 passos de aceleraÃ§Ã£o = 4 marchas)

  float qtd_passos; // VariÃ¡vel que define quantas vezes o motor irÃ¡ girar. POSIÃ‡ÃƒO ATUAL - MEDIDA DE DESTINO (CÃ“DIGO DE BARRAS)

  qtd_passos = abs((posicao_atual - medida_lida)); // Usamos a funÃ§Ã£o abs() para a subtraÃ§Ã£o sempre retornar um valor positivo, isto Ã©, para nÃ£o correr o risco de termos um valor negativo (sempre mÃ³dulo)!

  if (qtd_passos > 0.50){

    int velocidade_atual = velocidade_inicial; // Velocidade inicial recebe a velocidade inicial (2400)

    for (int i = 0; i < (qtd_passos * 4548.038658328596); i++){ // O motor gira x vezes de acordo com a expressÃ£o anterior. Altere essa condiÃ§Ã£o de acordo com seu referencial de medidas

      if(estado_botao_parada_total == HIGH){ // VerificaÃ§Ã£o de parada total durante as voltas do motor
        parada_total(); 
      }

      // IF PARA ACELERAR O MOTOR

      if (i < passos_aceleracao) {
        // Ajustar a velocidade gradualmente atÃ© a velocidade final
        velocidade_atual -= (velocidade_inicial - velocidade_final) / passos_aceleracao;
      }

      // IF PARA DESACELERAR O MOTOR - QUANDO FALTA 10% PRA CHEGAR NA MEDIDA DE DESTINO (1m = desacelera no 0,90)!

      if (i > (0.95 * (qtd_passos * 4548.038658328596))) {
        // Ajustar a velocidade gradualmente atÃ© a velocidade final
        velocidade_atual -= 750;
      }

      digitalWrite(pino_pulso, HIGH);
      delayMicroseconds(velocidade_atual);           // Velocidade de giro do motor durante as leituras de largura e altura (400 = Valor mais adequado para o motor girar razoavelmente rÃ¡pido e sem ruÃ­dos)
      digitalWrite(pino_pulso, LOW);
    
    }

  }

  else{

    for (int i = 0; i < (qtd_passos * 4548.038658328596); i++){ // O motor gira x vezes de acordo com a expressÃ£o anterior. Altere essa condiÃ§Ã£o de acordo com seu referencial de medidas

      if(estado_botao_parada_total == HIGH){ // VerificaÃ§Ã£o de parada total durante as voltas do motor
        parada_total(); 
      }

      digitalWrite(pino_pulso, HIGH);
      delayMicroseconds(1200);           // Velocidade menor para peÃ§as com menos de 0.5m
    
    } 

  }

}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 5) FUNCAO volta motor

void retorna_motor(){

   estado_botao_afasta_motor = digitalRead(botao_afasta_motor);

   if (estado_botao_afasta_motor == LOW){
    
      digitalWrite(pino_direcao, LOW);
      delayMicroseconds(1000);
      Serial.println("\n MOTOR RETORNANDO NO SENTIDO ANTI-HORARIO \n");

      for (int x = 0; x < 400; x++) {

        digitalWrite(pino_pulso, HIGH);
        delayMicroseconds(2400);
        digitalWrite(pino_pulso, LOW);
      }  
  
    }
  
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 6) FUNCÃƒO LOOP - LOOP PRINCIPAL DO PROGRAMA

void loop() { 

  calibra_motor();

  digitalWrite(enable_pin, LOW); // HABILITA O MOTOR APÃ“S UMA POSSÃVEL PARADA TOTAL

  // 6.1) MOTOR GIRA UMA VOLTA LENTAMENTE PARA NÃƒO MANTER O BOTÃƒO DE PARADA TOTAL DA CALIBRAÃ‡ÃƒO PRESSIONADO!!!

  digitalWrite(pino_direcao, HIGH); // Atribui HIGH ao pino de direÃ§Ã£o -> HIGH = HORÃRIO
  delayMicroseconds(500); //Atraso de meio segundo
  Serial.println("\n INÃCIO DO PROGRAMA PRINCIPAL!!! \n");
  Serial.println("\n RETIRANDO O MOTOR DO BOTÃƒO \n"); // Printa no monitor serial em qual sentido o motor estÃ¡ girando

  for(int x = 0; x < 800; x++){ //DÃ UMA VOLTA PARA SAIR DO BOTÃƒO
     
    digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
    delayMicroseconds(2000); // Atraso para regular a velocidade do motor (QUANTO MENOR ESSE ATRASO MAIS RÃPIDO O MOTOR GIRA!!!)
    digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

  } // FIM DO IF

  posicao_atual = 4.743; // POSIÃ‡ÃƒO DA MESA DE CORTE ONDE O MOTOR FICA APÃ“S SAIR DO BOTÃƒO!

  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // !!! PRIMEIRA VERIFICAÃ‡ÃƒO DE PARADA TOTAL - ANTES DA MEDIDA DE LARGURA

  parada_total(); 

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // 6.2) PRIMEIRO CICLO DO VOID LOOP - LARGURAS

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  for(contador = 0; contador < num_pedidos * 2; contador++){ // REPETE A LEITURA DE ETIQUETAS ATÃ‰ A QUANTIDADE DE PEDIDOS (*2 PARA BATER O CÃLCULO)

    // SEGUNDA VERIFICAÃ‡ÃƒO DE PARADA TOTAL 

    parada_total();

    // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    retorna_motor();
    
    delay(1000);

    Serial.println("CONTAGEM ATUAL DO NÃšMERO DE PEDIDOS ATÃ‰ A POSIÃ‡ÃƒO INICIAL: \n");

    Serial.println(contador); // EXIBE O VALOR DE CONTADOR NO SERIAL PRINT 

    // O programa nÃ£o continua atÃ© que o usuÃ¡rio digite o nÃºmero de voltas

    Serial.println("Insira o numero do codigo de barras:"); 
    
    while (!Serial.available());

    long codigo = Serial.parseInt(); // VariÃ¡vel cÃ³digo recebe o que o usuÃ¡rio inseriu na porta serial

    // LOOP FOR PARA ITERAR ATÃ‰ O NÃšMERO DE PEDIDOS

    for (int i = 0; i < num_pedidos; i++) {

      // !!! VERIFICAÃ‡ÃƒO DE PARADA TOTAL !!!

      parada_total();

      // O PROGRAMA SÃ“ INICIA SE O CÃ“DIGO DE BARRAS DIGITADO BATER COM ALGUM DOS CÃ“DIGOS NO VETOR

      if (codigos[i] == codigo) { // Verifica se o cÃ³digo de barras digitado ou bipado corresponde Ã  alguns dos cÃ³digos no vetor cÃ³digos[]

        // VERIFICAÃ‡ÃƒO DE PARADA TOTAL  

        digitalWrite(pino_rele, LOW); // PINO DO RELÃ‰ LOW = PRENSA SOBE!

        delay(500); //PEQUENO ATRASO PARA NÃƒO MOVIMENTAR PRENSA E MOTOR SIMULTANEAMENTE

        parada_total(); 

        medida_lida = larguras[i]; // Aqui vamos armazenar o valor correspondente a largura do respectivo Ã­ndice

        Serial.println(larguras[i]); // Printamos a medida que o motor irÃ¡ movimentar para nÃ£o nos perdermos

        // GIRANDO O MOTOR NO PRIMEIRO SENTIDO

        sentido_rotacao(posicao_atual, medida_lida); // Define o sentido de rotaÃ§Ã£o do motor de acordo com os parÃ¢metros da medida da largura
        
        gira_motor(pino_pulso, posicao_atual, medida_lida); // Define a velocidade e quantidade de voltas do motor de acordo com os parÃ¢metros atuais

        posicao_atual = medida_lida;  // POSICAO ATUAL RECEBE O VALOR DA MEDIDA LIDA PARA PRÃ“XIMAS COMPARAÃ‡Ã•ES

        delay(1000); // ATRASO PARA NÃƒO DESCER A PRENSA ANTES DE TERMINAR O MOVIMENTO DO MOTOR

        digitalWrite(pino_rele, HIGH); // PINO DO RELÃ‰ HIGH = PRENSA DESCE!
      
      }    // FIM DO IF
    
    }  // FIM DO LOOP FOR QUE ITERA ATÃ‰ O NÃšMERO DE  PEDIDOS (CÃ“DIGOS DE BARRAS)
    
  } // FIM DO LOOP FOR QUE ITERA ATÃ‰ O NÃšMERO DO CONTADOR

}  // FIM DA FUNÃ‡ÃƒO VOID LOOP

// -----------------------------------------------------------------------------------------------------------------------
