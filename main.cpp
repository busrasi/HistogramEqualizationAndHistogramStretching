// Büşra_Şirin_1306160068
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "ImagePainter.h"
#include "HistogramController.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // Register Histogram Controller
    qmlRegisterType<HistogramController>("com.busra.histogramcontroller", 1, 0, "HistogramController");
    qmlRegisterType<ImagePainter>("com.busra.imagepainter", 1, 1, "ImagePainter");



    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
