/*
  Custom Limit Counter chip example for Wokwi.
  https://wokwi.com/projects/407612178959256577
  built from https://wokwi.com/projects/406335146945275905
  built from https://wokwi.com/projects/327458636089524820

  Discord at https://discord.com/channels/787627282663211009/954892209486966825/1282855086418300938
  
  Start the simulation. 
    The green LEDs show the Normally Closed in-limits condition.
    The red LEDs show the Normally Open outside of limit condition.

    The limits and counter values show up in the Serial monitor.
    Click on the limitCounterChip to adjust the limits.

    Limit checks only occur when the count is changed.

  For more information on custom chips: https://docs.wokwi.com/chips-api/getting-started
*/

#include "TimerHelpers.h" // https://gammon.com.au/timers // to configure PWM

const int scheme = 8; // 0..8 Choose one 0 is slowest range, 6 is fastest, 8 is variable
const unsigned long Period = 1000; // 0..? keep lower than 16k for delayMicrosecond() modes
unsigned long varPeriod = Period; // 
const byte PIN = 9;  // this is OC1A (timer 1 output compare A) for sharing with PWM modes

void setup() {
  pinMode(PIN, OUTPUT);
}

void loop() {
  switch (scheme) {
    case 0: // millisecond period with divided delay()
      digitalWrite(PIN, LOW);
      delay(Period / 2); // 50%
      digitalWrite(PIN, HIGH);
      delay(Period - Period / 2); // Compliment of 50% to deal with odd numbers and zeros
      break;
    case 1: // microsecond period with divided delayMicroseconds()
      digitalWrite(PIN, LOW);
      delayMicroseconds(Period / 2);
      digitalWrite(PIN, HIGH);
      delayMicroseconds(Period - Period / 2);
      break;
    case 2: // microsecond period direct port manipulation
      PORTB &= ~bit(1);
      delayMicroseconds(Period / 2);
      PORTB |= bit(1);
      delayMicroseconds(Period - Period / 2);
      break;
    case 3: // PINx toggle in loop()
      PINB = bit(1);
      break;
    case 4: // PINx toggle, one cycle unrolled per loop()
      PINB = bit(1);
      PINB = bit(1);
      break;
    case 5: // unrolled PINx toggle to reduce loop() overhead
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      PINB = bit(1);
      break;
    case 6: // PWM output
      static bool init6 = false;
      if (!init6 ) {
        init6 = true;
        set8MHz(0);
      }
      delay(10);
      break;
    case 7: // broken PWM output
      static bool init7 = false;
      if (!init7 ) {
        init7 = true;
        set8MHz(1); // alternate
      }
      delay(10);
      break;
    case 8: // variable 0-Period millis
      static int lastA0 = -1;
      int potVal = analogRead(A0);
      if (potVal != lastA0) { // change detect pot
        varPeriod = map(potVal, 1023, 0, 0, Period);
        lastA0 = potVal;
      }
      digitalWrite(PIN, LOW);
      delay(varPeriod / 2);
      digitalWrite(PIN, HIGH);
      delay(varPeriod - varPeriod / 2);
      break;
    default:
      static bool initDefault = false;
      if (!initDefault) {
        initDefault = true;
        Serial.begin(115200);
        Serial.print("unsuppored scheme: ");
        Serial.print(scheme);
      }
  }
}

void set8MHz(const byte scheme) {
  TCCR1A = 0;        // reset timer 1
  TCCR1B = 0;
  // set up Timer 1
  TCNT1 = 0;  // reset counter
  if (scheme == 0) { // 8MHz one way:
    OCR1A = 1;  // compare A register value //
    // WGM 15: FastPWM with TOP=OCR1A
    Timer1::setMode (15, Timer1::PRESCALE_1, Timer1::SET_A_ON_COMPARE);
  }
  else if (scheme == 1) { // 8MHz the other way // Doesn't work on Wokwi
    OCR1A = 0;  // compare A register value //
    // WGM 15: FastPWM with TOP=OCR1A
    Timer1::setMode (15, Timer1::PRESCALE_1, Timer1::TOGGLE_A_ON_COMPARE);
    /*
      Page 103 of https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

      A frequency (with 50% duty cycle) waveform output in fast PWM mode can be achieved by setting OC1A to toggle its logical
      level on each compare match (COM1A1:0 = 1). This applies only if OCR1A is used to define the TOP value
      (WGM13:0 = 15). The waveform generated will have a maximum frequency of fOC1A = fclk_I/O/2 when OCR1A is set to zero
      (0x0000). This feature is similar to the OC1A toggle in CTC mode, except the double buffer feature of the output compare
      unit is enabled in the fast PWM mode.
    */
  }
}
