#include "HistogramAlgorithms.h"

#include <QVector>
#include <QImage>
#include <QDebug>

#include <math.h>

QByteArray serialize(int* ar, int ar_size)
{
    QByteArray result;
    QDataStream stream (&result, QIODevice::WriteOnly);
    stream<<ar_size;

    for (int i=0; i<ar_size; i++)
        stream<<ar[i];

    return result;
}

HistogramAlgorithms::HistogramAlgorithms()
{

}

HistogramAlgorithms::~HistogramAlgorithms()
{

}

QImage HistogramAlgorithms::equalizeHistogram(int* pdata, int width, int height, int max_val)
{
    int total = width*height*4;
    int n_bins = max_val + 1;
    qDebug()<<"bsr total::"<<total;

    // Compute Histogram
    QVector<int> histogram(n_bins, 0);
    for(int i = 0; i < total; ++i)
    {
        histogram[pdata[i]]++;
    }
    // Find first non-zero bin
    int i = 0;
    for(i= 0; i< histogram.size(); i++ )
    {
        if(histogram[i]!= 0)
        {
            break;
        }
    }

    if (histogram[i] == total)
    {
        for (int j = 0; j < total; ++j)
        {
            pdata[j] = i;
        }
        return QImage();
    }

    // Compute scale
    float scale = (n_bins - 1.f) / (total - histogram[i]);

    // Initialize lut
    QVector<int> lut(n_bins, 0);
    i++;

    // Calculating cumutative histogram (lut = Dynamic histogram)
    int sum = 0;
    for (; i < histogram.size(); ++i)
    {
        sum += histogram[i];
        // the value is saturated in range [0, max_val]
        lut[i] = std::max(0, std::min(int(std::round(sum * scale)), max_val));
    }

    // Apply equalization
    for (int i = 0; i < total; ++i)
    {
        pdata[i] = lut[pdata[i]];
    }

    QImage my_image;
    QByteArray img_buf= serialize(pdata,total);

    unsigned char* img_char= new unsigned char[total];
    for(int i=0 ; i< total; i++ ){
        img_char[i]= static_cast<char>(pdata[i]);
    }
    return QImage(img_char, width, height, QImage::Format_RGB32);
}

QImage HistogramAlgorithms::convertToGrayscale(QImage &image)
{
    for (int ii = 0; ii < image.height(); ii++)
    {
        uchar* scan = image.scanLine(ii);
        int depth =4;
        for (int jj = 0; jj < image.width(); jj++)
        {
            QRgb* rgbpixel = reinterpret_cast<QRgb*>(scan + jj*depth);
            int gray = qGray(*rgbpixel);
            *rgbpixel = QColor(gray, gray, gray).rgba();
        }
    }
    return image;
}

QImage HistogramAlgorithms::stretchHistogram(int *pdata, int width, int height, int max_val)
{
    int total = width*height*4;
    int n_bins = max_val + 1;
    qDebug()<<"bsr total::"<<total;

    // Compute Histogram
    QVector<int> histogram(n_bins, 0);
    for(int i = 0; i < total; ++i)
    {
        histogram[pdata[i]]++;
    }
    // Find first non-zero bin
    int i = 0;
    for(i= 0; i< histogram.size(); i++ )
    {
        if(histogram[i]!= 0)
        {
            break;
        }
    }

    if (histogram[i] == total)
    {
        for (int j = 0; j < total; ++j)
        {
            pdata[j] = i;
        }
        return QImage();
    }

    // Find darkest and brightest values
    int darkest = 255;
    int brightest = 0;
    for (int i = 0; i < total; ++i)
    {
        if(darkest > pdata[i])
        {
            darkest = pdata[i];
        }

        if(brightest < pdata[i])
        {
            brightest = pdata[i];
        }
    }

    // Apply streching
    for (int i = 0; i < total; ++i)
    {
        // newvalue = int((current value - darkest)*255/(brightest-darkest))
        int currentValue = pdata[i];
        int newvalue = ((currentValue - darkest) * 255) / (brightest - darkest);
        pdata[i] = newvalue;
    }

    QImage my_image;
    QByteArray img_buf= serialize(pdata,total);

    unsigned char* img_char= new unsigned char[total];
    for(int i=0 ; i< total; i++ ){
        img_char[i]= static_cast<char>(pdata[i]);
    }
    return QImage(img_char, width, height, QImage::Format_RGB32);
}


