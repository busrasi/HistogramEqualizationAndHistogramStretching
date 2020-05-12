#include "ImagePainter.h"

#include <QPainter>
#include <QDebug>
ImagePainter::ImagePainter(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , image_{}
{
    setFlags(ItemHasContents);
}

void ImagePainter::setImage(QImage image)
{
    calculateAspectRatio(image.width(), image.height());
    image_ = image;
    originalImage_ = image;
    emit imageChanged();
    update();
}

QImage ImagePainter::emptyImage(QUrl image)
{
    QString image_content = image.path().replace("image/png;base64,", "");
    QByteArray img_array{ QByteArray::fromBase64(image_content.toLocal8Bit()) };

    return QImage(img_array);
}


const QImage &ImagePainter::image() const
{
    return image_;
}

const QImage &ImagePainter::originalImage() const
{
  return originalImage_;
}

bool ImagePainter::hasImage() const
{
    return !image_.isNull();
}

QSizeF ImagePainter::sourceSize() const
{
    return image_.size();
}

float ImagePainter::aspectRatio() const
{
    return aspectRatio_;
}


ImagePainter::FillMode ImagePainter::fillMode() const
{
    return fillMode_;
}

void ImagePainter::setFillMode(FillMode mode)
{
    if (fillMode_ != mode) {
        fillMode_ = mode;
        emit fillModeChanged(fillMode_);
    }
}

void ImagePainter::reset()
{
    setImage({});
}

bool ImagePainter::recolor(QColor color)
{
    auto const width = originalImage_.width();
    auto const height = originalImage_.height();

    auto image = QImage{width, height, QImage::Format_RGB32};

    auto maximum = qreal{0};
    auto minimum = qreal{1};
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto color = originalImage_.pixelColor(x, y);
            auto intensity = std::clamp<qreal>((1 - color.valueF()) * color.alphaF(), 0, 1);
            minimum = std::min(minimum, intensity);
            maximum = std::max(maximum, intensity);
        }
    }

    auto const scale = qreal{1} / (maximum - minimum);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto sourceColor = originalImage_.pixelColor(x, y);
            auto value = std::clamp<qreal>((1 - sourceColor.valueF()) * sourceColor.alphaF(), 0, 1);
            auto intensity = (value - minimum) * scale;

            auto resultColor = color;
            resultColor.setAlphaF(intensity);
            image.setPixelColor(x, y, resultColor);
        }
    }

    image_ = image;

    update();
    emit imageChanged();
    return true;
}

bool ImagePainter::loadFromData(QByteArray data)
{
    auto image = QImage::fromData(data);
    if (image.isNull()) {
        return false;
    }

    setImage(image);
    return true;

}

void ImagePainter::paint(QPainter *painter)
{
    painter->save();
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    QRectF rect = boundingRect();
    if (fillMode_ == PreserveAspectFit) {
        auto const expanded = QSizeF{image_.size()}.scaled(rect.size(), Qt::KeepAspectRatio);
        auto const delta = (rect.size() - expanded) / 2;

        rect.translate(delta.width(), delta.height());
        rect.setSize(expanded);
        qDebug() << image_.size() << " "<<rect.size();
    } else if (fillMode_ == PreserveAspectCrop) {
        auto const expanded = QSizeF{image_.size()}.scaled(rect.size(), Qt::KeepAspectRatioByExpanding);
        auto const delta = (rect.size() - expanded) / 2;
        rect.translate(delta.width(), delta.height());
        rect.setSize(expanded);
        qDebug() << image_.size() << " rectsize"<<rect.size();
    }

    calculateAspectRatio(rect.width(), rect.height());
    painter->drawImage(rect, image_);
    painter->restore();
}

void ImagePainter::calculateAspectRatio(const float width, const float height)
{
    aspectRatio_ = width / height;
}
