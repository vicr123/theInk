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
#include "document.h"
#include <QDataStream>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

struct DocumentPrivate {
    QList<DocumentPage*> pages;
    QString title;
};

Document::Document(QObject *parent) : QObject(parent)
{
    d = new DocumentPrivate();
    d->pages.append(new DocumentPage());
    d->title = tr("Untitled Notebook");
}

Document::~Document() {
    delete d;
}

QString Document::title() {
    return d->title;
}

DocumentPage* Document::getPage(int page) {
    return d->pages.at(page);
}

QByteArray Document::save() {
    QJsonObject rootObj;
    rootObj.insert("version", 0);


    QJsonArray pages, penDict;
    for (DocumentPage* p : d->pages) {
        pages.append(p->save(penDict));
    }
    rootObj.insert("pages", pages);
    rootObj.insert("pens", penDict);

    QJsonDocument doc(rootObj);
    QByteArray data = doc.toJson(QJsonDocument::Compact);


    return data;
}
