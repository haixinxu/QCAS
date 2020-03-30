/*  Copyright (C) 2012 Le Coq Loïc
*    Initial version 2012 by Le Coq Loïc
*    Later modifications by Han Frederic
*    This file is part of QCAS.
*
*    QCAS is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    QCAS is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with Foobar.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef WIZARDCATALOG_H
#define WIZARDCATALOG_H
#include <QWidget>
#include <QUrl>
class QLineEdit;
class QTextBrowser;
class QStringList;
class MainWindow;
class QAction;

class WizardCatalog :public QWidget{
    Q_OBJECT
public:
    WizardCatalog(MainWindow *parent=0);
    void displayPage(QUrl);
    void displayHome();
private:
    void changeEvent(QEvent *);
    void retranslate();
    void createGui();
    void addHistory(const QString &st);
    void updateButtons();
    MainWindow* mainWindow;
    QStringList *history;
    int historyIndex;
    QLineEdit* lineEdit;
    QTextBrowser *zone;
    QAction* previousAction;
    QAction* nextAction;
    QAction *findAction;
    QAction *homeAction;

public slots:
   void newPage(QUrl);

private slots:
    void find();
    void home();
    void goBack();
    void goNext();
};


#endif // WIZARDCATALOG_H
