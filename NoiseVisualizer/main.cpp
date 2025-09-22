#include <QApplication>

#include "clientaudiosender.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ClientAudioSender client;
    client.start("127.0.0.1", 45454); // локальный сервер на порту 45454

    return a.exec();
}
