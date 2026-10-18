const int irSensorPin = 2;

unsigned long blinkStartTime = 0;
unsigned long lastActivityTime = 0;
unsigned long blinkDuration = 0;
bool isBlinking = false;
bool spacePrinted = true; 

String currentMorse = ""; 
String finalSentence = ""; 

void setup() {
  Serial.begin(9600);
  pinMode(irSensorPin, INPUT);
  Serial.println("Eye-Blink Morse Code Glasses (Locked-in Syndrome)");
  Serial.println("Natural blink (<250ms): Ignored");
  Serial.println("Dot (250ms - 1000ms) | Dash (>1000ms)");
  Serial.println("Letter pause: 2.25s | Word space pause: 5.0s");
  Serial.println("--------------------------------------------------");
}

void loop() {
  int sensorState = digitalRead(irSensorPin);

  // Eyelid closes (Object detected)
  if (sensorState == LOW && !isBlinking) {
    isBlinking = true;
    blinkStartTime = millis();
  }
  
  // Eyelid opens (Object removed)
  if (sensorState == HIGH && isBlinking) {
    isBlinking = false;
    blinkDuration = millis() - blinkStartTime;
    lastActivityTime = millis(); 
    spacePrinted = false;        
    
    // Natural blink filter: ignore anything under 250ms
    if (blinkDuration >= 250) { 
      if (blinkDuration < 1000) {
        currentMorse += ".";
        Serial.print("."); 
      } else {
        currentMorse += "-";
        Serial.print("-");
      }
    }
  }

  // LETTER TIMEOUT: 2.25 seconds idle after blinking
  if (currentMorse.length() > 0 && (millis() - lastActivityTime > 2250)) {
    char letter = translateMorse(currentMorse);
    
    if (letter != '?') {
      finalSentence += letter; 
    } else {
      Serial.print(" [Unknown: ");
      Serial.print(currentMorse);
      Serial.print("] ");
    }
    
    currentMorse = ""; 
    
    Serial.println(); 
    Serial.print("Message: ");
    Serial.println(finalSentence); 
  }

  // WORD SPACE TIMEOUT: 5.0 seconds idle after last letter
  if (finalSentence.length() > 0 && !spacePrinted && (millis() - lastActivityTime > 5000)) {
    finalSentence += " "; 
    spacePrinted = true; 
    
    Serial.println("\n--- [SPACE ADDED] ---");
    Serial.print("Message: ");
    Serial.println(finalSentence);
  }
}

// Full Morse dictionary
char translateMorse(String code) {
  if (code == ".-") return 'A';
  if (code == "-...") return 'B';
  if (code == "-.-.") return 'C';
  if (code == "-..") return 'D';
  if (code == ".") return 'E';
  if (code == "..-.") return 'F';
  if (code == "--.") return 'G';
  if (code == "....") return 'H';
  if (code == "..") return 'I';
  if (code == ".---") return 'J';
  if (code == "-.-") return 'K';
  if (code == ".-..") return 'L';
  if (code == "--") return 'M';
  if (code == "-.") return 'N';
  if (code == "---") return 'O';
  if (code == ".--.") return 'P';
  if (code == "--.-") return 'Q';
  if (code == ".-.") return 'R';
  if (code == "...") return 'S';
  if (code == "-") return 'T';
  if (code == "..-") return 'U';
  if (code == "...-") return 'V';
  if (code == ".--") return 'W';
  if (code == "-..-") return 'X';
  if (code == "-.--") return 'Y';
  if (code == "--..") return 'Z';
  
  if (code == ".----") return '1';
  if (code == "..---") return '2';
  if (code == "...--") return '3';
  if (code == "....-") return '4';
  if (code == ".....") return '5';
  if (code == "-....") return '6';
  if (code == "--...") return '7';
  if (code == "---..") return '8';
  if (code == "----.") return '9';
  if (code == "-----") return '0';
  
  return '?'; 
}