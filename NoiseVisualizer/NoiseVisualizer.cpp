#include "NoiseVisualizer.h"
#include "ui_NoiseVisualizer.h"
#include "QCustomPlot/qcustomplot/qcustomplot.h"

NoiseVisualizer::NoiseVisualizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoiseVisualizer),
    levelBar(new QProgressBar(this)),
    timer(new QTimer(this)),
    plot(new QCustomPlot(this)),
    managerTcp(new ManagerTcp(this)) // Инициализация ManagerTcp
{
    ui->setupUi(this);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(levelBar);
    layout->addWidget(plot); // Добавляем график в интерфейс
    levelBar->setRange(0, 100);

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

    // Настройка TCP-соединения
    connect(managerTcp, &ManagerTcp::newDataReceived, this, &NoiseVisualizer::onNewDataReceived);

    // Подключение к серверу
    managerTcp->connectToServer("localhost", 12345); // Укажите адрес и порт сервера

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
    delete managerTcp;  // Удаляем ManagerTcp
}

void NoiseVisualizer::onNewDataReceived(const QByteArray &data)
{
    // Данные получены, обновляем график
    QVector<double> x, y;
    qint16 maxAmplitude = 0;
    const qint16 *dataPtr = reinterpret_cast<const qint16 *>(data.data());
    int size = data.size() / 2;

    for (int i = 0; i < size; ++i) {
        qint16 sample = dataPtr[i];
        maxAmplitude = std::max(maxAmplitude, static_cast<qint16>(std::abs(sample)));

        x.append(i);          // Время (индекс сэмпла)
        y.append(sample);     // Амплитуда
    }

    plot->graph(0)->setData(x, y);
    plot->replot();

    double level = (static_cast<double>(maxAmplitude) / 32767.0) * 100.0;
    levelBar->setValue(static_cast<int>(level));
}

void NoiseVisualizer::updateSoundLevel()
{
    // Если нужно, можно добавить логику для аудио уровня, но для TCP обновления теперь достаточно использовать onNewDataReceived
}
