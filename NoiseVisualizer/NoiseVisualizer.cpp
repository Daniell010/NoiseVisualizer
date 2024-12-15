#include "NoiseVisualizer.h"
#include "ui_NoiseVisualizer.h"

NoiseVisualizer::NoiseVisualizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoiseVisualizer),
    levelBar(new QProgressBar(this)),
    timer(new QTimer(this))
{
    ui->setupUi(this);
    // Настройка пользовательского интерфейса
    auto layout = new QVBoxLayout(this);
    layout->addWidget(levelBar);
    levelBar->setRange(0, 100);

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

    // Обновление уровня звука каждые 100 мс
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
}

void NoiseVisualizer::updateSoundLevel()
{
    if (!audioBuffer->isOpen()) {
                //audioBuffer->open(QIODevice::ReadWrite);
                 levelBar->setValue(0);
                return; // Проверка: буфер должен быть открыт
            }

            QByteArray audioData = audioBuffer->data(); // Получаем данные из буфера
            if (audioData.isEmpty()) {
                levelBar->setValue(levelBar->value() - 1);

                return; // Если нет данных, ничего не делаем
            }

            // Вычисление максимальной амплитуды
            qint16 maxAmplitude = 0;
            const qint16 *data = reinterpret_cast<const qint16 *>(audioData.data());
            int size = audioData.size() / 2; // Данные представляют собой 16-битные числа

            for (int i = 0; i < size; ++i) {
                maxAmplitude = std::max(maxAmplitude, static_cast<qint16>(std::abs(data[i])));
            }

            // Обновление прогресс-бара
            double level = (static_cast<double>(maxAmplitude) / 32767.0) * 100.0; // Преобразуем в диапазон 0-100
            levelBar->setValue(static_cast<int>(level));

            // Очищаем буфер после обработки
            audioBuffer->buffer().clear();

}
