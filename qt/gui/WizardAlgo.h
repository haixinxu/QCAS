/*   F.Han
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

#ifndef WIZARDALGO_H
#define WIZARDALGO_H
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
class QGroupBox;
class QPushButton;
class MainWindow;
class WizardAlgo : public QWidget{
    Q_OBJECT
public:
    WizardAlgo(MainWindow *parent=0,bool scol=false);
    void sendCommand(const QString &);
    void cursorUp();
private:
    MainWindow* mainWindow;
    void changeEvent(QEvent *);
    void retranslate();
    void createGui();
    bool scolar;
    QStackedWidget* pages;
    QComboBox *list;

};

class AlgoTabChild:public QWidget{
    Q_OBJECT
public:
    AlgoTabChild(QWidget *parent=0);
public slots:
    virtual void sendCommand()=0;
};

class FunctionPanel:public AlgoTabChild{
    Q_OBJECT
public:
    FunctionPanel(WizardAlgo *parent=0);
public slots:
    void sendCommand();

private:
    WizardAlgo* algoPanel;
    QLineEdit *prototype;
    QLabel *labelProto;
    QLineEdit *varloc;
    QLabel *labelVarloc;
    QLineEdit *symbloc;
    QLabel *labelSymbloc;
    QPlainTextEdit *instructions;
    QLineEdit *retour;
    QLabel *labelRetour;
    QLabel *labelEnd;


    void retranslate();
    void changeEvent(QEvent *);
};

class LoopPanel:public AlgoTabChild{
    Q_OBJECT
public:
    LoopPanel(WizardAlgo *parent=0);
public slots:
    void sendCommand();

private:
    WizardAlgo* algoPanel;
    QLineEdit *varname;
    QLabel *labelVarname;
    QLineEdit *deb;
    QLabel *labelDeb;
    QLineEdit *fin;
    QLabel *labelFin;
    QLineEdit *pas;
    QLabel *labelPas;
    QPlainTextEdit *instructions;
    QGroupBox *group;
    QLabel *labelEnd;


    void retranslate();
    void changeEvent(QEvent *);
};

class TestPanel:public AlgoTabChild{
    Q_OBJECT
public:
    TestPanel(WizardAlgo *parent=0);
public slots:
    void sendCommand();

private:
    WizardAlgo* algoPanel;
    QLineEdit *condi;
    QLabel *labelCondi;
    QPlainTextEdit *instruction1;
    QPlainTextEdit *instruction2;
    QGroupBox *groupalors;
    QGroupBox *groupsinon;
    QLabel *labelEnd;

    void retranslate();
    void changeEvent(QEvent *);
};

class TantquePanel:public AlgoTabChild{
    Q_OBJECT
public:
    TantquePanel(WizardAlgo *parent=0);
public slots:
    void sendCommand();

private:
    WizardAlgo* algoPanel;
    QLineEdit *condi;
    QLabel *labelCondi;
    QPlainTextEdit *instructions;
    QGroupBox *group;
    QLabel *labelEnd;

    void retranslate();
    void changeEvent(QEvent *);
};

class FuncPanel:public AlgoTabChild{
    Q_OBJECT
public:
    FuncPanel(WizardAlgo *parent=0);
public slots:
    void sendCommand();

private:
    WizardAlgo* algoPanel;
    QLineEdit *func;
    QLabel *labelFunc;
    QLineEdit *varloc;
    QLabel *labelVarloc;
    QLineEdit *symbloc;
    QLabel *labelSymbloc;
    QPlainTextEdit *instructions;
    QGroupBox *group;
    QLineEdit *retour;
    QLabel *labelRetour;
    QLabel *labelEnd;


    void retranslate();
    void changeEvent(QEvent *);
};

class IfPanel:public AlgoTabChild{
    Q_OBJECT
public:
    IfPanel(WizardAlgo *parent=0);
public slots:
    void sendCommand();

private:
    WizardAlgo* algoPanel;
    QLineEdit *condi;
    QLabel *labelCondi;
    QPlainTextEdit *instruction1;
    QPlainTextEdit *instruction2;
    QGroupBox *groupalors;
    QGroupBox *groupsinon;
    QLabel *labelEnd;

    void retranslate();
    void changeEvent(QEvent *);
};


class ForPanel:public AlgoTabChild{
    Q_OBJECT
public:
    ForPanel(WizardAlgo *parent=0);
public slots:
    void sendCommand();

private:
    WizardAlgo* algoPanel;
    QLineEdit *ini;
    QLabel *labelIni;
    QLineEdit *arret;
    QLabel *labelArret;
    QLineEdit *incr;
    QLabel *labelIncr;
    QPlainTextEdit *instructions;
    QGroupBox *group;
    QLabel *labelEnd;


    void retranslate();
    void changeEvent(QEvent *);
};

class WhilePanel:public AlgoTabChild{
    Q_OBJECT
public:
    WhilePanel(WizardAlgo *parent=0);
public slots:
    void sendCommand();

private:
    WizardAlgo* algoPanel;
    QLineEdit *condi;
    QLabel *labelCondi;
    QPlainTextEdit *instructions;
    QGroupBox *group;
    QLabel *labelEnd;

    void retranslate();
    void changeEvent(QEvent *);
};

#endif // WIZARDALGO_H
