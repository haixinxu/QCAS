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

#include "WizardAlgo.h"
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
#include "../config.h"

WizardAlgo::WizardAlgo(MainWindow *parent,bool scol)
    :QWidget(parent){
    mainWindow=parent;
    scolar=scol;
    createGui();
}

void WizardAlgo::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}
void WizardAlgo::retranslate(){
   if(scolar){
        list->setItemText(0,tr("fonction...ffonction"));
        list->setItemText(1,tr("si alors (sinon)"));
        list->setItemText(2,tr("pour...faire...fpour"));
        list->setItemText(3,tr("tantque...faire...ftantque"));
    }
    else{
       list->setItemText(0,tr("Nouvelle fonction: ")+"f(  ):={   }");
       list->setItemText(1,tr("Test: ")+"if(  ){   }...");
       list->setItemText(2,tr("Boucle: ")+"for( ; ; ){   }");
       list->setItemText(3,tr("Boucle: ")+"while(  ){   }");
    }
}
void WizardAlgo::createGui(){
    pages=new QStackedWidget;
    list=new QComboBox;

    if(!scolar){
        list->addItem("");
        FuncPanel *funcPanel=new FuncPanel(this);
        list->addItem("");
        ForPanel *forPanel=new ForPanel(this);
        list->addItem("");
        WhilePanel *whilePanel=new WhilePanel(this);
        list->addItem("");
        IfPanel *ifPanel=new IfPanel(this);

        pages->addWidget(funcPanel);
        pages->addWidget(ifPanel);
        pages->addWidget(forPanel);
        pages->addWidget(whilePanel);
    }
    else{
        list->addItem("");
        FunctionPanel *functionPanel=new FunctionPanel(this);
        list->addItem("");
        TestPanel *testPanel=new TestPanel(this);
        list->addItem("");
        LoopPanel *loopPanel=new LoopPanel(this);
        list->addItem("");
        TantquePanel *tantquePanel=new TantquePanel(this);

        pages->addWidget(functionPanel);
        pages->addWidget(testPanel);
        pages->addWidget(loopPanel);
        pages->addWidget(tantquePanel);
    }

    QVBoxLayout *layout=new QVBoxLayout;
    layout->addWidget(list);
    layout->addWidget(pages);
    this->setLayout(layout);
    connect(list,SIGNAL(activated(int)),pages,SLOT(setCurrentIndex(int)));

    retranslate();
}
void WizardAlgo::sendCommand(const QString &s){
    mainWindow->sendText(s);
}

AlgoTabChild::AlgoTabChild(QWidget *parent):QWidget(parent){
}

/**************************************************************
 *                       Functions Panel
 **************************************************************/

void FunctionPanel::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}
void FunctionPanel::retranslate(){
    prototype->setToolTip(tr("<center><b>fonction</b></center>Saisir le nom et les arguments<br> <b>Exemple1: </b> f(a,b)<br><b>Exemple2: </b> g()"));
    labelProto->setText(tr("<b>fonction</b>"));
    labelProto->setToolTip(prototype->toolTip());
    varloc->setToolTip(tr("<center><b>local</b></center>Saisir les noms des variables qui ne sont utilisées qu'à l'intérieur du programme.<br>NB: Votre programme devra les initialiser.<br> <b> Exemple:</b> j,k"));
    labelVarloc->setText(tr("Variables locales:"));
    labelVarloc->setToolTip(varloc->toolTip());
    symbloc->setToolTip(tr("<center><b>supposons(    ,symbol)</b></center>Un symbole est une lettre utilisable dans un calcul mathématique. On peut utiliser une variable globale non initialisée. En revanche si l'on souhaite avoir des symboles locaux, il faut les déclarer ici.<br> <b>Exemple:</b> t,u"));
    labelSymbloc->setText(tr("Symboles locaux:"));
    labelSymbloc->setToolTip(symbloc->toolTip());
    retour->setToolTip(tr("<center><b>retourne</b></center>Objet retourné par la fonction. On peut ne rien retourner, ou retourner une liste ou une séquence.<br><b>Exemple1:</b>a,b,c<br><b>Exemple2:</b>[a,b,c]"));
    labelRetour->setText(tr("<b>retourne</b>"));
    labelRetour->setToolTip(retour->toolTip());
    instructions->setToolTip(tr("Vous pouvez si vous le souhaitez utiliser cette partie pour compléter votre programme."));
    labelEnd->setText(tr("<b>ffonction</b>"));
}

FunctionPanel::FunctionPanel(WizardAlgo *parent):AlgoTabChild(parent){
        algoPanel=parent;

        prototype=new QLineEdit;
        labelProto=new QLabel("");
        labelProto->setBuddy(prototype);

        varloc=new QLineEdit("");
        labelVarloc=new QLabel("");
        labelVarloc->setBuddy(varloc);

        symbloc=new QLineEdit("");
        labelSymbloc=new QLabel("");
        labelSymbloc->setBuddy(symbloc);


        retour=new QLineEdit("");
        labelRetour=new QLabel("");
        labelRetour->setBuddy(retour);

        labelEnd=new QLabel("");

        QPushButton *button=new QPushButton;
        button->setIcon(QIcon(":/images/right.png"));


        QGroupBox *group=new QGroupBox();
        QHBoxLayout *layout=new QHBoxLayout;
        instructions=new QPlainTextEdit(this);
        layout->addWidget(instructions);
        //group->setMaximumWidth(200);
        group->setLayout(layout);

        QGridLayout *grid=new QGridLayout();
        grid->addWidget(labelProto,0,0);
        grid->addWidget(prototype,0,1);
        grid->addWidget(labelVarloc,1,0);
        grid->addWidget(varloc,1,1);
        grid->addWidget(labelSymbloc,2,0);
        grid->addWidget(symbloc,2,1);
        grid->addWidget(group,3,0,1,2);
        grid->addWidget(labelRetour,4,0);
        grid->addWidget(retour,4,1);
        grid->addWidget(labelEnd,5,0);
        grid->addWidget(button,5,1,Qt::AlignVCenter);
        this->setLayout(grid);

        connect(button,SIGNAL(clicked()),this,SLOT(sendCommand()));

    retranslate();
   }

void FunctionPanel::sendCommand(){
    QString s;
    if(Config::language==Config::FRENCH){
        s="fonction ";
        QString tmp;
        QStringList listsymb;
        QString indent="\t";
        int i;
        s.append(prototype->text()).append(("\n"));

        tmp=varloc->text().trimmed();
        if(!tmp.isEmpty()){
            s.append(indent+"local ");
        if(!tmp.endsWith(";"))
            s.append(tmp).append(";\n");
        }

        tmp=symbloc->text().trimmed();
        if(!tmp.isEmpty()){
          listsymb=tmp.split(",");
          for(i=0;i<listsymb.length();i++){
              s.append(indent+"supposons(");//trad assume
               s.append(listsymb.at(i)).append(",symbol);\n");
           }
        }

        tmp=(instructions->document()->toPlainText()).trimmed();
        tmp.replace("\n","\n"+indent);
        if(!tmp.isEmpty()){
              s.append(indent+tmp+"\n");
        }
        s.append(indent+"\n");
        tmp=retour->text();
        if(!tmp.trimmed().isEmpty()){
            s.append(indent+"retourne ").append(tmp).append(";\n");
        }
        s.append("ffonction;\n");// trad: end_proc
    }
    else{
        s="proc ";
        QString tmp;
        QStringList listsymb;
        QString indent="\t";
        int i;
        s.append(prototype->text()).append(("\n"));

        tmp=varloc->text().trimmed();
        if(!tmp.isEmpty()){
            s.append(indent+"local ");
        if(!tmp.endsWith(";"))
            s.append(tmp).append(";\n");
        }

        tmp=symbloc->text().trimmed();
        if(!tmp.isEmpty()){
          listsymb=tmp.split(",");
          for(i=0;i<listsymb.length();i++){
              s.append(indent+"assume(");//trad assume
               s.append(listsymb.at(i)).append(",symbol);\n");
           }
        }

        tmp=(instructions->document()->toPlainText()).trimmed();
        tmp.replace("\n","\n"+indent);
        if(!tmp.isEmpty()){
              s.append(indent+tmp+"\n");
        }
        s.append(indent+"\n");
        tmp=retour->text();
        if(!tmp.trimmed().isEmpty()){
            s.append(indent+"return ").append(tmp).append(";\n");
        }
        s.append("end_proc;\n");

    }

    algoPanel->sendCommand(s);

}

/******************************************************************
 *                 Loop Panel
 ******************************************************************/
void LoopPanel::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}

void LoopPanel::retranslate(){
    varname->setToolTip(tr("<center><b>pour</b></center>Saisir le nom de la variable. (Attention, i est reservé pour le nombre complexe)<br> <b>Exemple:</b> j"));
    labelVarname->setText(tr("<b>pour</b>"));
    labelVarname->setToolTip(varname->toolTip());
    deb->setToolTip(tr("<center><b>de</b></center>Valeur de départ:<br> <b>Exemple:</b> 0"));
    labelDeb->setText(tr("<b>de</b>"));
    labelDeb->setToolTip(deb->toolTip());
    fin->setToolTip(tr("<center><b>jusque</b></center>valeur de fin"));
    labelFin->setText(tr("<b>jusque</b>"));
    labelFin->setToolTip(fin->toolTip());
    pas->setToolTip(tr("<center><b>pas</b></center>Précisez le pas s'il est différent de 1. Sinon inutile de remplir ce champ."));
    labelPas->setText(tr("<b>pas</b>"));
    labelPas->setToolTip(pas->toolTip());
    labelEnd->setText(tr("<b>fpour</b>"));

    instructions->setToolTip(tr("Vous pouvez si vous le souhaitez utiliser cette partie pour le corps de la boucle."));
    group->setTitle(tr("faire"));
}

LoopPanel::LoopPanel(WizardAlgo *parent):AlgoTabChild(parent){
        algoPanel=parent;

        varname=new QLineEdit;
        labelVarname=new QLabel("");
        labelVarname->setBuddy(varname);

        deb=new QLineEdit;
        labelDeb=new QLabel("");
        labelDeb->setBuddy(deb);

        fin=new QLineEdit;
        labelFin=new QLabel("");
        labelFin->setBuddy(varname);

        pas=new QLineEdit;
        labelPas=new QLabel("");
        labelPas->setBuddy(varname);

        labelEnd=new QLabel("");

        QPushButton *button=new QPushButton;
        button->setIcon(QIcon(":/images/right.png"));


        group=new QGroupBox();
        QHBoxLayout *layout=new QHBoxLayout;
        instructions=new QPlainTextEdit(this);
        layout->addWidget(instructions);
        //group->setMaximumWidth(200);
        group->setLayout(layout);

        QGridLayout *grid=new QGridLayout();
        grid->addWidget(labelVarname,0,0);
        grid->addWidget(varname,0,1);

        grid->addWidget(labelDeb,1,0);
        grid->addWidget(deb,1,1);
        grid->addWidget(labelFin,2,0);
        grid->addWidget(fin,2,1);
        grid->addWidget(labelPas,3,0);
        grid->addWidget(pas,3,1);
        grid->addWidget(group,4,0,1,2);
        grid->addWidget(labelEnd,5,0);
        grid->addWidget(button,5,1,Qt::AlignVCenter);
        this->setLayout(grid);

        connect(button,SIGNAL(clicked()),this,SLOT(sendCommand()));

    retranslate();


}

void LoopPanel::sendCommand(){
    QString s;
    if(Config::language==Config::FRENCH){
        s="pour ";
        QString tmp;
        QString indent="\t";

        s.append(varname->text());
        s.append(" de ").append(deb->text());
        s.append(" jusque ").append(fin->text());
        tmp=pas->text();
        if(!tmp.trimmed().isEmpty()){
            s.append(" pas "+tmp+" ");
        }
        s.append(" faire\n");
        tmp=instructions->document()->toPlainText();
        tmp.replace("\n","\n"+indent);
        if(!tmp.trimmed().isEmpty()){
            s.append(indent+tmp+"\n");
        }
        else(s.append("\n"));
            s.append("fpour;\n");
    }
    else{
        s="for ";
        QString tmp;
        QString indent="\t";

        s.append(varname->text());
        s.append(" from ").append(deb->text());
        s.append(" to ").append(fin->text());
        tmp=pas->text();
        if(!tmp.trimmed().isEmpty()){
            s.append(" by "+tmp+" ");
        }
        s.append(" do\n");
        tmp=instructions->document()->toPlainText();
        tmp.replace("\n","\n"+indent);
        if(!tmp.trimmed().isEmpty()){
            s.append(indent+tmp+"\n");
        }
        else(s.append("\n"));
            s.append("end_do;\n");

    }
    algoPanel->sendCommand(s);
}

/**********************************************************
 *                      Test Panel
 *********************************************************/

void TestPanel::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}

void TestPanel::retranslate(){
    condi->setToolTip(tr("Saisir une condition.<br> <b>Exemple1:</b> j == 2<br><b>Exemple2:</b> j != 2<br><b>Exemple3:</b> j &lt;= 2 ou j&gt;3<br>"));
    labelCondi->setText(tr("<b>si</b>"));
    labelCondi->setToolTip(condi->toolTip());
    groupalors->setTitle(tr("alors"));
    instruction1->setToolTip(tr("Vous pouvez (si vous souhaitez le faire maintenant), remplir dans ce cadre les instructions à éxécuter si la condition est satisfaite.<br><b>Exemple:</b>a:=a+1;\na:=a*a;"));
    groupsinon->setTitle(tr("sinon"));
    instruction2->setToolTip(tr("Il faut modifier cette entrée pour obtenir un <em>sinon</em><br><b>Exemple:</b><br>a:=a+1;<br>a:=a*a;"));
    labelEnd->setText(tr("<b>fsi</b>"));
}

TestPanel::TestPanel(WizardAlgo *parent):AlgoTabChild(parent){

    algoPanel=parent;

    condi=new QLineEdit;
    labelCondi=new QLabel("");
    labelCondi->setBuddy(condi);

    QPushButton *button=new QPushButton;
    button->setIcon(QIcon(":/images/right.png"));

    labelEnd=new QLabel("");

    groupalors=new QGroupBox();
    QHBoxLayout *layoutalors=new QHBoxLayout;
    instruction1=new QPlainTextEdit(this);
    layoutalors->addWidget(instruction1);
    groupalors->setLayout(layoutalors);

    groupsinon=new QGroupBox();
    QHBoxLayout *layoutsinon=new QHBoxLayout;
    instruction2=new QPlainTextEdit(this);
    layoutsinon->addWidget(instruction2);
    groupsinon->setLayout(layoutsinon);

    QGridLayout *grid=new QGridLayout();
    grid->addWidget(labelCondi,0,0);
    grid->addWidget(condi,0,1);


    grid->addWidget(groupalors,1,0,1,2);
    grid->addWidget(groupsinon,2,0,1,2);
    grid->addWidget(labelEnd,3,0);
    grid->addWidget(button,3,1,Qt::AlignVCenter);
    this->setLayout(grid);

    connect(button,SIGNAL(clicked()),this,SLOT(sendCommand()));

    retranslate();

}

void TestPanel::sendCommand(){
    QString s;
    if(Config::language==Config::FRENCH){
        s="si ";
        QString tmp;
        QString indent="\t";
        s.append(condi->text()).append(" alors\n");
        tmp=instruction1->document()->toPlainText();
        tmp.replace("\n","\n"+indent);
        if(!tmp.isEmpty()){
            s.append(indent+tmp+"\n");
        }
        else{
            s.append("\n");
        }
        tmp=instruction2->document()->toPlainText();
        tmp.replace("\n","\n"+indent);
        if(!tmp.isEmpty()){
            s.append("sinon\n"+indent+tmp+"\n");
        }
        s.append("fsi;\n");
    }
    else{
        s="if ";
        QString tmp;
        QString indent="\t";
        s.append(condi->text()).append(" then\n");
        tmp=instruction1->document()->toPlainText();
        tmp.replace("\n","\n"+indent);
        if(!tmp.isEmpty()){
            s.append(indent+tmp+"\n");
        }
        else{
            s.append("\n");
        }
        tmp=instruction2->document()->toPlainText();
        tmp.replace("\n","\n"+indent);
        if(!tmp.isEmpty()){
            s.append("else\n"+indent+tmp+"\n");
        }
        s.append("end_if;\n");
    }
    algoPanel->sendCommand(s);
}


/*****************************************************************
 *                  Panel Tanque
 *****************************************************************/

void TantquePanel::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}

void TantquePanel::retranslate(){
    condi->setToolTip(tr("Saisir une condition.<br> <b>Exemple1:</b> j == 1<br><b>Exemple1:</b> j != 2<br><b>Exemple3:</b> j &lt;= 2 ou j&gt;3<br>"));
    labelCondi->setText(tr("<b>tantque</b>"));
    group->setTitle(tr("faire"));
    instructions->setToolTip(tr("Vous pouvez  (si vous souhaitez le faire maintenant), remplir dans ce cadre les instructions à éxécuter tantque la condition est satisfaite.<br><b>Exemple:</b>a:=a+1;\na:=a*a;"));
    labelEnd->setText(tr("<b>ftantque</b>"));
    labelCondi->setToolTip(condi->toolTip());
}

TantquePanel::TantquePanel(WizardAlgo *parent):AlgoTabChild(parent){

    algoPanel=parent;

    condi=new QLineEdit;
    labelCondi=new QLabel("");
    labelCondi->setBuddy(condi);

    QPushButton *button=new QPushButton;
    button->setIcon(QIcon(":/images/right.png"));

    labelEnd=new QLabel("");

    group=new QGroupBox();
    QHBoxLayout *layout=new QHBoxLayout;
    instructions=new QPlainTextEdit(this);
    layout->addWidget(instructions);
    group->setLayout(layout);

    QGridLayout *grid=new QGridLayout();
    grid->addWidget(labelCondi,0,0);
    grid->addWidget(condi,0,1);


    grid->addWidget(group,1,0,1,2);
    grid->addWidget(labelEnd,2,0);
    grid->addWidget(button,2,1,Qt::AlignVCenter);
    this->setLayout(grid);

    connect(button,SIGNAL(clicked()),this,SLOT(sendCommand()));

    retranslate();
}

void TantquePanel::sendCommand(){
    QString s;
    if(Config::language==Config::FRENCH){

        s="tantque ";
        QString tmp;
        QString indent="\t";
        s.append(condi->text()).append(" faire\n");
        tmp=instructions->document()->toPlainText();
        tmp.replace("\n","\n"+indent);
        if(!tmp.isEmpty()){
            s.append(indent+tmp+"\n");
        }
        else{
            s.append("\n");
        }

        s.append("ftantque;\n");
        }
    else{

        s="while ";
        QString tmp;
        QString indent="\t";
        s.append(condi->text()).append(" do\n");
        tmp=instructions->document()->toPlainText();
        tmp.replace("\n","\n"+indent);
        if(!tmp.isEmpty()){
            s.append(indent+tmp+"\n");
        }
        else{
            s.append("\n");
        }

        s.append("end_do;\n");

    }
    algoPanel->sendCommand(s);
}

/*******************************************************
 *               Function Panel C style
 ********************************************************/
void FuncPanel::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}

void FuncPanel::retranslate(){
    func->setToolTip(tr("Saisir le nom de la fonction suivi de ses arguments entre parenthèses et séparés par des virgules.<br><b>Exemple1:</b> f(x,y)<br><b>Exemple2: </b> g()"));
    labelFunc->setText(tr("fonction+args:"));
    labelFunc->setToolTip(func->toolTip());
    retour->setToolTip(tr("<center><b>return ??</b></center>Objet retourné par la fonction. On peut ne rien retourner, ou retourner une liste ou une séquence.<br><b>Exemple1:</b>a,b,c<br><b>Exemple2:</b>[a,b,c]"));
    labelRetour->setToolTip(retour->toolTip());
    varloc->setToolTip(tr("<center><b>local ?? ;</b></center>Saisir les noms des variables qui ne sont utilisées qu'à l'intérieur du programme.<br>NB: Votre programme devra les initialiser.<br> <b> Exemple:</b> j,k"));
    labelVarloc->setToolTip(varloc->toolTip());
    symbloc->setToolTip(tr("<center><b>assume(  ?? ,symbol);</b></center>Un symbole est une lettre utilisable dans un calcul mathématique. On peut utiliser une variable globale non initialisée. En revanche si l'on souhaite avoir des symboles locaux, il faut les déclarer ici.<br> <b>Exemple:</b> t,u"));
    labelSymbloc->setText(tr("Symboles locaux:"));
    labelSymbloc->setToolTip(symbloc->toolTip());

    instructions->setToolTip(tr(" Si vous souhaitez le faire maintenant, vous pouvez utiliser cette partie pour le corps de la fonction."));

}


FuncPanel::FuncPanel(WizardAlgo *parent):AlgoTabChild(parent){
    algoPanel=parent;

    func=new QLineEdit;
    labelFunc=new QLabel("");
    labelFunc->setBuddy(func);
    labelEnd=new QLabel("<center><b>}</b></center>");

    QLabel *f1=new QLabel("<b>:={</b>");
    QLabel *f2=new QLabel("<b>;</b>");

    varloc=new QLineEdit("");
    labelVarloc=new QLabel("<b>local</b>");
    labelVarloc->setBuddy(varloc);


    symbloc=new QLineEdit("");
    labelSymbloc=new QLabel("");
    labelSymbloc->setBuddy(symbloc);

    QPushButton *button=new QPushButton;
    button->setIcon(QIcon(":/images/right.png"));


    group=new QGroupBox();

    QHBoxLayout *layout=new QHBoxLayout;
    instructions=new QPlainTextEdit(this);
    layout->addWidget(instructions);
    group->setLayout(layout);
    //group->setTitle(":={");

    retour=new QLineEdit("");
    labelRetour=new QLabel("<b>return</b>");
    labelRetour->setBuddy(retour);

    QGridLayout *grid=new QGridLayout();

    grid->addWidget(labelFunc,0,0);
    grid->addWidget(func,0,1);
    grid->addWidget(f1,0,2);
    grid->addWidget(labelVarloc,1,0);
    grid->addWidget(varloc,1,1);
    grid->addWidget(f2,1,2);
    grid->addWidget(labelSymbloc,2,0);
    grid->addWidget(symbloc,2,1);
    grid->addWidget(group,3,0,1,2);
    grid->addWidget(labelRetour,4,0);
    grid->addWidget(retour,4,1);
    grid->addWidget(labelEnd,5,0);
    grid->addWidget(button,5,1,Qt::AlignVCenter);
    this->setLayout(grid);

    connect(button,SIGNAL(clicked()),this,SLOT(sendCommand()));

    retranslate();


}

void FuncPanel::sendCommand(){
    QString s;
    QString tmp;
    QString indent="\t";
    QStringList listsymb;
    int i;

    tmp=func->text();
    if(tmp.trimmed().endsWith(";"))
        tmp.remove(";");
    s=tmp.append(":={\n");

    tmp=varloc->text().trimmed();
    if(!tmp.isEmpty()){
      s.append(indent+"local ");
      if(!tmp.endsWith(";"))
          s.append(tmp).append(";\n");
    }

    tmp=symbloc->text().trimmed();
    if(!tmp.isEmpty()){
        listsymb=tmp.split(",");
        for(i=0;i<listsymb.length();i++){
            s.append(indent+"assume(");//trad assume
            s.append(listsymb.at(i)).append(",symbol);\n");
        }
    }

    tmp=instructions->document()->toPlainText();
    tmp.replace("\n","\n"+indent);
    tmp=tmp.trimmed();
    if(!tmp.isEmpty()){
        if(!(tmp.endsWith(";") || tmp.endsWith(":;")))
            tmp.append(";");
        s.append(indent+tmp+"\n");
    }
    else(s.append("\n"));
    tmp=retour->text();
    if(!tmp.trimmed().isEmpty()){
        s.append(indent+"return "+tmp+"\n");
    }

    s.append("};\n");
    algoPanel->sendCommand(s);
}


/**********************************************************
 *                      If Panel C Style
 *********************************************************/

void IfPanel::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}

void IfPanel::retranslate(){
    condi->setToolTip(tr("<center><b>if( ?? ){...</b></center>Saisir une condition.<br> <b>Exemple1:</b> j == 2<br><b>Exemple2:</b> j != 2<br><b>Exemple3:</b> j &lt;= 2 || j&gt;3<br><b>ou: </b><code>||</code> (synonyme: <code>or</code>)<br><b>et: </b><code>&&</code> (synonyme: <code>and</code>)<br><b>negation</b><code>!</code> (synonyme: <code>not</code>)"));
    labelCondi->setToolTip(condi->toolTip());
    instruction1->setToolTip(tr("Vous pouvez (si vous souhaitez le faire maintenant), remplir dans ce cadre les instructions à éxécuter si la condition est satisfaite.<br><b>Exemple:</b>a:=a+1;\na:=a*a;"));
    instruction2->setToolTip(tr("Facultatif. Il faut modifier cette entrée pour obtenir un <em>else{</em><br><b>Exemple:</b><br>a:=a+1;<br>a:=a*a;"));
    groupsinon->setToolTip(instruction2->toolTip());
}

IfPanel::IfPanel(WizardAlgo *parent):AlgoTabChild(parent){

    algoPanel=parent;

    condi=new QLineEdit;
    labelCondi=new QLabel("<b>if(</b>");
    labelCondi->setBuddy(condi);
    QLabel *pt1=new QLabel("<b>){</b>");

    QPushButton *button=new QPushButton;
    button->setIcon(QIcon(":/images/right.png"));

    labelEnd=new QLabel("<b>}</b>");

    groupalors=new QGroupBox();
    QHBoxLayout *layoutalors=new QHBoxLayout;
    instruction1=new QPlainTextEdit(this);
    layoutalors->addWidget(instruction1);
    groupalors->setLayout(layoutalors);

    groupsinon=new QGroupBox();
    QHBoxLayout *layoutsinon=new QHBoxLayout;
    groupsinon->setTitle("else{");
    instruction2=new QPlainTextEdit(this);
    layoutsinon->addWidget(instruction2);
    groupsinon->setLayout(layoutsinon);
    QLabel *pt2=new QLabel("<b>}</b>");


    QGridLayout *grid=new QGridLayout();
    grid->addWidget(labelCondi,0,0);
    grid->addWidget(condi,0,1);
    grid->addWidget(pt1,0,2);

    grid->addWidget(groupalors,1,0,1,2);
    grid->addWidget(pt2,1,2,Qt::AlignBottom);
    grid->addWidget(groupsinon,2,0,1,2);
    grid->addWidget(labelEnd,3,0);
    grid->addWidget(button,3,1,Qt::AlignVCenter);
    this->setLayout(grid);

    connect(button,SIGNAL(clicked()),this,SLOT(sendCommand()));

    retranslate();

}

void IfPanel::sendCommand(){
    QString s="if(";
    QString tmp;
    QString indent="\t";
    s.append(condi->text()).append(" ){\n");
    tmp=instruction1->document()->toPlainText();
    tmp.replace("\n","\n"+indent);
    if(!tmp.isEmpty()){
        s.append(indent+tmp+"\n");
    }
    else{
        s.append("\n");
    }
    tmp=instruction2->document()->toPlainText();
    tmp.replace("\n","\n"+indent);
    if(!tmp.isEmpty()){
        s.append("}\nelse{\n"+indent+tmp+"\n");
    }
    s.append("};\n");
    algoPanel->sendCommand(s);
}



/******************************************************************
 *                 For Panel C style
 ******************************************************************/
void ForPanel::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}

void ForPanel::retranslate(){
    ini->setToolTip(tr("<center><b>for(??;...;...){...}</b></center>Saisir l'initialisation de la variable de boucle. (Attention, i est reservé pour le nombre complexe)<br> <b>Exemple:</b> j:=0"));
    labelIni->setText(tr("Initialisation:"));
    labelIni->setToolTip(ini->toolTip());
    arret->setToolTip(tr("<center><b>for(...;??;...){...}</b></center>Condition pour continuer la boucle<br> <b>Exemple:</b>j&lt;10 (pour continuer tantque j&lt;10)"));
    labelArret->setText(tr("Condition:"));
    labelArret->setToolTip(arret->toolTip());
    incr->setToolTip(tr("<center><b>for(...;...;??){...}</b></center>instruction pour modifier la variable de boucle.<br><b>Exemple1</b>: j++<br><b>Exemple2:</b>j:=j+2"));
    labelIncr->setText(tr("Incrémentation:"));
    labelIncr->setToolTip(incr->toolTip());
    instructions->setToolTip(tr("Vous pouvez (si vous le souhaitez le faire maintenant) utiliser cette partie pour le corps de la boucle."));

}

ForPanel::ForPanel(WizardAlgo *parent):AlgoTabChild(parent){
        algoPanel=parent;

        ini=new QLineEdit;
        labelIni=new QLabel("");
        labelIni->setBuddy(ini);

        arret=new QLineEdit;
        labelArret=new QLabel("");
        labelArret->setBuddy(arret);

        incr=new QLineEdit;
        labelIncr=new QLabel("");
        labelIncr->setBuddy(incr);

        labelEnd=new QLabel("<center><b>}</b></center>");

        QPushButton *button=new QPushButton;
        button->setIcon(QIcon(":/images/right.png"));


        group=new QGroupBox();

        QHBoxLayout *layout=new QHBoxLayout;
        instructions=new QPlainTextEdit(this);
        layout->addWidget(instructions);
        group->setLayout(layout);
        group->setTitle("...){");
        QLabel *labelTitre=new QLabel("<b><center>for(...</b><center>");
        QLabel *pv1=new QLabel("<b>;</b>");
        QLabel *pv2=new QLabel("<b>;</b>");
        QLabel *pv3=new QLabel("<b>;</b>");


        QGridLayout *grid=new QGridLayout();

        grid->addWidget(labelTitre,0,0);
        grid->addWidget(labelIni,1,0);
        grid->addWidget(ini,1,1);
        grid->addWidget(pv1,1,2);
        grid->addWidget(labelArret,2,0);
        grid->addWidget(arret,2,1);
        grid->addWidget(pv2,2,2);
        grid->addWidget(labelIncr,3,0);
        grid->addWidget(incr,3,1);
        grid->addWidget(pv3,3,2);
        grid->addWidget(group,4,0,1,2);
        grid->addWidget(labelEnd,5,0);
        grid->addWidget(button,5,1,Qt::AlignVCenter);
        this->setLayout(grid);

        connect(button,SIGNAL(clicked()),this,SLOT(sendCommand()));

        retranslate();

}

void ForPanel::sendCommand(){

    QString s="for( ";
    QString tmp;
    QString indent="\t";

    tmp=ini->text();
    if(tmp.trimmed().endsWith(";"))
        tmp.remove(";");
    s.append(tmp+"; ");
    tmp=arret->text();
    if(tmp.trimmed().endsWith(";"))
        tmp.remove(";");
    s.append(tmp+"; ");
    tmp=incr->text();
    if(tmp.trimmed().endsWith(";"))
        tmp.remove(";");
    s.append(tmp+"){\n");

    tmp=instructions->document()->toPlainText();
    tmp.replace("\n","\n"+indent);
    if(!tmp.trimmed().isEmpty()){
        s.append(indent+tmp+"\n");
    }
    else(s.append("\n"));
    s.append("};\n");
    algoPanel->sendCommand(s);
}

/*****************************************************************
 *                  Panel While C Style
 *****************************************************************/

void WhilePanel::changeEvent(QEvent *event){
    if(event->type() == QEvent::LanguageChange)
        {
            retranslate();
        }
    QWidget::changeEvent(event);
}

void WhilePanel::retranslate(){
    condi->setToolTip(tr("Saisir une condition.<br> <b>Exemple1:</b> j == 1<br><b>Exemple2:</b> j != 2<br><b>Exemple3:</b> j &lt;= 2 ou j&gt;3<br>"));
    labelCondi->setToolTip(condi->toolTip());
    instructions->setToolTip(tr("Vous pouvez  (si vous souhaitez le faire maintenant), remplir dans ce cadre les instructions à éxécuter tantque la condition est satisfaite.<br><b>Exemple:</b>a:=a+1;\na:=a*a;"));
    labelCondi->setToolTip(condi->toolTip());
}

WhilePanel::WhilePanel(WizardAlgo *parent):AlgoTabChild(parent){

    algoPanel=parent;

    condi=new QLineEdit;
    labelCondi=new QLabel("<b>while(</b>");
    labelCondi->setBuddy(condi);
    QLabel *pt1=new QLabel("<b>){</b>");

    QPushButton *button=new QPushButton;
    button->setIcon(QIcon(":/images/right.png"));

    labelEnd=new QLabel("<b>}</b>");

    group=new QGroupBox();
    QHBoxLayout *layout=new QHBoxLayout;
    instructions=new QPlainTextEdit(this);
    layout->addWidget(instructions);
    group->setLayout(layout);

    QGridLayout *grid=new QGridLayout();
    grid->addWidget(labelCondi,0,0);
    grid->addWidget(condi,0,1);
    grid->addWidget(pt1,0,2);


    grid->addWidget(group,1,0,1,2);
    grid->addWidget(labelEnd,2,0);
    grid->addWidget(button,2,1,Qt::AlignVCenter);
    this->setLayout(grid);

    connect(button,SIGNAL(clicked()),this,SLOT(sendCommand()));

    retranslate();
}

void WhilePanel::sendCommand(){
    QString s="while( ";
    QString tmp;
    QString indent="\t";
    s.append(condi->text()).append(" ){\n");
    tmp=instructions->document()->toPlainText();
    tmp.replace("\n","\n"+indent);
    if(!tmp.isEmpty()){
        s.append(indent+tmp+"\n");
    }
    else{
        s.append("\n");
    }

    s.append("};\n");
    algoPanel->sendCommand(s);
}
