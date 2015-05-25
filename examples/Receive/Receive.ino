/*
 Copyright (c) 2014-2015 NicoHood
 See the readme for credit to other people.

 IRL Receive

 Receives IR signals from different protocols and prints them to the Serial monitor.
 Choose your protocols that should be decoded. Remove the not used ones to save flash/ram/speed.
 You can choose a custom debounce time to not trigger a button two times in a row too fast.

 The following pins are usable for PinInterrupt or PinChangeInterrupt*:
 Arduino Uno/Nano/Mini: 2, 3, All pins* are usable
 Arduino Mega: 2, 3, 18, 19, 20, 21,
               10*, 11*, 12*, 13*, 50*, 51*, 52*, 53*, A8 (62)*, A9 (63)*, A10 (64)*,
               A11 (65)*, A12 (66)*, A13 (67)*, A14 (68)*, A15 (69)*
 Arduino Leonardo/Micro: 0, 1, 2, 3, 7, 8*, 9*, 10*, 11*, 14 (MISO)*, 15 (SCK)*, 16 (MOSI)*
 HoodLoader2: All (broken out 1-7*) pins are usable
 Attiny 24/44/84: 8, All pins* are usable
 Attiny 25/45/85: 2, All pins* are usable
 ATmega644P/ATmega1284P: 10, 11, All pins* are usable

 *: PinChangeInterrupts requires a special library which can be downloaded here:
  https://github.com/NicoHood/PinChangeInterrupt
*/

// include PinChangeInterrupt library* BEFORE IRLremote to acces more pins if needed
//#include "PinChangeInterrupt.h"

#include "IRLremote.h"
// choose a valid PinInterrupt or PinChangeInterrupt* pin of your Arduino board
#define pinIR 2
#define IRL_DEBOUCE 300
CIRLremote<IRL_DEBOUCE, IR_NEC, IR_PANASONIC, IR_SONY12> IRLremote;

#define pinLed LED_BUILTIN

void setup() {
  // start serial debug output
  while(!Serial);
  Serial.begin(115200);
  Serial.println(F("Startup"));

  // set LED to output
  pinMode(pinLed, OUTPUT);

  // start reading the remote. PinInterrupt or PinChangeInterrupt* will automatically be selected
  if (!IRLremote.begin(pinIR))
    Serial.println(F("You did not choose a valid pin."));
}

void loop() {
  if (IRLremote.available()) {
    // light Led
    digitalWrite(pinLed, HIGH);

    // get the new data from the remote
    IR_data_t data = IRLremote.read();

    // print protocol number
    Serial.println();
    Serial.print(F("Protocol: "));
    Serial.print(data.protocol);

    // see readme to terminate what number is for each protocol
    switch (data.protocol) {
      case IR_NEC:
        Serial.println(F(" NEC"));
        break;
      case IR_PANASONIC:
        Serial.println(F(" Panasonic"));
        break;
      case IR_SONY12:
        Serial.println(F(" Sony12"));
        break;
      default:
        Serial.println(F(" Unknown"));
        break;
    }

    // print the protocol data
    Serial.print(F("Address: 0x"));
    Serial.println(data.address, HEX);
    Serial.print(F("Command: 0x"));
    Serial.println(data.command, HEX);

    // turn Led off after printing the data
    digitalWrite(pinLed, LOW);
  }
}
