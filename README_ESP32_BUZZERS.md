# 🎮 Sistema de Buzzers Físicos ESP32 - Passa ou Repassa

Este sistema implementa buzzers físicos para o jogo "Passa ou Repassa" usando ESP32, permitindo que as equipes usem botões reais conectados ao microcontrolador.

## 📋 Componentes Necessários

### Hardware:
- **1x ESP32** (qualquer modelo com USB)
- **2x Botões de fliperama** (ou botões grandes)
- **2x LEDs** (vermelho e verde recomendados)
- **2x Resistores 220Ω** (para os LEDs)
- **Jumpers e protoboard**
- **Cabo USB** para conectar ESP32 ao computador

### Software:
- **Arduino IDE** (versão 1.8.x ou 2.x)
- **Biblioteca ESP32** instalada no Arduino IDE
- **Navegador web** para o jogo

## 🔌 Esquema de Conexões

```
ESP32 Pinout:
┌─────────────────────────────────┐
│  GPIO 2  ←→  Botão Equipe 1     │
│  GPIO 4  ←→  Botão Equipe 2     │
│  GPIO 12 ←→  LED Equipe 1       │
│  GPIO 14 ←→  LED Equipe 2       │
│  GPIO 13 ←→  LED Status (interno)│
│  GND     ←→  GND dos componentes │
│  3.3V    ←→  VCC dos LEDs       │
└─────────────────────────────────┘
```

### Conexão dos Botões:
```
Botão Equipe 1:
- Um terminal → GPIO 2
- Outro terminal → GND
(Pull-up interno ativado no código)

Botão Equipe 2:
- Um terminal → GPIO 4
- Outro terminal → GND
(Pull-up interno ativado no código)
```

### Conexão dos LEDs:
```
LED Equipe 1 (Vermelho):
- Anodo (+) → GPIO 12
- Catodo (-) → Resistor 220Ω → GND

LED Equipe 2 (Verde):
- Anodo (+) → GPIO 14
- Catodo (-) → Resistor 220Ω → GND
```

## 🛠️ Instalação e Configuração

### 1. Preparar Arduino IDE:

1. **Instalar Arduino IDE**: Baixe em https://www.arduino.cc/en/software
2. **Adicionar ESP32**:
   - Vá em `Arquivo > Preferências`
   - Em "URLs Adicionais do Gerenciador de Placas", adicione:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Vá em `Ferramentas > Placa > Gerenciador de Placas`
   - Procure por "ESP32" e instale "esp32 by Espressif Systems"

### 2. Configurar a Placa:

1. Conecte o ESP32 via USB
2. No Arduino IDE:
   - `Ferramentas > Placa > ESP32 Arduino > ESP32 Dev Module`
   - `Ferramentas > Porta > [Selecione a porta COM do ESP32]`
   - `Ferramentas > Upload Speed > 115200`

### 3. Carregar o Código:

1. Abra o arquivo `esp32_buzzers.ino` no Arduino IDE
2. Clique em "Verificar" (✓) para compilar
3. Clique em "Carregar" (→) para enviar ao ESP32
4. Abra o Monitor Serial (`Ctrl+Shift+M`) para ver os logs

## 🎯 Como Usar

### 1. Preparação:
1. **Monte o circuito** conforme o esquema
2. **Carregue o código** no ESP32
3. **Conecte o ESP32** ao computador via USB
4. **Abra o jogo** no navegador (`index.html`)

### 2. Operação:

1. **Iniciar o jogo**: Clique em "INICIAR" no navegador
2. **Aguardar pergunta**: O sistema mostra a pergunta
3. **Apertar buzzer**: A equipe que souber a resposta aperta seu botão físico
4. **Indicação visual**: 
   - LED da equipe acende
   - Timer de 30s inicia
   - Botão virtual no jogo fica destacado
5. **Responder**: Equipe seleciona a resposta na tela
6. **Verificar**: Mediador clica "Verificar Resposta"
7. **Próxima rodada**: Clique "Nova Pergunta"

### 3. Feedback do Sistema:

**LEDs Indicadores:**
- 🔴 **LED Equipe 1**: Acende quando Equipe 1 aperta primeiro
- 🟢 **LED Equipe 2**: Acende quando Equipe 2 aperta primeiro
- 🔵 **LED Status**: Pisca quando sistema está pronto

**Monitor Serial:**
- Mostra qual equipe apertou
- Comandos de debug disponíveis
- Status do sistema em tempo real

## 🔧 Comandos de Debug

No Monitor Serial (115200 baud), você pode usar:

- `reset` ou `r` - Reset manual do sistema
- `status` ou `s` - Mostra status atual dos botões e sistema
- `help` ou `h` - Lista todos os comandos disponíveis

## ⚡ Funcionamento Técnico

### Fluxo do Sistema:
```
1. ESP32 monitora botões continuamente
2. Detecta qual botão foi pressionado primeiro
3. Envia comando via Serial USB:
   - 'a' para Equipe 1
   - 'd' para Equipe 2
4. Jogo HTML recebe a tecla e ativa o buzzer virtual
5. Sistema se reseta automaticamente após 2 segundos
```

### Recursos Implementados:
- ✅ **Debounce**: Evita múltiplos acionamentos
- ✅ **Prioridade**: Detecta quem apertou primeiro
- ✅ **Reset automático**: Sistema se prepara para próxima rodada
- ✅ **Feedback visual**: LEDs indicam status
- ✅ **Comunicação USB**: Envia comandos para o jogo
- ✅ **Debug**: Monitor serial com comandos úteis

## 🐛 Solução de Problemas

### Problema: ESP32 não aparece na porta COM
**Solução:**
- Instale drivers CH340 ou CP2102 (dependendo do ESP32)
- Verifique se o cabo USB transmite dados (não apenas energia)
- Pressione o botão BOOT no ESP32 durante o upload

### Problema: Botões não respondem
**Solução:**
- Verifique as conexões dos botões
- Confirme que estão conectados ao GND
- Teste com o comando `status` no Monitor Serial

### Problema: Jogo não recebe comandos
**Solução:**
- Certifique-se que o navegador está em foco
- Verifique se o ESP32 está enviando dados no Monitor Serial
- Teste pressionando 'a' e 'd' manualmente no teclado

### Problema: LEDs não acendem
**Solução:**
- Verifique polaridade dos LEDs (anodo no GPIO, catodo no GND)
- Confirme se os resistores estão conectados
- Teste com multímetro se há tensão nos GPIOs

## 📊 Especificações Técnicas

- **Tensão de operação**: 3.3V
- **Corrente dos LEDs**: ~15mA cada
- **Debounce**: 50ms
- **Reset automático**: 2 segundos
- **Comunicação**: Serial USB 115200 baud
- **Compatibilidade**: Qualquer ESP32 com USB

## 🎉 Recursos Extras

### Expansões Possíveis:
- **Buzzer sonoro**: Adicione um buzzer no GPIO 15 para feedback sonoro
- **Display LCD**: Mostre informações no próprio hardware
- **WiFi**: Conecte via rede sem fio (futuro)
- **Mais equipes**: Expanda para 4 ou mais equipes

### Personalização:
- Modifique os pinos no código conforme sua montagem
- Ajuste tempos de debounce e reset
- Adicione mais comandos de debug
- Customize as mensagens do Monitor Serial

---

## 📞 Suporte

Para dúvidas ou problemas:
1. Verifique as conexões físicas
2. Consulte o Monitor Serial para debug
3. Teste os componentes individualmente
4. Verifique se o código foi carregado corretamente

**Divirta-se com seu sistema de buzzers físicos!** 🎮🎉