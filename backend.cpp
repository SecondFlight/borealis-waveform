#include "backend.h"

Backend::Backend(QObject *parent) : QObject(parent) {
    m_data = "huston we have liftoff";
}

QString Backend::data() {
    qDebug("m_data has been accessed.");
    return m_data;
}

void Backend::setData(const QString &data) {
    qDebug("setData has been called");

    if (data == m_data)
        return;

    m_data = data;
    qDebug("m_data has been set to " + data.toLatin1());
    emit dataChanged();
}
