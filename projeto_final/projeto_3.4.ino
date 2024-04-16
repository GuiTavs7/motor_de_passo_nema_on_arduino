// PROJETO: PERSOL INC
// AUTOR: GUILHERME TAVARES PINHEIRO
// DATA: 16/04/2024
// OBJETIVO: AUTOMAÇÃO DA MESA DE CORTE DE TECIDO - MOVIMENTAR MOTOR DE PASSO NEMA - LARGURA

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 1) ATRIBUINDO O NÚMERO DOS PINOS A SUAS RESPECTIVAS FUNÇÕES

const int pino_pulso = 8;           // CONFIGURA O PINO DO PULSO / PASSO (PRECISÃO)
const int pino_direcao = 9;         // CONFIGURA O PINO DA DIREÇÃO (HORÁRIO / ANTI-HORÁRIO)
const int enable_pin = 13;          // CONFIGURA O PINO ENA
const int pino_rele_medida = 7;     // CONFIGURA O PINO DO RELE DA MEDIDA (CICLO DAS LARGURAS)
const int pino_rele_garra = 11;     // CONFIGURA O PINO DO RELE DA GARRA (CICLO DAS ALTURAS)

const int botao_inicio = 3;         // CONFIGURA O PINO DO BOTÃO DE INÍCIO
const int botao_fim_de_curso = 2;   // CONFIGURA O PINO DO BOTÃO DE FIM DE CURSO
const int botao_parada_total = 4;   // CONFIGURA O PINO DO BOTÃO DE PARADA TOTAL
const int botao_afasta_motor = 6;   // CONFIGURA O PINO DO BOTÃO QUE AFASTA MOTOR
const int botao_altura = 5;         // CONFIGURA O PINO DO BOTÃO DE ALTURA

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 2) CRIAÇÃO DAS VARIÁVEIS - VETORES E ITERÁVEIS

long codigos[] = {15832775,15832738,15832701,15832667,15832630,15832593,15832559};    // Vetor de códigos de barras
float larguras[] = {4.5, 4.2, 4.3, 4.2, 4.1, 4.7, 4.75};                              // Vetor de larguras correspondentes às peças conforme os códigos de barras (em metros)
float alturas[] = {4.0, 4.3, 4.3, 4.3, 4.4, 4.5, 4.65};                               // Vetor de alturas correspondentes às peças conforme os códigos de barras (em metros)

int num_pedidos = 7;                                      // Número de pedidos (etiquetas) - altere conforme necessário                                                        
int segundo_ciclo = 0;                                    // Inicializa segundo ciclo com 0
int contador = 0;                                         // Inicializa contador com 0
int estado_botao_parada_total = HIGH;                     // Estado do botão de parada total

float posicao_atual;                                      // Variável para armazenar a posição atual do motor                       
float medida_lida;                                        // Variável para armazenar a posição de destino 
                               
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 3) FUNÇÃO SETUP - CONFIGURAÇÕES INICIAIS - ENTRADA, SAÍDA E COMUNICAÇÃO SERIAL

void setup() {

  pinMode(pino_pulso, OUTPUT);               // DEFINE PINO PULSO COMO SAÍDA
  pinMode(pino_direcao, OUTPUT);             // DEFINE PINO DIRECAO COMO SAÍDA
  pinMode(enable_pin, OUTPUT);               // DEFINE PINO ENABLE COMO SAÍDA
  pinMode(pino_rele_medida, OUTPUT);         // DEFINE PINO DO RELÉ COMO SAÍDA
  pinMode(pino_rele_garra, OUTPUT);          // DEFINE PINO DO RELÉ GARRA COMO SAÍDA


  pinMode(botao_inicio, INPUT_PULLUP);         // DEFINE O BOTÃO DE INÍCIO COMO ENTRADA E COM RESISTOR INTERNO
  pinMode(botao_fim_de_curso, INPUT_PULLUP);   // DEFINE O BOTÃO FIM DE CURSO COMO ENTRADA E COM RESISTOR INTERNO
  pinMode(botao_parada_total, INPUT_PULLUP);   // DEFINE O BOTÃO PARADA TOTAL COMO ENTRADA E COM RESISTOR INTERNO
  pinMode(botao_afasta_motor, INPUT_PULLUP);   // DEFINE O BOTÃO AFASTA MOTOR COMO ENTRADA E COM RESISTOR INTERNO
  pinMode(botao_altura, INPUT_PULLUP);         // DEFINE O BOTÃO DE ALTURA COMO ENTRADA E COM RESISTOR INTERNO

  Serial.begin(9600);                        // INICIALIZA A COMUNICAÇÃO SERIAL COM UMA TAXA DE 9600 BAUD

} // FIM DA FUNÇÃO SETUP

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // 4) FUNÇÃO DE CALIBRAÇÃO DO MOTOR 

void calibra_motor(){

  if (estado_botao_parada_total == HIGH){ // Continua a calibração enquanto o botão de parada total não for pressionado

    delay(100); //PEQUENO ATRASO PARA NÃO MOVIMENTAR PRENSA E MOTOR SIMULTANEAMENTE

    digitalWrite(pino_rele_medida, LOW); // PINO DO RELÉ LOW = PRENSA SOBE!

    digitalWrite(pino_rele_garra, LOW); // PINO DO RELÉ GARRA LOW = GARRA SOBE!

    delay(100); //PEQUENO ATRASO PARA NÃO MOVIMENTAR PRENSA E MOTOR SIMULTANEAMENTE

    digitalWrite(pino_direcao, LOW); // Atribui LOW ao pino de direção -> LOW = ANTI-HORÁRIO - Sentido de calibração
    delayMicroseconds(1000); //Atraso de 1 segundo
    Serial.println("\n CALIBRANDO O MOTOR ATÉ POSIÇÃO INICIAL \n"); // Mensagem inicial de calibração do motor

    for(int x = 0; x < 24000; x++){ // Loop For para girar o motor no sentido de calibração - 24000 = 30 Voltas em torno de si ~= 17,5cm * 30 = 5.25m - Calibra em qualquer posição da mesa

      while (estado_botao_parada_total == HIGH){ // Permanece calibrando até que o botão de parada total seja acionado

        parada_total(); // Chama a função parada total

        digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
        delayMicroseconds(1100); // VELOCIDADE DE CALIBRAÇÃO
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

    digitalWrite(enable_pin, LOW); // HABILITA O MOTOR APÓS UMA POSSÍVEL PARADA TOTAL

    // 4.1) MOTOR GIRA UMA VOLTA LENTAMENTE PARA NÃO MANTER O BOTÃO DE PARADA TOTAL DA CALIBRAÇÃO PRESSIONADO!!!

    digitalWrite(pino_direcao, HIGH); // Atribui HIGH ao pino de direção -> HIGH = HORÁRIO
    delayMicroseconds(500); //Atraso de meio segundo
    Serial.println("\n INÍCIO DO PROGRAMA PRINCIPAL!!! \n");
    Serial.println("\n RETIRANDO O MOTOR DO BOTÃO \n"); // Printa no monitor serial em qual sentido o motor está girando

    for(int x = 0; x < 800; x++){ //DÁ UMA VOLTA PARA SAIR DO BOTÃO
     
      digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
      delayMicroseconds(2000); // Atraso para regular a velocidade do motor (QUANTO MENOR ESSE ATRASO MAIS RÁPIDO O MOTOR GIRA!!!)
      digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

    } // FIM DO For

    posicao_atual = 4.727; // POSIÇÃO DA MESA DE CORTE ONDE O MOTOR FICA APÓS SAIR DO BOTÃO!

  }

}

// FIM DA CALIBRAÇÃO DO MOTOR!!!

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 5) FUNÇÃO DE PARADA TOTAL !!! 

void parada_total() {

  estado_botao_parada_total = digitalRead(botao_parada_total); // LÊ O ESTADO DO BOTÃO DE PARADA TOTAL

  if(estado_botao_parada_total == LOW){ // Se o botão de parada total for pressionado:

    Serial.println("\n BOTÃO PARADA TOTAL PRESSIONADO!!! \n");

    digitalWrite(enable_pin, HIGH); // Ativa o pino ENA - Desabilita o movimento do motor

    delay(400); // Atraso por segurança

  }
  
} // FIM DA FUNÇÃO DE PARADA TOTAL

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 6) FUNÇÃO SENTIDO DE GIRO

void sentido_rotacao(float posicao_atual, float medida_lida) {   // Função recebe a posição atual e a medida bipada como parâmetros

  if (posicao_atual > medida_lida){ // Se a medida for menor do que a posição atual do motor:

    digitalWrite(pino_direcao, HIGH);   // Atribui HIGH ao pino de direção -> HIGH = HORÁRIO
    delayMicroseconds(1000);   //Atraso de 1 micro-segundo(imperceptível)
    Serial.println("\n MOTOR GIRANDO NO SENTIDO HORÁRIO \n");   // Printa no monitor serial em qual sentido o motor está girando

  }
  else{   // Se a medida for maior que a posição atual do motor:

    digitalWrite(pino_direcao, LOW);   // Atribui LOW ao pino de direção -> LOW = ANTI-HORÁRIO
    delayMicroseconds(1000);   //Atraso de 1 micro-segundo(imperceptível)
    Serial.println("\n MOTOR GIRANDO NO SENTIDO ANTI-HORÁRIO \n");   // Printa no monitor serial em qual sentido o motor está girando

  }
  
} // FIM DA FUNÇÃO DE ESCOLHER SENTIDO 

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 7) FUNÇÃO DE MOVIMENTO - MOVIMENTA O MOTOR 1 CICLO (1 CICLO = 800 PULSOS = 175,98mm = 17,598cm) -> (4545.971133083305 Pulsos = 1000mm = 1m) - USANDO TODAS CASAS DECIMAIS PARA AUMENTAR A PRECISÃO

void gira_motor(int pino_pulso, float medida_lida, float posicao_atual){   // Função recebe o pino de pulso, medida bipada e a posição atual como parâmetros

  const int velocidade_inicial = 2400;   // Velocidade inicial - Um pouco maior que a velocidade de calibração
  const int velocidade_final = 600;      // Velocidade final - Mais rápida e adequada para o motor
  const int passos_aceleracao = 600;     // Número de passos para alcançar a velocidade final - de 600 em 600 - (600 -> 2400 = 4 passos de aceleração = 4 marchas)

  float distancia_por_ciclo = 175.98;   // Variável de distância por ciclo! Alterar para testar calibração

  float constante = (800000 / distancia_por_ciclo);  // Cálculo para definir a constante de precisão do motor

  float qtd_passos; // Variável que define quantas vezes o motor irá girar. POSIÇÃO ATUAL - MEDIDA DE DESTINO (CÓDIGO DE BARRAS)

  qtd_passos = abs((posicao_atual - medida_lida)); // Usamos a função abs() para a subtração sempre retornar um valor positivo, isto é, para não correr o risco de termos um valor negativo (sempre módulo)!

  if (qtd_passos > 0.50){    // Se a diferença entre a largura da peça e a posição atual do motor for maior que meio-metro:

    int velocidade_atual = velocidade_inicial; // Velocidade inicial recebe a velocidade inicial (2400)

    for (int i = 0; i < (qtd_passos * constante); i++){ // O motor gira x vezes de acordo com a expressão anterior. Altere essa condição de acordo com seu referencial de medidas

      if(estado_botao_parada_total == HIGH){ // Verificação de parada total durante as voltas do motor
        parada_total(); 
      }

      // 7.1) IF PARA ACELERAR O MOTOR

      if (i < passos_aceleracao) {
        // Ajustar a velocidade gradualmente até a velocidade final
        velocidade_atual -= (velocidade_inicial - velocidade_final) / passos_aceleracao;
      }

      // 7.2) IF PARA DESACELERAR O MOTOR - QUANDO FALTA 5% PRA CHEGAR NA MEDIDA DE DESTINO (1m = desacelera no 0,95)!

      if (i > (0.95 * (qtd_passos * constante))) {
        // Ajustar a velocidade gradualmente até a velocidade final
        velocidade_atual = 1200;
      }

      digitalWrite(pino_pulso, HIGH);
      delayMicroseconds(velocidade_atual);  // Velocidade de giro do motor durante as leituras de largura e altura (400 = Valor mais adequado para o motor girar razoavelmente rápido e sem ruídos)
      digitalWrite(pino_pulso, LOW);
    
    }

  }

  // 7.3) Peças menores de até 0,5m

  else{

    for (int i = 0; i < (qtd_passos * constante); i++){   // O motor gira x vezes de acordo com a expressão anterior. Altere essa condição de acordo com seu referencial de medidas

      if(estado_botao_parada_total == HIGH){ // Verificação de parada total durante as voltas do motor
        parada_total(); 
      }

      digitalWrite(pino_pulso, HIGH);
      delayMicroseconds(1200);          // Velocidade menor para peças com menos de 0.5m
      digitalWrite(pino_pulso, LOW);
    
    } 

  }

} // FIM DA FUNÇÃO DE MOVIMENTO DO MOTOR

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// 8) FUNÇÃO AFASTA MOTOR !!! 

void afasta_motor(float &posicao_atual){   //"&" torna a variável posição_atual intercambiável entre a função "afasta_motor()" e void loop() 

  int x = 0;   // Declara e inicializa x com 0

  digitalWrite(pino_rele_medida, LOW);   // PINO DO RELÉ LOW = PRENSA SOBE!

  digitalWrite(pino_rele_garra, LOW); // PINO DO RELÉ GARRA LOW = GARRA SOBE!

  delay(150);   // ATRASO DE MEIO SEGUNDO PARA NÃO TRANCAR A MESA

  int estado_botao_afasta_motor = digitalRead(botao_afasta_motor); // Lê o estado do botao_afasta_motor

  if (estado_botao_afasta_motor == LOW){ // Se o botão for apertado:
    
    digitalWrite(pino_direcao, LOW);
    delayMicroseconds(1000);
    Serial.println("\n MOTOR AFASTANDO NO SENTIDO ANTI-HORÁRIO \n");

    for (x = 0; x < 24000; x++) {   // 24000 = 30 Voltas em torno de si ~= 17,5cm * 30 = 5.25m - Capaz de afastar o motor em toda extensão da mesa
      
      estado_botao_afasta_motor = digitalRead(botao_afasta_motor); // Verifica continuamente o estado do botão dentro do loop

      posicao_atual = posicao_atual + 0.000219875; //posicao_atual = posicao_atual + (x * 0.000219875); / Deslocamento por passo / Passos de 1 volta = 0,1759m / 800 passos = 0,000219875

      if (estado_botao_afasta_motor == HIGH) { // || posicao_atual > 4.80
        break; // Sai do loop se o botão de afastar for solto
      }
      else if (posicao_atual > 4.80){
        calibra_motor();               // Chama a função de calibração de motor após 4.80 por segurança!
      }

      digitalWrite(pino_pulso, HIGH);
      delayMicroseconds(1000);         // VELOCIDADE DO MOTOR DURANTE O AFASTAMENTO
      digitalWrite(pino_pulso, LOW);
     
    }  

   Serial.println(posicao_atual); // Printa no monitor serial a posição em que foi solto o botão
  
  }

} // FIM DA FUNÇÃO DE AFASTAR O MOTOR


// 9) FUNÇÃO INÍCIO !!! 

void inicio(float &posicao_atual){   //"&" torna a variável posição_atual intercambiável entre a função "afasta_motor()" e void loop() 

  int estado_botao_inicio = digitalRead(botao_inicio); // LÊ O ESTADO DO BOTÃO DE INÍCIO

  if(estado_botao_inicio == LOW){ // Se o botão de início for pressionado:

  float medida_lida = 4.40;

  float qtd_passos = abs((posicao_atual - 4.40)); // Substitua o 4.40 para sua posição inicial

    Serial.println("\n BOTÃO DE INÍCIO PRESSIONADO!!! \n");

    digitalWrite(pino_rele_medida, LOW);   // PINO DO RELÉ LOW = PRENSA SOBE!

    sentido_rotacao(posicao_atual, medida_lida);

    for (int x = 0; x < (qtd_passos * 4545.971133083305); x++) {   
 
      digitalWrite(pino_pulso, HIGH);
      delayMicroseconds(1000);         // VELOCIDADE DO MOTOR DURANTE IDA PARA O INICIO
      digitalWrite(pino_pulso, LOW);
     
    }

    digitalWrite(pino_rele_garra, HIGH); // PINO DO RELÉ GARRA HIGH = GARRA DESCE!

    float posicao_inicial = 4.40;

    posicao_atual = posicao_inicial;  // 4.40

  }
  
} // FIM DA FUNÇÃO DE INÍCIO

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 10) FUNCÃO LOOP - LOOP PRINCIPAL DO PROGRAMA

void loop() {

  // 10.1) CHAMA A FUNÇÃO DE CALIBRAÇÃO DO MOTOR

  if(contador == 0){ // GARANTE QUE O MOTOR SEJA CALIBRADO APENAS UMA VEZ POR LOOP (APERTAR RESET PARA CALIBRAR!)

    calibra_motor(); 

    contador = 1;

  }

  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // !!! PRIMEIRA VERIFICAÇÃO DE PARADA TOTAL - ANTES DA MEDIDA DE LARGURA

  parada_total(); 

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // 10.2) ESPERA ATÉ APERTAR O BOTÃO DE AFASTAR MOTOR, OU BIPAR PEÇA

  Serial.println("\n BIPE O CÓDIGO DE BARRAS OU APERTE O BOTÃO DE AFASTAR O MOTOR! ");

  while ((Serial.available() == 0) && digitalRead(botao_afasta_motor) == HIGH && digitalRead(botao_inicio) == HIGH){

  }

  if (digitalRead(botao_inicio) == LOW) {
      
      inicio(posicao_atual); // CHAMA A FUNÇÃO DE INÍCIO - MOTOR VAI PARA 1.45 (PREPARADO PARA PEGAR TECIDO)

  }
  
  // LOOP INTERROMPIDO - BOTÃO AFASTAR MOTOR APERTADO OU ETIQUETA BIPADA

  // 10.3) 1º CASO - BOTÃO DE AFASTAR O MOTOR APERTADO
  
  if (digitalRead(botao_afasta_motor) == LOW) {
      
    afasta_motor(posicao_atual);   // Chama a função de afastar o motor

    posicao_atual = posicao_atual;   // Posição atual recebe a posição em que o botão foi solto

  }
    
  // 10.4) 2º CASO - ETIQUETA BIPADA
  
  if (Serial.available() > 0) { // Caso dados sejam recebidos na porta serial:

    long codigo = Serial.parseInt(); // Variável código recebe o que o usuário inseriu na porta serial

    // LOOP FOR PARA ITERAR ATÉ O NÚMERO DE PEDIDOS

    for (int i = 0; i < num_pedidos; i++) {

      // !!! VERIFICAÇÃO DE PARADA TOTAL !!!

      parada_total();

      if (codigos[i] == codigo) { // Verifica se o código de barras digitado ou bipado corresponde à alguns dos códigos no vetor códigos[]

        digitalWrite(pino_rele_medida, LOW); // PINO DO RELÉ LOW = PRENSA SOBE!

        digitalWrite(pino_rele_garra, LOW); // PINO DO RELÉ GARRA LOW = GARRA SOBE!

        delay(150); //PEQUENO ATRASO PARA NÃO MOVIMENTAR PRENSA E MOTOR SIMULTANEAMENTE

        parada_total(); // Verifica estado do botão de parada total

        medida_lida = alturas[i]; // Aqui vamos armazenar o valor correspondente a largura do respectivo índice

        Serial.println(alturas[i]); // Printamos a medida que o motor irá movimentar para não nos perdermos

        sentido_rotacao(posicao_atual, medida_lida); // Define o sentido de rotação do motor de acordo com os parâmetros da medida da largura

        segundo_ciclo = 0; // REINICIA O ESTADO DO BOTÃO PARA 0 - IMPORTANTE PARA COMEÇAR O SEGUNDO LOOP (ALTURA)
        
        gira_motor(pino_pulso, posicao_atual, medida_lida); // Define a velocidade e quantidade de voltas do motor de acordo com os parâmetros atuais

        posicao_atual = medida_lida;  // POSICAO ATUAL RECEBE O VALOR DA MEDIDA LIDA PARA PRÓXIMAS COMPARAÇÕES

        delay(350); // ATRASO PARA NÃO DESCER A PRENSA ANTES DE TERMINAR O MOVIMENTO DO MOTOR

        digitalWrite(pino_rele_garra, HIGH); // PINO DO RELÉ HIGH = GARRA DESCE!

        delay(500);

        digitalWrite(pino_direcao, LOW);
        delayMicroseconds(1000);
        Serial.println("\n MOTOR AFASTANDO 10cm (SENTIDO HORÁRIO) \n");

        for (int x = 0; x < 454.5971133083305; x++) {   // x < 454.60 aproximadamente - afasta 10cm (800 = 17,598cm)
 
          digitalWrite(pino_pulso, HIGH);
          delayMicroseconds(1000);         // VELOCIDADE DO MOTOR AFASTANDO 10cm
          digitalWrite(pino_pulso, LOW);
     
        }

        posicao_atual = medida_lida + 0.10; // Posição atual recebe mais 10cm (trabalhamos em metros)

        digitalWrite(pino_rele_garra, LOW); // PINO DO RELÉ LOW = GARRA SOBE! (NÃO SE MOVIMENTA NO CICLO DE LARGURAS!)
      
      }    // FIM DO IF
    
    } // FIM DO LOOP FOR QUE ITERA ATÉ O NÚMERO DE  PEDIDOS (CÓDIGOS DE BARRAS)

    // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // VERIFICAÇÃO DE PARADA TOTAL - ENTRE CICLOS DE LARGURA E ALTURA

    parada_total();

    // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // 10.5) SEGUNDO CICLO - ALTURAS - SÓ PROSSEGUE SE O BOTÃO PARA LER A MEDIDA DA ALTURA FOR PRESSIONADO

    while(segundo_ciclo == 0){

      // O programa só continua se o botão for pressionado

      if (digitalRead(botao_altura) == LOW) { 

        digitalWrite(pino_rele_medida, LOW); // PINO DO RELÉ LOW = PRENSA SOBE!

        delay(500); //PEQUENO ATRASO PARA NÃO MOVIMENTAR PRENSA E MOTOR SIMULTANEAMENTE

        // !!! VERIFICAÇÃO DE PARADA TOTAL DURANTE SEGUNDO CICLO - ALTURA !!!

        parada_total();

        // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        Serial.println("BOTÃO DO SEGUNDO CICLO PRESSIONADO!!! - Iniciando segundo ciclo - ALTURA! \n");

        // LOOP FOR PARA ITERAR ATÉ O NÚMERO DE PEDIDOS

        for (int i = 0; i < num_pedidos; i++) {

          // !!! VERIFICAÇÃO DE PARADA TOTAL !!!  

          parada_total();

          if (codigos[i] == codigo) {  // Verifica se o código de barras digitado ou bipado corresponde à alguns dos códigos no vetor códigos[]

            medida_lida = larguras[i]; // Aqui vamos armazenar o valor correspondente a altura do respectivo índice

            Serial.println(larguras[i]); // Printa o valor correspondente ao índice altura = quantidade de voltas do motor
         
            sentido_rotacao(posicao_atual, medida_lida);  // Define o sentido de rotação do motor de acordo com os parâmetros da medida da altura

            gira_motor(pino_pulso, posicao_atual, medida_lida);  // Define a velocidade e quantidade de voltas do motor de acordo com os parâmetros atuais

            posicao_atual = medida_lida;  // POSICAO ATUAL RECEBE O VALOR DA MEDIDA LIDA PARA PRÓXIMAS COMPARAÇÕES

            delay(1000); // ATRASO PARA NÃO DESCER A PRENSA ANTES DE TERMINAR O MOVIMENTO DO MOTOR

            digitalWrite(pino_rele_medida, HIGH); // PINO DO RELÉ HIGH = PRENSA DESCE!
          
          } // FIM DO IF
        
        } // FIM DO LOOP FOR QUE ITERA ATÉ O NÚMERO DE  PEDIDOS (CÓDIGOS DE BARRAS)

        break; // Força a saída do loop
      
      } // FIM DO IF DO BOTÃO

      delay(1000); // Pequeno atraso para evitar detecção falsa de pressionamento do botão
    
    } // FIM DO LOOP WHILE DO SEGUNDO CICLO

    segundo_ciclo = 1; //ATRIBUI 1 AO SEGUNDO CICLO PRA NÃO FICAR PRESO NO BOTÃO - VOLTAR AO PRIMEIRO CICLO (LARGURA)
  
  }  // FIM DO IF DO SERIAL AVAILABLE
    
}  // FIM DA FUNÇÃO VOID LOOP

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
