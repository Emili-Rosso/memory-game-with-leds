int botao1 = 5;
int botao2 = 6;

int LED1 = 2;
int LED2 = 3;
int LED3 = 4;

int passoGeral = 1; 
int passoUsuario = 0; // Para contar os teus acertos

unsigned long tempoAnterior = 0;
unsigned long tempoBotao2 = 0;
unsigned long tempoFeedback = 0; // Novo cronómetro para o brilho do clique

int contador = 0;
int ledFeedback = 0; // Guarda qual LED deve apagar depois do clique

bool ultimoEstadoBotao1 = HIGH;
bool ultimoEstadoBotao2 = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(botao1, INPUT_PULLUP);
  pinMode(botao2, INPUT_PULLUP);
}

void loop() {
  unsigned long tempoAtual = millis();

  // --- PARTE 1: SEQUÊNCIA AUTOMÁTICA (LED1 -> LED3 -> LED2) ---
  if (passoGeral == 1) {
    digitalWrite(LED1, HIGH);
    if (tempoAtual - tempoAnterior >= 600) {
      digitalWrite(LED1, LOW);
      tempoAnterior = tempoAtual;
      passoGeral = 2;
    }
  }

  if (passoGeral == 2) {
    if (tempoAtual - tempoAnterior >= 300) {
      digitalWrite(LED3, HIGH);
      if (tempoAtual - tempoAnterior >= 900) {
        digitalWrite(LED3, LOW);
        tempoAnterior = tempoAtual;
        passoGeral = 3;
      }
    }
  }

  if (passoGeral == 3) {
    if (tempoAtual - tempoAnterior >= 300) {
      digitalWrite(LED2, HIGH);
      if (tempoAtual - tempoAnterior >= 900) {
        digitalWrite(LED2, LOW);
        tempoAnterior = tempoAtual;
        passoGeral = 4; // Vez do utilizador
      }
    }
  }

  // --- PARTE 2: LÓGICA DOS BOTÕES ---
  if (passoGeral == 4) {
    int estadoBotao1 = digitalRead(botao1);
    int estadoBotao2 = digitalRead(botao2);

    // Botão 1
    if (estadoBotao1 == LOW && ultimoEstadoBotao1 == HIGH) {
      if (tempoAtual - tempoAnterior > 50) {
        digitalWrite(LED1, HIGH);
        ledParaDesligar = LED1;
        tempoFeedback = tempoAtual;
      }
    }
    ultimoEstadoBotao1 = estadoBotao1;

    // Botão 2 (Clique Único ou Duplo)
    if (estadoBotao2 == LOW && ultimoEstadoBotao2 == HIGH) {
      if (estadoAtual - estadoAnterior > 50) {
        contador++;
        if (contador == 1) {
          tempoBotao2 = tempoAtual;
        }
      }
    }
    ultimoEstadoBotao2 = estadoBotao2;

    // Decide se foi LED 2 ou LED 3 sem travar o código
    if (contador > 0 && (tempoAtual - tempoBotao2 > 400)) {
      if (contador >= 2) {
        digitalWrite(LED3, HIGH);
        ledFeedback = LED3;
      } 
      else {
        digitalWrite(LED2, HIGH);
        ledFeedback = LED2;
      }
      tempoFeedback = tempoAtual;
      contador = 0;
    }
  }

  // --- PARTE 3: DESLIGAR O LED DO CLIQUE (Substitui o delay de 200) ---
  
  if (ledParaDesligar > 0) {
    if (tempoAtual - tempoFeedback >= 200) {
      digitalWrite(ledFeedback, LOW);
      ledParaDesligar = 0; // Reset para a próxima jogada
    }
  }
}