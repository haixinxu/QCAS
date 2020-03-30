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

#ifndef INTERACTIVE2D_H
#define INTERACTIVE2D_H
#include <QObject>
#include <QStyle>
#if QT_VERSION < 0x050000
    #include <QWindowsStyle>
#else
    #include <QProxyStyle>
#endif
#include "../output.h"
#include <QWidget>
#include "gui/CentralTabWidget.h"
class MainWindow;
class QToolBar;
class QPushButton;
class QToolButton;
class QAction;
class MainWindow;

class Interactive2d:public QWidget,MainSheet{
    Q_OBJECT
public:
Interactive2d(MainWindow *parent=0);
private:
    MainWindow* mainWindow;
    QToolBar* toolBar;
    GraphWidget* canvas;
    QPushButton* buttonPt;
    QToolButton* buttonLine;
    QToolButton* buttonCircle;
    QAction* singlept;
    QAction * inter;
    QAction* midpoint;
    QAction* line;
    QAction* halfline;
    QAction* segment;
    QAction* circle2pt;
    QAction* circle3pt;


    void initGui();

};


#endif // INTERACTIVE2D_H
