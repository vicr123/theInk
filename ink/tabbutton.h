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
#ifndef TABBUTTON_H
#define TABBUTTON_H

#include <QPushButton>
#include <QPaintEvent>
#include <QPainter>

class DocumentView;

class TabButton : public QPushButton
{
    Q_OBJECT
public:
    explicit TabButton(QWidget *parent = 0);
    explicit TabButton(DocumentView* editor, QWidget *parent = 0);

    void setActive(bool active);
signals:

public slots:

private slots:
    void updateTitle(QString title);
    void updateIcon(QIcon icon);

private:
    DocumentView* editor;
    bool active;

    void paintEvent(QPaintEvent* event);
    QSize sizeHint() const;
};

#endif // TABBUTTON_H
