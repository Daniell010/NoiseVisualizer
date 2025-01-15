#include "ManagerTcp.h"

ManagerTcp::ManagerTcp(QObject *parent) :
    QObject(parent),
    tcpSocket(new QTcpSocket(this))
{
    connect(tcpSocket, &QTcpSocket::readyRead, this, &ManagerTcp::onReadyRead);
    connect(tcpSocket, &QTcpSocket::connected, this, &ManagerTcp::onConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &ManagerTcp::onDisconnected);
}

ManagerTcp::~ManagerTcp()
{
    delete tcpSocket;
}

void ManagerTcp::connectToServer(const QString &host, quint16 port)
{
    tcpSocket->connectToHost(host, port);
}

QByteArray ManagerTcp::getData()
{
    return tcpData;
}

void ManagerTcp::onReadyRead()
{
    // Чтение данных из сокета
    tcpData.append(tcpSocket->readAll());

    // Отправляем сигнал с новыми данными
    emit newDataReceived(tcpData);
}

void ManagerTcp::onConnected()
{
    qDebug() << "Подключено к серверу!";
}

void ManagerTcp::onDisconnected()
{
    qDebug() << "Соединение разорвано!";
}
