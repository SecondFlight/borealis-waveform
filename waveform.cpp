#include "waveform.h"

#include <QtQuick/QSGNode>
#include <QtQuick/QSGFlatColorMaterial>
#include <cmath>

Waveform::Waveform(QQuickItem* parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
    m_data = std::make_shared<WaveData>("C:\\Users\\qbgee\\Documents\\Image-Line\\FL Studio\\Projects\\cello2.wav");
    m_left = 0;
    m_right = 1;
}

Waveform::~Waveform() {
}

QSGNode *Waveform::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData *) {
    QSGGeometryNode *waveNode = nullptr;
    QSGGeometry *wave = nullptr;

    if (!oldNode) {
        waveNode = new QSGGeometryNode;

        wave = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), this->width() * 2);
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
        wave->allocate(this->width() * 2);
    }

    auto wtf = this->m_data;

    QSizeF itemSize = size();
    QSGGeometry::Point2D *topCurvePoints = wave->vertexDataAsPoint2D();
    for (int i = 0; i < itemSize.width(); i++) {
        qreal t = i / qreal(itemSize.width() - 1);

        Packet packet = m_data->getValue(t, m_left, m_right, static_cast<int>(itemSize.width() + 0.5));

        double yTop = packet.max * -itemSize.height() * 0.5 + 0.5 * itemSize.height();
        double yBottom = packet.min * -itemSize.height() * 0.5 + 0.5 * itemSize.height();
        double x = t * itemSize.width();

        auto pointInd = i * 2;
        topCurvePoints[pointInd].set(x, yTop);
        topCurvePoints[pointInd + 1].set(x, yBottom);
    }
    waveNode->markDirty(QSGNode::DirtyGeometry);
//    parentNode->markDirty();
    return waveNode;
}


void Waveform::setLeft(const double &val)
{
    if (val == m_left)
        return;

    if (val > m_right - s_minZoomLevel) {
        m_left = m_right - s_minZoomLevel;
        if (m_left < 0)
            m_left = 0;
    }
    else {
        m_left = val;
    }

    emit leftChanged(val);
    update();
}

void Waveform::setRight(const double &val)
{
    if (val == m_right)
        return;

    if (val < m_left + s_minZoomLevel) {
        m_right = m_left + s_minZoomLevel;
        if (m_right > 1)
            m_right = 1;
    }
    else {
        m_right = val;
    }

    emit rightChanged(val);
    update();
}
