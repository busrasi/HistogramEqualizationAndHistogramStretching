#ifndef HISTOGRAM_ALGORITHMS_H
#define HISTOGRAM_ALGORITHMS_H

#include <QObject>
#include <QByteArray>

class HistogramAlgorithms : public QObject
{

public:

    // C-tor
    HistogramAlgorithms();

    // D-tor
    ~HistogramAlgorithms();

    // Histogram Equalization
    QImage equalizeHistogram(int* pdata, int width, int height,int max_value= 255);

    // Convert to Grayscale
    QImage convertToGrayscale(QImage &image);

    QImage stretchHistogram(int* pdata, int width, int height, int max_val = 255);

};



#endif //HISTOGRAM_ALGORITHMS_H
