#ifndef HISTOGRAM_CONTROLLER_H
#define HISTOGRAM_CONTROLLER_H

#include <QObject>
#include <QImage>
#include <QUrl>

#include "HistogramAlgorithms.h"

class HistogramController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QUrl imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged)

public:
    // C-tor
    HistogramController(QObject *parent=nullptr);

    // D-tor
    ~HistogramController();

    // Getter Image
    QImage image();

    // Setter Image
    void setImage(QImage image);

    // Getter ImageUrl
    QUrl imageUrl();

    // Setter ImageUrl
    void setImageUrl(QUrl imageUrl);

    // Setter Image From QUrl
    void readImageFromUrl(QUrl imageUrl);

    // Equalize Image
    Q_INVOKABLE void equalizeImage(QUrl imageUrl);

    // Stretching Image
    Q_INVOKABLE void stretchImage(QUrl imageUrl);



signals:
    void imageChanged();
    void imageUrlChanged();

private:

    QImage m_image;
    QUrl m_imageUrl;
    HistogramAlgorithms m_algorithm;

};


#endif // HISTOGRAM_CONTROLLER_H




