#pragma once

#include <QWidget>
#include <QThread>
#include <QLabel>

#include "car.h"
#include "sensor_server.h"

using std::vector;

float polyval(float x, vector<float> &p);

class World
{
public:
    Car car;
    vector<vector<Point>> Road;

public:
    void initRoad();
};

class DrawWorld : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWorld(QWidget *parent = 0);
    ~DrawWorld();

protected:
    World world;

    int height, width;
    int min_x, max_x, min_y, max_y;
    float theta, psi;
    Point veh;
    float Lf, Lr, D;
    float scale_xy, scale;

    QLabel *l_xyz;

    void paintEvent(QPaintEvent *event);
    void drawLines(QPainter *qp);
    void drawCar(QPainter *qp);
    void updateDraw();

    QPoint w2i(Point &p);
    Point  v2w(Point &p, CS &cs);
    QThread tcp_listen_;

    SensorServer server;

public slots:
    void updateLabel(std::vector<float>&);
    void bindClient();
};
