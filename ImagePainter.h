#ifndef IMAGE_PAINTER_H
#define IMAGE_PAINTER_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPixmap>

class ImagePainter : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QSizeF sourceSize READ sourceSize  NOTIFY imageChanged)
    Q_PROPERTY(bool hasImage READ hasImage NOTIFY imageChanged)
    Q_PROPERTY(float aspectRatio READ aspectRatio NOTIFY imageChanged)
    Q_PROPERTY(QImage originalImage READ originalImage NOTIFY imageChanged)
    Q_PROPERTY(FillMode fillMode READ fillMode WRITE setFillMode NOTIFY fillModeChanged)
public:
    explicit ImagePainter(QQuickItem *parent = nullptr);

    enum FillMode {
        Stretch,
        PreserveAspectFit,
        PreserveAspectCrop
    };
    Q_ENUM(FillMode)


    void setImage(QImage image);
    QImage const &image() const;
    QImage const &originalImage() const;
    bool hasImage() const;

    QSizeF sourceSize() const;

    float aspectRatio() const;

    FillMode fillMode() const;
    void setFillMode(FillMode mode);

    Q_INVOKABLE void reset();

    Q_INVOKABLE bool recolor(QColor color);

    Q_INVOKABLE bool loadFromData(QByteArray data);

    Q_INVOKABLE QImage emptyImage(QUrl image);

    void paint(QPainter *painter) override;

signals:
    void imageChanged();
    void fillModeChanged(FillMode);

private:
    void calculateAspectRatio(const float width, const float height);

private:
    QImage image_;
    QImage originalImage_;
    float aspectRatio_ = 1;
    FillMode fillMode_{Stretch};


};



#endif // IMAGE_PAINTER_H
