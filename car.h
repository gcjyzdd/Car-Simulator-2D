#ifndef CAR_H
#define CAR_H
#include <math.h>

const float D2R = M_PI / 180.;
const float R2D = 180. / M_PI;


struct Point
{
    float x, y;
};


class Wheel
{
public:
    float theta;
    float D, width;
    float x_shift, y_shift;
    Point pts[4];

    void updatePts();
};

class Car
{

private:
    float delta_x, delta_y;

    float width;        // width of the car
    float Lf, Lr, Lh;   // CoG to front, CoG to rear, hood length
    float D;            // diameter of wheels
    float Lrear;        // rear length
    float tyre_width;
    float m_x, m_y, m_z, m_theta, m_psi, m_v;
    float acc_max, acc_min, steer_max, steer_min;

public:
    float dt;           // sample time
    float throtle, steer;
    Wheel wheels[4];
    Point pts[4];

public:
    Car();

    // getters
    float x(){return m_x;}
    float y(){return m_y;}
    float psi(){return m_psi;}
    float theta(){return m_theta;}

    // setters
    void setX(float x_){m_x = x_;}
    void setY(float y_){m_y = y_;}
    void setPsi(float psi_){m_psi = psi_;}
    void setTheta(float theta_){m_theta = theta_;}

    // move the car
    void move();
    void updateWheel();
    void initPts();
};
#endif // CAR_H
