#include "NoiseVisualizer.h"
#include "ui_NoiseVisualizer.h"

#include <QAudioFormat>
#include <QAudioSource>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QBuffer>
#include <QVBoxLayout>
#include <QDebug>
#include "QCustomPlot/qcustomplot/qcustomplot.h"


NoiseVisualizer::NoiseVisualizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoiseVisualizer),
    levelBar(new QProgressBar(this)),
    timer(new QTimer(this)),
    plot(new QCustomPlot(this)),
    audioSource(nullptr),
    audioBuffer(nullptr)
{
    ui->setupUi(this);

    // Настройка интерфейса
    auto layout = new QVBoxLayout(this);
    layout->addWidget(levelBar);
    layout->addWidget(plot);
    levelBar->setRange(0, 100);

    // Настройка QCustomPlot
    plot->addGraph();
    plot->xAxis->setLabel("Time");
    plot->yAxis->setLabel("Amplitude");
    plot->xAxis->setRange(0, 100);
    plot->yAxis->setRange(-32767, 32767);

    // Аудио настройка (Qt6)
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);  // вместо setSampleSize/Type/Codec

    QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();

    if (!inputDevice.isFormatSupported(format)) {
        qWarning() << "Формат не поддерживается, используем ближайший.";
        format = inputDevice.preferredFormat();
    }

    // Создаем источник аудио
    audioSource = new QAudioSource(inputDevice, format, this);

    // Буфер для аудиоданных
    audioBuffer = audioSource->start();  // возвращает QIODevice*

    // Обновление уровня звука каждые 10 мс
    connect(timer, &QTimer::timeout, this, &NoiseVisualizer::updateSoundLevel);
    timer->start(10);
}

NoiseVisualizer::~NoiseVisualizer()
{
    delete ui;
    if (audioSource) {
        audioSource->stop();
        delete audioSource;
    }
    delete levelBar;
    delete plot;
}

void NoiseVisualizer::updateSoundLevel()
{
    if (!audioBuffer || !audioBuffer->isOpen()) {
        levelBar->setValue(0);
        return;
    }

    QByteArray audioData = audioBuffer->readAll();  // Чтаем данные
    if (audioData.isEmpty()) {
        levelBar->setValue(levelBar->value() - 1);
        return;
    }

    QVector<double> x, y;
    qint16 maxAmplitude = 0;
    const qint16 *data = reinterpret_cast<const qint16 *>(audioData.data());
    int size = audioData.size() / 2;

    for (int i = 0; i < size; ++i) {
        qint16 sample = data[i];
        maxAmplitude = std::max(maxAmplitude, static_cast<qint16>(std::abs(sample)));
        x.append(i);
        y.append(sample);
    }

    plot->graph(0)->setData(x, y);
    plot->replot();

    double level = (static_cast<double>(maxAmplitude) / 32767.0) * 100.0;
    levelBar->setValue(static_cast<int>(level));
}
