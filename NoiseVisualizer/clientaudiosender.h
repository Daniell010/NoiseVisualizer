#ifndef CLIENTAUDIOSENDER_H
#define CLIENTAUDIOSENDER_H

#include <QObject>
#include <QTcpSocket>
#include <QAudioSource>
#include <QMediaDevices>
#include <QIODevice>

class ClientAudioSender : public QObject
{
    Q_OBJECT
public:
    explicit ClientAudioSender(QObject* parent = nullptr);

    void start(const QString& host, quint16 port);
    void stop();

private slots:
    void readMore();

private:
    QTcpSocket* socket = nullptr;
    QAudioSource* audioSource = nullptr;
    QIODevice* audioBuffer = nullptr;
};

#endif // CLIENTAUDIOSENDER_H
