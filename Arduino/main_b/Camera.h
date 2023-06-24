#ifndef CAMERA_H
#define CAMERA_H

class Camera {
  private:
    long baud;
    int timeout;
    int _ox = -1;
    int _oy = -1;
    int _bx = -1;
    int _by = -1;
    int _yx = -1;
    int _yy = -1;
    int _bi = -1;
    int _yi = -1;
    int _bw = -1;
    int _yw = -1;
    int querySize = 0;
    String queryString = "";

    bool includes(String arr[], int len, String target);
  public:
    Camera ();
    void begin ();

    int direction(int value, int splits);

    void del();
    void add(String string);
    bool call();
    void save(String result);

    int ox () { return _ox; }; // orange-x (ball)
    int oy () { return _oy; }; // orange-y (ball)
    int bx () { return _bx; }; // blue-x (goal)
    int by () { return _by; }; // blue-y (goal)
    int yx () { return _yx; }; // yellow-x (goal)
    int yy () { return _yy; }; // yellow-y (goal)
    int bi () { return _bi; }; // blue-intercept (goal&ball)
    int yi () { return _yi; }; // yellow-intercept (goal&ball)
    int bw () { return _bw; }; // blue-weight (goal)
    int yw () { return _yw; }; // yellow-weight (goal)

    int width;
    int height;
};

#endif