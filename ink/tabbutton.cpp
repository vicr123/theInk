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
#include "tabbutton.h"
#include "documentview.h"

TabButton::TabButton(QWidget *parent) : QPushButton(parent)
{
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    this->setAutoExclusive(true);
}

TabButton::TabButton(DocumentView *editor, QWidget *parent) : QPushButton(parent)
{
    this->editor = editor;
}

void TabButton::setActive(bool active) {
    this->active = active;
    this->repaint();
}

void TabButton::updateIcon(QIcon icon) {
    this->setIcon(icon);
}

void TabButton::updateTitle(QString title) {
    this->setText(title);
}

void TabButton::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPalette pal = this->palette();
    QRect rect = event->rect();

    painter.setPen(Qt::transparent);

    QBrush brush;
    QPen textPen;

#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    brush = QBrush(pal.color(QPalette::Highlight));
#else
    brush = QBrush(pal.color(QPalette::Button));
#endif

    /*if (button->state & QStyle::State_HasFocus) {
        brush = QBrush(pal.color(QPalette::Button).lighter(125));
    }

    if (button->state & QStyle::State_MouseOver) {
        brush = QBrush(pal.color(QPalette::Button).lighter());
    }*/

    if (this->active) {
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
        brush = QBrush(pal.color(QPalette::Highlight).darker());
#elif defined(Q_OS_LINUX)
        brush = QBrush(pal.color(QPalette::Button).darker());
#endif
    }
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    textPen = pal.color(QPalette::HighlightedText);
#elif defined(Q_OS_LINUX)
    textPen = pal.color(QPalette::ButtonText);
#endif

    painter.setBrush(brush);
    painter.drawRect(rect);

    QString text = this->text();

    QRect textRect, iconRect;
    textRect.setLeft(rect.left() + (rect.width() / 2) - (this->fontMetrics().width(text) / 2));
    textRect.setWidth(this->fontMetrics().width(text));
    textRect.setTop(rect.top() + (rect.height() / 2) - (this->fontMetrics().height() / 2));
    textRect.setHeight(this->fontMetrics().height());

    if (!this->icon().isNull()) {
        int fullWidth = textRect.width() + this->iconSize().width();
        int iconLeft = rect.left() + (rect.width() / 2) - (fullWidth / 2);

        iconRect.setLeft(iconLeft);
        iconRect.setTop(rect.top() + (rect.height() / 2) - (this->iconSize().height() / 2));
        iconRect.setSize(this->iconSize());

        textRect.moveLeft(iconRect.right() + 4);

        QIcon icon = this->icon();
        QImage image = icon.pixmap(this->iconSize()).toImage();
        image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);

        painter.drawImage(iconRect, image);
    }

    //Draw text
    painter.setPen(textPen);
    painter.drawText(textRect, Qt::AlignCenter, text.remove("&"));
}

QSize TabButton::sizeHint() const {
    QSize size;
    size.setHeight(this->fontMetrics().height() + 20);
    //size.setWidth(this->fontMetrics().width(this->text()) + 20 + this->iconSize().width());
    size.setWidth(size.height());
    return size;
}
