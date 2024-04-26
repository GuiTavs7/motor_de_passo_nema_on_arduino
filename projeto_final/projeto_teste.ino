// PROJETO: PERSOL INC
// AUTOR: GUILHERME TAVARES PINHEIRO
// DATA: 26/04/2024
// OBJETIVO: AUTOMAÇÃO DA MESA DE CORTE DE TECIDO - MOVIMENTAR MOTOR DE PASSO NEMA - ALTURAS E LARGURAS

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 1) ATRIBUINDO O NÚMERO DOS PINOS A SUAS RESPECTIVAS FUNÇÕES

const int pino_pulso = 8;           // CONFIGURA O PINO DO PULSO / PASSO (PRECISÃO)
const int pino_direcao = 9;         // CONFIGURA O PINO DA DIREÇÃO (HORÁRIO / ANTI-HORÁRIO)
const int enable_pin = 13;          // CONFIGURA O PINO ENA
const int pino_rele_medida = 7;     // CONFIGURA O PINO DO RELE DA MEDIDA (CICLO DAS LARGURAS)
const int pino_rele_garra_1 = 11;   // CONFIGURA O PINO DO RELE DA GARRA (CICLO DAS ALTURAS) - SOBE GARRA
const int pino_rele_garra_2 = 12;   // CONFIGURA O PINO DO RELE DA GARRA (CICLO DAS ALTURAS) - DESCE GARRA

const int botao_inicio = 3;         // CONFIGURA O PINO DO BOTÃO DE INÍCIO
const int botao_fim_de_curso = 2;   // CONFIGURA O PINO DO BOTÃO DE FIM DE CURSO
const int botao_parada_total = 4;   // CONFIGURA O PINO DO BOTÃO DE PARADA TOTAL
const int botao_afasta_motor = 6;   // CONFIGURA O PINO DO BOTÃO QUE AFASTA MOTOR
const int botao_largura = 5;        // CONFIGURA O PINO DO BOTÃO DE ALTURA

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 2) CRIAÇÃO DAS VARIÁVEIS - VETORES E ITERÁVEIS - VETORES ALIMENTADOS COM OS 3 ÚLTIMOS PCP DAS PEÇAS ATUAIS

long codigos[] = {15832775,15832738,15832701,15832667,15832630,15832593,15832559,15898277,15898316,15898358,15898359,15898395,15898431,15870164,15870199,15870233,15870277,15870320,15870354,15870388,15870432,15870475,15870509,15870544,15870579,15870614,15867382,15867426,15891631,15891854,15891900,15891946,15892038,15892085,15892131,15892177,15892269,15861880,15861881,15890026,15898470,15894447,15886812,15886843,15886875,15892315,15883157,15883205,15883252,15883295,15883338,15883386,15883433,15883476,15899022,15898984,15891109,15891146,15891183,15891220,15891257,15891294,15891341,15891387,15892390,15890101,15890138,15890350,15890385,15890420,15890454,15890488,15890524,15891433,15891466,15891499,15891532,15891565,15891598,15890799,15890832,15890965,15891002,15892500,15892535,15892801,15892839,15892877,15892915,15892953,15892987,15893021,15893593,15893641,15893719,15898836};  // VETOR DE CÓDIGO DE BARRAS
float larguras[] = {1.19, 4.2, 4.3, 4.2, 4.1, 4.7, 4.75, 2.816, 3.098, 1.813, 1.813, 1.813, 1.749, 3.328, 3.218, 2.358, 2.158, 3.378, 3.258, 2.358, 2.158, 3.208, 3.653, 3.653, 3.348, 3.358, 1.908, 1.908, 1.823, 2.548, 2.548, 2.548, 1.668, 2.668, 2.668, 2.328, 1.668, 3.168, 3.168, 1.618, 1.536, 1.773, 3.138, 2.798, 2.458, 2.375, 2.673, 2.663, 3.188, 3.128, 2.748, 2.748, 3.258, 2.758, 2.235, 1.377, 1.868, 1.868, 1.868, 1.868, 1.868, 1.708, 1.408, 1.708, 2.679, 2.175, 2.175, 2.068, 2.068, 2.948, 2.898, 1.717, 1.717, 1.371, 1.371, 1.801, 1.871, 2.458, 2.458, 1.373, 1.373, 2.123, 2.168, 1.685, 1.685, 2.288, 2.288, 2.208, 2.208, 1.393, 1.393, 1.393, 2.173, 2.107, 2.368, 2.890};   // VETOR DE LARGURAS DAS PEÇAS (EM METROS)
float alturas[] = {4.81, 4.3, 4.3, 4.3, 4.4, 4.5, 4.65, 3.230, 4.940, 1.420, 1.420, 1.420, 1.320, 2.260, 2.260, 2.510, 2.510, 2.510, 2.510, 2.510, 2.510, 2.510, 2.580, 2.580, 2.260, 2.510, 4.660, 4.660, 2.810, 2.990, 2.990, 2.990, 2.990, 2.990, 2.990, 2.990, 2.990, 1.560, 1.560, 3.510, 1.650, 1.530, 1.610, 1.610, 1.610, 1.510, 2.020, 2.020, 1.860, 2.260, 2.490, 2.510, 2.260, 2.490, 2.080, 1.350, 3.170, 3.170, 3.170, 3.170, 3.170, 3.170, 3.170, 3.170, 2.510, 1.610, 1.610, 1.410, 1.410, 3.220, 3.220, 1.755, 1.755, 2.180, 2.180, 3.070, 3.070, 2.490, 2.490, 1.550, 1.605, 1.330, 1.855, 1.560, 1.498, 2.560, 2.560, 2.560, 2.560, 1.950, 1.950, 1.950, 1.580, 1.530, 1.360, 1.530 };   // VETOR DAS ALTURAS DAS PEÇAS (EM METROS)

int num_pedidos = 7;                                      // NÚMERO DE PEDIDOS / ETIQUETAS (ALTERE CONFORME NECESSÁRIO)                                                 
int segundo_ciclo = 0;                                    // CONTADOR DO SEGUNDO CICLO (INICIALIZA COM 0)
int contador = 0;                                         // CONTADOR PARA CALIBRAÇÃO DO MOTOR (INICIALIZA COM 0)
int estado_botao_parada_total = HIGH;                     // ESTADO DO BOTÃO DE PARADA TOTAL - INICIALIZA COM HIGH (NÃO PRESSIONADO)

float posicao_atual;                                      // VARIÁVEL PARA ARMAZENAR POSIÇÃO ATUAL DO MOTOR                     
float medida_lida;                                        // VARIÁVEL PARA ARMAZENAR POSIÇÃO DE DESTINO DO MOTOR

float medida_minima = 1.30;
float medida_maxima = 4.80;
                               
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 3) FUNÇÃO SETUP - CONFIGURAÇÕES INICIAIS - ENTRADA, SAÍDA E COMUNICAÇÃO SERIAL

void setup() {

  pinMode(pino_pulso, OUTPUT);               // DEFINE PINO PULSO COMO SAÍDA
  pinMode(pino_direcao, OUTPUT);             // DEFINE PINO DIRECAO COMO SAÍDA
  pinMode(enable_pin, OUTPUT);               // DEFINE PINO ENABLE COMO SAÍDA
  pinMode(pino_rele_medida, OUTPUT);         // DEFINE PINO DO RELÉ COMO SAÍDA
  pinMode(pino_rele_garra_1, OUTPUT);        // DEFINE PINO DO RELÉ SOBE GARRA COMO SAÍDA
  pinMode(pino_rele_garra_2, OUTPUT);        // DEFINE PINO DO RELÉ DESCE GARRA COMO SAÍDA


  pinMode(botao_inicio, INPUT_PULLUP);         // DEFINE O BOTÃO DE INÍCIO COMO ENTRADA E COM RESISTOR INTERNO
  pinMode(botao_fim_de_curso, INPUT_PULLUP);   // DEFINE O BOTÃO FIM DE CURSO COMO ENTRADA E COM RESISTOR INTERNO
  pinMode(botao_parada_total, INPUT_PULLUP);   // DEFINE O BOTÃO PARADA TOTAL COMO ENTRADA E COM RESISTOR INTERNO
  pinMode(botao_afasta_motor, INPUT_PULLUP);   // DEFINE O BOTÃO AFASTA MOTOR COMO ENTRADA E COM RESISTOR INTERNO
  pinMode(botao_largura, INPUT_PULLUP);        // DEFINE O BOTÃO DE ALTURA COMO ENTRADA E COM RESISTOR INTERNO
  
  Serial.begin(9600);                          // INICIALIZA COMUNICAÇÃO SERIAL COM UMA TAXA DE 9600 BAUD RATE

} // FIM DA FUNÇÃO SETUP

  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // 4) FUNÇÃO DE CALIBRAÇÃO DO MOTOR 

void calibra_motor(){

  if (estado_botao_parada_total == HIGH){ // Continua a calibração enquanto o botão de parada total não for pressionado

    delay(100); //PEQUENO ATRASO PARA NÃO MOVIMENTAR PRENSA E MOTOR SIMULTANEAMENTE

    digitalWrite(pino_rele_medida, LOW);  // LOW = PRENSA SOBE!

    digitalWrite(pino_rele_garra_1, LOW);  // 1 LOW && 2 LOW  = GARRA SOBE!

    digitalWrite(pino_rele_garra_2, LOW);  // 1 LOW && 2 LOW = GARRA SOBE!

    delay(600); // ATRASO PARA PERMITIR SUBIDA TOTAL DA GARRA

    digitalWrite(pino_rele_garra_1, LOW);   // 1 LOW && 2 HIGH = SEM AR - PARA O PISTÃO - SOLENOIDE DESLIGADO PARA NÃO ESQUENTAR

    digitalWrite(pino_rele_garra_2, HIGH);  // 1 LOW && 2 HIGH = SEM AR - PARA O PISTÃO - SOLENOIDE DESLIGADO PARA NÃO ESQUENTAR

    digitalWrite(pino_direcao, LOW); // LOW = ANTI-HORÁRIO - SENTIDO DE CALIBRAÇÃO
    delayMicroseconds(1000);
    Serial.println("\n CALIBRANDO O MOTOR! \n"); 

    for(int x = 0; x < 24000; x++){ // Loop For para girar o motor no sentido de calibração - 24000 = 30 Voltas em torno de si ~= 17,5cm * 30 = 5.25m - Calibra em qualquer posição da mesa

      while (estado_botao_parada_total == HIGH){ // Permanece calibrando até que o botão de parada total seja acionado

        parada_total(); // Chama a função parada total 

        digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
        delayMicroseconds(1200); // VELOCIDADE DE CALIBRAÇÃO
        digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

        int estado_botao_fim_de_curso = digitalRead(botao_fim_de_curso); // Lê o estado do botão de fim de curso

        if (estado_botao_fim_de_curso == LOW){ // Reduz a velocidade de calibração quando o botão de fim de curso for pressionado!
 
          Serial.println("BOTÃO FIM DE CURSO PRESSIONADO - VELOCIDADE REDUZIDA!!!");

          digitalWrite(pino_pulso, HIGH);
          delayMicroseconds(4000); // Velocidade de calibração reduzida (1200 -> 4000); 
          digitalWrite(pino_pulso, LOW);
        }

      }

    }

    digitalWrite(enable_pin, LOW); // HABILITA O MOTOR APÓS UMA POSSÍVEL PARADA TOTAL

    // 4.1) MOTOR GIRA UMA VOLTA LENTAMENTE PARA NÃO MANTER O BOTÃO DE PARADA TOTAL DA CALIBRAÇÃO PRESSIONADO!!!

    digitalWrite(pino_direcao, HIGH); // HIGH = HORÁRIO
    delayMicroseconds(500);
    Serial.println("\n INÍCIO DO PROGRAMA PRINCIPAL!!! \n");
    Serial.println("\n RETIRANDO O MOTOR DO BOTÃO \n"); 

    for(int x = 0; x < 800; x++){ // Á UMA VOLTA PARA SAIR DO BOTÃO
     
      digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
      delayMicroseconds(2000); 
      digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA

    } // FIM DO For

    posicao_atual = 4.727; // POSIÇÃO ATUAL RECEBE A POSIÇÃO APÓS CALIBRAÇÃO! (MUITO IMPORTANTE!!!) 

  }

}

// FIM DA CALIBRAÇÃO DO MOTOR!!!

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 5) FUNÇÃO DE PARADA TOTAL !!! 

void parada_total() {

  estado_botao_parada_total = digitalRead(botao_parada_total); // Lê o estado do botão de parada total

  if(estado_botao_parada_total == LOW){ // Se o botão de parada total for pressionado:

    Serial.println("\n BOTÃO PARADA TOTAL PRESSIONADO!!! \n");

    digitalWrite(enable_pin, HIGH); // Ativa o pino ENA - Desabilita o movimento do motor

    delay(400); // Atraso por segurança

  }
  
} // FIM DA FUNÇÃO DE PARADA TOTAL

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 6) FUNÇÃO SENTIDO DE GIRO

void sentido_rotacao(float posicao_atual, float medida_lida) {   // Função recebe a posição atual e a medida bipada como parâmetros

  if (posicao_atual > medida_lida){     // Se a medida for menor do que a posição atual do motor:

    digitalWrite(pino_direcao, HIGH);   // HIGH = HORÁRIO - EM DIREÇÃO AO COMEÇO DA MESA
    delayMicroseconds(1000);            // Imperceptível

    Serial.println("\n MOTOR GIRANDO NO SENTIDO HORÁRIO \n");   

  }
  else{   // Se a medida for maior que a posição atual do motor:

    digitalWrite(pino_direcao, LOW);   // LOW = ANTI-HORÁRIO - EM DIREÇÃO AO FIM DA MESA 
    delayMicroseconds(1000);           // Imperceptível

    Serial.println("\n MOTOR GIRANDO NO SENTIDO ANTI-HORÁRIO \n");   

  }
  
} // FIM DA FUNÇÃO DE ESCOLHER SENTIDO 

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 7) FUNÇÃO DE MOVIMENTO - MOVIMENTA O MOTOR 1 CICLO (1 CICLO = 800 PULSOS = 175,98mm = 17,598cm) -> (4545.971133083305 Pulsos = 1000mm = 1m = Constante) - USANDO TODAS CASAS DECIMAIS PARA AUMENTAR A PRECISÃO

void gira_motor(int pino_pulso, float medida_lida, float posicao_atual){   // Função recebe o pino de pulso, medida de destino e a posição atual como parâmetros (argumentos)

  const int velocidade_inicial = 2400;   // Velocidade inicial 
  const int velocidade_final = 600;      // Velocidade final - Mais rápida e adequada para o motor
  const int passos_aceleracao = 600;     // Número de passos para alcançar a velocidade final - de 600 em 600 - (600 -> 2400 = 4 passos de aceleração = 4 marchas)

  float distancia_por_ciclo = 175.98;   // Variável de distância por ciclo! Alterar para testar calibração

  float constante = (800000 / distancia_por_ciclo);  // Cálculo para definir a constante de precisão do motor

  float qtd_passos; // Variável que define quantas vezes o motor irá girar. POSIÇÃO ATUAL - MEDIDA DE DESTINO (CÓDIGO DE BARRAS)

  qtd_passos = abs((posicao_atual - medida_lida)); // Usamos a função abs() para a subtração sempre retornar um valor positivo, isto é, para não correr o risco de termos um valor negativo (sempre módulo)!

  if (qtd_passos > 0.50){    // Se a diferença entre a largura da peça e a posição atual do motor for maior que meio-metro:

    int velocidade_atual = velocidade_inicial; // Velocidade inicial recebe a velocidade inicial (2400)

    for (int i = 0; i < (qtd_passos * constante); i++){ // O motor gira x vezes de acordo com a expressão anterior

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
      delayMicroseconds(velocidade_atual);  // Velocidade de giro do motor durante as leituras de largura e altura 
      digitalWrite(pino_pulso, LOW);
    
    }

  }

  // 7.3) Peças menores (de até 0,5m)

  else{

    for (int i = 0; i < (qtd_passos * constante); i++){   // O motor gira x vezes de acordo com a expressão anterior. Altere essa condição de acordo com seu referencial de medidas

      digitalWrite(pino_pulso, HIGH);
      delayMicroseconds(1200);          // Velocidade constante e aceitável para peças com menos de 0.5m
      digitalWrite(pino_pulso, LOW);
    
    } 

  }

} // FIM DA FUNÇÃO DE MOVIMENTO DO MOTOR

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// 8) FUNÇÃO AFASTA MOTOR !!! 

void afasta_motor(float &posicao_atual){   //"&" torna a variável posição_atual intercambiável entre a função "afasta_motor()" e void loop() 

  int x = 0;   // Declara e inicializa x com 0

  digitalWrite(pino_rele_medida, LOW);   // LOW = PRENSA SOBE!
  
  digitalWrite(pino_rele_garra_1, LOW);   // 1 LOW & LOW  = GARRA SOBE!

  digitalWrite(pino_rele_garra_2, LOW);   // 1 LOW && 2 LOW = GARRA SOBE!

  delay(600);   // ATRASO PARA PERMITIR SUBIDA TOTAL DA GARRA

  digitalWrite(pino_rele_garra_1, LOW);   // 1 LOW && 2 HIGH = SEM AR - PARA O PISTÃO - SOLENOIDE DESLIGADO PARA NÃO ESQUENTAR

  digitalWrite(pino_rele_garra_2, HIGH);  // 1 LOW && 2 HIGH = SEM AR - PARA O PISTÃO - SOLENOIDE DESLIGADO PARA NÃO ESQUENTAR

  int estado_botao_afasta_motor = digitalRead(botao_afasta_motor); // Lê o estado do botao_afasta_motor

  if (estado_botao_afasta_motor == LOW){ // Se o botão for apertado:
    
    digitalWrite(pino_direcao, LOW);
    delayMicroseconds(1000);
    Serial.println("\n MOTOR AFASTANDO NO SENTIDO ANTI-HORÁRIO \n");

    for (x = 0; x < 24000; x++) {   // 24000 = 30 Voltas em torno de si ~= 17,5cm * 30 = 5.25m - Capaz de afastar o motor em toda extensão da mesa
      
      estado_botao_afasta_motor = digitalRead(botao_afasta_motor); // Verifica continuamente o estado do botão dentro do loop

      posicao_atual = posicao_atual + 0.000219875; // Atualização frequente da posicao_atual => posicao_atual = posicao_atual + (x * 0.000219875); / Deslocamento por passo / Passos de 1 volta = 0,1759m / 800 passos = 0,000219875

      if (estado_botao_afasta_motor == HIGH) { 
        break; // Sai do loop se o botão de afastar for solto
      }
      else if (posicao_atual > 4.80){
        calibra_motor();               // Chama a função de calibração de motor após 4.80 por segurança!
      }

      digitalWrite(pino_pulso, HIGH);
      delayMicroseconds(1200);         // VELOCIDADE DO MOTOR DURANTE O AFASTAMENTO
      digitalWrite(pino_pulso, LOW);
     
    }  

   Serial.println(posicao_atual); // Printa no monitor serial a posição em que foi solto o botão
  
  }

} // FIM DA FUNÇÃO DE AFASTAR O MOTOR


// 9) FUNÇÃO INÍCIO !!! 

void inicio(float &posicao_atual){   //"&" torna a variável posição_atual intercambiável entre a função "afasta_motor()" e void loop() 

  int estado_botao_inicio = digitalRead(botao_inicio); // LÊ O ESTADO DO BOTÃO DE INÍCIO

  if(estado_botao_inicio == LOW){ // Se o botão de início for pressionado:

    float medida_lida = 4.40; // Troque para a medida que deseja ser a posição inicial - futuramente 1.45

    Serial.println("\n BOTÃO DE INÍCIO PRESSIONADO!!! \n");

    digitalWrite(pino_rele_medida, LOW);   // LOW = PRENSA SOBE!

    sentido_rotacao(posicao_atual, medida_lida); // Define o sentido de giro de acordo com os parâmetros atuais

    gira_motor(pino_pulso, posicao_atual, medida_lida); // Define a velocidade e quantidade de voltas do motor de acordo com os parâmetros atuais

    delay(500); // ATRASO PARA NÃO DESCER A GARRA RAPIDAMENTE

    digitalWrite(pino_rele_garra_1, HIGH);  // 1 HIGH && 2 HIGH  = GARRA DESCE! (TOTALMENTE PARA AGUARDAR TECIDO SER DESENROLADO) 

    digitalWrite(pino_rele_garra_2, HIGH);  // 1 HIGH && 2 HIGH  = GARRA DESCE! (TOTALMENTE PARA AGUARDAR TECIDO SER DESENROLADO)

    delay(600); // ATRASO PARA PERMITIR SUBIDA TOTAL DA GARRA

    digitalWrite(pino_rele_garra_1, LOW);   // 1 LOW && 2 HIGH = SEM AR - PARA O PISTÃO - SOLENOIDE DESLIGADO PARA NÃO ESQUENTAR

    digitalWrite(pino_rele_garra_2, HIGH);  // 1 LOW && 2 HIGH = SEM AR - PARA O PISTÃO - SOLENOIDE DESLIGADO PARA NÃO ESQUENTAR

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

  parada_total(); // VERIFICAÇÃO IMPORTANTE PARA GARANTIR A CALIBRAÇÃO DE SEGURANÇA NO AFASTAMENTO COM "POSICAO_ATUAL > 4.80    

  // 10.2) ESPERA ATÉ APERTAR O BOTÃO DE INÍCIO, BOTÃO DE AFASTAMENTO, OU BIPAR CÓDIGO DE BARRAS

  Serial.println("\n APERTE O BOTÃO DE POSICÃO INICIAL, AFASTAMENTO OU BIPE UM CÓDIGO DE BARRAS! ");

  while ((Serial.available() == 0) && digitalRead(botao_afasta_motor) == HIGH && digitalRead(botao_inicio) == HIGH){

  }

  // 10.3) 1º CASO - BOTÃO DE INÍCIO PRESSIONADO - VAI PARA POSIÇÃO INICIAL - 1.45 (PREPARADO PARA PEGAR TECIDO)

  if (digitalRead(botao_inicio) == LOW) {
      
      inicio(posicao_atual); // CHAMA A FUNÇÃO DE INÍCIO 

  }

  // 10.4) 2º CASO - BOTÃO DE AFASTAR O MOTOR APERTADO
  
  if (digitalRead(botao_afasta_motor) == LOW) {
      
    afasta_motor(posicao_atual);   // Chama a função de afastar o motor

    posicao_atual = posicao_atual;   // Posição atual recebe a posição em que o botão foi solto

  }
    
  // 10.5) 3º CASO - ETIQUETA BIPADA
  
  if (Serial.available() > 0) { // Caso dados sejam recebidos na porta serial:

    long codigo = Serial.parseInt(); // Variável código recebe o que o usuário inseriu na porta serial

    // LOOP FOR PARA ITERAR ATÉ O NÚMERO DE PEDIDOS

    for (int i = 0; i < num_pedidos; i++) {

      // !!! VERIFICAÇÃO DE PARADA TOTAL !!!

      parada_total();

      if (codigos[i] == codigo) { // Verifica se o código de barras digitado ou bipado corresponde à alguns dos códigos no vetor códigos[]

        // 10.6) CICLO DAS ALTURAS - GARRA PEGA TECIDO 

        digitalWrite(pino_rele_medida, LOW);    // LOW = PRENSA SOBE!

        digitalWrite(pino_rele_garra_1, LOW);   // 1 LOW && 2 LOW = GARRA SOBE!

        digitalWrite(pino_rele_garra_2, LOW);  // 1 LOW && 2 LOW = GARRA SOBE!

        delay(600); // ATRASO PARA PERMITIR SUBIDA TOTAL DA GARRA

        digitalWrite(pino_rele_garra_1, LOW);   // 1 LOW && 2 HIGH = SEM AR - PARA O PISTÃO - SOLENOIDE DESLIGADO PARA NÃO ESQUENTAR

        digitalWrite(pino_rele_garra_2, HIGH);  // 1 LOW && 2 HIGH = SEM AR - PARA O PISTÃO - SOLENOIDE DESLIGADO PARA NÃO ESQUENTAR

        segundo_ciclo = 0; // REINICIA O ESTADO DO BOTÃO PARA 0 - IMPORTANTE PARA COMEÇAR O SEGUNDO LOOP (LARGURA)

        medida_lida = alturas[i]; // Aqui vamos armazenar o valor correspondente a altura do respectivo índice

        if((medida_lida > medida_minima) && (medida_lida < medida_maxima)){

          Serial.println(alturas[i]); // Printa na tela a medida encontrada de altura no vetor

          sentido_rotacao(posicao_atual, medida_lida); // Define o sentido de rotação do motor de acordo com os parâmetros da medida 
        
          gira_motor(pino_pulso, posicao_atual, medida_lida); // Define a velocidade e quantidade de voltas do motor de acordo com os parâmetros atuais

          posicao_atual = medida_lida;  // POSICAO ATUAL RECEBE O VALOR DA MEDIDA LIDA PARA PRÓXIMAS COMPARAÇÕES
  
          delay(300); // ATRASO PARA NÃO DESCER A PRENSA ANTES DE TERMINAR O MOVIMENTO DO MOTOR

          digitalWrite(pino_rele_garra_1, HIGH);  // 1 HIGH && 2 HIGH = GARRA DESCE! 

          digitalWrite(pino_rele_garra_2, HIGH);  // 1 HIGH && 2 HIGH = GARRA DESCE! 

          delay(140); // ATRASO PARA PISTÃO DESCER APENAS O SUFICIENTE PARA LIBERAR O TECIDO DA GARRA

          digitalWrite(pino_rele_garra_1, LOW);   // 1 LOW && 2 HIGH = SEM AR - PARA O PISTÃO - SOLENOIDE DESLIGADO PARA NÃO ESQUENTAR

          digitalWrite(pino_rele_garra_2, HIGH);  // 1 LOW && 2 HIGH = SEM AR - PARA O PISTÃO - SOLENOIDE DESLIGADO PARA NÃO ESQUENTAR

          delay(400);

          digitalWrite(pino_direcao, LOW);
          delayMicroseconds(1000);
          Serial.println("\n MOTOR AFASTANDO 10cm (SENTIDO HORÁRIO) \n");

          for (int x = 0; x < 454.5971133083305; x++) {   // x < 454.60 aproximadamente - afasta 10cm (800 = 17,598cm)
 
            digitalWrite(pino_pulso, HIGH);
            delayMicroseconds(1000);         // VELOCIDADE DO MOTOR AFASTANDO 10cm
            digitalWrite(pino_pulso, LOW);
     
          }

          posicao_atual = medida_lida + 0.10; // Posição atual recebe mais 10cm (trabalhamos em metros)

          delay(100);

          digitalWrite(pino_rele_garra_1, LOW);   // 1 LOW && 2 LOW = GARRA SOBE! (NÃO SE MOVIMENTA NO CICLO DE LARGURAS!)

          digitalWrite(pino_rele_garra_2, LOW);   // 1 LOW && 2 LOW = GARRA SOBE! (NÃO SE MOVIMENTA NO CICLO DE LARGURAS!)

          delay(600); // ATRASO PARA PERMITIR SUBIDA TOTAL DA GARRA

          digitalWrite(pino_rele_garra_1, LOW);   // 1 LOW && 2 HIGH = SEM AR - PARA O PISTÃO - SOLENOIDE DESLIGADO PARA NÃO ESQUENTAR

          digitalWrite(pino_rele_garra_2, HIGH);  // 1 LOW && 2 HIGH = SEM AR - PARA O PISTÃO - SOLENOIDE DESLIGADO PARA NÃO ESQUENTAR
          
        } // FIM DO IF
        
      } // FIM DO IF CODIGOS
    
    } // FIM DO LOOP FOR QUE ITERA ATÉ O NÚMERO DE PEDIDOS (CÓDIGOS DE BARRAS)

    // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // 10.7) SEGUNDO CICLO - LARGURA - SÓ PROSSEGUE SE O BOTÃO PARA LER A MEDIDA DA LARGURA FOR PRESSIONADO

    while(segundo_ciclo == 0){

      if (digitalRead(botao_largura) == LOW) { 

        digitalWrite(pino_rele_medida, LOW); // LOW = PRENSA SOBE!

        delay(500); //PEQUENO ATRASO PARA NÃO MOVIMENTAR PRENSA E MOTOR SIMULTANEAMENTE

        Serial.println("BOTÃO DO SEGUNDO CICLO PRESSIONADO!!! \n");

        // LOOP FOR PARA ITERAR ATÉ O NÚMERO DE PEDIDOS

        for (int i = 0; i < num_pedidos; i++) {

          if (codigos[i] == codigo) {  // Verifica se o código de barras digitado ou bipado corresponde à alguns dos códigos no vetor códigos[]

            medida_lida = larguras[i]; // Aqui vamos armazenar o valor correspondente a largura do respectivo índice

            Serial.println(larguras[i]); // Printa o valor correspondente a largura

            if((medida_lida > medida_minima) && (medida_lida < medida_maxima)){
         
              sentido_rotacao(posicao_atual, medida_lida);  // Define o sentido de rotação do motor de acordo com os parâmetros da medida da largura

              gira_motor(pino_pulso, posicao_atual, medida_lida);  // Define a velocidade e quantidade de voltas do motor de acordo com os parâmetros atuais

              posicao_atual = medida_lida;  // POSICAO ATUAL RECEBE O VALOR DA MEDIDA LIDA PARA PRÓXIMAS COMPARAÇÕES

              delay(1000); // ATRASO PARA NÃO DESCER A PRENSA ANTES DE TERMINAR O MOVIMENTO DO MOTOR

              digitalWrite(pino_rele_medida, HIGH); // PINO DO RELÉ HIGH = PRENSA DESCE!

            }
          
          } // FIM DO IF
        
        } // FIM DO LOOP FOR QUE ITERA ATÉ O NÚMERO DE  PEDIDOS (CÓDIGOS DE BARRAS)

        break; // Força a saída do loop
      
      } // FIM DO IF DO BOTÃO

      delay(1000); // Pequeno atraso para evitar detecção falsa de pressionamento do botão
    
    } // FIM DO LOOP WHILE DO SEGUNDO CICLO

    segundo_ciclo = 1; //ATRIBUI 1 AO SEGUNDO CICLO PRA NÃO FICAR PRESO NO LOOP LARGURA
  
  }  // FIM DO IF DO SERIAL AVAILABLE
    
}  // FIM DA FUNÇÃO VOID LOOP

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
