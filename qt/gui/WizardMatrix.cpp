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

#include <QGroupBox>
#include <QSpinBox>
#include <iostream>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "WizardMatrix.h"

WizardMatrix::WizardMatrix(MainWindow *parent)
    : QWidget(parent)
{
    mainWindow=parent;
    createGui();
}
void WizardMatrix::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}
void WizardMatrix::retranslate(){
    box->setTitle(tr("Dimensions:"));
    rowLabel->setText(tr("Lignes:"));
    columnLabel->setText(tr("Colonnes:"));
}
void WizardMatrix::createGui(){
    box=new QGroupBox;
    rowLabel=new QLabel(box);
    columnLabel=new QLabel(box);


    rowSpin=new QSpinBox(this);
    rowSpin->setMaximum(10);
    rowSpin->setMinimum(1);
    rowSpin->setValue(2);

    columnSpin=new QSpinBox(this);
    columnSpin->setMaximum(10);
    columnSpin->setMinimum(1);
    columnSpin->setValue(2);

    QGridLayout *grid=new QGridLayout;
    grid->addWidget(rowLabel,0,0,Qt::AlignLeft);
    grid->addWidget(columnLabel,1,0,Qt::AlignLeft);
    grid->addWidget(rowSpin,0,1,Qt::AlignLeft);
    grid->addWidget(columnSpin,1,1,Qt::AlignLeft);

    QPushButton* button=new QPushButton;
    button->setIcon(QIcon(":/images/right.png"));

    QPushButton* helpbutton=new QPushButton;
    helpbutton->setIcon(QIcon(":/images/ampoule.png"));


    QHBoxLayout *hLayout=new QHBoxLayout;
    hLayout->addWidget(box);
    hLayout->addWidget(helpbutton);
    hLayout->addWidget(button);

    table=new QTableWidget;
    table->setRowCount(0);
    table->setColumnCount(0);

    table->setRowCount(2);
    table->setColumnCount(2);;
    QHBoxLayout *bottom=new QHBoxLayout;
    table->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    bottom->addWidget(table);

    QVBoxLayout *vLayout=new QVBoxLayout;
    box->setLayout(grid);
    vLayout->addLayout(hLayout);
    vLayout->addLayout(bottom);

    this->setLayout(vLayout);

    connect(rowSpin,SIGNAL(valueChanged(int)),this,SLOT(modifyRow(int)));
    connect(columnSpin,SIGNAL(valueChanged(int)),this,SLOT(modifyColumn(int)));
    connect(button,SIGNAL(clicked()),this,SLOT(writeMatrix()));
    connect(helpbutton,SIGNAL(clicked()),this,SLOT(helpMatrix()));


    retranslate();

    show();
}

void WizardMatrix::modifyRow(int row){
    if (row>table->rowCount()){
        table->insertRow(row-1);
    }
    else {
        table->removeRow(row);
    }
}
void WizardMatrix::modifyColumn(int col){
    if (col>table->columnCount()){
        table->insertColumn(col-1);
    }
    else {
        table->removeColumn(col);
    }
}

void WizardMatrix::writeMatrix(){
    QString s("[");
    for (int i=0;i<table->rowCount();++i){
        s.append("[");
        for(int j=0;j<table->columnCount();++j){
            if (table->item(i,j)!=0)
                s.append(table->item(i,j)->text());
            if (j!=table->columnCount()-1){
                s.append(",");
            }
        }
        s.append("]");
        if (i!=table->rowCount()-1){
            s.append(",");
        }
    }
    s.append("]");
    mainWindow->sendText(s);
}

void WizardMatrix::helpMatrix(){
    QMessageBox::about(this,tr("Astuces"),tr("<center><h2>Vecteurs</h2></center>"
                                                      "<hr>"
                                             "<ul>Dans Giac/Xcas, les vecteurs sont donnés par leurs coordonnées séparées par des virgules et entre crochets."
                                             "Ci dessous v1,v2,v3 sont des vecteurs. v1 et v2 sont identiques."
                                             "<li><b>Exemple1:</b> <code>v1:=[0,1,2,3,4];</code></li>"
                                             "<li><b>Exemple2:</b> <code>v2:=[seq(j,j=0..4)];</code></li>"
                                             "<li><b>Exemple3:</b> <code>v3:=v1+2*v2;</code>"
                                             "</ul>"
                                             "<center><h2>Matrices</h2></center>"
                                             "<ul>Une matrice est donnée par la liste de ses lignes entre crochets. Chaque ligne est entre crochets séparée par des virgules."
                                             "Ci dessous les matrices m1 et m2 sont identiques et ont 2 lignes 5 colonnes."
                                             "<li><b>Exemple1:</b> <code>m1:=[[0,1,2,3,4],[0,3,6,9,12]];</code></li>"
                                             "<li><b>Exemple2:</b> <code>m2:=[v1,v3];</code></li>"
                                             "</ul><ul>Les matrices m3 et m4 sont identiques et sont des matrices 1 ligne 5 colonnes."
                                             "Elles diffèrent de m5 qui est une matrice 1 colonne et 5 lignes. Aucune des matrices mi n'est un vecteur. v4 est le vecteur [30,90]"

                                             "<li><b>Exemple3:</b> <code>m3:=[[0,1,2,3,4]]</code></li>"
                                             "<li><b>Exemple4:</b> <code>m4:=[v1];</code></li>"
                                             "<li><b>Exemple5:</b> <code>m5:=[[0],[1],[2],[3],[4]];</code></li>"
                                             "<li><b>Exemple6:</b> <code>v4:=m1*v1;</code></li>"
                                             "</ul><ul>De nombreuses instructions (Ex ker) retournent une liste de vecteurs entre crochets."
                                             "Dans l'exemple suivant, N est une base du noyau de m1. N[0] le premier vecteur de cette base N[1] le second ..."
                                             "Remarque: N affichera donc ces vecteurs en <u>ligne</u>."
                                             "<li><b>Exemple7:</b> <code>N:=ker m1;</code></li>"
                                             "</ul><hr>"
                                                      ));



}
