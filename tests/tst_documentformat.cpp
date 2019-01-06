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
#include <QtTest>
#include <QCoreApplication>

#include "../ink/documentformat.h"
#include "../ink/documentformat.cpp"

class DocumentFormatTest : public QObject
{
        Q_OBJECT

    private slots:
        void encode();
        void encode_data();

};

void DocumentFormatTest::encode_data() {
    QTest::addColumn<qreal>("encode");
    QTest::addColumn<QString>("result");

    QTest::newRow("Zero case")   << 0.0                          << "";
    QTest::newRow("Test Case 1") << 1.209                        << "S5";
    QTest::newRow("Test Case 2") << 5.9381627                    << "Bcy";
    QTest::newRow("Test Case 3") << 12.0                         << "C7g";
    QTest::newRow("Test Case 4") << 681762581342986.348193746551 << "l2G3SiGiCA";
    QTest::newRow("Test Case 5") << 68719476.736                 << "BAAAAAA";
}

void DocumentFormatTest::encode() {
    QFETCH(qreal, encode);
    QFETCH(QString, result);

    QCOMPARE(DocumentFormat::encodeFloat(encode), result);
}

QTEST_MAIN(DocumentFormatTest)

#include "tst_documentformat.moc"
