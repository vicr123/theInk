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
