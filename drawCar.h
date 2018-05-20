#pragma once

#include <QWidget>
#include <QThread>
#include <QLabel>
#include <math.h>

#include "sensor_server.h"

const float D2R = M_PI / 180.;
const float R2D = 180. / M_PI;


struct Point
{
    float x, y;
};

class Lines : public QWidget
{
    Q_OBJECT
public:
    explicit Lines(QWidget *parent = 0);
    ~Lines();

protected:
    int height, width;
    int min_x, max_x, min_y, max_y;
    float theta, psi;
    Point veh;
    float Lf, Lr, D;
    float scale_xy, scale;

    QLabel *l_xyz;

    void paintEvent(QPaintEvent *event);
    void drawLines(QPainter *qp);
    void updateDraw();

    QPoint w2i(Point p);

    QThread tcp_listen_;

    SensorServer server;

public slots:
    void updateLabel(std::vector<float>&);
    void bindClient();
};
