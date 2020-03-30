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
#ifndef CENTRALTABWIDGET_H
#define CENTRALTABWIDGET_H

#include <QTabWidget>

#include "../sizeof_void_p.h"
#include <giac/giac.h>

class QToolButton;
class QMenu;
class MainWindow;

class MainSheet{
public:
    enum sheetType{FORMAL_TYPE,G2D_TYPE,SPREADSHEET_TYPE,PROGRAMMING_TYPE};
    MainSheet(sheetType);
    sheetType getType() const;
protected:
    sheetType type;

};
class MainTabWidget:public QTabWidget{
    Q_OBJECT
public:
    MainTabWidget(MainWindow *main=0);
private:
    QToolButton *add;
    QMenu* menu;
    QAction* formalAction;
    QAction* spreadsheetAction;
    QAction* programmingAction;
    QAction* g2dAction;

    MainWindow *mainWindow;

    void changeEvent(QEvent *);
    void retranslate();

public slots:
    void addFormalSheet();
    void addSpreadSheet();
    void addProgrammingSheet();
    void addG2dSheet();
    void addG2dSheetfromLine(const QString &);
    void insertG2dSheet(int,const QString &);
    void addG2dSheet(giac::context *); //cf. giacpy
    void closeTab(int);
    void closeTabwithoutWarning(int);
    void changeTab(int);
};


#endif // CENTRALTABWIDGET_H
