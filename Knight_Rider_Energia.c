/* "Knight Rider" TI LaunchPad Energia Program
 *  Created by: Justin Russo
*/

int buttonPin = PUSH2;                    // Using switch 2
int buttonState;
int timer = 50;                           // Setting the delay for the LEDs

void setup() {                
  pinMode(buttonPin, INPUT_PULLUP);       // Initialize the button pin as an input
    for (int thisPin = 11; thisPin < 20; thisPin++){
                                          // Initialize the LED pins as outputs
      if (thisPin == 12)
        pinMode(32, OUTPUT);
      else
        pinMode(thisPin, OUTPUT);     
    }
}

void loop() {
  buttonState = digitalRead(buttonPin);   // Read the switch pin
    if (buttonState == LOW){              // If the switch is pressed
      for (int thisPin = 11; thisPin < 20; thisPin++){
        if (thisPin == 12){               // Cycle forwards through the LEDs
          digitalWrite(32, HIGH);
          delay(timer);
          digitalWrite(32, LOW);
        }
        digitalWrite(thisPin, HIGH);
        delay(timer);
        digitalWrite(thisPin,LOW);
      }
      for (int thisPin = 19; thisPin > 10; thisPin--){
        if (thisPin == 12){               // Cycle backwards through the LEDs
          digitalWrite(32, HIGH);
          delay(timer);
          digitalWrite(32, LOW);
        }
        digitalWrite(thisPin, HIGH);
        delay(timer);
        digitalWrite(thisPin,LOW);
      }
    }
}