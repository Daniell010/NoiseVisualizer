#include "NoiseVisualizer.h"
#include "ui_NoiseVisualizer.h"
#include "QCustomPlot/qcustomplot/qcustomplot.h"

NoiseVisualizer::NoiseVisualizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoiseVisualizer),
    levelBar(new QProgressBar(this)),
    timer(new QTimer(this)),
    plot(new QCustomPlot(this)) // Новый график
{
    ui->setupUi(this);
    // Настройка пользовательского интерфейса
    auto layout = new QVBoxLayout(this);
    layout->addWidget(levelBar);
    layout->addWidget(plot); // Добавляем график в интерфейс
    levelBar->setRange(0, 100);

    // Настройка QCustomPlot
    plot->addGraph();
    plot->xAxis->setLabel("Time");
    plot->yAxis->setLabel("Amplitude");
    plot->xAxis->setRange(0, 100); // Диапазон времени
    plot->yAxis->setRange(-32767, 32767); // Диапазон амплитуд

    // Аудио настройка
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qWarning() << "Формат аудио не поддерживается, используем ближайший.";
        format = info.nearestFormat(format);
    }

    audioInput = new QAudioInput(format, this);

    // Буфер для аудиоданных
    audioBuffer = new QBuffer(this);
    audioBuffer->open(QIODevice::ReadWrite);
    audioInput->start(audioBuffer);

    // Обновление уровня звука каждые 10 мс
    connect(timer, &QTimer::timeout, this, &NoiseVisualizer::updateSoundLevel);
    timer->start(10);
}

NoiseVisualizer::~NoiseVisualizer()
{
    delete ui;
    audioInput->stop();
    delete audioInput;
    delete audioBuffer;
    delete levelBar;
    delete plot;
}

void NoiseVisualizer::updateSoundLevel()
{
    if (!audioBuffer->isOpen()) {
        levelBar->setValue(0);
        return;
    }

    QByteArray audioData = audioBuffer->data(); // Читаем данные из буфера
    if (audioData.isEmpty()) {
        levelBar->setValue(levelBar->value() - 1);
        return;
    }

    // Преобразование данных в массив qint16
    QVector<double> x, y; // Данные для графика
    qint16 maxAmplitude = 0;
    const qint16 *data = reinterpret_cast<const qint16 *>(audioData.data());
    int size = audioData.size() / 2; // Количество 16-битных сэмплов

    for (int i = 0; i < size; ++i) {
        qint16 sample = data[i];
        maxAmplitude = std::max(maxAmplitude, static_cast<qint16>(std::abs(sample)));

        x.append(i);          // Время (индекс сэмпла)
        y.append(sample);     // Амплитуда
    }

    // Обновление графика
    plot->graph(0)->setData(x, y);
    plot->replot(); // Перерисовка графика

    // Обновление прогресс-бара
    double level = (static_cast<double>(maxAmplitude) / 32767.0) * 100.0;
    levelBar->setValue(static_cast<int>(level));
    audioBuffer->reset();

}
