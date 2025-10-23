/*
 * ESP32 Buzzers para Jogo "Passa ou Repassa"
 * * Este código implementa um sistema de buzzers físicos para o jogo,
 * detectando qual equipe apertou primeiro e enviando comandos via USB Serial.
 * * Conexões:
 * - Botão Equipe 1: GPIO 2 (com pull-up interno)
 * - Botão Equipe 2: GPIO 4 (com pull-up interno)
 * - LED Equipe 1: GPIO 12
 * - LED Equipe 2: GPIO 14
 * - LED Status: GPIO 13 (LED interno)
 * * Funcionamento:
 * - Detecta qual botão foi pressionado primeiro
 * - Envia 'a' para Equipe 1 ou 'd' para Equipe 2 via Serial
 * - Acende LED da equipe que apertou primeiro
 * - Reset automático após 2 segundos
 */

// Definições dos pinos
#define BUTTON_TEAM1 2    // Botão da Equipe 1
#define BUTTON_TEAM2 4    // Botão da Equipe 2
#define LED_TEAM1 12      // LED da Equipe 1
#define LED_TEAM2 14      // LED da Equipe 2
#define LED_STATUS 13     // LED interno (status)

// Variáveis de controle
bool gameActive = true;         // Se o jogo está ativo (aceita buzzers)
bool buttonPressed = false;       // Se algum botão foi pressionado
unsigned long pressTime = 0;      // Tempo do último pressionamento
unsigned long debounceDelay = 50; // Delay para debounce (ms)
unsigned long resetDelay = 2000;  // Delay para reset automático (ms)

// Variáveis para debounce
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
bool lastButtonState1 = HIGH;
bool lastButtonState2 = HIGH;
bool buttonState1 = HIGH;
bool buttonState2 = HIGH;

void setup() {
  // Inicializa comunicação serial
  Serial.begin(115200);
  
  // Configura os pinos dos botões como entrada com pull-up interno
  pinMode(BUTTON_TEAM1, INPUT_PULLUP);
  pinMode(BUTTON_TEAM2, INPUT_PULLUP);
  
  // Configura os pinos dos LEDs como saída
  pinMode(LED_TEAM1, OUTPUT);
  pinMode(LED_TEAM2, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  
  // Inicializa LEDs apagados
  digitalWrite(LED_TEAM1, LOW);
  digitalWrite(LED_TEAM2, LOW);
  digitalWrite(LED_STATUS, HIGH); // LED de status aceso = pronto
  
  // Mensagem de inicialização (Comentadas para não interferir com o site)
  // Serial.println("ESP32 Buzzers - Passa ou Repassa");
  // Serial.println("Sistema pronto! Aguardando buzzers...");
  
  delay(1000);
}

void loop() {
  // Lê o estado atual dos botões
  bool reading1 = digitalRead(BUTTON_TEAM1);
  bool reading2 = digitalRead(BUTTON_TEAM2);
  
  // Debounce para Botão 1
  if (reading1 != lastButtonState1) {
    lastDebounceTime1 = millis();
  }
  
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
      
      // Botão 1 foi pressionado (LOW = pressionado devido ao pull-up)
      if (buttonState1 == LOW && gameActive && !buttonPressed) {
        teamBuzzed(1);
      }
    }
  }
  
  // Debounce para Botão 2
  if (reading2 != lastButtonState2) {
    lastDebounceTime2 = millis();
  }
  
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if (reading2 != buttonState2) {
      buttonState2 = reading2;
      
      // Botão 2 foi pressionado (LOW = pressionado devido ao pull-up)
      if (buttonState2 == LOW && gameActive && !buttonPressed) {
        teamBuzzed(2);
      }
    }
  }
  
  // Salva o estado anterior dos botões
  lastButtonState1 = reading1;
  lastButtonState2 = reading2;
  
  // Reset automático após o delay
  if (buttonPressed && (millis() - pressTime) > resetDelay) {
    resetSystem();
  }
  
  // Pisca LED de status quando sistema está pronto
  if (gameActive && !buttonPressed) {
    static unsigned long lastBlink = 0;
    static bool ledState = false;
    
    if (millis() - lastBlink > 1000) {
      ledState = !ledState;
      digitalWrite(LED_STATUS, ledState);
      lastBlink = millis();
    }
  }
}

void teamBuzzed(int team) {
  if (!gameActive || buttonPressed) return;
  
  buttonPressed = true;
  gameActive = false;
  pressTime = millis();
  
  // Apaga LED de status
  digitalWrite(LED_STATUS, LOW);
  
  if (team == 1) {
    // Equipe 1 apertou primeiro
    digitalWrite(LED_TEAM1, HIGH);
    digitalWrite(LED_TEAM2, LOW);
    Serial.print("a"); // Envia tecla 'a' para o computador
    // Serial.println(); // REMOVIDO
    // Serial.println("EQUIPE 1 BUZZOU!"); // REMOVIDO
    
  } else if (team == 2) {
    // Equipe 2 apertou primeiro
    digitalWrite(LED_TEAM1, LOW);
    digitalWrite(LED_TEAM2, HIGH);
    Serial.print("d"); // Envia tecla 'd' para o computador
    // Serial.println(); // REMOVIDO
    // Serial.println("EQUIPE 2 BUZZOU!"); // REMOVIDO
  }
  
  // Feedback sonoro (opcional - se tiver buzzer conectado)
  // tone(BUZZER_PIN, 1000, 200);
}

void resetSystem() {
  // Reset do sistema para próxima rodada
  buttonPressed = false;
  gameActive = true;
  
  // Apaga todos os LEDs das equipes
  digitalWrite(LED_TEAM1, LOW);
  digitalWrite(LED_TEAM2, LOW);
  digitalWrite(LED_STATUS, HIGH);
  
  // Serial.println("Sistema resetado - Pronto para próximo buzzer!"); // REMOVIDO
}

// Função para reset manual via comando serial (opcional)
// Esta função é para debug. Para evitar qualquer conflito com o site,
// as mensagens de resposta também foram comentadas.
void serialEvent() {
  if (Serial.available()) {
    String command = Serial.readString();
    command.trim();
    
    if (command == "reset" || command == "r") {
      resetSystem();
      // Serial.println("Reset manual executado!");
    }
    else if (command == "status" || command == "s") {
      // Serial.println("=== STATUS DO SISTEMA ===");
      // Serial.print("Game Active: ");
      // Serial.println(gameActive ? "SIM" : "NÃO");
      // Serial.print("Button Pressed: ");
      // Serial.println(buttonPressed ? "SIM" : "NÃO");
      // Serial.print("Team 1 Button: ");
      // Serial.println(digitalRead(BUTTON_TEAM1) == LOW ? "PRESSIONADO" : "SOLTO");
      // Serial.print("Team 2 Button: ");
      // Serial.println(digitalRead(BUTTON_TEAM2) == LOW ? "PRESSIONADO" : "SOLTO");
    }
    else if (command == "help" || command == "h") {
      // Serial.println("=== COMANDOS DISPONÍVEIS ===");
      // Serial.println("reset ou r - Reset manual do sistema");
      // Serial.println("status ou s - Mostra status atual");
      // Serial.println("help ou h - Mostra esta ajuda");
    }
  }
}