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
#include "documentview.h"

#include <QMouseEvent>

struct DocumentViewPrivate {
    Document* loadedDoc;
    int pageNo;

    QPointF lastCoordinates;
};

DocumentView::DocumentView(QWidget *parent) : QGraphicsView(parent)
{
    d = new DocumentViewPrivate();
    setDocument(new Document());
}

DocumentView::~DocumentView() {
    delete d;
}

void DocumentView::setDocument(Document* doc) {
    d->loadedDoc = doc;
    setPageNumber(0);
}

Document* DocumentView::document() {
    return d->loadedDoc;
}

void DocumentView::setPageNumber(int pageNo) {
    d->pageNo = pageNo;
    this->setScene(d->loadedDoc->getPage(pageNo));
}

void DocumentView::resizeEvent(QResizeEvent *event) {
    float scaleFactor = (this->width() - 20) / this->sceneRect().width();
    QTransform transform;
    transform.scale(scaleFactor, scaleFactor);
    this->setTransform(transform);
}

void DocumentView::mousePressEvent(QMouseEvent *event) {
    d->lastCoordinates = event->pos();
}

void DocumentView::mouseMoveEvent(QMouseEvent *event) {
    //Draw a line
    QPen linePen;
    linePen.setColor(Qt::black);
    linePen.setCapStyle(Qt::RoundCap);
    linePen.setWidthF(0.5);

    QLineF line;
    line.setP1(this->mapToScene(d->lastCoordinates.toPoint()));
    line.setP2(this->mapToScene(event->pos()));

    this->scene()->addLine(line, linePen);

    d->lastCoordinates = event->pos();
}

void DocumentView::mouseReleaseEvent(QMouseEvent *event) {

}
