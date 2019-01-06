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
#include "documentformat.h"

#include <QJsonObject>
#include <QVariant>
#include <QColor>
#include <math.h>

DocumentFormat::DocumentFormat()
{

}

int DocumentFormat::getPen(QPen pen, QJsonArray& penDictionary) {
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
}

QString DocumentFormat::encodeFloat(qreal point) {
    qint64 encodedNumber = qRound64(point * 1000);

    //Now create the representation in base64
    QString retval;
    bool firstDigitCreated = false;
    for (int i = 10; i >= 0; i--) {
        //Obtain the current column
        qint64 b64 = 1;
        for (int j = 0; j < i; j++) {
            b64 *= 64;
        }

        qint64 numberToEncode = encodedNumber / b64;
        encodedNumber %= b64;
        if (numberToEncode == 0) {
            if (firstDigitCreated) {
                retval.append('A');
            }
        } else {
            firstDigitCreated = true;
            if (numberToEncode <= 25) {
                // Capital B-Z
                retval.append(QChar((ushort) (0x41 /* A */ + numberToEncode)));
            } else if (numberToEncode <= 51) {
                // Lowercase a-z
                retval.append(QChar((ushort) (0x61 /* a */ + numberToEncode - 26)));
            } else if (numberToEncode <= 61) {
                // Numeric 0-9
                retval.append(QChar((ushort) (0x30 /* 0 */ + numberToEncode - 52)));
            } else if (numberToEncode == 62) {
                // +
                retval.append('+');
            } else {
                // /
                retval.append('/');
            }
        }
    }

    return retval;
}

qreal DocumentFormat::decodeFloat(QString point) {
    return 0;
}
