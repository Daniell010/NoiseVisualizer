#ifndef NOISEVISUALIZER_H
#define NOISEVISUALIZER_H

#include <QWidget>
#include <QProgressBar>
#include <QTimer>
#include <QAudioSource>
#include <QIODevice>

QT_BEGIN_NAMESPACE
namespace Ui { class NoiseVisualizer; }
QT_END_NAMESPACE

class QCustomPlot;

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
    QProgressBar *levelBar;
    QTimer *timer;
    QCustomPlot *plot;


    QAudioSource *audioSource = nullptr;
    QIODevice *audioBuffer = nullptr;
};

#endif // NOISEVISUALIZER_H
