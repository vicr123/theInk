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
#include <QScroller>
#include <QEventLoop>
#include <QFileDialog>
#include "tabbutton.h"

struct DocumentViewPrivate {
    Document* loadedDoc = nullptr;
    TabButton* tabButton;
    QString currentFile;
    int pageNo;

    bool tabletListening = false;
    bool active = false;

    QPointF lastCoordinates;
    DocumentView::Tools currentTool = DocumentView::Pen;
};

DocumentView::DocumentView(QWidget *parent) : QGraphicsView(parent)
{
    d = new DocumentViewPrivate();

    d->tabButton = new TabButton(this);

    setDocument(new Document());
    this->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);

    QScroller::grabGesture(this->viewport(), QScroller::TouchGesture);
}

DocumentView::~DocumentView() {
    if (d->tabButton != nullptr) {
        d->tabButton->setVisible(false);
        d->tabButton->deleteLater();
    }
    delete d;
}

void DocumentView::setDocument(Document* doc) {
    if (d->loadedDoc != nullptr) {
        disconnect(d->loadedDoc);
        d->loadedDoc->deleteLater();
    }

    d->loadedDoc = doc;
    tabButton()->setText(d->loadedDoc->title());

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
    if (event->source() == Qt::MouseEventSynthesizedByQt) return;
    d->lastCoordinates = event->pos();
}

void DocumentView::mouseMoveEvent(QMouseEvent *event) {
    if (event->source() == Qt::MouseEventSynthesizedByQt) return;
    if (d->currentTool == Pen) {
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
}

void DocumentView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->source() == Qt::MouseEventSynthesizedByQt) return;

}

void DocumentView::tabletEvent(QTabletEvent *event) {
    if (event->type() == QEvent::TabletPress) {
        d->tabletListening = true;
        d->lastCoordinates = event->posF();

        switch (event->pointerType()) {
            case QTabletEvent::Pen:
            case QTabletEvent::Cursor:
            case QTabletEvent::UnknownPointer:
                d->currentTool = DocumentView::Pen;
                break;
            case QTabletEvent::Eraser:
                d->currentTool = DocumentView::Eraser;
        }
    } else if (event->type() == QEvent::TabletMove) {
        if (d->currentTool == Pen) {
            //Draw a line
            QPen linePen;
            linePen.setColor(Qt::black);
            linePen.setCapStyle(Qt::RoundCap);
            linePen.setWidthF(0.5);

            QLineF line;
            line.setP1(this->mapToScene(d->lastCoordinates.toPoint()));
            line.setP2(this->mapToScene(event->pos()));

            this->scene()->addLine(line, linePen);

            d->lastCoordinates = event->posF();
        } else if (d->currentTool == Eraser) {
            QRectF searchRect;
            searchRect.setTopLeft(this->mapToScene(event->pos()) - QPointF(0.5, 0.5));
            searchRect.setWidth(1);
            searchRect.setHeight(1);
            for (QGraphicsItem* item : this->scene()->items(searchRect, Qt::IntersectsItemShape, Qt::AscendingOrder, this->transform())) {
                if (item == ((DocumentPage*) this->scene())->pageRect()) continue;
                this->scene()->removeItem(item);
            }
        }
    } else if (event->type() == QEvent::TabletRelease) {
        d->tabletListening = false;
    }
    event->accept();
}

void DocumentView::touchEvent(QTouchEvent *event) {
    event->accept();
}

TabButton* DocumentView::tabButton() {
    return d->tabButton;
}

void DocumentView::setActive(bool active) {
    d->tabButton->setActive(active);
    d->active = active;
}

bool DocumentView::save() {
    if (d->currentFile == "") {
        return saveAs();
    }

    //Attempt to save the current document
    QFile file(d->currentFile);

    if (!file.open(QFile::WriteOnly)) {
        if (!(file.permissions() & QFile::WriteUser)) {
            //error = "Permissions";
        } else {
            //error = "Can't open file";
        }
        return false;
    }

    QByteArray data = d->loadedDoc->save();
    if (d->currentFile.endsWith(".tinz")) {
        data = qCompress(data, 9);
    }

    qint64 written = file.write(data);
    if (written == -1) {
        return false;
    }

    file.close();
    return true;
}

bool DocumentView::saveAs() {
    QEventLoop* loop = new QEventLoop();
    QFileDialog* saveDialog = new QFileDialog(this->window(), Qt::Sheet);
    saveDialog->setWindowModality(Qt::WindowModal);
    saveDialog->setAcceptMode(QFileDialog::AcceptSave);
    saveDialog->setDirectory(QDir::home());
    saveDialog->setNameFilters(QStringList() << "Compressed theInk Notebook (*.tinz)"
                                             << "theInk Notebook (*.tink)"
                                             << "All Files (*)");
    connect(saveDialog, SIGNAL(finished(int)), saveDialog, SLOT(deleteLater()));
    connect(saveDialog, SIGNAL(finished(int)), loop, SLOT(quit()));
    saveDialog->show();

    //Block until dialog is finished
    loop->exec();
    loop->deleteLater();

    if (saveDialog->result() == QDialog::Accepted) {
        d->currentFile = saveDialog->selectedFiles().first();
        return save();
    }
    return false;
}
