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

#include <QStackedWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QGroupBox>
#include "../MainWindow.h"
#include "prefdialog.h"

#include "../sizeof_void_p.h"
#include <giac/global.h>
#include <giac/plot.h>
#include "config.h"
#include <QDebug>



/**
   **********     PREFERENCES CAS PANEL    *************************

  **/

CasPanel::CasPanel(MainWindow *parent):QWidget(parent){
    mainWindow=parent;
    initGui();
}
void CasPanel::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}
void CasPanel::retranslate(){
    labelProg->setText(tr("Langage utilisé"));
    labelProg->setToolTip(tr("<ul>"
                             "<li><b>XCAS:</b><br> <tt> maple_mode(0)</tt></li>"
                             "<li><b>Maple:</b><br> <tt> maple_mode(1)</tt></li>"
                             "<li><b>MuPad:</b><br> <tt> maple_mode(2)</tt></li>"
                             "<li><b>TI 89/92:</b><br> <tt> maple_mode(3)</tt></li>"
                             "</ul>"));

    labelFloat->setText(tr("Format des nombres à virgule"));
    labelFloat->setToolTip("<p><ul>"
                           "<li><b>Standard:</b><p>150.12 s'affiche 150.12</p></li>"
                           "<li><b>Scientifique:</b><p> 150.12 s'affiche 1.5012e+2</p></li>"
                           "<li><b>Standard:</b><p> 150.12 s'affiche 150.12e+0</p></li>"
                           "</ul></p>");

    labelBasis->setText(tr("Entiers en base ..."));

    labelDigits->setText(tr("Nombre de décimales"));
    labelDigits->setToolTip(tr("<p><b>Nombre de chiffres significatifs à l'affichage</b></p>"
                               "<tt>DIGITS:=3;evalf(pi);<br>3.14 </tt>"));

    checkSymbolic->setText(tr("Calcul symbolique"));
    checkSymbolic->setToolTip(tr("<p><b>Calcul en mode exact ou approché.</b></p> <p><tt>approx_mode:=1 </tt> (mode approximatif)</p><p><tt>approx_mode:=0 </tt> (mode exact)</p>"));

    checkRadian->setText(tr("Angle en radian"));
    checkRadian->setToolTip(tr("<p><b>Spécifie l'unité choisie pour les angles.</b></p>"
                               "<p><tt>angle_radian:=1</tt> (radians)</p>"
                               "<p><tt>angle_radian:=0</tt> (degrés)</p>"
                               ));

    checkComplex->setText(tr("Calcul en mode complexe"));
    checkComplex->setToolTip(tr("<p><b>Spécifie si l'on travaille en mode réel ou complexe.</b></p>"
                               "<p><tt>complex_mode:=1</tt> (mode complexe)</p>"
                               "<p><tt>complex_mode:=0</tt> (mode réel)</p>"
                                "<b>Exemple: </b> <tt>factor(x^4-1)</tt>"));
    checkComplexVar->setText(tr("Variables à valeur complexe"));
    checkComplexVar->setToolTip(tr("<p><b>Spécifie si l'on travaille avec des variables réelles ou complexes.</b></p>"
                               "<p><tt>complex_variables:=1</tt> (mode complexe)</p>"
                               "<p><tt>complex_variables:=0</tt> (mode réel)</p>"));

    checkPolynomialDecrease->setText(tr("Affichage des polynômes en puissances décroissantes"));
    checkPolynomialDecrease->setToolTip(tr("<p><center><tt>1+x+x^2+x^3+x^4+x^5</tt><br>au lieu de ... <br><tt>x^5+x^4+x^3+x^2+x+1</tt></center></p>"));

    checkAlltrig->setText(tr("Solutions trigonométriques générales"));
    checkAlltrig->setToolTip(tr("<p><ul>"
                                "<li><tt>solve(cos (x)=1)</tt><br><tt><b>Réponse:</b> [0]</tt></li>"
                                "<li>Sinon,<br><tt>solve(cos (x)=1)</tt><br><b>Réponse:</b> <tt>[2*n*pi]</tt></li>"
                                "</ul></p>"));
    checkSqrt->setText(tr("factorisation avec racines carrées"));
    buttonAdvanced->setText(tr("Avancé..."));

    comboFloat->setItemText(0,tr("Standard"));
    comboFloat->setItemText(1,tr("Scientifique"));
    comboFloat->setItemText(2,tr("Ingénieur"));

    labelDigits->setText(tr("Nombre de décimales"));
    labelDigits->setToolTip(tr("<p><b>Nombre de chiffres significatifs à l'affichage</b></p>"
                               "<tt>DIGITS:=3;evalf(pi);<br>3.14 </tt>"));

    labelEpsilon->setToolTip(tr("<p>Les nombres dont la valeur absolue est inférieure à cette valeur peuvent être considérés comme nuls. (Par défaut: 1e-10)</p>"));

    labelProbaEpsilon->setToolTip(tr("<p>Si cette valeur est non nulle, Giac peut utiliser des algorithmes "
                                "non déterministes et renvoyer une réponse qui a alors une probabilité d’être "
                               "fausse inférieure à la valeur donnée. C’est par exemple le cas pour le calcul"
                                " du déterminant d’une grande matrice à coefficients entiers. (Par défaut: 1e-15)</p>"));
    labelEvalRecurs->setText(tr("Nombre maximal de substitutions récursives<br> de variables en mode interactif:"));
    labelEvalRecurs->setToolTip(tr("<p>Nombre maximum d’évaluations récursives en mode interactif. Par"
                                   "exemple, si on exécute dans l’ordre <tt> a:=b+1 </tt> et <tt>b:=5</tt> puis on évalue <tt>a</tt>, la"
                                   "valeur renvoyée sera <tt>b+1</tt> si cette valeur vaut 1 et 6 si cette valeur est plus grande que 1. (Par défaut: 25)</p>"));
    labelProgRecurs->setText(tr("Nombre maximal d'appels <br> récursifs dans un programme:"));
    labelProgRecurs->setToolTip(tr("<p>Nombre maximum d’appels récursifs : par défaut c’est 50. Il n’y a pas de limite imposée; "
                                     "par contre, si on met une borne trop élevée, c’est la pile du programme qui risque "
                                   "de déborder en provoquant un segmentation fault. Le moment où cela se "
                                   "produit dépend de la fonction, car la pile est aussi utilisée pour les appels "
                                   "récursifs des fonctions C++ de giac.</p>"));
    labelEvalInProg->setText(tr("Nombre maximal de substitutions récursives<br> de variables à l'exécution d'un programme:"));
    labelEvalInProg->setToolTip(tr("<p>Comme ci-dessus, mais lorsqu’un programme est exécuté. On utilise "
                                   "en principe le niveau 1 d’évaluation à l’intérieur d’un programme.</p>"));

    labelDebugInfo->setText(tr("Niveau de verbosité de Giac:"));
    labelDebugInfo->setToolTip(tr("<p>Affiche des informations intermédiaires (en bleu) sur les algorithmes"
                               "utilisés par giac en fonction du niveau (0 = pas d’info).</p>"));

    labelNewton->setText(tr("Nombre maximal d’itérations pour la méthode de Newton"));
    labelNewton->setToolTip(tr("Nombre maximal d’itérations pour la méthode de Newton"));

    backButton->setText(tr("Basique..."));

}

void CasPanel::initGui(){

   // Initialize main Panel //

    QGridLayout* grid=new QGridLayout;

    labelProg=new QLabel("");

    comboProg=new QComboBox(this);
    comboProg->addItem(tr("XCAS"));
    comboProg->addItem(tr("Maple"));
    comboProg->addItem(tr("MuPad"));
    comboProg->addItem(tr("TI 89/92"));

    labelFloat=new QLabel("");

    comboFloat=new QComboBox(this);
    comboFloat->addItem(tr("Standard"));
    comboFloat->addItem(tr("Scientifique"));
    comboFloat->addItem(tr("Ingénieur"));


    labelBasis=new QLabel("");
    comboBasis=new QComboBox(this);
    comboBasis->addItem(tr("10"));
    comboBasis->addItem(tr("16"));
    comboBasis->addItem(tr("8"));

    labelDigits=new QLabel("");

    editDigits=new QLineEdit(this);
    QIntValidator* validator=new QIntValidator;
    editDigits->setValidator(validator);

    checkSymbolic=new QCheckBox("");

    checkRadian=new QCheckBox("");

    checkComplex=new QCheckBox("");

    checkComplexVar=new QCheckBox("");

    checkPolynomialDecrease=new QCheckBox("");

    checkAlltrig=new QCheckBox("");

    checkSqrt=new QCheckBox("");
    buttonAdvanced=new QPushButton("");
    buttonAdvanced->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    connect(buttonAdvanced,SIGNAL(clicked()),this,SLOT(switchPanel()));

    grid->addWidget(labelProg,0,0);
    grid->addWidget(comboProg,0,1);
    grid->addWidget(labelFloat,1,0);
    grid->addWidget(comboFloat,1,1);
    grid->addWidget(labelBasis,2,0);
    grid->addWidget(comboBasis,2,1);
    grid->addWidget(labelDigits,3,0);
    grid->addWidget(editDigits,3,1);
    grid->addWidget(checkSymbolic,4,0);
    grid->addWidget(checkRadian,5,0);
    grid->addWidget(checkComplex,6,0);
    grid->addWidget(checkComplexVar,7,0);
    grid->addWidget(checkPolynomialDecrease,4,1);
    grid->addWidget(checkAlltrig,5,1);
    grid->addWidget(checkSqrt,6,1);
    grid->addWidget(buttonAdvanced,7,1,Qt::AlignCenter);

    mainPanel=new QWidget;
 //   mainPanel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    mainPanel->setLayout(grid);


    // Initialize advanced panel //
    advancedPanel=new QWidget;
//    advancedPanel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);

    QGridLayout *grid2=new QGridLayout;

    labelEpsilon=new QLabel(tr("epsilon:"));
    editEpsilon=new QLineEdit;
    editEpsilon->setValidator(new QDoubleValidator(editEpsilon));
    labelProbaEpsilon=new QLabel(tr("proba_epsilon:"));
    editProbaEpsilon=new QLineEdit;
    editProbaEpsilon->setValidator(new QDoubleValidator(editProbaEpsilon));

    labelProgRecurs=new QLabel("");

    spinRecursProg=new QSpinBox;
    spinRecursProg->setMinimum(0);
    spinRecursProg->setMaximum(500);

    labelEvalRecurs=new QLabel("");

    spinRecursEval=new QSpinBox;
    spinRecursEval->setMinimum(0);

    labelEvalInProg=new QLabel("");

    spinEvalInProg=new QSpinBox;
    spinEvalInProg->setMinimum(0);

    labelDebugInfo=new QLabel("");

    editDebugInfo=new QLineEdit;
    editDebugInfo->setValidator(new QIntValidator(editDebugInfo));

    labelNewton=new QLabel("");
    spinNewton=new QSpinBox;
    spinNewton->setMinimum(0);

    backButton=new QPushButton("");
    connect(backButton,SIGNAL(clicked()),this,SLOT(switchPanel()));
    grid2->setHorizontalSpacing(20);
    grid2->addWidget(labelEpsilon,0,0);
    grid2->addWidget(editEpsilon,0,1);
    grid2->addWidget(labelProbaEpsilon,1,0);
    grid2->addWidget(editProbaEpsilon,1,1);
    grid2->addWidget(labelDebugInfo,2,0);
    grid2->addWidget(editDebugInfo,2,1);
    grid2->addWidget(labelNewton,3,0);
    grid2->addWidget(spinNewton,3,1);
    grid2->addWidget(labelProgRecurs,0,2);
    grid2->addWidget(spinRecursProg,0,3);
    grid2->addWidget(labelEvalRecurs,1,2);
    grid2->addWidget(spinRecursEval,1,3);
    grid2->addWidget(labelEvalInProg,2,2);
    grid2->addWidget(spinEvalInProg,2,3);
    grid2->addWidget(backButton,3,2);
    advancedPanel->setVisible(false);
    advancedPanel->setLayout(grid2);


    // Lay out both panels //
    QVBoxLayout *vbox=new QVBoxLayout;
    vbox->addWidget(mainPanel);
    vbox->addWidget(advancedPanel);
    vbox->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(vbox);
//    setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);

    retranslate();

}
void CasPanel::switchPanel(){
    if (mainPanel->isVisible()) {
        mainPanel->hide();
        advancedPanel->show();
 //       adjustSize();

    }
    else{
        mainPanel->show();
        advancedPanel->hide();
   //     adjustSize();

    }
}
void CasPanel::initValue(){
    giac::context* c=mainWindow->getContext();
    comboProg->setCurrentIndex(giac::xcas_mode(c));
    comboFloat->setCurrentIndex(giac::scientific_format(c));
    int basis=giac::integer_format(c);
    if (basis==10) comboBasis->setCurrentIndex(0);
    else if (basis==16) comboBasis->setCurrentIndex(1);
    else if (basis==8) comboBasis->setCurrentIndex(2);

    editDigits->setText(QString::number(mainWindow->getDecimalDigit()));//QString::number(giac::decimal_digits(c)));
    if (giac::approx_mode(c)==0) checkSymbolic->setChecked(true);
    else checkSymbolic->setChecked(false);
    if (giac::angle_radian(c)==1) checkRadian->setChecked(true);
    else checkRadian->setChecked(false);
    if (giac::complex_mode(c)==1) checkComplex->setChecked(true);
    else checkComplex->setChecked(false);
    if (giac::complex_variables(c)==1) checkComplexVar->setChecked(true);
    else checkComplexVar->setChecked(false);
    if (giac::increasing_power(c)==1) checkPolynomialDecrease->setChecked(false);
    else checkPolynomialDecrease->setChecked(true);
    if (giac::all_trig_sol(c)==1) checkAlltrig->setChecked(true);
    else checkAlltrig->setChecked(false);
    if (giac::withsqrt(c)==1) checkSqrt->setChecked(true);
    else checkSqrt->setChecked(false);

    editEpsilon->setText(QString::number(giac::epsilon(c)));
    editProbaEpsilon->setText(QString::number(giac::proba_epsilon(c)));
    spinRecursEval->setValue(giac::eval_level(c));
    spinEvalInProg->setValue(giac::prog_eval_level_val(c));


    spinRecursProg->setValue(giac::MAX_RECURSION_LEVEL);
    editDebugInfo->setText(QString::number(giac::debug_infolevel));
    spinNewton->setValue(giac::NEWTON_DEFAULT_ITERATION
                         );
//    spinNewton->setValue(giac::);

   //spinRecursProg->setValue(giac::MAX_RECURSION_LEVEL);
   // spinRecursProg->setValue(giac::);

}
void CasPanel::apply(){
    giac::context* c=mainWindow->getContext();


    giac::xcas_mode(comboProg->currentIndex(),c);
    giac::scientific_format(comboFloat->currentIndex(),c);
    int d=comboBasis->itemText(comboBasis->currentIndex()).toInt();
    giac::integer_format(d,c);
    d=editDigits->text().toInt();
    if (d<0) d=12;
     giac::decimal_digits(d,c);
     mainWindow->setDecimalDigits(d);
    if (checkSymbolic->isChecked()) giac::approx_mode(0,c);
    else giac::approx_mode(1,c);
    if (checkRadian->isChecked()) giac::angle_radian(1,c);
    else giac::angle_radian(0,c);
    if (checkComplex->isChecked()) giac::complex_mode(1,c);
    else giac::complex_mode(0,c);
    if (checkComplexVar->isChecked()) giac::complex_variables(1,c);
    else giac::complex_variables(0,c);
    if (checkPolynomialDecrease->isChecked()) giac::increasing_power(0,c);
    else giac::increasing_power(1,c);
    if (checkAlltrig->isChecked()) giac::all_trig_sol(1,c);
    else giac::all_trig_sol(0,c);
    if (checkSqrt->isChecked()) giac::withsqrt(1,c);
    else giac::withsqrt(0,c);

    double dd=editEpsilon->text().toDouble();
    giac::epsilon(dd,c);
    dd=editProbaEpsilon->text().toDouble();
    c->globalptr->_proba_epsilon_=editProbaEpsilon->text().toDouble();
    c->globalptr->_eval_level=spinRecursEval->value();
    giac::prog_eval_level_val(spinEvalInProg->value(),c);
    giac::MAX_RECURSION_LEVEL=spinRecursProg->value();
    giac::debug_infolevel=editDebugInfo->text().toInt();
    giac::NEWTON_DEFAULT_ITERATION=spinNewton->value();
}

/**
   **********     PREFERENCES GENERAL PANEL    *************************

  **/

GeneralPanel::GeneralPanel():QWidget(){
    initGui();
}
/*():QWidget(){
    initGui();
}
*/
void GeneralPanel::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
	  retranslate();
        }
    QWidget::changeEvent(event);
}
void GeneralPanel::retranslate(){
    graphicGroup->setTitle(tr("Options graphiques"));
    labelWidth->setText(tr("Largeur (en pixel) des graphiques"));
    checkGridAttraction->setText(tr("Grille aimantée"));
    labelLanguage->setText(tr("Langue"));
    comboLanguage->setItemText(0,tr("Français"));
    comboLanguage->setItemText(1,tr("Anglais"));
    comboLanguage->setItemText(2,tr("Espagnol"));
    comboLanguage->setItemText(3,tr("Grec"));
    comboLanguage->setItemText(4,tr("Chinois"));
    policeGroup->setTitle(tr("Options polices"));
    labelMMLSize->setText(tr("Taille des polices MathML:"));
    labelUseTabCompletions->setText(tr("Utiliser Tab pour les complétions:"));
}

void GeneralPanel::initGui(){
    QVBoxLayout* mainLayout=new QVBoxLayout(this);

    graphicGroup=new QGroupBox("");
   /* graphicGroup->setStyleSheet("QGroupBox { "
                                "border: 2px solid gray; "
                               " border-radius: 3px;} ");*/
    QGridLayout* graphicGrid=new QGridLayout(graphicGroup);

    labelWidth=new QLabel("");
    editWidth=new QLineEdit(this);
    editWidth->setValidator(new QIntValidator(editWidth));
    editWidth->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    QLabel* labelXMin=new QLabel(tr("xmin:"),this);
    editXMin=new QLineEdit(this);
    editXMin->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    editXMin->setValidator(new QDoubleValidator(editXMin));
    QLabel* labelXMax=new QLabel(tr("xmax:"),this);
    editXMax=new QLineEdit(this);
    editXMax->setValidator(new QDoubleValidator(editXMax));
    QLabel* labelYMin=new QLabel(tr("ymin:"),this);
    editYMin=new QLineEdit(this);
    editYMin->setValidator(new QDoubleValidator(editYMin));
    QLabel* labelYMax=new QLabel(tr("ymax:"),this);
    editYMax=new QLineEdit(this);
    editYMax->setValidator(new QDoubleValidator(editYMax));
    QLabel* labelZMin=new QLabel(tr("zmin:"),this);
    editZMin=new QLineEdit(this);
    editZMin->setValidator(new QDoubleValidator(editZMin));
    QLabel* labelZMax=new QLabel(tr("zmax:"),this);
    editZMax=new QLineEdit(this);
    editZMax->setValidator(new QDoubleValidator(editZMax));
    QLabel* labelTMin=new QLabel(tr("tmin:"),this);
    editTMin=new QLineEdit(this);
    editTMin->setValidator(new QDoubleValidator(editTMin));
    QLabel* labelTMax=new QLabel(tr("tmax:"),this);
    editTMax=new QLineEdit(this);
    editTMax->setValidator(new QDoubleValidator(editTMax));
    checkAutoScale=new QCheckBox(tr("autoscale"));
    checkGridAttraction=new QCheckBox("");

    graphicGrid->addWidget(labelWidth,0,0,1,3);
    graphicGrid->addWidget(editWidth,0,3);
    graphicGrid->addWidget(checkAutoScale,0,6,1,2);
    graphicGrid->addWidget(labelXMin,1,0);
    graphicGrid->addWidget(editXMin,1,1);
    graphicGrid->addWidget(labelXMax,1,2);
    graphicGrid->addWidget(editXMax,1,3);
    graphicGrid->addWidget(labelYMin,1,4);
    graphicGrid->addWidget(editYMin,1,5);
    graphicGrid->addWidget(labelYMax,1,6);
    graphicGrid->addWidget(editYMax,1,7);
    graphicGrid->addWidget(labelZMin,2,0);
    graphicGrid->addWidget(editZMin,2,1);
    graphicGrid->addWidget(labelZMax,2,2);
    graphicGrid->addWidget(editZMax,2,3);
    graphicGrid->addWidget(labelTMin,2,4);
    graphicGrid->addWidget(editTMin,2,5);
    graphicGrid->addWidget(labelTMax,2,6);
    graphicGrid->addWidget(editTMax,2,7);
    graphicGrid->addWidget(checkGridAttraction,3,0,2,1);
    graphicGrid->setSizeConstraint(QLayout::SetFixedSize);

    labelLanguage=new QLabel("",this);
    comboLanguage=new QComboBox(this);
    comboLanguage->addItem(QIcon(":/images/french.png"),"");
    comboLanguage->addItem(QIcon(":/images/english.png"),"");
    comboLanguage->addItem(QIcon(":/images/spain.png"),"");
    comboLanguage->addItem(QIcon(":/images/greece.png"),"");
    comboLanguage->addItem(QIcon(":/images/china.png"),"");

    QWidget* langPanel=new QWidget(this);
    QHBoxLayout* hLayout=new QHBoxLayout(langPanel);
    hLayout->addWidget(labelLanguage,Qt::AlignLeft);
    hLayout->addWidget(comboLanguage,Qt::AlignLeft);
    hLayout->setSizeConstraint(QLayout::SetFixedSize);
    langPanel->setLayout(hLayout);

    policeGroup=new QGroupBox("",this);
    QGridLayout* policeGrid=new QGridLayout(policeGroup);

    labelMMLSize=new QLabel(tr("Taille des polices MathML:"),this);
    labelUseTabCompletions=new QLabel("",this);
    
    editMMLSize=new QSpinBox(this);
    editMMLSize->setRange(8,40);
    policeGrid->addWidget(editMMLSize,0,1);
    editMMLSize->adjustSize();
    editMMLSize->setValue(Config::mml_fontsize);
    policeGrid->addWidget(labelMMLSize,0,0);
    policeGrid->setSizeConstraint(QLayout::SetFixedSize);
    checkUseTabCompletions=new QCheckBox("");
    policeGrid->addWidget(labelUseTabCompletions,0,2);
    policeGrid->addWidget(checkUseTabCompletions,0,3);

    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(langPanel);
    mainLayout->addWidget(policeGroup);
    policeGroup->setLayout(policeGrid);
    mainLayout->addWidget(graphicGroup);
    graphicGroup->setLayout(graphicGrid);
    setLayout(mainLayout);
    retranslate();
}

void GeneralPanel::initValue(){
    editWidth->setText(QString::number(Config::graph_width));
    comboLanguage->setCurrentIndex(Config::language);
    editXMin->setText(QString::number(giac::gnuplot_xmin));
    editXMax->setText(QString::number(giac::gnuplot_xmax));
    editYMin->setText(QString::number(giac::gnuplot_ymin));
    editYMax->setText(QString::number(giac::gnuplot_ymax));
    editZMin->setText(QString::number(giac::gnuplot_zmin));
    editZMax->setText(QString::number(giac::gnuplot_zmax));
    editTMin->setText(QString::number(giac::gnuplot_tmin));
    editTMax->setText(QString::number(giac::gnuplot_tmax));
    checkAutoScale->setChecked(giac::autoscale);
    checkGridAttraction->setChecked(Config::gridAttraction);
    checkUseTabCompletions->setChecked(Config::useTabCompletions);

}
void GeneralPanel::apply(){
    Config::language=comboLanguage->currentIndex();
    int w=editWidth->text().toInt();
    if ((w>100) && (w < 1000)) Config::graph_width=w;
    else w=400;
    double min=editXMin->text().toDouble();
    double max=editXMax->text().toDouble();
    if (max>min) {

        giac::gnuplot_xmin=min;
        giac::gnuplot_xmax=max;
    }
    else {
        giac::gnuplot_xmin=-5;
        giac::gnuplot_xmax=5;
    }
    min=editYMin->text().toDouble();
    max=editYMax->text().toDouble();
    if (max>min) {
        giac::gnuplot_ymin=min;
        giac::gnuplot_ymax=max;
    }
    else {
        giac::gnuplot_ymin=-5;
        giac::gnuplot_ymax=5;
    }
    min=editZMin->text().toDouble();
    max=editZMax->text().toDouble();
    if (max>min) {
        giac::gnuplot_zmin=min;
        giac::gnuplot_zmax=max;
    }
    else {
        giac::gnuplot_zmin=-5;
        giac::gnuplot_zmax=5;
    }
    min=editTMin->text().toDouble();
    max=editTMax->text().toDouble();
    if (max>min) {
        giac::gnuplot_tmin=min;
        giac::gnuplot_tmax=max;
    }
    else {
        giac::gnuplot_tmin=-6;
        giac::gnuplot_tmax=6;
    }
    giac::autoscale=checkAutoScale->isChecked();
    Config::gridAttraction=checkGridAttraction->isChecked();
    Config::useTabCompletions=checkUseTabCompletions->isChecked();
    Config::mml_fontsize=editMMLSize->text().toInt();
}
/**
 *************** PREFERENCES INTERACTIVE 2D
 ****
 ***/
Interactive2dPanel::Interactive2dPanel():QWidget(){
    initGui();
}
void Interactive2dPanel::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}
void Interactive2dPanel::retranslate(){
    autovar2dGroup->setTitle(tr("Options variables automatiques"));
    labelGeoVarPrefix->setText(tr("Prefixe ajouté aux variables nommées automatiquement"));
}
void Interactive2dPanel::initGui(){
    QVBoxLayout* mainLayout=new QVBoxLayout(this);
    autovar2dGroup=new QGroupBox("",this);
    QGridLayout* autovar2dGrid=new QGridLayout(autovar2dGroup);
    labelGeoVarPrefix=new QLabel("",this);
    editGeoVarPrefix=new QLineEdit(this);
    editGeoVarPrefix->setValidator(new QRegExpValidator(QRegExp("[A-Za-z]+[A-Za-z0-9_\-]*"),editGeoVarPrefix));
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    autovar2dGrid->addWidget(editGeoVarPrefix,0,1);
    editGeoVarPrefix->adjustSize();
    autovar2dGrid->addWidget(labelGeoVarPrefix,0,0);
    //autovar2dGrid->setSizeConstraint(QLayout::SetFixedSize);

    /*
    retranslate();
    mainLayout->addWidget(autovar2dGroup);//obsolete
    */
    setLayout(mainLayout);
}

void Interactive2dPanel::initValue(){
  editGeoVarPrefix->setText(Config::GeoVarPrefix);
}
void Interactive2dPanel::apply(){
  Config::GeoVarPrefix=editGeoVarPrefix->text();
}


/**
   **********     PREFERENCES DIALOG BOX    *************************

  **/


PrefDialog::PrefDialog(MainWindow * parent):QDialog(parent){
    mainWindow=parent;
    setVisible(false);
    initGui();
}
void PrefDialog::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}
void PrefDialog::retranslate(){
    setWindowTitle(tr("Configuration de QCAS"));
    generalItem->setText(tr("Général"));
    interactive2dItem->setText(tr("Géométrie 2D"));
    spreadItem->setText(tr("Tableur"));
    cancelButton->setText(tr("Annuler"));

}
void PrefDialog::initGui(){


    generalPanel=new GeneralPanel();
    casPanel=new CasPanel(mainWindow);
    spreadSheetPanel=new QWidget(this);
    interactive2dPanel=new Interactive2dPanel;

    stackWidget=new QStackedWidget;
    stackWidget->addWidget(generalPanel);
    stackWidget->addWidget(casPanel);
    stackWidget->addWidget(interactive2dPanel);
    stackWidget->addWidget(spreadSheetPanel);


    listWidget=new QListWidget;
    listWidget->setIconSize(QSize(48,48));
    listWidget->setResizeMode(QListView::Adjust);
    listWidget->setFlow(QListView::LeftToRight);
    listWidget->setViewMode(QListView::IconMode);
//    listWidget->setUniformItemSizes(true);
    generalItem=new QListWidgetItem(QIcon(":/images/general.png"),"");
    generalItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    generalItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    QListWidgetItem* casItem=new QListWidgetItem(QIcon(":/images/cas.png"),tr("CAS"));
    casItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    casItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    spreadItem=new QListWidgetItem(QIcon(":/images/spreadsheet.png"),"");
    spreadItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    spreadItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    interactive2dItem=new QListWidgetItem(QIcon(":/images/line.png"),"");
    interactive2dItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    interactive2dItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    listWidget->addItem(generalItem);
    listWidget->addItem(casItem);
    listWidget->addItem(interactive2dItem);
//    listWidget->addItem(spreadItem);
    listWidget->adjustSize();
    listWidget->setCurrentRow(0);
    okButton=new QPushButton(tr("Ok"));
    okButton->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    cancelButton=new QPushButton("");
    cancelButton->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    QWidget* buttonsPanel=new QWidget;
    QHBoxLayout* hbox=new QHBoxLayout;
    hbox->addWidget(okButton,1,Qt::AlignRight);
    hbox->addWidget(cancelButton,0,Qt::AlignRight);
    buttonsPanel->setLayout(hbox);

    QVBoxLayout* vbox=new QVBoxLayout;
    listWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
    listWidget->setMaximumHeight(80);
    vbox->addWidget(listWidget);
    vbox->addWidget(stackWidget);
    vbox->addWidget(buttonsPanel);
    vbox->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(vbox);
    connect(listWidget,SIGNAL(currentRowChanged(int)),stackWidget,SLOT(setCurrentIndex(int)));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(okButton,SIGNAL(clicked()),this,SLOT(apply()));

    retranslate();
}
void PrefDialog::apply(){
    generalPanel->apply();
    casPanel->apply();
    interactive2dPanel->apply();
    mainWindow->retranslateInterface(Config::language);
    giac::set_language(Config::giaclanguage,mainWindow->getContext());//french is 1 in giac.
    close();

}

void PrefDialog::initValue(){
    generalPanel->initValue();
    casPanel->initValue();
    interactive2dPanel->initValue();

}
