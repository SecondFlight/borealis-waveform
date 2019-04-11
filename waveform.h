#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QtQuick/QQuickItem>

#include "wavedata.h"
#include <memory>

class Waveform : public QQuickItem {
    Q_OBJECT

    // q_properties for wave data?
public:
    Waveform(QQuickItem* parent = nullptr);
    ~Waveform();

    QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*);

    std::shared_ptr<WaveData> m_data;
};

#endif // WAVEFORM_H
