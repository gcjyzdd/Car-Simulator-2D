#include <QPainter>
#include <iostream>

#include "drawCar.h"

float polyval(float x, vector<float> &p)
{
    float r=0;
    for(size_t i=0;i<p.size();i++)
    {
        r+= p[i] * pow(x,i);
    }
    return r;
}

void World::initRoad()
{
    vector<float> pl{1.8, 0.05, 1e-3};
    vector<float> pr{-1.8, 0.05, 1e-3};

    vector<Point> left;
    vector<Point> right;

    int N = 20;
    float L = 50.;
    float dl = L/N;

    float x;
    for(int i=0;i<(N*2);i++)
    {
        x = -L+i*dl;
        left.push_back(Point(x, polyval(x,pl)));
        right.push_back(Point(x, polyval(x,pr)));
    }
    Road.push_back(left);
    Road.push_back(right);
}

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

    min_x = -20;
    max_x = -min_x;
    min_y = min_x * scale_xy;
    max_y = max_x * scale_xy;

    scale = (float)width / (max_x - min_x);

    veh.x = veh.y = 0.;
    Lf = 0.8;
    Lr = 1.;
    D = 0.8;

    theta = 90.;
    psi = 30.;

    char s[128];
    sprintf(s,"x: %2.3f\ty: %2.3f\tz: %2.3f", 0.,0.,9.8);
    QString str(s);
    l_xyz = new QLabel(s, this);
    //l_xyz->setFixedWidth(50);

    world.initRoad();

    // Create a server
    if(!server.listen(QHostAddress::Any, 31007))
    {
        std::cerr<<"Failed to bind to port"<<std::endl;
    }

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

    CS cs(world.car.x(), world.car.y(), world.car.theta());

    // draw the car frame
    for(int i=0;i<4;i++)
    {
        ptmp[i] = v2w(world.car.pts[i], cs);
        pCar[i] = w2i(ptmp[i]);
    }
    for(int i=0;i<4;i++)
    {
        qp->drawLine(pCar[i], pCar[(i+1)%4]);
    }

    // draw four wheels
    pen.setColor(Qt::red);
    qp->setPen(pen);
    QPoint pWheel[4];
    for(int j=0;j<4;j++)
    {
        for(int i=0;i<4;i++)
        {
            ptmp[i] = v2w(world.car.wheels[j].pts[i], cs);
            pWheel[i] = w2i(ptmp[i]);
        }
        for(int i=0;i<4;i++)
        {
            qp->drawLine(pWheel[i], pWheel[(i+1)%4]);
        }
    }

    // draw roads
    pen.setColor(Qt::yellow);
    qp->setPen(pen);
    QPoint P2[2];
    Point tmp;
    for(size_t i=0; i<world.Road.size();i++)
    {
        for(size_t j=0; j<world.Road[i].size();j++)
        {
            if(j<2)
            {
                tmp = v2w(world.Road[i][j], cs);
                P2[j] = w2i(tmp);
            }
            else
            {
                P2[0] = P2[1];
                tmp = v2w(world.Road[i][j], cs);
                P2[1] = w2i(tmp);
            }
            if(j>0)
            {
                qp->drawLine(P2[0], P2[1]);
            }
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
    world.car.setPsi(-asin(f3[1]/g));
    world.car.updateWheel();
}

void DrawWorld::bindClient()
{
    connect(server.client_socket, SIGNAL(dataReady(std::vector<float> &)), this, SLOT(updateLabel(std::vector<float>&)));
}
