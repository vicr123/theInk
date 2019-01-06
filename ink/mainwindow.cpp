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
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "documentview.h"
#include "tabbutton.h"

struct MainWindowPrivate {
    QList<DocumentView*> documents;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    d = new MainWindowPrivate();
    ui->setupUi(this);

    newTab();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete d;
}

void MainWindow::newTab() {
    DocumentView* v = new DocumentView();

    ui->docStack->addWidget(v);
    ui->docStack->setCurrentWidget(v);

    connect(v->tabButton(), &TabButton::clicked, [=]{
        ui->docStack->setCurrentWidget(v);
    });
    ui->tabButtons->addWidget(v->tabButton());

    d->documents.append(v);
}

void MainWindow::on_actionNew_triggered()
{
    newTab();
}

void MainWindow::on_docStack_currentChanged(int arg1)
{
    for (int i = 0; i < ui->docStack->count(); i++) {
        DocumentView* item = (DocumentView*) ui->docStack->widget(i);
        item->setActive(false);
    }

    DocumentView* current = (DocumentView*) ui->docStack->widget(arg1);
    if (current != nullptr) {
        current->setActive(true);
    }
}

bool MainWindow::closeCurrentTab() {
    DocumentView* current = currentDocument();

    ui->tabButtons->removeWidget(current->tabButton());
    ui->docStack->removeWidget(current);
    current->deleteLater();

    if (ui->docStack->count() == 0) {
        ui->closeButton->setVisible(false);
        //ui->actionSave->setEnabled(false);
        ui->actionClose->setEnabled(false);
    }
    return true;
}

void MainWindow::on_closeButton_clicked()
{
    closeCurrentTab();
}

DocumentView* MainWindow::currentDocument() {
    return (DocumentView*) ui->docStack->currentWidget();
}

void MainWindow::on_actionClose_triggered()
{
    closeCurrentTab();
}

void MainWindow::on_actionSave_triggered()
{
    currentDocument()->save();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    while (ui->docStack->count() > 0) closeCurrentTab();
}

void MainWindow::on_actionSaveAs_triggered()
{
    currentDocument()->saveAs();
}
