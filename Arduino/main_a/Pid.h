#ifndef PID_H
#define PID_H

class Pid {
  private:
    template <typename T>
    void format(T data, int spaces) {
      if(String(data).length() < spaces) printSpaces(spaces - String(data).length());
    };
    void printSpaces(int spaces) {
      for (int i = 0; i < spaces; i++) {
        Serial.print(" ");
      }
    }
  public:
    Pid();

    struct PidPackage {
        float kp;
        float ki;
        float kd;

        int delay;
        int minOutput;
        int maxOutput;

        int errorThreshold;
        int maxErrorSum;

        int errorSum = 0;
        int previousError = 0;

        bool firstIteration = true;
        unsigned long timer;
    };
    int computePID(int input, int target, int error, PidPackage * pidData);
    void clearPIDdata(PidPackage * pidData);
};

#endif