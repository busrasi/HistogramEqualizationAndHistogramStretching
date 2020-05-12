#include "HistogramController.h"

#include <QFile>
#include <QByteArray>
#include <QTextStream>
#include <QDebug>
#include <QPixmap>

// Constructor of Histogram Controller
HistogramController::HistogramController(QObject *parent)
    : QObject(parent)
{

}

// Constructor of Histogram Controller
HistogramController::~HistogramController()
{

}

// Image Getter Function
QImage HistogramController::image()
{
    return m_image;
}

// Image Setter Function
void HistogramController::setImage(QImage image)
{
    if(!image.isNull())
    {
       m_image = image;
       emit imageChanged();
    }
}

// Read Image Function
void HistogramController::readImageFromUrl(QUrl imageUrl)
{
    m_image= QImage(imageUrl.toLocalFile());
    emit imageChanged();
}

void HistogramController::equalizeImage(QUrl imageUrl)
{
    readImageFromUrl(imageUrl);
    m_image =  m_algorithm.convertToGrayscale(m_image);
    qDebug()<<"bsr pdata"<<m_image;
    // Convert image to int array data
    int* p_data= new int[m_image.sizeInBytes()];
    for(int i= 0; i<m_image.sizeInBytes(); i++)
    {

        p_data[i]= m_image.bits()[i];

    }
    m_image.save("deneme.png");

    // Do histogram equalization
    QImage img = m_algorithm.equalizeHistogram(p_data, m_image.width(), m_image.height());

    // Save image to file
    img.save("histogramResult.png");
    img.convertTo(QImage::Format_RGB32);
    img.save("sonbakış.png");
    setImage(img);
}

void HistogramController::stretchImage(QUrl imageUrl)
{
    readImageFromUrl(imageUrl);
    m_image =  m_algorithm.convertToGrayscale(m_image);

     m_image.save("grayimage.png");
    // Convert image to int array data

    int* p_data= new int[m_image.sizeInBytes()];
    qDebug()<<"bsr pdata::"<< m_image.sizeInBytes();

    for(int i= 0; i<m_image.sizeInBytes(); i++)
    {
            p_data[i]= m_image.bits()[i];


    }
    QImage img =m_algorithm.stretchHistogram(p_data, m_image.width(),m_image.height());
    img.save("histogramStretchResult.png");
    img.convertTo(QImage::Format_RGB32);
    setImage(img);
}

QUrl HistogramController::imageUrl()
{
    return m_imageUrl;
}

void HistogramController::setImageUrl(QUrl imageUrl)
{
    if(!imageUrl.isEmpty())
    {
       m_imageUrl = imageUrl;
       emit imageUrlChanged();
    }
}
