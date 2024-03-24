# Conexões Entre Motor e Arduino

Explicação da ligação dos cabos entre o Arduino Uno e o driver do motor de passo tecmaf tma dmp 860d:

1. ***PUL+ e DIR+*:** Esses são os pinos para controlar a direção e o pulso do motor de passo. Você pode conectar o PUL+ a qualquer pino digital do Arduino e o DIR+ a outro pino digital. Por exemplo, você pode conectar o PUL+ ao pino digital 8 e o DIR+ ao pino digital 9 do Arduino.
2. ***GND (Negativo)*:** Você deve conectar o terminal negativo (GND) do driver do motor de passo ao pino GND do Arduino. Isso é importante para garantir uma referência comum entre o Arduino e o driver.
3. ***ENA (Ativação)*:** O pino ENA (Enable) é usado para habilitar ou desabilitar o driver do motor de passo. Se você deseja controlar o motor, este pino deve ser conectado a um pino digital do Arduino. Por exemplo, você pode conectar o pino ENA a outro pino digital, como o pino digital 7 do Arduino.

Aqui está um resumo da ligação:

- **Conectar PUL+ a um pino digital (por exemplo, pino 8)**
- **Conectar DIR+ a outro pino digital (por exemplo, pino 9)**
- **Conectar GND (Negativo) ao pino GND do Arduino**
- **Conectar ENA (Ativação) a outro pino digital (por exemplo, pino 7)**

Após fazer essas conexões, você pode escrever o código no Arduino para controlar o motor de passo através do driver tecmaf tma dmp 860d. Certifique-se de consultar a documentação do fabricante para entender completamente as especificações e funcionalidades do driver e do motor de passo.

## Exemplo Projeto Persol

<img width = 600 height = 400 align="center" src="/imagens/exemplo_real.jpg">

# Código / Programação

**Pinos de Saída:** Os pinos pino_pulso, pino_direcao e enable_pin são configurados corretamente como saída (OUTPUT) no setup().

**Direção do Motor:** A direção do motor é controlada pelo pino pino_direcao, que é definido como LOW para um sentido de rotação e HIGH para o sentido inverso.

**Ativação do Motor:** O pino enable_pin é usado para ativar ou desativar o motor de passo. Ele é definido como LOW para ativar o motor e HIGH para desativá-lo.

**Laços de Movimento:** Existem dois loops for que controlam o número de pulsos enviados para o motor de passo em cada sentido. O primeiro loop envia 400 pulsos em um sentido, enquanto o segundo loop envia 800 pulsos no sentido inverso.

**Atrasos:** O código inclui atrasos (delay e delayMicroseconds) entre os pulsos para controlar a velocidade e observar o movimento do motor.

## Código IDE

```cpp
const int pino_pulso = 8; //CONFIGURA O PINO DO PULSO / PASSO (PRECISÃO)
const int pino_direcao = 9; // CONFIGURA O PINO DA DIREÇÃO (HORÁRIO / ANTI-HORÁRIO)
const int enable_pin = 13; // CONFIGURA O PINO ENA - ATIVA OU DESATIVA O MOTOR DE PASSO

// FUNÇÃO SETUP - CONFIGURAÇÕES INICIAIS

void setup() {
  pinMode(pino_pulso, OUTPUT); //DEFINE PINO PULSO COMO SAÍDA
  pinMode(pino_direcao, OUTPUT); // DEFINE PINO DIRECAO COMO SAÍDA
  pinMode(enable_pin, OUTPUT);
}

// -------------------------------------------------------------------

// FUNCÃO LOOP - LOOP PRINCIPAL DO PROGRAMA

void loop() {

  digitalWrite(pino_direcao, LOW); // GIRA O MOTOR EM UM SENTIDO -> HIGH E LOW = HORÁRIO E ANTI-HORÁRIO
  digitalWrite(enable_pin, LOW); // PINO ENA - ATIVADO
  delayMicroseconds(10); //ATRASO

  for(int x = 0; x < 400; x++){ // DEFINA A PRECISÃO DE PASSOS NESSE LOOP - QUANTO MAIOR A CONDIÇÃO MAIS PRECISO

    digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
    delayMicroseconds(1500); // ATRASO PARA OBSERVAR O MOVIMENTO
    digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA
  }

  digitalWrite(enable_pin, HIGH); //PINO ENA - DESATIVADO

  delay(1000); // INTERVALO ENTRE INVERSÃO DE SENTIDO

  digitalWrite(pino_direcao, HIGH); // GIRA O MOTOR NO SENTIDO INVERSO
  digitalWrite(enable_pin, LOW); // PINO ENA - ATIVADO
  delayMicroseconds(10); // ATRASO

  for(int y = 0; y < 800; y++){ // DEFINA A PRECISÃO DE PASSOS NESSE LOOP - QUANTO MAIOR A CONDIÇÃO MAIS PRECISO

    digitalWrite(pino_pulso, HIGH); // PINO DE PULSO INICIA
    delayMicroseconds(3000); // ATRASO PARA OBSERVAR O MOVIMENTO
    digitalWrite(pino_pulso,LOW); // PINO DE PULSO ENCERRA
  }

  digitalWrite(enable_pin, HIGH); //PINO ENA - DESATIVADO

  delay(1000); // ATRASO
}
```
# Requisitos do Projeto

**Movimentar motor de acordo com medida da etiqueta;**

**3 botões (ler medida da altura; fim de curso e parada total);**

**Calibração do motor (movimenta constantemente até apertar o botão parada total;**

**Motor ao iniciar saí do botão de calibração para evitar que fique pressionado;**

**Ler primeiro medida da largura e movimentar de acordo com ela;**

**Partir para a medida da altura SÓ depois de apertar o botão de altura;**

**Após x códigos de barras (no final serão 10) o motor volta sozinho para a posição inicial;**

**Durante a volta p/ posição inicial podemos reduzir a velocidade c/ o botão de fim de curso;**

**Possibilidade de apertar o botão de parada total a qualquer momento;**

## Checklist Projeto

- [x]  Corrigir o bug de leitura da medida da altura (independentemente da etiqueta a medida da altura sempre é a do primeiro índice do vetor - índice 0);

Solução era usar a mesma variável para receber o código de barras e fazer a comparação no Loop da largura e no Loop da altura!

### Lógica de Voltas - Conversão de Pulsos

- [x]  Configurar quantidade de voltas de acordo com medidas corretamente (1 CICLO = 800 PULSOS = 175,7mm = 17,57cm);

```cpp
(1 CICLO = 800 PULSOS = 175,7mm = 17,57cm) -> (4553,215 Pulsos = 1000mm = 1m)
```

# Montagem na Caixa de Automação

Lista de materiais utilizados na montagem:

- Cabos de rede (para tornar os cabos de conexão longos o suficiente);
- Alicates (para descascar cabos e para cortar);
- Arduino Uno + Cabo USB;
- Raspberry Pi 4 (plugado via USB nele o leitor de códigos de barras e o Arduino);
- Cooler de 12V para resfriamento interno dos componentes e da caixa;
- 3 botões  (fim de curso, parada total e leitor_larguras);
- 2 fontes externas (1 de 12V para o cooler e outra para o Raspberry;
- Motor de Passo Nema + Driver/Dissipador + Fonte Externa;
- Caixa de automação (onde as peças foram inseridas).

<img width = 600 height = 400 align="center" src="/imagens/caixa-1.jpeg">

## Visão Interna da Caixa de Automação

<img width = 600 height = 400 align="center" src="/imagens/caixa-2.jpeg">

# Links Importantes

***Vídeo sobre o driver:  https://www.youtube.com/watch?v=_8yUdaW47DQ***

***Vídeo sobre automação do boot Raspbian:  https://www.youtube.com/watch?v=lT8XNpfo950***

***Vídeo do projeto em funcionamento:  https://photos.google.com/photo/AF1QipPslGTFXpz8OATvj5d3EI_oGv40nXjeqUd6lBZ8***
