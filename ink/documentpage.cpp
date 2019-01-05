/****************************************
 *
 *   theInk - Journal
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

struct DocumentPagePrivate {
    QGraphicsRectItem* pageRect = nullptr;
};

DocumentPage::DocumentPage(QObject *parent) : QGraphicsScene(parent)
{
    d = new DocumentPagePrivate();

    //this->setSceneRect(0, 0, 210, 297);
    setPageSize(210, 297);
}

DocumentPage::~DocumentPage() {
    delete d;
}

void DocumentPage::setPageSize(int width, int height) {
    if (d->pageRect != nullptr) {
        this->removeItem(d->pageRect);
    }
    this->setSceneRect(0, 0, width, height);
    d->pageRect = this->addRect(this->sceneRect(), QColor(255, 0, 0), QColor(255, 255, 255));

}

QGraphicsItem* DocumentPage::pageRect() {
    return d->pageRect;
}

QJsonObject DocumentPage::save(QJsonArray& penDictionary) {
    QJsonObject data;
    data.insert("pageWidth", this->sceneRect().size().width());
    data.insert("pageHeight", this->sceneRect().size().height());

    const int lineType = QGraphicsLineItem().type();

    auto getPen = [=, &penDictionary](QPen pen) -> int {
        for (int i = 0; i < penDictionary.count(); i++) {
            QJsonObject penDef = penDictionary.at(i).toObject();
            QColor color = penDef.value("color").toVariant().value<QColor>();
            qreal width = penDef.value("width").toDouble();

            if (pen.color() == color && pen.widthF() == width) {
                return i;
            }
        }

        //No matching pen definition found, create a new one
        QJsonObject penDef;
        penDef.insert("color", QJsonValue::fromVariant(pen.color()));
        penDef.insert("width", pen.widthF());
        penDictionary.append(penDef);
        return penDictionary.count() - 1;
    };

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
            qreal lastX1 = -1, lastX2 = -1, lastY1 = -1, lastY2 = -1;
            if (lastItemData.value("type") == "line") {
                QJsonObject lastPoints = lastItemData.value("points").toObject();
                lastX1 = lastPoints.value("x1").toDouble();
                lastY1 = lastPoints.value("y1").toDouble();
                lastX2 = lastPoints.value("x2").toDouble();
                lastY2 = lastPoints.value("y2").toDouble();
            }

            if ((lastX1 == line->line().x1() && lastY1 == line->line().y1()) || (lastX2 == line->line().x1() && lastY2 == line->line().y1())) {
                //Save the second set normally because that's the data we don't have
                points.insert("x2", line->line().x2());
                points.insert("y2", line->line().y2());
            } else if ((lastX1 == line->line().x2() && lastY1 == line->line().y2()) || (lastX2 == line->line().x2() && lastY2 == line->line().y2())) {
                //Save the first set as the second set normally because that's the data we don't have
                points.insert("x2", line->line().x1());
                points.insert("y2", line->line().y1());
            } else {
                //Save everything normally
                points.insert("x1", line->line().x1());
                points.insert("y1", line->line().y1());
                points.insert("x2", line->line().x2());
                points.insert("y2", line->line().y2());
            }

            itemData.insert("points", points);

            itemData.insert("pen", getPen(line->pen()));
        } else {
            itemData.insert("type", "undefined");
        }
        pathData.append(itemData);
        lastItemData = itemData;
    }
    data.insert("pathData", pathData);

    return data;
}
