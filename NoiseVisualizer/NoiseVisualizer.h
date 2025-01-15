#ifndef NOISEVISUALIZER_H
#define NOISEVISUALIZER_H

#include "ManagerTcp.h"

#include <QWidget>
#include <QAudioInput>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QTimer>
#include <QWidget>
#include <QDebug>
#include <algorithm>
#include <QBuffer>

class QCustomPlot;

namespace Ui {
class NoiseVisualizer;
}

class NoiseVisualizer : public QWidget
{
    Q_OBJECT

public:
    explicit NoiseVisualizer(QWidget *parent = nullptr);
    ~NoiseVisualizer();

private slots:
    void onNewDataReceived(const QByteArray &data);  // Обработчик новых данных от ManagerTcp
    void updateSoundLevel();  // Обновление уровня звука

private:
    Ui::NoiseVisualizer *ui;
    QProgressBar *levelBar;
    QTimer *timer;
    QCustomPlot *plot;
    QAudioInput *audioInput;
    QBuffer *audioBuffer;
    ManagerTcp *managerTcp;   // Объект ManagerTcp
};

#endif // NOISEVISUALIZER_H
