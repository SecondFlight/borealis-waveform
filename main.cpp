#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "wavedata.h"
#include "waveform.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<Waveform>("Borealis.Modules.Audio.Static", 1, 0, "Waveform");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

//    WaveData data = WaveData("C:\\Program Files (x86)\\Image-Line\\FL Studio 20\\Data\\Patches\\Packs\\Drums\Toms\\Linn Tom.wav");
//    WaveData data = WaveData("C:\\Users\\qbgee\\Documents\\Image-Line\\FL Studio\\Projects\\cello2.wav");

//    qDebug() << data.summaries.length();

//    for (auto item : data.summaries) {
//        qDebug() << item.m_packets[0].length();
//    }

    return app.exec();
}
