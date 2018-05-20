#include <QPainter>
#include <iostream>

#include "drawCar.h"

QPoint DrawWorld::w2i(Point &p)
{
    return QPoint((p.x - min_x) * scale, height - (p.y - min_y) * scale);
}

Point DrawWorld::v2w(Point &p, CS &cs)
{
    return Point(cos(cs.theta)*p.x - sin(cs.theta)*p.y+cs.x,
                 sin(cs.theta)*p.x + cos(cs.theta)*p.y+cs.y);
}

DrawWorld::~DrawWorld(){}

DrawWorld::DrawWorld(QWidget *parent)
    : QWidget(parent)
{
    width = 960;
    height = 720;
    scale_xy = (float)height / width;

    min_x = -6;
    max_x = 6;
    min_y = min_x * scale_xy;
    max_y = max_x * scale_xy;

    scale = (float)width / (max_x - min_x);

    veh.x = veh.y = 0.;
    Lf = 0.8;
    Lr = 1.;
    D = 0.8;

    theta = 90.;
    psi = 30.;

    l_xyz = new QLabel("Hello", this);
    l_xyz->setFixedWidth(50);
    // Create a server
    if(!server.listen(QHostAddress::Any, 31007))
    {
        std::cerr<<"Failed to bind to port"<<std::endl;
    }
    std::cout<<"car.pts[0] = "<<car.pts[0].x <<","<< car.pts[0].y<<std::endl;
    connect(&server, SIGNAL(clientReady()), this, SLOT(bindClient()));
}

void DrawWorld::paintEvent(QPaintEvent *e)
{

    Q_UNUSED(e);

    updateDraw();
    this->update();
}

void DrawWorld::updateDraw()
{
    QPainter qp(this);
    //drawLines(&qp);
    drawCar(&qp);
    this->resize(width, height);
}

void DrawWorld::drawLines(QPainter *qp)
{

    Point p1, p2, p3, p4;
    p1.x = veh.x - Lr * cos(theta * D2R);
    p1.y = veh.y - Lr * sin(theta * D2R);

    p2.x = veh.x + Lf * cos(theta * D2R);
    p2.y = veh.y + Lf * sin(theta * D2R);

    //std::cout<<"psi = "<< psi << std::endl;
    p3.x = p2.x - D / 2. * cos((theta + psi) * D2R);
    p3.y = p2.y - D / 2. * sin((theta + psi) * D2R);

    p4.x = p2.x + D / 2. * cos((theta + psi) * D2R);
    p4.y = p2.y + D / 2. * sin((theta + psi) * D2R);

    QPoint P1, P2, P3, P4;

    P1 = w2i(p1);
    P2 = w2i(p2);
    P3 = w2i(p3);
    P4 = w2i(p4);

    QPen pen(Qt::black, 2, Qt::SolidLine);
    qp->setPen(pen);
    qp->drawLine(P1.x(), P1.y(), P2.x(), P2.y());

    pen.setColor(Qt::red);
    qp->setPen(pen);
    qp->drawLine(P3.x(), P3.y(), P4.x(), P4.y());
}

void DrawWorld::drawCar(QPainter *qp)
{
    QPen pen(Qt::black, 2, Qt::SolidLine);
    qp->setPen(pen);
    QPoint pCar[4];
    Point ptmp[4];

    CS cs(car.x(), car.y(), car.theta());
    for(int i=0;i<4;i++)
    {
        ptmp[i] = v2w(car.pts[i], cs);
        pCar[i] = w2i(ptmp[i]);
    }
    for(int i=0;i<4;i++)
    {
        qp->drawLine(pCar[i], pCar[(i+1)%4]);
    }

    pen.setColor(Qt::red);
    qp->setPen(pen);
    QPoint pWheel[4];
    for(int j=0;j<4;j++)
    {
        for(int i=0;i<4;i++)
        {
            ptmp[i] = v2w(car.wheels[j].pts[i], cs);
            pWheel[i] = w2i(ptmp[i]);
        }
        for(int i=0;i<4;i++)
        {
            qp->drawLine(pWheel[i], pWheel[(i+1)%4]);
        }
    }
}

void DrawWorld::updateLabel(std::vector<float> &f3)
{
    char s[128];
    sprintf(s,"x: %2.3f\ty: %2.3f\tz: %2.3f", f3[0],f3[1],f3[2]);
    QString str(s);
    //std::cout<<s<<std::endl;
    l_xyz->setText(str);
    float g = sqrt(pow(f3[0],2) + pow(f3[1],2) + pow(f3[2],2));
    psi = -asin(f3[1]/g) * R2D;
    car.setPsi(-asin(f3[1]/g));
    car.updateWheel();
}

void DrawWorld::bindClient()
{
    connect(server.client_socket, SIGNAL(dataReady(std::vector<float> &)), this, SLOT(updateLabel(std::vector<float>&)));
}
