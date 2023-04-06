#ifndef CAMERA_H
#define CAMERA_H

class Camera {
  private:
    long baud;
    int timeout;
  public:
    Camera ();
    void init (long bd, int tmo);
    void reset();
    void test ();
    void begin (long bd, int tmo);

    int callOrange ();
    int callBlue ();
    int callYellow ();
};

#endif
