/*
 * main.cpp
 * Kaveh Pezeshki 6/22/2021
 * Ozone sensor controller (ULPSM-O3) using a Teensy
 */

#include "Arduino.h"

#define VGAS0 0       // voltage output gas signal in clean-air environment
#define SENS_CODE -60 // sensitivity code
#define GAIN 499      // TIA gain (kV/A)

#define VGAS_PIN A0
#define VREF_PIN A1
#define VTEMP_PIN A2

float voltageToConc(float voltage) {
    // the calibration here is specified in datasheet ULPSM-O3 968-046

    float M = ((float)SENS_CODE) * ((float)GAIN) * 1e-6;
    float conc = (1/M) * (voltage - ((float) VGAS0));

    return conc;
}

float readConc() {
    // measures differential voltage between Vgas and Vref, returns output voltage
    // datasheet recommends buffer here

    int inVoltage = analogRead(VGAS_PIN) - analogRead(VREF_PIN);

    Serial.print("inVoltage "); Serial.println(inVoltage);

    float diffVoltage = ((float) inVoltage) * 3.3 / 1024.0;

    Serial.print("diffVoltage "); Serial.println(diffVoltage);

    return voltageToConc(diffVoltage);
}

void setup() {
    Serial.begin(115200);
    Serial.println("ULPSM-O3 Driver");

}

void loop() {

    float conc = readConc();

    //char concChar[10];
    //dtostre(conc, concChar, 3, 0);

    Serial.print("Derived concentration ");
    Serial.println(conc);

    Serial.print("temperature ");
    Serial.println(analogRead(VTEMP_PIN));
}

