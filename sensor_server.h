#ifndef SENSOR_SERVER_H
#define SENSOR_SERVER_H

#include <QWidget>
#include <QtCore>
#include <QtNetwork>
#include <iostream>
class SensorClient;

class SensorServer : public QTcpServer
{
    Q_OBJECT

public:
    SensorServer(QObject *parent=0);
    SensorClient *client_socket;

public slots:

signals:
    void clientReady();

private:
    void incomingConnection(qintptr socketID);
};

class SensorClient : public QTcpSocket
{
    Q_OBJECT

public:
    SensorClient(QObject *parent=0);

public slots:

    void readClient();

signals:

    void dataReady(std::vector<float> &);


private:

    quint16 nextBlockSize;
};
#endif // SENSOR_SERVER_H
