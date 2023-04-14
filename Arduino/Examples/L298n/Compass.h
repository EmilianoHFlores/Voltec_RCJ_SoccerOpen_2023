#ifndef COMPASS_H
#define COMPASS_H

class Compass {
  private:
    String type; // navx / adafruit
  public:
    Compass ();
    void init (String typ);
    void reset();
    void test ();
    void begin (String typ);
    
    void initAdaf();
    void initNavx();
    double checkAngle ();
    double checkAngleNavx();
    double checkAngleAdaf();
};

#endif
