#include "car.h"

void Wheel::updatePts()
{
    float x[4];
    float y[4];

    x[0] = D/2.;
    x[1] = D/2.;
    x[2] = -D/2.;
    x[3] = -D/2.;

    y[0] = width /2.;
    y[1] = -width/2.;
    y[2] = -width /2.;
    y[3] = width/2.;

    for(int i=0;i<4;i++)
    {
        pts[i].x = cos(theta) * x[i] - sin(theta) * y[i] + x_shift;
        pts[i].y = -sin(theta) * x[i] + cos(theta) * y[i] + y_shift;
    }
}

Car::Car()
{
    width = 2.05;
    Lf = 1.17;
    Lr = 1.77;
    Lh = 1.0;
    D = 0.34*2.;
    Lrear = 1.28;
    m_x = m_y = m_theta = m_v = 0.;
    m_z = 0.55;

    m_psi = 30*D2R;
    tyre_width = 0.2;

    acc_max = 10;
    acc_min = -9.8;

    steer_max = 25 * D2R;
    steer_min = -25 * D2R;

    dt = 1/30.;
    throtle = 0.;
    steer = 0.;
}

void Car::initPts()
{
    pts[0].x = Lf + Lh;
    pts[1].x = Lf + Lh;
    pts[2].x = -(Lr + Lrear);
    pts[3].x = -(Lr + Lrear);

    pts[0].y = width / 2.;
    pts[1].y = -width / 2.;
    pts[2].y = -width / 2.;
    pts[3].y = width / 2.;
}

void Car::updateWheel()
{
    float x[4];
    float y[4];

    x[0] = Lf;
    x[1] = Lf;
    x[2] = -Lr;
    x[3] = -Lr;

    y[0] = width / 2.;
    y[1] = -width / 2.;
    y[2] = -width / 2.;
    y[3] = width / 2.;

    for(int i=0;i<4;i++)
    {
        wheels[i].x_shift = x[i];
        wheels[i].y_shift = y[i];
        wheels[i].D = D;
        wheels[i].theta = m_psi;
        wheels[i].width = tyre_width;
        wheels[i].updatePts();
    }
}

void Car::move()
{

}
