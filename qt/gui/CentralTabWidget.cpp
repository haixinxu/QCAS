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


#include <QTabBar>
#include <QDebug>
#include <QToolButton>
#include <QMenu>
#include <QLabel>
#include <QMessageBox>
#include "MainWindow.h"
#include "FormalSheet.h"
#include "Interactive2d.h"
#include "CentralTabWidget.h"
#include "output.h"

void MainTabWidget::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}
void MainTabWidget::retranslate(){
    formalAction->setText(tr("Calcul formel"));
    spreadsheetAction->setText(tr("Tableur"));
    programmingAction->setText(tr("Editeur de programmes"));
    g2dAction->setText(tr("Géométrie 2D"));

}
MainTabWidget::MainTabWidget(MainWindow *main):QTabWidget(){
    mainWindow=main;
    setTabsClosable(true);
    setMovable(true);
    setIconSize(QSize(20,20));
    add=new QToolButton;
    add->setIcon(QIcon(":/images/add.png"));
    add->setPopupMode(QToolButton::InstantPopup);

    formalAction=new QAction(tr("Calcul formel"),add);
    formalAction->setIcon(QIcon(":/images/formal.png"));
    spreadsheetAction=new QAction(tr("Tableur"),add);
    spreadsheetAction->setIcon(QIcon(":/images/spreadsheet.png"));
    programmingAction=new QAction(tr("Editeur de programmes"),add);
    programmingAction->setIcon(QIcon(":/images/programming.png"));
    g2dAction=new QAction(tr("Géométrie 2D"),add);
    g2dAction->setIcon(QIcon(":/images/function.png"));


    menu=new QMenu;
    menu->addAction(formalAction);
  //  menu->addAction(spreadsheetAction);
  //  menu->addAction(programmingAction);
    menu->addAction(g2dAction);
    add->setMenu(menu);

    connect(formalAction,SIGNAL(triggered()),this,SLOT(addFormalSheet()));
    connect(spreadsheetAction,SIGNAL(triggered()),this,SLOT(addSpreadSheet()));
    connect(programmingAction,SIGNAL(triggered()),this,SLOT(addProgrammingSheet()));
    connect(g2dAction,SIGNAL(triggered()),this,SLOT(addG2dSheet()));

    addTab(new FormalWorkSheet(mainWindow),QIcon(":/images/formal.png"),tr("Feuille n°1"));
    addTab(new QLabel(""),"");
    tabBar()->setTabButton(1,QTabBar::RightSide,add);

    connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    connect(this,SIGNAL(currentChanged(int)),this,SLOT(changeTab(int)));
//    setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
//    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);


    retranslate();

}
void MainTabWidget::changeTab(int id){
    MainSheet* sheet=dynamic_cast<MainSheet*>(widget(id));
    if (sheet==0) return;
    mainWindow->updateInterface(sheet->getType());
}
void MainTabWidget::closeTab(int id){

  //
     if (count()-id==1) return;//on macosx the empty tab with the button shows a close button, so we prevent it from closing

     int r=QMessageBox::warning(this,tr("Confirmation"),tr("Vous allez supprimer cet objet ainsi \n que toutes ses dépendances. \n Voulez-vous poursuivre?"),QMessageBox::Yes|QMessageBox::Default,QMessageBox::Cancel|QMessageBox::Escape);
     if (r!=QMessageBox::Yes) return;

    closeTabwithoutWarning(id);

}

void MainTabWidget::closeTabwithoutWarning(int id){

  //
     if (count()-id==1) return;//on macosx the empty tab with the button shows a close button, so we prevent it from closing

    MainSheet* sheet=dynamic_cast<MainSheet*>(widget(id));
    switch(sheet->getType()){
    case MainSheet::FORMAL_TYPE:
        {FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(widget(id));
            this->removeTab(id);
            delete form;
        }
        break;
    case MainSheet::SPREADSHEET_TYPE:
        break;
    case MainSheet::PROGRAMMING_TYPE:
        break;
    case MainSheet::G2D_TYPE:{
        GraphWidget *graph=qobject_cast<GraphWidget*>(widget(id));
        removeTab(id);
         delete graph;
    }
        break;
    }
    for (int i=id;i<=count()-2;i++){
        tabBar()->setTabText(i,tr("Feuille n°")+QString::number(i+1));
    }


}

void MainTabWidget::insertG2dSheet(int i, const QString &titre){
    //qDebug()<<"i="<<i;
    if((i<count())&&(i>=0)){
    this->insertTab(i,new GraphWidget(mainWindow->getContext(),true,mainWindow),QIcon(":/images/g2d.png"),titre);
    this->setCurrentIndex(i);
    (qobject_cast<GraphWidget*>(widget(i)))->setFocus(Qt::OtherFocusReason);
    mainWindow->updateInterface(MainSheet::G2D_TYPE);
    }
}
void MainTabWidget::addG2dSheet(){
    this->insertTab(count()-1,new GraphWidget(mainWindow->getContext(),true,mainWindow),QIcon(":/images/g2d.png"),tr("Feuille n°")+QString::number(count()));
    this->setCurrentIndex(count()-2);
    (qobject_cast<GraphWidget*>(widget(count()-2)))->setFocus(Qt::OtherFocusReason);
    mainWindow->updateInterface(MainSheet::G2D_TYPE);
}
void MainTabWidget::addG2dSheet(giac::context * ct){
    giac::context * localcontext=new giac::context(*ct);  // a local copy of the context
    this->insertTab(count()-1,new GraphWidget(localcontext,false,mainWindow),QIcon(":/images/g2d.png"),tr("Feuille n°")+QString::number(count()));
    this->setCurrentIndex(count()-2);
    (qobject_cast<GraphWidget*>(widget(count()-2)))->setFocus(Qt::OtherFocusReason);
    mainWindow->updateInterface(MainSheet::G2D_TYPE);
}
void MainTabWidget::addG2dSheetfromLine(const QString  &ligne){
    this->insertTab(count()-1,new GraphWidget(mainWindow->getContext(),true,mainWindow),QIcon(":/images/g2d.png"),ligne.left(8)+tr(" n°")+QString::number(count()));
    this->setCurrentIndex(count()-2);
    (qobject_cast<GraphWidget*>(widget(count()-2)))->setFocus(Qt::OtherFocusReason);
    mainWindow->updateInterface(MainSheet::G2D_TYPE);
}
void MainTabWidget::addFormalSheet(){
    this->insertTab(count()-1,new FormalWorkSheet(mainWindow),QIcon(":/images/formal.png"),tr("Feuille n°")+QString::number(count()));
    this->setCurrentIndex(count()-2);
    (qobject_cast<FormalWorkSheet*>(widget(count()-2)))->setFocus(Qt::OtherFocusReason);
    mainWindow->updateInterface(MainSheet::FORMAL_TYPE);
}
void MainTabWidget::addSpreadSheet(){
    qDebug()<<"spreadsheet added";
}

void MainTabWidget::addProgrammingSheet(){
    qDebug()<<"programmaing editor added";
}
MainSheet::MainSheet(sheetType s){
    type=s;
}
MainSheet::sheetType MainSheet::getType()const{
return type;
}


