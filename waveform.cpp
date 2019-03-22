#include "waveform.h"

#include <QtQuick/QSGNode>
#include <QtQuick/QSGFlatColorMaterial>

Waveform::Waveform(QQuickItem* parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
}

Waveform::~Waveform() {
}

QSGNode *Waveform::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData *) {
    QSGGeometryNode *node = nullptr;
    QSGGeometry *geometry = nullptr;

    if (!oldNode) {
        node = new QSGGeometryNode;

        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 32);
        geometry->setLineWidth(5);
        geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
//        geometry->setDrawingMode(QSGGeometry::DrawingMode::DrawTriangleStrip);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(QColor(255, 0, 0));
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);

    } else {
        node = static_cast<QSGGeometryNode *>(oldNode);
        geometry = node->geometry();
        geometry->allocate(32);
    }

    QSizeF itemSize = size();
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
    for (int i = 0; i < 32; ++i) {
        qreal t = i / qreal(32 - 1);
        qreal invt = 1 - t;

        QPointF pos = invt * invt * invt * QPointF(0, 0)
                    + 3 * invt * invt * t * QPointF(1, 0)
                    + 3 * invt * t * t * QPointF(0, 1)
                    + t * t * t * QPointF(1, 1);

        float x = pos.x() * itemSize.width();
        float y = pos.y() * itemSize.height();

        vertices[i].set(x, y);
    }
    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}
