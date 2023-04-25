#ifndef CAMERA_H
#define CAMERA_H

class Camera {
  private:
    String team;
    long baud;
    int timeout;
  public:
    Camera ();
    void init (long bd, int tmo, String TEAM);
    void reset();
    void test ();
    void begin (long bd, int tmo, String TEAM);

    int xOrange ();
    int yOrange();

    int xOwnGoal();
    int xEnemyGoal();

    // int yOwnGoal();
    // int yEnemyGoal();

    int xBlue ();
    int xYellow ();
    // int yBlue ();
    // int yYellow();
};

#endif
