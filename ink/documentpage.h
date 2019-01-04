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
#ifndef DOCUMENTPAGE_H
#define DOCUMENTPAGE_H

#include <QObject>
#include <QGraphicsScene>

struct DocumentPagePrivate;

class DocumentPage : public QGraphicsScene
{
        Q_OBJECT
    public:
        explicit DocumentPage(QObject *parent = nullptr);
        ~DocumentPage();

        QGraphicsItem* pageRect();
    signals:

    public slots:
        void setPageSize(int width, int height);

    private:
        DocumentPagePrivate* d;
};

#endif // DOCUMENTPAGE_H
