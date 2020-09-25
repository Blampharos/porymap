#include "mapruler.h"

#include <QGraphicsItem>
#include <QPainter>
#include <QColor>

int MapRuler::padding = 24;
int MapRuler::thickness = 3;


void MapRuler::init() {
    setVisible(false);
    setEnabled(false);
    setPoints(QPoint(), QPoint());
    xRuler = QRect();
    yRuler = QRect();
    widthTextBox = QRect();
    heightTextBox = QRect();
}

QRectF MapRuler::boundingRect() const {
    return QRectF(-padding, -padding, pixWidth() + padding * 2, pixHeight() + padding * 2);
}

QPainterPath MapRuler::shape() const {
    QPainterPath ruler;
    ruler.setFillRule(Qt::WindingFill);
    ruler.addRect(xRuler);
    ruler.addRect(yRuler);
    ruler = ruler.simplified();
    return ruler;
}

void MapRuler::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setPen(exteriorColor);
    painter->setBrush(QBrush(interiorColor));
    painter->drawPath(shape());
    painter->drawText(widthTextBox, Qt::AlignCenter, QString("%1").arg(width()));
    painter->drawText(heightTextBox, Qt::AlignCenter, QString("%1").arg(height()));
}

void MapRuler::setAnchor(const QPoint &tilePos) {
    setEnabled(true);
    setPoints(tilePos, tilePos);
    updateShape();
    setVisible(true);
}

void MapRuler::endAnchor() {
    setVisible(false);
    setEnabled(false);
    prepareGeometryChange();
    init();
}

void MapRuler::setEndPos(const QPoint &tilePos) {
    if (!isEnabled() || (tilePos == endPos() && tilePos != anchor()))
        return;
    prepareGeometryChange();
    setP2(tilePos);
    updateShape();
}

void MapRuler::updateShape() {
    setPos(QPoint(left() * 16 + 7, top() * 16 + 7));
    /* Determines what quadrant the ruler exists in, relative to the anchor point. The anchor point
     * is the top-left corner of the metatile the ruler starts in, so a zero-length(s) ruler is
     * treated as being in the bottom-right quadrant from the anchor point. */
    if (deltaX() < 0 && deltaY() < 0) {
        // Top-left
        xRuler = QRect(0, pixHeight(), pixWidth() + thickness, thickness);
        yRuler = QRect(0, 0, thickness, pixHeight() + thickness);
        widthTextBox = QRect(0, pixHeight(), pixWidth(), padding);
        heightTextBox = QRect(-padding, 0, padding, pixHeight());
    } else if (deltaX() < 0) {
        // Bottom-left
        xRuler = QRect(0, 0, pixWidth() + thickness, thickness);
        yRuler = QRect(0, 0, thickness, pixHeight() + thickness);
        widthTextBox = QRect(0, -padding, pixWidth(), padding);
        heightTextBox = QRect(-padding, 0, padding, pixHeight());
    } else if (deltaY() < 0) {
        // Top-right
        xRuler = QRect(0, pixHeight(), pixWidth() + thickness, thickness);
        yRuler = QRect(pixWidth(), 0, thickness, pixHeight() + thickness);
        widthTextBox = QRect(0, pixHeight(), pixWidth(), padding);
        heightTextBox = QRect(pixWidth(), 0, padding, pixHeight());
    } else {
        // Bottom-right
        xRuler = QRect(0, 0, pixWidth() + thickness, thickness);
        yRuler = QRect(pixWidth(), 0, thickness, pixHeight() + thickness);
        widthTextBox = QRect(0, -padding, pixWidth(), padding);
        heightTextBox = QRect(pixWidth(), 0, padding, pixHeight());
    }
}
