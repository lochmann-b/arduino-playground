#define LED_COUNT 4

#define ERROR_PIN 13
#define FIRST_INPUT_PIN 2
#define FIRST_OUTPUT_PIN 8

#define ERROR_DELAY 250
#define OUTPUT_DURATION 3000

byte input_history[LED_COUNT];
byte output_state[LED_COUNT];

unsigned long latest_action;
unsigned long last_error_millis;

byte current_led;

void setup() {
	last_error_millis = 0;
	latest_action = 0;
	current_led = 0;
	randomSeed(analogRead(0));
	Serial.begin(9600);
	pinMode(ERROR_PIN, OUTPUT);
	for (int i = 0; i < LED_COUNT; i++){
		pinMode(FIRST_INPUT_PIN + i, INPUT);
		pinMode(FIRST_OUTPUT_PIN + i, OUTPUT);
		input_history[i] = 0;
		output_state[i] = LOW;
	}  
}

void nextLed() {	
	digitalWrite(current_led, LOW);    
	byte next = random(FIRST_OUTPUT_PIN, FIRST_OUTPUT_PIN + LED_COUNT);
	while(next == current_led){
		next = random(FIRST_OUTPUT_PIN, FIRST_OUTPUT_PIN + LED_COUNT);
	}
	current_led = next;
	digitalWrite(current_led, HIGH);
	latest_action = millis();
}

void checkError(unsigned long now){
	if (now > last_error_millis + ERROR_DELAY) {
		noTone(ERROR_PIN);
	}
}

void setError(unsigned long now) {
	last_error_millis = now;
	tone(ERROR_PIN, 550);
}

bool isButtonPressed(byte pin){	
	bool pressed = false;    	
	input_history[pin - FIRST_INPUT_PIN] = input_history[pin - FIRST_INPUT_PIN] << 1;
	input_history[pin - FIRST_INPUT_PIN] |= digitalRead(pin);
	if ((input_history[pin - FIRST_INPUT_PIN] & 0b11000111) == 0b00000111){ 
		pressed = true;
		input_history[pin - FIRST_INPUT_PIN] = 0b11111111;
	}
	return pressed;
}


void loop() {
	
	unsigned long now = millis();
	checkError(now);
	if (now >= latest_action + OUTPUT_DURATION) {
		nextLed();
	} else {		
		for (int i = 0; i < LED_COUNT; i++){
			if (isButtonPressed(FIRST_INPUT_PIN + i)){
				Serial.print("pressed ");
				Serial.println(i);
				if (FIRST_OUTPUT_PIN + i == current_led){
					//win
					digitalWrite(ERROR_PIN, LOW);
					nextLed();
				} else {
					setError(now);
				}
			}
		}
	}
}
