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
    QAudioInput *audioInput;
    QBuffer *audioBuffer;
    QProgressBar *levelBar;
    QTimer *timer;
};

#endif // NOISEVISUALIZER_H
