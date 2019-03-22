#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QtQuick/QQuickItem>

class Waveform : public QQuickItem {
    Q_OBJECT

    // q_properties for wave data?
public:
    Waveform(QQuickItem* parent = nullptr);
    ~Waveform();

    QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*);
};

#endif // WAVEFORM_H
