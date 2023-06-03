#ifndef CAMERA_H
#define CAMERA_H

class Camera {
  private:
    long baud;
    int timeout;
    int width;
    int height;

    int _ox = -1;
    int _oy = -1;
    int _bx = -1;
    int _by = -1;
    int _yx = -1;
    int _yy = -1;
    int _bi = -1;
    int _yi = -1;
    int querySize = 0;
    String queryString = "";

    bool includes(String arr[], int len, String target);
  public:
    Camera ();
    void begin ();

    int direction(int value, int splits);

    void del();
    void add(String string);
    void call();
    void save(String result);

    int ox () { return _ox; };
    int oy () { return _oy; };
    int bx () { return _bx; };
    int by () { return _by; };
    int yx () { return _yx; };
    int yy () { return _yy; };
    int bi () { return _bi; };
    int yi () { return _yi; };
};

#endif