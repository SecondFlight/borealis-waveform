#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QtQuick/QQuickItem>

#include "wavedata.h"
#include <QString>
#include <memory>

class Waveform : public QQuickItem {
    Q_OBJECT

    Q_PROPERTY(double left READ left WRITE setLeft NOTIFY leftChanged)
    Q_PROPERTY(double right READ right WRITE setRight NOTIFY rightChanged)

    Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY filenameChanged)
public:
    Waveform(QQuickItem* parent = nullptr);
    ~Waveform();

    double m_left;
    double m_right;
    QString m_filename;
    const double s_minZoomLevel = 0.0005;

    double left() const { return m_left; }
    double right() const { return m_right; }
    QString filename() const { return m_filename; }

    void setLeft(const double &val);
    void setRight(const double &val);
    void setFilename(const QString &val);

    QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*);

    std::shared_ptr<WaveData> m_data;

signals:
    void leftChanged(const double &val);
    void rightChanged(const double &val);
    void filenameChanged(const QString &val);
};

#endif // WAVEFORM_H
