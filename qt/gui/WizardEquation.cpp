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

#include "WizardEquation.h"
#include "../MainWindow.h"
#include <QStackedWidget>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QString>
#include <iostream>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QStringList>
#include <QSpinBox>

WizardEquation::WizardEquation(MainWindow *parent)
    :QWidget(parent){
    mainWindow=parent;
    createGui();
}
void WizardEquation::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}
void WizardEquation::retranslate(){
    list->setItemText(0,tr("Equation/Inéquation"));
    list->setItemText(1,tr("Equation différentielle"));
    list->setItemText(2,tr("Système d'équations"));

}
void WizardEquation::createGui(){
    pages=new QStackedWidget;
    list=new QComboBox;
    list->addItem("");
    list->addItem("");
    list->addItem("");

    EqPanel *eqPanel=new EqPanel(this);
    DiffPanel *diffPanel=new DiffPanel(this);
    SystPanel *systPanel=new SystPanel(this);


    pages->addWidget(eqPanel);
    pages->addWidget(diffPanel);
    pages->addWidget(systPanel);

    QVBoxLayout *layout=new QVBoxLayout;
    layout->addWidget(list);
    layout->addWidget(pages);
    this->setLayout(layout);
    connect(list,SIGNAL(activated(int)),pages,SLOT(setCurrentIndex(int)));

    retranslate();
}
void WizardEquation::sendEquation(const QString &s){
//    QString s=qobject_cast<TabChild*>(pages->currentWidget())->sendEquation();
    mainWindow->sendText(s);
}


TabChild::TabChild(QWidget *parent):QWidget(parent){
}
void EqPanel::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}
void EqPanel::retranslate(){
    eq->setToolTip(tr("Saisir l'équation à résoudre <br> <b>Exemple: </b> x^2+2*x=x^3"));
    labelEq->setText(tr("Equation/Inéquation"));
    labelEq->setToolTip(eq->toolTip());
    labelVar->setText(tr("Variable:"));
    var->setToolTip(tr("Indiquez le nom de l'inconnue (une seule variable).<br><b>Exemple: </b>x"));
    labelVar->setToolTip(var->toolTip());
    numeric->setText(tr("Résolution numérique"));
    inC->setText(tr("Solutions complexes"));
    hyp->setToolTip(tr("<center><b>supposons</b></center>(Facultatif)."
                       "Pour résoudre, il faut  parfois faire une hypothèse sur la variable."
                       "Par exemple pour <code>solve(sin(2*x)>1/2)</code>, on donnerait un intervalle borné.<br>"
                       "<b>Exemple: </b>x&gt;-2*pi and x&lt;2*pi<br>"
                       "On doit exprimer la condition avec la variable en premier:<br>"
                       "<b>Exemple: </b>x&gt;-5<br> est correct <u>mais pas:</u> -5 &lt;x"
                       ));
    labelHyp->setText(tr("Hypothèses"));
    labelHyp->setToolTip(hyp->toolTip());
    purge->setToolTip(tr("<center><b>purge(??)</b></center>Nettoyer la variable aprés la résolution pour enlever les hypothèses."));

}
EqPanel::EqPanel(WizardEquation *parent):TabChild(parent){
        eqPanel=parent;
        labelEq=new QLabel("");
        eq=new QLineEdit;
        labelEq->setBuddy(eq);


        labelVar=new QLabel("");
        var=new QLineEdit("x");
        labelVar->setBuddy(var);

        labelHyp=new QLabel("");
        hyp=new QLineEdit("");
        labelHyp->setBuddy(hyp);

        numeric=new QCheckBox("");
        inC=new QCheckBox("");
        purge=new QCheckBox("");
        purge->setChecked(true);

        QPushButton *button=new QPushButton;
        button->setIcon(QIcon(":/images/right.png"));

        QGridLayout *grid=new QGridLayout();
        grid->addWidget(labelEq,0,0);
        grid->addWidget(eq,0,1);
        grid->addWidget(labelVar,1,0);
        grid->addWidget(var,1,1);
        grid->addWidget(labelHyp,2,0);
        grid->addWidget(hyp,2,1);
        grid->addWidget(purge,2,2);
        grid->addWidget(numeric,3,0,2,1,Qt::AlignTop);
        grid->addWidget(inC,4,0,2,1,Qt::AlignTop);
        grid->addWidget(button,5,1,Qt::AlignTop);
        this->setLayout(grid);

        connect(button,SIGNAL(clicked()),this,SLOT(sendEquation()));
        connect(numeric,SIGNAL(stateChanged(int)),this,SLOT(numericCheck(int)));
        connect(inC,SIGNAL(stateChanged(int)),this,SLOT(inC_Check(int)));
        connect(purge,SIGNAL(stateChanged(int)),this,SLOT(purge_Check(int)));

	retranslate();
   }
void EqPanel::inC_Check(int a){
    if (a==Qt::Checked){
        if (numeric->isChecked()) {
            numeric->setChecked(false);
        }
    }
}

void EqPanel::numericCheck(int a){
    if(a==Qt::Checked){
        if (inC->isChecked()){
            inC->setChecked(false);
        }
    }
}

void EqPanel::purge_Check(int a){
    if (a==Qt::Checked){
        if (purge->isChecked()) {
            purge->setChecked(false);
        }
    }
}

void EqPanel::sendEquation(){
    QString s;
    if(!hyp->text().trimmed().isEmpty()){
        s="assume(";
        s.append(hyp->text().append(" ):;\n"));
    }
    if (inC->isChecked()) {
        s.append("csolve(");
    }
    else if (numeric->isChecked()){
        s.append("fsolve(");
    }
    else s.append("solve(");
    s.append(eq->text());
    s.append(",");
    s.append(var->text());
    s.append(");");
    if(purge->isChecked() && !hyp->text().trimmed().isEmpty())
        s.append("\npurge( ").append(var->text()).append("):;");
    eqPanel->sendEquation(s);
}

DiffPanel::DiffPanel(WizardEquation *parent):TabChild(parent){
    eqPanel=parent;
    QLabel *labelEq=new QLabel(tr("&Equation:"));
    eq=new QLineEdit;
    labelEq->setBuddy(eq);

    eq->setToolTip(tr("Saisir l'équation différentielle<br> <b>Exemple: </b> y''+y=sin(x)"));
    QLabel *labelVar=new QLabel(tr("&Variable:"));
    var=new QLineEdit("y");
    labelVar->setBuddy(var);

    QGroupBox *group=new QGroupBox(tr("Conditions initiales"));
    QHBoxLayout *layout=new QHBoxLayout;
    editor=new QPlainTextEdit();
    editor->setToolTip(tr("<b>Exemples:</b><br> y(0)=1<br>y'(0)=2"));
    layout->addWidget(editor);
    group->setLayout(layout);

    QPushButton *button=new QPushButton;
    button->setIcon(QIcon(":/images/right.png"));

    QGridLayout *grid=new QGridLayout();
    grid->addWidget(labelEq,0,0);
    grid->addWidget(eq,0,1);
    grid->addWidget(labelVar,1,0);
    grid->addWidget(var,1,1);
    grid->addWidget(group,2,0,2,2);
    grid->addWidget(button,4,1);
    this->setLayout(grid);

    connect(button,SIGNAL(clicked()),this,SLOT(sendEquation()));
}

void DiffPanel::sendEquation(){
    QString s("desolve(");
    QString init=editor->document()->toPlainText();
    if (init.isEmpty()){
        s.append(eq->text());
    }
    else{
        s.append("[");
        s.append(eq->text());
        s.append(",");
        QStringList list=init.split("\n",QString::SkipEmptyParts);
        for(int i=0;i<list.size();++i){
            s.append(list.at(i));
            if (i!=list.size()-1){
                s.append(",");
            }
        }
        s.append("]");
    }
    s.append(",");
    s.append(var->text());
    s.append(");");
    eqPanel->sendEquation(s);
}
MySpin::MySpin(QWidget *parent):QSpinBox(parent){
    setMinimum(2);
    setMaximum(10);
    setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    lineEdit()->setReadOnly(true);

}
SystPanel::SystPanel(WizardEquation *parent):TabChild(parent){
    eqPanel=parent;
    labelSpin=new QLabel(tr("Nombre d'équations:"));
    spin=new MySpin;
    QLineEdit *eq1=new QLineEdit;
    eq1->setToolTip(tr("<b>Exemple:<br> </b> 2*x+3*y=10"));

    QLineEdit *eq2=new QLineEdit;
    eq2->setToolTip(tr("<b>Exemple: </b><br> x^2-y=3"));

    label=new QVector<QLabel*>;
    edit=new QVector<QLineEdit*>;
    label->append(new QLabel(tr("Equation n°1:")));
    label->append(new QLabel(tr("Equation n°2:")));
    edit->append(eq1);
    edit->append(eq2);

    labelVar=new QLabel(tr("Variable:"));
    var=new QLineEdit("x,y");
    var->setToolTip(tr("Liste des inconnues séparées par des virgules.<br><b>Exemple: <b>x,y,z"));
    labelVar->setBuddy(var);
    labelVar->setToolTip(var->toolTip());

    check=new QCheckBox(tr("Systèmes linéaires"));
    button=new QPushButton;
    button->setIcon(QIcon(":/images/right.png"));


    grid=new QGridLayout;
    updateGrid();

    setLayout(grid);

    connect(button,SIGNAL(clicked()),this,SLOT(sendEquation()));
    connect(spin,SIGNAL(valueChanged(int)),this,SLOT(addEquations(int)));

}
void SystPanel::updateGrid(){
    int line=0;
    grid->addWidget(labelSpin,line,0);
    grid->addWidget(spin,line,1);
    ++line;
    for(int i=0;i<edit->size();++i){
        grid->addWidget(label->at(i),line,0);
        grid->addWidget(edit->at(i),line,1);
        ++line;
    }
    grid->addWidget(labelVar,line,0);
    grid->addWidget(var,line,1);
    ++line;
    grid->addWidget(check,line,0,1,2);
    ++line;
    grid->addWidget(button,line,1);
}
void SystPanel::addEquations(int id){
    delete layout();
    // Add equations
    if (id>edit->size()){
        QString s=tr("Equation n°");
        QString str;
        str.setNum(id);
        s.append(str);
        s.append(":");
        label->append(new QLabel(s));
        edit->append(new QLineEdit);
    }
    // remove equations
    else {
        delete label->at(label->size()-1);
        delete edit->at(edit->size()-1);
        label->remove(label->size()-1);
        edit->remove(edit->size()-1);
    }
    grid=new QGridLayout;
    updateGrid();
    setLayout(grid);
}
void SystPanel::sendEquation(){
   QString s;
   if (check->isChecked()){
        s.append("linsolve([");
   }
   else {
    s.append("solve([");
   }
   for(int i=0;i<edit->size();++i){
        s.append(edit->at(i)->text());
        if (i!=edit->size()-1) {
            s.append(",");
        }
   }
   s.append("],[");
   QStringList list=var->text().split(",",QString::SkipEmptyParts);
   for(int i=0;i<list.size();++i){
       s.append(list.at(i));
       if (i!=list.size()-1){
           s.append(",");
       }
   }
   s.append("]);");
    eqPanel->sendEquation(s);;
}
