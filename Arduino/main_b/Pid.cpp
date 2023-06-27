#include <Arduino.h>
#include "Pid.h"

Pid::Pid() {};

void Pid::clearPIDdata (PidPackage * pidData) {
    pidData -> errorSum = 0;
    pidData -> previousError = 0;
    pidData -> firstIteration = true;
}

int Pid::computePID(int input, int target, int error, PidPackage * pidPackage) {
    if (pidPackage -> firstIteration) {
        pidPackage -> timer = millis();
        pidPackage -> firstIteration = false;
    }

    if (millis() - pidPackage -> timer >= pidPackage -> delay) {
        pidPackage -> timer = millis();

        pidPackage -> errorSum += error;
        if (pidPackage -> errorSum > pidPackage -> maxErrorSum) pidPackage -> errorSum = pidPackage -> maxErrorSum;
        else if (pidPackage -> errorSum < -pidPackage -> maxErrorSum) pidPackage -> errorSum = -pidPackage -> maxErrorSum;

        float proportional_term = pidPackage -> kp * error;
        float integral_term = pidPackage -> ki * pidPackage -> errorSum;
        float derivate_term = pidPackage -> kd * (error - pidPackage -> previousError);
        int output = proportional_term + integral_term + derivate_term;
        if (abs(output) > pidPackage->maxOutput) output = pidPackage->maxOutput * (output > 0 ? 1 : -1);
        if (abs(output) < pidPackage->minOutput) output = pidPackage->minOutput * (output > 0 ? 1 : -1);
        if (abs(error) < pidPackage->errorThreshold) {
            output = 0;
            clearPIDdata(pidPackage);
        };

        pidPackage -> previousError = error;

        Serial.print("Input: "); Serial.print(input); format(input, 7);
        Serial.print("Target: "); Serial.print(target); format(target, 7);
        Serial.print("Error: "); Serial.print(error); format(error, 7);
        Serial.print("ErrorSum: "); Serial.print(pidPackage -> errorSum); format(pidPackage -> errorSum, 7);
        Serial.print("P: "); Serial.print(proportional_term); format(proportional_term, 7);
        Serial.print("I: "); Serial.print(integral_term); format(integral_term, 7);
        Serial.print("D: "); Serial.print(derivate_term); format(derivate_term, 7);
        Serial.print("Output: "); Serial.print(output); format(output, 7);
        Serial.println();

        return output;
    }
    return 9999;
}