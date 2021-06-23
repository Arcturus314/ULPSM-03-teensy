/*
 * main.cpp
 * Kaveh Pezeshki 6/22/2021
 * Ozone sensor controller (ULPSM-O3) using a Teensy
 */

#define SENS_CODE -60 // sensitivity code
#define GAIN 499      // TIA gain (kV/A)

#define VGAS_PIN A0
#define VREF_PIN A1
#define VTEMP_PIN A2

float vgas0 = 0;       // voltage output gas signal in clean-air environment


float voltageToConc(float voltage) {
    // the calibration here is specified in datasheet ULPSM-O3 968-046

    float M = ((float)SENS_CODE) * ((float)GAIN) * 1e-6;
    float conc = (1/M) * (voltage - vgas0);

    return conc;
}

float readConc() {
    // measures differential voltage between Vgas and Vref, returns output voltage
    // datasheet recommends buffer here

    int inVoltage = analogRead(VGAS_PIN) - 512; // analogRead(VREF_PIN);

    //Serial.print("inVoltage "); Serial.println(inVoltage);

    float diffVoltage = ((float) inVoltage) * 3.3 / 1024.0;

    //Serial.print("diffVoltage "); Serial.println(diffVoltage);

    return voltageToConc(diffVoltage);
}

void setup() {
    Serial.begin(115200);
    Serial.println("ULPSM-O3 Driver");
    
    double buf;
    int numsamples;

    for (int i = 0; i < 1000; i++) {
      buf += analogRead(VGAS_PIN) - 512;
      numsamples += 1;
      delay(1);
    }

    vgas0 = (buf / ((float) numsamples)) * 3.3/1024.0;

}

void loop() {

    double buf;
    int numsamples;

    for (int i = 0; i < 1000; i++) {
      buf += readConc();
      numsamples += 1;
      delay(1);
    }

    double conc = buf / numsamples;

    //char concChar[10];
    //dtostre(conc, concChar, 3, 0);

    Serial.print("Derived concentration ");
    Serial.println(conc);

    //Serial.print("temperature ");
    //Serial.println(analogRead(VTEMP_PIN));

    Serial.println();

}
