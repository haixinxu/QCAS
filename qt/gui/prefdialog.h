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

#ifndef PREFDIALOG_H
#define PREFDIALOG_H
#include <QDialog>
#include <QLabel>
#include <QGroupBox>
#include <QListWidgetItem>

class MainWindow;
class QStackedWidget;
class QListWidget;
class PrefDialog;
class QComboBox;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QSpinBox;




class CasPanel:public QWidget{
    Q_OBJECT
public:
    CasPanel(MainWindow*);
    void initValue();
    void apply();


private:
    MainWindow* mainWindow;
    QWidget* mainPanel;
    QWidget* advancedPanel;
    QComboBox * comboProg;
    QComboBox * comboFloat;
    QComboBox * comboBasis;
    QLineEdit * editDigits;
    QCheckBox * checkSymbolic;
    QCheckBox * checkRadian;
    QCheckBox * checkComplex;
    QCheckBox * checkComplexVar;
    QCheckBox * checkPolynomialDecrease;
    QCheckBox * checkAlltrig;
    QCheckBox * checkSqrt;
    QPushButton* buttonAdvanced;

    QLineEdit*editEpsilon;
    QLineEdit*editProbaEpsilon;
    QSpinBox* spinRecursProg;
    QSpinBox* spinRecursEval;
    QSpinBox* spinEvalInProg;
    QSpinBox* spinNewton;
    QLineEdit* editDebugInfo;

    QPushButton* backButton;

    QLabel * labelProg;
    QLabel * labelFloat;
    QLabel * labelBasis;
    QLabel * labelDigits;
    QLabel* labelProbaEpsilon;
    QLabel* labelEpsilon;
    QLabel* labelProgRecurs;
    QLabel* labelEvalRecurs;
    QLabel* labelEvalInProg;
    QLabel* labelDebugInfo;
    QLabel* labelNewton;


    void changeEvent(QEvent *);
    void retranslate();
    void initGui();

private slots:
    void switchPanel();
};

class GeneralPanel:public QWidget{
Q_OBJECT
public:
    GeneralPanel();
    void initValue();
    void apply();
private:
    MainWindow* mainWindow;
    QLineEdit* editWidth;
    QCheckBox* checkGridAttraction;
    QComboBox* comboLanguage;
    QCheckBox* checkUseTabCompletions;
    QLineEdit* editXMin;
    QLineEdit* editXMax;
    QLineEdit* editYMin;
    QLineEdit* editYMax;
    QLineEdit* editZMin;
    QLineEdit* editZMax;
    QLineEdit* editTMin;
    QLineEdit* editTMax;
    QSpinBox* editMMLSize;
    QCheckBox* checkAutoScale;
    QGroupBox * graphicGroup;
    QGroupBox * policeGroup;
    QLabel * labelWidth;
    QLabel* labelLanguage;
    QLabel* labelMMLSize;
    QLabel* labelUseTabCompletions;

    void changeEvent(QEvent *);
    void retranslate();
    void initGui();


};
//class Interactive
class Interactive2dPanel:public QWidget{
Q_OBJECT
public:
    Interactive2dPanel();
    void initValue();
    void apply();
private:
    QLineEdit* editGeoVarPrefix;
    QGroupBox* autovar2dGroup;
    QLabel* labelGeoVarPrefix;

    void initGui();
    void retranslate();
    void changeEvent(QEvent *);

};

//


class PrefDialog: public QDialog{
    Q_OBJECT
public:
    PrefDialog(MainWindow* );
    void initValue();
private:
    MainWindow* mainWindow;
    QListWidget* listWidget;
    QStackedWidget* stackWidget;
    CasPanel* casPanel;
    GeneralPanel* generalPanel;
    QWidget* spreadSheetPanel;
    Interactive2dPanel* interactive2dPanel;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QListWidgetItem* generalItem;
    QListWidgetItem* interactive2dItem;
    QListWidgetItem* spreadItem;

    void initGui();
    void retranslate();
    void changeEvent(QEvent *);
private slots:
    void apply();
};



#endif // PREFDIALOG_H
