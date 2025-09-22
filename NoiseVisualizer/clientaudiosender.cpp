#include "clientaudiosender.h"

#include <QAudioFormat>
#include <QDebug>

ClientAudioSender::ClientAudioSender(QObject* parent)
    : QObject(parent)
{
}

void ClientAudioSender::start(const QString& host, quint16 port)
{
    // TCP-соединение
    socket = new QTcpSocket(this);
    socket->connectToHost(host, port);
    if (!socket->waitForConnected(3000)) {
        qWarning() << "Не удалось подключиться к серверу";
        return;
    }

    // Формат аудио
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();
    if (!inputDevice.isFormatSupported(format)) {
        qWarning() << "Формат не поддерживается, используем ближайший.";
        format = inputDevice.preferredFormat();
    }

    // Источник аудио
    audioSource = new QAudioSource(inputDevice, format, this);
    audioBuffer = audioSource->start();

    // Читаем из микрофона и пересылаем
    connect(audioBuffer, &QIODevice::readyRead, this, &ClientAudioSender::readMore);

    qDebug() << "Клиент запущен, идёт запись и передача на сервер...";
}

void ClientAudioSender::stop()
{
    if (audioSource) {
        audioSource->stop();
        delete audioSource;
        audioSource = nullptr;
    }
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
        socket = nullptr;
    }
}

void ClientAudioSender::readMore()
{
    if (!socket || !audioBuffer)
        return;

    QByteArray data = audioBuffer->readAll();
    if (!data.isEmpty()) {
        socket->write(data);
    }
}
