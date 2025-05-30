#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "diagrammanager.h"

int main(int argc, char *argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    qmlRegisterType<DiagramManager>("com.example.diagram", 1, 0, "DiagramManager");

    QQmlApplicationEngine engine;
    DiagramManager*        diagramManager = new DiagramManager(&engine);
    engine.rootContext()->setContextProperty("diagramManager", diagramManager);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    QObject::connect(diagramManager, &DiagramManager::errorOccurred,
                     [](const QString &message) { qDebug() << "Error:" << message; });

    engine.load(url);

    return app.exec();
}