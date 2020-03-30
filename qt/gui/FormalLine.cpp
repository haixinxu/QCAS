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
#include "sizeof_void_p.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QScrollArea>
#include <QToolButton>
#include <QLayoutItem>
#include <QDebug>
#include "../CasManager.h"
#include "../MainWindow.h"
#include "FormalLineWidgets.h"
#include "FormalSheet.h"
#include "FormalLine.h"
#include "qtmmlwidget.h"
#include "../output.h"


Line::Line(int id,FormalWorkSheet *parent):QWidget(parent){
    this->id=id;
    workSheet=parent;

    out=0;
    giacmessages=0;

    check=new QCheckBox(QString::number(id+1));
    checkshowout=new QCheckBox(this);    

    input=new TextInput(this);
    textformula=new TextEditOutput(this);
    textformula->hide();

    QPalette p=textformula->palette();
    p.setColor(QPalette::Base,p.window().color());
    textformula->setPalette(p);
    checkshowout->setTristate(true);
    checkshowout->setToolTip(tr("Affichage des réponses<center><b>Mathml</b> ou <b>Texte</b></center>"));

    mainLayout=new QGridLayout;
    mainLayout->addWidget(input,0,1,Qt::AlignTop);
    mainLayout->addWidget(check,0,0,Qt::AlignTop);
    mainLayout->addWidget(textformula,2,1,Qt::AlignTop);
    mainLayout->addWidget(checkshowout,3,0,Qt::AlignRight);
    checkshowout->hide();
    setLayout(mainLayout);
    connect(check,SIGNAL(clicked()),this,SLOT(selectLevel()));
    connect(checkshowout,SIGNAL(clicked()),this,SLOT(showhideOutWidgets()));

}
Line::~Line(){
    delete mainLayout;
    delete check;
    delete input;
    delete textformula;
    delete out;
    delete checkshowout;
    delete giacmessages;

}
/** When the user selects or unselects the checkBox number
  **/
void Line::selectLevel(){
 //   qDebug()<<"shift"<<check->hasShift();
    getWorkSheet()->addSelectedLevel(id);
   // check->shiftToFalse();
}
/**
  When user starts a new task, add The Stop Button under the QCheckBox
**/
void Line::addStopButton(QToolButton *stop){
    mainLayout->addWidget(stop,1,0);
    stop->show();
}
void Line::removeStopButton(){
    mainLayout->removeWidget(workSheet->getApp()->getStopButton());

//    qDebug()<<workSheet->getApp()->getStopButton()->parentWidget();
    }
int Line::getId()const{
    return id;
}
void Line::setId(const int & a){
    id=a;
    check->setText(QString::number(id+1));
}
TextInput* Line::getTextInput() const{
    return input;
}

FormalWorkSheet* Line::getWorkSheet() const{
    return workSheet;
}
void Line::evaluate(const QString & formula){
    if (out!=0){
        //qInfo() << " delete out in line: " << id << "to evaluate " << formula;
        mainLayout->removeWidget(out);
        delete out;
        out=0;

        if(giacmessages != 0){
            mainLayout->removeWidget(giacmessages);
            delete giacmessages;
            giacmessages=0;
        }
    }
    getWorkSheet()->getApp()->evaluate(formula);

}

OutputWidget* Line::getOuputWidget(){
    return out;
}

void Line::displayResult(OutputWidget* uuu){
    //qInfo() << "previousout = " << this->out <<"  id= " << this->id;
    this->out=uuu;
    if(!this->getgiacmessages().isEmpty()){
        giacmessages=new QPlainTextEdit(this);
        giacmessages->setReadOnly(true);
        giacmessages->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        giacmessages->setMinimumWidth(input->width());
        giacmessages->setPlainText(this->getgiacmessages());
        giacmessages->setFrameStyle(QFrame::NoFrame);
        int nblignes=giacmessages->document()->size().height();
        //qDebug()<<"nb lignes: "<<nblignes;
        if(nblignes>4)
            nblignes=5;
        giacmessages->setFixedHeight(fontMetrics().lineSpacing()+fontMetrics().descent()+\
                                 2*giacmessages->document()->documentMargin()+6\
                                 +fontMetrics().lineSpacing()*(nblignes-1)\
                                 );
        QPalette p=giacmessages->palette();
        p.setColor(QPalette::Base,p.window().color());
        p.setColor(QPalette::Text,Qt::darkGreen);
        giacmessages->setPalette(p);
        mainLayout->addWidget(giacmessages,1,1,Qt::AlignLeft);

    }

    textformula->setPlainText("");

    textformula->appendPlainText(out->getTextOutput());
    textformula->adjustHeight();

    out->setMinimumSize(out->sizeHint());
    out->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    mainLayout->addWidget(out,3,1,Qt::AlignLeft);
    if(out->isFormula())
        checkshowout->show();
    showhideOutWidgets();
    getWorkSheet()->goToNextLine();

}

void Line::showhideOutWidgets(){
    if(out==0)
        return;
    if(checkshowout->checkState()==Qt::Unchecked){
       mainLayout->removeWidget(checkshowout);
       mainLayout->addWidget(checkshowout,3,0,Qt::AlignRight);
       out->show();textformula->hide();
    }
    if(checkshowout->checkState()==Qt::PartiallyChecked){
       out->hide();
       mainLayout->removeWidget(checkshowout);
       mainLayout->addWidget(checkshowout,2,0,Qt::AlignRight);
       textformula->show();
       textformula->adjustHeight();
    }
    if(checkshowout->checkState()==Qt::Checked){
       out->show();
       textformula->show();
       textformula->adjustHeight();
    }
}

QString Line::getgiacmessages(){
    return getWorkSheet()->getApp()->lastgiacmessage;
}
