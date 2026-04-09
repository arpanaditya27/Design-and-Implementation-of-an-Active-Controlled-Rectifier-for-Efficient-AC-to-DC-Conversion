#define ZC_PIN   2
#define Q1_PIN   8
#define Q2_PIN   9
#define Q3_PIN   10
#define Q4_PIN   11

#define HALF_CYCLE_US  10000
#define FIRING_DELAY   5000
#define ON_TIME (HALF_CYCLE_US - FIRING_DELAY - 300)

volatile bool zeroCrossed = false;
volatile bool halfCycle   = false;

void zeroCrossISR() {
  halfCycle   = !halfCycle;
  zeroCrossed = true;
}

void setup() {
  pinMode(Q1_PIN, OUTPUT);
  pinMode(Q2_PIN, OUTPUT);
  pinMode(Q3_PIN, OUTPUT);
  pinMode(Q4_PIN, OUTPUT);
  pinMode(ZC_PIN, INPUT_PULLUP);

  digitalWrite(Q1_PIN, LOW);
  digitalWrite(Q2_PIN, LOW);
  digitalWrite(Q3_PIN, LOW);
  digitalWrite(Q4_PIN, LOW);

  for (int i = 0; i < 3; i++) {
    digitalWrite(Q2_PIN, HIGH); delayMicroseconds(2000);
    digitalWrite(Q2_PIN, LOW);  delayMicroseconds(300);
    digitalWrite(Q3_PIN, HIGH); delayMicroseconds(2000);
    digitalWrite(Q3_PIN, LOW);  delayMicroseconds(300);
  }

  delay(100);

  // INVERTED from before — this locks correct phase
  halfCycle = (digitalRead(ZC_PIN) == LOW);

  attachInterrupt(digitalPinToInterrupt(ZC_PIN), zeroCrossISR, CHANGE);
}

void loop() {
  if (zeroCrossed) {

    noInterrupts();
    zeroCrossed   = false;
    bool thisHalf = halfCycle;
    interrupts();

    delayMicroseconds(FIRING_DELAY);

    if (!thisHalf) {
      // Q1 + Q2
      digitalWrite(Q1_PIN, HIGH);
      digitalWrite(Q2_PIN, HIGH);
      delayMicroseconds(ON_TIME);
      digitalWrite(Q1_PIN, LOW);
      digitalWrite(Q2_PIN, LOW);

    } else {
      // Q4 + Q3
      digitalWrite(Q4_PIN, HIGH);
      digitalWrite(Q3_PIN, HIGH);
      delayMicroseconds(ON_TIME);
      digitalWrite(Q4_PIN, LOW);
      digitalWrite(Q3_PIN, LOW);
    }
  }
}