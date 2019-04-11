#include "waveform.h"

#include <QtQuick/QSGNode>
#include <QtQuick/QSGFlatColorMaterial>

Waveform::Waveform(QQuickItem* parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
    m_data = std::make_shared<WaveData>("C:\\Users\\qbgee\\Documents\\Image-Line\\FL Studio\\Projects\\cello2.wav");
}

Waveform::~Waveform() {
}

QSGNode *Waveform::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData *) {
    QSGGeometryNode *waveNode = nullptr;
    QSGGeometry *wave = nullptr;

    if (!oldNode) {
        waveNode = new QSGGeometryNode;

        wave = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 512 * 2);
//        wave->setLineWidth(2);

//        wave->setDrawingMode(QSGGeometry::DrawLineStrip);
//        geometry->setDrawingMode(QSGGeometry::DrawingMode::DrawTriangleStrip);
        waveNode->setGeometry(wave);
        waveNode->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *topMaterial = new QSGFlatColorMaterial;
        topMaterial->setColor(QColor(55, 164, 131));
        waveNode->setMaterial(topMaterial);
        waveNode->setFlag(QSGNode::OwnsMaterial);
    } else {
        waveNode = static_cast<QSGGeometryNode *>(oldNode);
        wave = waveNode->geometry();
        wave->allocate(512 * 2);
    }

    QSizeF itemSize = size();
    QSGGeometry::Point2D *topCurvePoints = wave->vertexDataAsPoint2D();
    for (int i = 0; i < 512; i++) {
        qreal t = i / qreal(512 - 1);

        Packet packet = m_data->getValue(t, 1);

        double yTop = packet.max * -itemSize.height() + 0.5 * itemSize.height();
        double yBottom = packet.min * -itemSize.height() + 0.5 * itemSize.height();
        double x = t * itemSize.width();

        auto pointInd = i * 2;
        topCurvePoints[pointInd].set(x, yTop);
        topCurvePoints[pointInd + 1].set(x, yBottom);
    }
    waveNode->markDirty(QSGNode::DirtyGeometry);
//    parentNode->markDirty();
    return waveNode;
}
