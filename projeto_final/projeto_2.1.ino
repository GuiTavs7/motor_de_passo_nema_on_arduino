// PROJETO: PERSOL INC
// AUTOR: GUILHERME TAVARES PINHEIRO
// DATA: 01/04/2024
// OBJETIVO: AUTOMAÇÃO DA MESA DE CORTE DE TECIDO - MOTOR DE PASSO NEMA CONTROLANDO A MESA

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ATRIBUINDO O NÚMERO DOS PINOS A SUAS RESPECTIVAS FUNÇÕES

const int pino_pulso = 8;           // CONFIGURA O PINO DO PULSO / PASSO (PRECISÃO)
const int pino_direcao = 9;         // CONFIGURA O PINO DA DIREÇÃO (HORÁRIO / ANTI-HORÁRIO)
const int enable_pin = 13;          // CONFIGURA O PINO ENA
const int pino_rele = 7;            // CONFIGURA O PINO DO RELE

const int botao_fim_de_curso = 2;   // CONFIGURA O PINO DO BOTÃO DE FIM DE CURSO
const int botao_parada_total = 4;   // CONFIGURA O PINO DO BOTÃO DE PARADA TOTAL

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// CRIAÇÃO DAS VARIÁVEIS - VETORES E ITERÁVEIS

long codigos[] = {15756719, 15756540, 15755617, 15755656, 15755512};     // Vetor de longs para os códigos de barras
float larguras[] = {4.285, 3.548, 4.549, 3.5, 4.70};                     // Vetor de larguras correspondentes às peças conforme os códigos de barras (em metros)

int num_pedidos = 5;                                      // Número de pedidos (etiquetas) - altere conforme necessário                                                        
int contador = 0;                                         // Inicializa contador com 0
int estado_botao_parada_total = HIGH;                     // Estado do botão de parada total

float posicao_atual;                                      // Variável para armazenar a posição atual do motor                       
float medida_lida;                                        // Variável para armazenar a posição de destino 
                               
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// FUNÇÃO SETUP - CONFIGURAÇÕES INICIAIS - ENTRADA, SAÍDA E COMUNICAÇÃO SERIAL

void setup() {

  pinMode(pino_pulso, OUTPUT);                 // DEFINE PINO PULSO COMO SAÍDA
  pinMode(pino_direcao, OUTPUT);               // DEFINE PINO DIRECAO COMO SAÍDA
  pinMode(enable_pin, OUTPUT);                 // DEFINE PINO ENABLE COMO SAÍDA
  pinMode(pino_rele, OUTPUT);                  // DEFINE PINO DO RELÉ COMO SAÍDA

  pinMode(botao_fim_de_curso, INPUT_PULLUP);   // DEFINE O BOTÃO FIM DE CURSO COMO ENTRADA E COM RESISTOR INTERNO
  pinMode(botao_parada_total, INPUT_PULLUP);   // DEFINE O BOTÃO PARADA TOTAL COMO ENTRADA E COM RESISTOR INTERNO

  Serial.begin(9600);                          // INICIALIZA A COMUNICAÇÃO SERIAL COM UMA TAXA DE 9600 BAUD

} // FIM DA FUNÇÃO SETUP

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // 1) CALIBRAÇÃO DO MOTOR 

void calibra_motor(){

  if (estado_botao_parada_total == HIGH){ // Continua a calibração enquanto o botão de parada total não for pressionado

    delay(2000); // Aguarda 2 segundos antes de subir a prensa

    digitalWrite(pino_rele, LOW); // PINO DO RELÉ LOW = PRENSA SOBE!

    delay(500); //PEQUENO ATRASO PARA NÃO MOVIMENTAR PRENSA E MOTOR SIMULTANEAMENTE

    digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direção -> LOW = ANTI-HORÁRIO - Sentido de calibração
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("\n CALIBRANDO O MOTOR ATÉ POSIÇÃO INICIAL \n"); // Mensagem inicial de calibração do motor

    for(int x = 0; x < 24000; x++){ // Loop For para girar o motor no sentido de calibração - 24000 = 30 Voltas em torno de si ~= 17,5cm * 30 = 5.25m - Calibra em qualquer posição da mesa

      while (estado_botao_parada_total == HIGH){ // Permanece calibrando até que o botão de parada total seja acionado

        parada_total(); // Chama a função parada total

        digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
        delayMicroseconds(2000); // VELOCIDADE DE CALIBRAÇÃO
        digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

        int estado_botao_fim_de_curso = digitalRead(botao_fim_de_curso); // Lê o estado do botão de fim de curso

        if (estado_botao_fim_de_curso == LOW){ // Reduz a velocidade de calibração se o botão de fim de curso for pressionado!
 
          Serial.println("BOTÃO FIM DE CURSO PRESSIONADO - VELOCIDADE REDUZIDA!!!");

          digitalWrite(pino_pulso, HIGH);
          delayMicroseconds(4000); // Velocidade de calibração reduzida (2000 -> 4000); 
          digitalWrite(pino_pulso, LOW);
        }

      }

    } 

  }

}

// FIM DA CALIBRAÇÃO DO MOTOR!!!

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

// 3) FUNÇÃO ESCOLHE SENTIDO HORÁRIO OU ANTI-HORÁRIO DE ACORDO COM A COMPARAÇÃO ENTRE A POSIÇÃO ATUAL (MEDIDA ANTERIOR) E A NOVA ETIQUETA !!! 

void sentido_rotacao(float posicao_atual, float medida_lida) { // Função recebe a posição atual e a medida bipada como parâmetros

  if (posicao_atual > medida_lida){ // Se a medida for menor do que a posição atual do motor:

    digitalWrite(pino_direcao, HIGH); // Atribui HIGH ao pino de direção -> HIGH = HORÁRIO
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("\n MOTOR GIRANDO NO SENTIDO HORÁRIO \n"); // Printa no monitor serial em qual sentido o motor está girando

  }
  else{ // Se a medida for maior que a posição atual do motor

    digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direção -> LOW = ANTI-HORÁRIO
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("\n MOTOR GIRANDO NO SENTIDO ANTI-HORÁRIO \n"); // Printa no monitor serial em qual sentido o motor está girando

  }
  
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 4) FUNÇÃO DE GIRO DO MOTOR - MOVIMENTA O MOTOR 1 CICLO (1 CICLO = 800 PULSOS = 175,9mm = 17,59cm) -> (4548.038658328596 Pulsos = 1000mm = 1m) - USANDO TODAS CASAS DECIMAIS PARA AUMENTAR A PRECISÃO!

void gira_motor(int pino_pulso, float medida_lida, float posicao_atual){ // Função recebe o pino de pulso, medida bipada e a posição atual como parâmetros

  const int velocidade_inicial = 2400;   // Velocidade inicial - Um pouco maior que a velocidade de calibração
  const int velocidade_final = 600;      // Velocidade final - Mais rápida e adequada para o motor
  const int passos_aceleracao = 600;     // Número de passos para alcançar a velocidade final - de 600 em 600 - (600 -> 2400 = 4 passos de aceleração = 4 marchas)

  float qtd_passos; // Variável que define quantas vezes o motor irá girar. POSIÇÃO ATUAL - MEDIDA DE DESTINO (CÓDIGO DE BARRAS)

  qtd_passos = abs((posicao_atual - medida_lida)); // Usamos a função abs() para a subtração sempre retornar um valor positivo, isto é, para não correr o risco de termos um valor negativo (sempre módulo)!

  if (qtd_passos > 0.50){

    int velocidade_atual = velocidade_inicial; // Velocidade inicial recebe a velocidade inicial (2400)

    for (int i = 0; i < (qtd_passos * 4548.038658328596); i++){ // O motor gira x vezes de acordo com a expressão anterior. Altere essa condição de acordo com seu referencial de medidas

      if(estado_botao_parada_total == HIGH){ // Verificação de parada total durante as voltas do motor
        parada_total(); 
      }

      // IF PARA ACELERAR O MOTOR

      if (i < passos_aceleracao) {
        // Ajustar a velocidade gradualmente até a velocidade final
        velocidade_atual -= (velocidade_inicial - velocidade_final) / passos_aceleracao;
      }

      // IF PARA DESACELERAR O MOTOR - QUANDO FALTA 10% PRA CHEGAR NA MEDIDA DE DESTINO (1m = desacelera no 0,90)!

      if (i > (0.95 * (qtd_passos * 4548.038658328596))) {
        // Ajustar a velocidade gradualmente até a velocidade final
        velocidade_atual = 2400;
      }

      digitalWrite(pino_pulso, HIGH);
      delayMicroseconds(velocidade_atual);           // Velocidade de giro do motor durante as leituras de largura e altura (400 = Valor mais adequado para o motor girar razoavelmente rápido e sem ruídos)
      digitalWrite(pino_pulso, LOW);
    
    }

  }

  else{

    for (int i = 0; i < (qtd_passos * 4548.038658328596); i++){ // O motor gira x vezes de acordo com a expressão anterior. Altere essa condição de acordo com seu referencial de medidas

      if(estado_botao_parada_total == HIGH){ // Verificação de parada total durante as voltas do motor
        parada_total(); 
      }

      digitalWrite(pino_pulso, HIGH);
      delayMicroseconds(1200);           // Velocidade menor para peças com menos de 0.5m
      digitalWrite(pino_pulso, LOW);
    
    } 

  }

}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 5) FUNCÃO LOOP - LOOP PRINCIPAL DO PROGRAMA

void loop() { 

  calibra_motor();

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

  posicao_atual = 4.742; // POSIÇÃO DA MESA DE CORTE ONDE O MOTOR FICA APÓS SAIR DO BOTÃO!

  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // !!! PRIMEIRA VERIFICAÇÃO DE PARADA TOTAL - ANTES DA MEDIDA DE LARGURA

  parada_total(); 

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // 5.2) PRIMEIRO CICLO DO VOID LOOP - LARGURAS

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  for(contador = 0; contador < num_pedidos * 2; contador++){ // REPETE A LEITURA DE ETIQUETAS ATÉ A QUANTIDADE DE PEDIDOS (*2 PARA BATER O CÁLCULO)

    // SEGUNDA VERIFICAÇÃO DE PARADA TOTAL 

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

        digitalWrite(pino_rele, LOW); // PINO DO RELÉ LOW = PRENSA SOBE!

        delay(500); //PEQUENO ATRASO PARA NÃO MOVIMENTAR PRENSA E MOTOR SIMULTANEAMENTE

        parada_total(); 

        medida_lida = larguras[i]; // Aqui vamos armazenar o valor correspondente a largura do respectivo índice

        Serial.println(larguras[i]); // Printamos a medida que o motor irá movimentar para não nos perdermos

        // GIRANDO O MOTOR NO PRIMEIRO SENTIDO

        sentido_rotacao(posicao_atual, medida_lida); // Define o sentido de rotação do motor de acordo com os parâmetros da medida da largura
        
        gira_motor(pino_pulso, posicao_atual, medida_lida); // Define a velocidade e quantidade de voltas do motor de acordo com os parâmetros atuais

        posicao_atual = medida_lida;  // POSICAO ATUAL RECEBE O VALOR DA MEDIDA LIDA PARA PRÓXIMAS COMPARAÇÕES

        delay(1000); // ATRASO PARA NÃO DESCER A PRENSA ANTES DE TERMINAR O MOVIMENTO DO MOTOR

        digitalWrite(pino_rele, HIGH); // PINO DO RELÉ HIGH = PRENSA DESCE!
      
      }    // FIM DO IF
    
    }  // FIM DO LOOP FOR QUE ITERA ATÉ O NÚMERO DE  PEDIDOS (CÓDIGOS DE BARRAS)
    
  } // FIM DO LOOP FOR QUE ITERA ATÉ O NÚMERO DO CONTADOR

}  // FIM DA FUNÇÃO VOID LOOP

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
