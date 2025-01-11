# NoiseVisualizer

**NoiseVisualizer** is a Qt-based application designed to visualize audio noise levels in real time. It integrates QCustomPlot to render dynamic graphs for detailed audio data visualization.

## Features

- Real-time visualization of audio levels with a progress bar.
- Dynamic plotting of audio data using QCustomPlot.
- Customizable audio input settings (sample rate, channel count, etc.).
- Automatic scaling and updates for accurate noise level representation.
- Modular design for easy extension and maintenance.

## Screenshots

![App Screenshot](screenshot.png)
*Example of NoiseVisualizer displaying real-time audio levels.*

## Getting Started

### Prerequisites

Ensure you have the following installed:
- [Qt Framework](https://www.qt.io/download)
- C++ Compiler (e.g., GCC, MSVC, Clang)

### Build Instructions

#### Using qmake:
1. Clone the repository:
   ```bash
   git clone https://github.com/Daniell010/NoiseVisualizer.git
   cd NoiseVisualizer
   ```
2. Open the project file in Qt Creator (`NoiseVisualizer.pro`).
3. Configure the project to include the necessary modules:
   ```plaintext
   QT += core gui widgets multimedia printsupport
   ```
4. Build and run the project from Qt Creator.

#### Using CMake:
1. Clone the repository:
   ```bash
   git clone https://github.com/Daniell010/NoiseVisualizer.git
   cd NoiseVisualizer
   ```
2. Create a build directory and configure:
   ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt
   make
   ```
3. Run the application:
   ```bash
   ./NoiseVisualizer
   ```

## Usage

1. Launch the application.
2. The progress bar will display the current noise level in real time.
3. The graph will dynamically plot the audio signal for detailed analysis.
4. Adjust the audio input settings in the code if needed.

## Development

### Code Structure

- **NoiseVisualizer.cpp / NoiseVisualizer.h**: Core application logic.
- **qcustomplot.cpp / qcustomplot.h**: Graph rendering library.
- **main.cpp**: Application entry point.

### Extending the Project

- **Custom Features**: Add new visual elements using QCustomPlot.
- **Audio Processing**: Integrate additional audio analysis tools (e.g., FFT, filters).

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your changes. Ensure your code follows the existing coding style and includes relevant documentation.

## Acknowledgments

- [QCustomPlot](https://www.qcustomplot.com/) for the excellent plotting library.
- [Qt Framework](https://www.qt.io/) for providing a robust platform for GUI development.

---
**Developed by [Danil](https://github.com/Daniell010).**
