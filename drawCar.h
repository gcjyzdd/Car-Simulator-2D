#pragma once

#include <QWidget>
#include <math.h>

const float D2R = M_PI/180.;

struct Point
{
  float x, y;
};

class Lines : public QWidget
{

public:
  Lines(QWidget *parent = 0);

protected:
  int height, width;
  int min_x, max_x, min_y, max_y;
  float theta, psi;  
  Point veh;
  float Lf, Lr, D;
  float scale_xy, scale;

  void paintEvent(QPaintEvent *event);
  void drawLines(QPainter *qp);

  QPoint w2i(Point p);
};
