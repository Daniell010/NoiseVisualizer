#include "mainwindow.h"
#include "NoiseVisualizer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NoiseVisualizer n;

    n.show();
    return a.exec();
}
