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
    int len = 16;
    std::vector<float> f4(4);

    in.readRawData((char*)f4.data(), len);

    emit dataReady(f4);
}
