
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

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTabWidget>
#include "../output.h"
#include "plotfunctiondialog.h"


PlotFunctionDialog::PlotFunctionDialog(Canvas2D *parent) :
    QDialog(parent)
{
    context=parent->getContext();
    initGui();
}
void PlotFunctionDialog::initGui(){
    setWindowTitle(tr("Définir une courbe représentative"));

    QVBoxLayout * vbox=new QVBoxLayout;
    tabWidget=new QTabWidget;

    cartesianPanel=new QWidget;
    QHBoxLayout* boxCartesian=new QHBoxLayout;
    QLabel* labelF=new QLabel("f(x)=");
    editF=new QLineEdit;
    boxCartesian->addWidget(labelF);
    boxCartesian->addWidget(editF);
    cartesianPanel->setLayout(boxCartesian);

    polarPanel=new QWidget;
    QHBoxLayout* boxPolar=new QHBoxLayout;
    QLabel* labelR=new QLabel(QString("%1(t)=").arg(QChar(0x03c1)));
    editR=new QLineEdit;
    boxPolar->addWidget(labelR);
    boxPolar->addWidget(editR);
    polarPanel->setLayout(boxPolar);

    implicitPanel=new QWidget;
    QHBoxLayout* boxImplicit=new QHBoxLayout;
    QLabel* labelE=new QLabel("f(x,y)=");
    editE=new QLineEdit;
    boxImplicit->addWidget(labelE);
    boxImplicit->addWidget(editE);
    /*   QLabel* labelXmin=new QLabel(tr("x min="));
    QLabel* labelXmax=new QLabel(tr("x max="));
    editXmin=new QLineEdit;
    editXmax=new QLineEdit;
    boxImplicit->addWidget(labelXmin);
    boxImplicit->addWidget(editXmin);
    boxImplicit->addWidget(labelXmax);
    boxImplicit->addWidget(editXmax);
    */ 
   implicitPanel->setLayout(boxImplicit);

    parametricPanel=new QWidget;
    QGridLayout* grid=new QGridLayout;
    QLabel* labelX=new QLabel(tr("x(t)="));
    QLabel* labelY=new QLabel(tr("y(t)="));
    editX=new QLineEdit;
    editY=new QLineEdit;
    grid->addWidget(labelX,0,0);
    grid->addWidget(editX,0,1);
    grid->addWidget(labelY,1,0);
    grid->addWidget(editY,1,1);
    parametricPanel->setLayout(grid);

    tabWidget->addTab(cartesianPanel,tr("Cartésienne"));
    tabWidget->addTab(polarPanel,tr("Polaire"));
    tabWidget->addTab(implicitPanel,tr("Implicite"));
    tabWidget->addTab(parametricPanel,tr("Paramétrique"));


    QGroupBox * rangePanel=new QGroupBox;
    rangePanel->setTitle(tr("Bornes"));
    QHBoxLayout * hbox=new QHBoxLayout;
    QLabel* labMin=new QLabel(tr("Min:"));
    QLabel* labMax=new QLabel(tr("Max:"));
    editMin=new QLineEdit;
    editMax=new QLineEdit;
    hbox->addWidget(labMin);
    hbox->addWidget(editMin);
    hbox->addWidget(labMax);
    hbox->addWidget(editMax);
    rangePanel->setLayout(hbox);


    QWidget * buttonPanel=new QWidget;
    QHBoxLayout * hbox2=new QHBoxLayout;
    ok=new QPushButton(tr("Ok"));
    cancel=new QPushButton(tr("Annuler"));
    hbox2->addWidget(ok);
    hbox2->addWidget(cancel);
    buttonPanel->setLayout(hbox2);

    vbox->addWidget(tabWidget);
    vbox->addWidget(rangePanel);
    vbox->addWidget(buttonPanel);


    setLayout(vbox);

    vbox->setSizeConstraint(QLayout::SetFixedSize);
    connect(ok,SIGNAL(clicked()),this,SLOT(closeDialog()));
    connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
    editF->setFocus();
}
QString PlotFunctionDialog::getString()const{
    return command;
}
void PlotFunctionDialog::closeDialog(){
    command.clear();
    switch(tabWidget->currentIndex()){
        // cartesian
        case 0:{
        if (!checkFunction(editF)) reject();
        command.append("plot(");
        command.append(editF->text());
        command.append(",x=");
        command.append(editMin->text());
        command.append("..");
        command.append(editMax->text());
        command.append(");");
    }
        break;
        // polar
        case 1:{
        if (!checkFunction(editR)) reject();
        command.append("polarplot(");
        command.append(editR->text());
        command.append(",t=");
        command.append(editMin->text());
        command.append("..");
        command.append(editMax->text());
        command.append(");");
    }
        break;
        // implicit
        case 2:{
        if (!checkFunction(editE)) reject();
        command.append("plotimplicit(");
        command.append(editE->text());
        command.append(",x=");
        command.append(editMin->text());
        command.append("..");
        command.append(editMax->text());
        command.append(",y=");
        command.append(editMin->text());
        command.append("..");
        command.append(editMax->text());
        command.append(");");
    }
        break;
        // parametric
    default:{
        if (!checkFunction(editX)) reject();
        if (!checkFunction(editY)) reject();
        command.append("plotparam([");
        command.append(editX->text());
        command.append(",");
        command.append(editY->text());
        command.append("],t=");
        command.append(editMin->text());
        command.append("..");
        command.append(editMax->text());
        command.append(");");
        }
    }
    accept();
}
bool PlotFunctionDialog::checkFunction(QLineEdit* ed){
    giac::gen func(ed->text().toStdString(),context);
    if (func.type==giac::_SYMB) return true;
    else return false;
}
