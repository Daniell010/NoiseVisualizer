#ifndef MANAGERTCP_H
#define MANAGERTCP_H

#include <QObject>
#include <QTcpSocket>

class ManagerTcp : public QObject
{
    Q_OBJECT

public:
    explicit ManagerTcp(QObject *parent = nullptr);
    ~ManagerTcp();

    void connectToServer(const QString &host, quint16 port);
    QByteArray getData();  // Метод для получения данных

signals:
    void newDataReceived(const QByteArray &data);  // Сигнал при получении новых данных

private slots:
    void onReadyRead();      // Обработчик чтения данных из сокета
    void onConnected();      // Обработчик успешного подключения
    void onDisconnected();   // Обработчик разрыва соединения

private:
    QTcpSocket *tcpSocket;
    QByteArray tcpData;     // Буфер для данных TCP
};

#endif // MANAGERTCP_H
