#define SWITCH_PIN 2
#define LED_PIN 13

byte ledState = LOW;
byte priorReading = LOW;
byte switchState = LOW;

byte buttonHistory = 0;

void setup() {
	pinMode(SWITCH_PIN, INPUT);
	pinMode(LED_PIN, OUTPUT);
}

bool isButtonPressed(byte pin){
	int reading = digitalRead(pin);
	buttonHistory = buttonHistory << 1;
	buttonHistory |= reading;
	
	return (buttonHistory & 0b10011111) == 0b10011111;
	
}

void loop() {
	int reading = isButtonPressed(SWITCH_PIN);
	
	if (reading == HIGH && priorReading == LOW) {      
		ledState = ledState == LOW ? HIGH : LOW;
		digitalWrite(LED_PIN, ledState);          
	}
	priorReading = reading;
}
