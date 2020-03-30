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

#ifndef WIZARDEQUATION_H
#define WIZARDEQUATION_H
#include <QWidget>
#include <QVector>
#include <QSpinBox>
class QComboBox;
class QStackedWidget;
class QLineEdit;
class QCheckBox;
class QPlainTextEdit;
class QGridLayout;
class QLabel;
class QPushButton;
class MainWindow;
class WizardEquation : public QWidget{
    Q_OBJECT
public:
    WizardEquation(MainWindow *parent=0);
  void sendEquation(const QString &);
private:
    MainWindow* mainWindow;
    void changeEvent(QEvent *);
    void retranslate();
    void createGui();
    QStackedWidget* pages;
    QComboBox *list;



};

class TabChild:public QWidget{
    Q_OBJECT
public:
    TabChild(QWidget *parent=0);
public slots:
    virtual void sendEquation()=0;
};

class EqPanel:public TabChild{
    Q_OBJECT
public:
    EqPanel(WizardEquation *parent=0);
public slots:
    void sendEquation();
    void numericCheck(int);
    void inC_Check(int);
    void purge_Check(int);

private:
    WizardEquation* eqPanel;
    QLineEdit *eq;
    QLabel    *labelEq;
    QLineEdit *var;
    QLabel *labelVar;
    QLineEdit *hyp;
    QLabel    *labelHyp;
    QCheckBox *purge;
    QCheckBox *numeric;
    QCheckBox *inC;

    void retranslate();
    void changeEvent(QEvent *);
};

class DiffPanel:public TabChild{
    Q_OBJECT
public:
    DiffPanel(WizardEquation *parent=0);
public slots:
    void sendEquation();
private:
    WizardEquation* eqPanel;

    QLineEdit *eq;
    QLineEdit *var;
    QPlainTextEdit *editor ;
};
class MySpin:public QSpinBox{
public:
    MySpin(QWidget *parent=0);
};
class SystPanel:public TabChild{
    Q_OBJECT
public:
    SystPanel(WizardEquation *parent=0);
public slots:
    void sendEquation();

private:
    WizardEquation* eqPanel;
    void updateGrid();
    QGridLayout* grid;
    MySpin *spin;
    QLabel *labelSpin,*labelVar;
    QPushButton *button;
    QVector<QLabel*> *label;
    QLineEdit *var;
    QVector<QLineEdit*> *edit;
    QCheckBox *check;
private slots:
    void addEquations(int);
};


#endif // WIZARDEQUATION_H
