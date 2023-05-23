#ifndef CAMERA_H
#define CAMERA_H

class Camera {
  private:
    String team;
    long baud;
    int timeout;
    int width;
    int height;
  public:
    Camera ();
    void init (String TEAM);
    void reset();
    void test ();
    void begin (String TEAM);

    int direction(int value, int splits);
    int intercept();
    int xOrange ();
    int yOrange();
    int xEnemyGoal();

    int xBlue ();
    int xYellow ();
};

#endif