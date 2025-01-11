#ifndef NOISEVISUALIZER_H
#define NOISEVISUALIZER_H

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
    void updateSoundLevel();

private:
    Ui::NoiseVisualizer *ui;
    QAudioInput *audioInput = nullptr;
    QBuffer *audioBuffer = nullptr;
    QProgressBar *levelBar = nullptr;
    QTimer *timer = nullptr;
    QCustomPlot *plot = nullptr;
};

#endif // NOISEVISUALIZER_H
