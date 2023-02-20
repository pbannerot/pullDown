
#include "pullDown.h"

#include "TimerOne.h"

#define NUM_OF_SEGMENTS 8
uint8_t segments[NUM_OF_SEGMENTS] = { SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F,
SEG_G, SEG_DP };

#define NUM_OF_DIGITS 4
uint8_t digits[NUM_OF_DIGITS] = { DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4 };
uint8_t digitValues[NUM_OF_DIGITS] = { 0, 0, 0, 0 };

int16_t currentValue = 0;

uint8_t zero[NUM_OF_SEGMENTS] = { LOW, LOW, LOW, LOW, LOW, LOW, HIGH, HIGH };
uint8_t one[NUM_OF_SEGMENTS] = { LOW, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW };
uint8_t two[NUM_OF_SEGMENTS] = { HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH, LOW };
uint8_t three[NUM_OF_SEGMENTS] = { HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH, LOW };
uint8_t four[NUM_OF_SEGMENTS] = { LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW };
uint8_t five[NUM_OF_SEGMENTS] = { HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH, LOW };
uint8_t six[NUM_OF_SEGMENTS] = { HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH, LOW };
uint8_t seven[NUM_OF_SEGMENTS] = { HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW };
uint8_t eight[NUM_OF_SEGMENTS] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, HIGH };
uint8_t nine[NUM_OF_SEGMENTS] = { HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH, LOW };

#define NOP __asm__("nop\n\t")

void clearSegments() {
	for (uint8_t segment = 0; segment < NUM_OF_SEGMENTS; segment++) {
		digitalWrite(segments[segment], HIGH);
	}
}

void clearDigits() {
	for (uint8_t digit = 0; digit < NUM_OF_DIGITS; digit++) {
		digitalWrite(digits[digit], LOW);
	}
}

void clearLEDs() {
	clearSegments();
	clearDigits();
}

uint8_t* getDisplayValue(uint8_t value) {
	uint8_t *displayValue = NULL;
	switch (value) {
	case 0:
		displayValue = zero;
		break;
	case 1:
		displayValue = one;
		break;
	case 2:
		displayValue = two;
		break;
	case 3:
		displayValue = three;
		break;
	case 4:
		displayValue = four;
		break;
	case 5:
		displayValue = five;
		break;
	case 6:
		displayValue = six;
		break;
	case 7:
		displayValue = seven;
		break;
	case 8:
		displayValue = eight;
		break;
	case 9:
		displayValue = nine;
		break;
	default:
		break;
	}
	return displayValue;
}

void displayNumber() {
	for (uint8_t digit = 0; digit < NUM_OF_DIGITS; digit++) {
		clearLEDs();
		digitalWrite(digits[digit], HIGH);
		for (uint8_t segment = 0; segment < NUM_OF_SEGMENTS; segment++) {
			digitalWrite(segments[segment],
					getDisplayValue(digitValues[digit])[segment]);
		}
		NOP;
	}
}

void printValue(int16_t value) {
	digitValues[0] = (int8_t) (value / 1000);
	digitValues[1] = (int8_t) ((value % 1000) / 100);
	digitValues[2] = (int8_t) ((value % 100) / 10);
	digitValues[3] = (int8_t) (value % 10);

	displayNumber();
}

void updateCurrentValue() {
	if (currentValue == 0) {
		currentValue = 8888;
	} else {
		currentValue = 0;
	}
	PORTB ^= (1 << PB7);
}

void setup() {
	DDRB = (1 << PB7);

	pinMode(SEG_A, OUTPUT);
	pinMode(SEG_B, OUTPUT);
	pinMode(SEG_C, OUTPUT);
	pinMode(SEG_D, OUTPUT);
	pinMode(SEG_E, OUTPUT);
	pinMode(SEG_F, OUTPUT);
	pinMode(SEG_G, OUTPUT);
	pinMode(SEG_DP, OUTPUT);

	pinMode(DIGIT_1, OUTPUT);
	pinMode(DIGIT_2, OUTPUT);
	pinMode(DIGIT_3, OUTPUT);
	pinMode(DIGIT_4, OUTPUT);

//	Timer1.initialize(1000000);
	Timer1.initialize(500000);
	Timer1.attachInterrupt(updateCurrentValue);
}

// The loop function is called in an endless loop
void loop() {
	printValue(currentValue);
}
