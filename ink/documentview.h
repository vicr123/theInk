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
#ifndef DOCUMENTVIEW_H
#define DOCUMENTVIEW_H

#include <QWidget>
#include <QTouchEvent>
#include <QGraphicsView>
#include "document.h"

struct DocumentViewPrivate;
class TabButton;

class DocumentView : public QGraphicsView
{
        Q_OBJECT
    public:
        explicit DocumentView(QWidget *parent = nullptr);
        ~DocumentView();

        enum Tools {
            Pen,
            Eraser
        };

        Document* document();
        TabButton* tabButton();

    signals:

    public slots:
        void setDocument(Document* doc);
        void setPageNumber(int pageNo);
        void setActive(bool active);

        bool save();
        bool saveAs();

    private:
        DocumentViewPrivate* d;

        void resizeEvent(QResizeEvent* event);
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
        void tabletEvent(QTabletEvent* event);
        void touchEvent(QTouchEvent* event);
};

#endif // DOCUMENTVIEW_H
