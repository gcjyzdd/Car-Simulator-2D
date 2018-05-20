#include "sensor_server.h"
#include <byteswap.h>

SensorServer::SensorServer(QObject *parent): QTcpServer(parent)
{    
}

void SensorServer::incomingConnection(qintptr socketID)
{
    //std::cout<<"Connected ...\n";
    client_socket = new SensorClient(this);
    client_socket->setSocketDescriptor(socketID);
    emit clientReady();
}

SensorClient::SensorClient(QObject *parent):QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
    //connect(this, SIGNAL(disconnect()), this, SLOT(deleteLater()));
}

void SensorClient::readClient()
{
    QDataStream in(this);
    int len = 12;
    std::vector<float> f3(3);

    in.readRawData((char*)f3.data(), len);

    emit dataReady(f3);
}
