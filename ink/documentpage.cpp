/****************************************
 *
 *   theInk - Interactive Notebook
 *   Copyright (C) 2019 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#include "documentpage.h"

#include <QGraphicsRectItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QPainter>
#include "documentformat.h"

struct DocumentPagePrivate {
    QGraphicsRectItem* pageRect = nullptr;
    DocumentPage::PageBackground pageBackground;

    int mmScaleFactor;
};

DocumentPage::DocumentPage(QObject *parent) : QGraphicsScene(parent)
{
    d = new DocumentPagePrivate();

    //this->setSceneRect(0, 0, 210, 297);
    d->mmScaleFactor = 4;
    setPageSize(210, 297);
    setPageBackground(Lined);
}

DocumentPage::~DocumentPage() {
    delete d;
}

void DocumentPage::setPageSize(int width, int height) {
    this->setSceneRect(0, 0, width * mmScaleFactor(), height * mmScaleFactor());
    updateBackground();
}

void DocumentPage::setPagePixelSize(int width, int height) {
    this->setSceneRect(0, 0, width, height);
    updateBackground();
}

QGraphicsItem* DocumentPage::pageRect() {
    return d->pageRect;
}

void DocumentPage::setPageBackground(PageBackground background) {
    d->pageBackground = background;
    updateBackground();
}

void DocumentPage::updateBackground() {
    if (d->pageRect != nullptr) {
        this->removeItem(d->pageRect);
    }


    switch (d->pageBackground) {
        case Lined: {
            QPixmap linedBackground(this->sceneRect().size().toSize());
            linedBackground.fill(Qt::white);

            QPainter painter(&linedBackground);

            //Draw lines
            painter.setPen(QPen(QColor(0, 200, 255, 200), 1));
            for (int i = 20 * mmScaleFactor(); i < linedBackground.height(); i += 8 * mmScaleFactor()) {
                painter.drawLine(0, i, linedBackground.width(), i);
            }

            //Draw margin
            painter.setPen(QPen(QColor(255, 100, 150, 200), 1));
            painter.drawLine(20 * mmScaleFactor(), 0, 20 * mmScaleFactor(), linedBackground.height());

            d->pageRect = this->addRect(this->sceneRect(), QColor(255, 0, 0), linedBackground);
            break;
        }
        case None:
            d->pageRect = this->addRect(this->sceneRect(), QColor(255, 0, 0), QColor(255, 255, 255));
    }
}

QJsonObject DocumentPage::save(QJsonArray& penDictionary) {
    QJsonObject data;
    data.insert("pageWidth", this->sceneRect().size().width());
    data.insert("pageHeight", this->sceneRect().size().height());
    data.insert("pageBackground", "lined");
    data.insert("mmScaleFactor", d->mmScaleFactor);

    const int lineType = QGraphicsLineItem().type();

    QJsonArray pathData;
    QJsonObject lastItemData;
    for (QGraphicsItem* i : this->items()) {
        if (i == d->pageRect) continue;
        QJsonObject itemData;
        if (i->type() == lineType) {
            //Line
            QGraphicsLineItem* line = qgraphicsitem_cast<QGraphicsLineItem*>(i);
            itemData.insert("type", "line");

            QJsonObject points;
            QString x1 = DocumentFormat::encodeFloat(line->line().x1());
            QString y1 = DocumentFormat::encodeFloat(line->line().y1());
            QString x2 = DocumentFormat::encodeFloat(line->line().x2());
            QString y2 = DocumentFormat::encodeFloat(line->line().y2());

            QString lastX1 = "", lastX2 = "", lastY1 = "", lastY2 = "";
            if (lastItemData.value("type") == "line") {
                QJsonObject lastPoints = lastItemData.value("points").toObject();
                lastX1 = lastPoints.value("x1").toString();
                lastY1 = lastPoints.value("y1").toString();
                lastX2 = lastPoints.value("x2").toString();
                lastY2 = lastPoints.value("y2").toString();
            }

            if ((lastX1 == x1 && lastY1 == y1) || (lastX2 == x1 && lastY2 == y1)) {
                //Save the second set normally because that's the data we don't have
                points.insert("x2", x2);
                points.insert("y2", y2);
            } else if ((lastX1 == x2 && lastY1 == y2) || (lastX2 == x2 && lastY2 == y2)) {
                //Save the first set as the second set normally because that's the data we don't have
                points.insert("x2", x1);
                points.insert("y2", y1);
            } else {
                //Save everything normally
                points.insert("x1", x1);
                points.insert("y1", y1);
                points.insert("x2", x2);
                points.insert("y2", y2);
            }

            itemData.insert("points", points);

            itemData.insert("pen", DocumentFormat::getPen(line->pen(), penDictionary));
        } else {
            itemData.insert("type", "undefined");
        }
        pathData.append(itemData);
        lastItemData = itemData;
    }
    data.insert("pathData", pathData);

    return data;
}

int DocumentPage::mmScaleFactor() {
    return d->mmScaleFactor;
}
