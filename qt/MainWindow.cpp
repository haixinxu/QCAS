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

#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>
#include <QSplitter>
#include <QCloseEvent>
#include <QtDebug>
#include <QLabel>
#include <QToolBox>
#include <QSettings>
#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStackedWidget>
#include <QStatusBar>
#include <QMenu>
#include <QScrollBar>
#include <QPlainTextEdit>
#include <QToolBar>
#include "MainWindow.h"
#include "gui/FormalSheet.h"
#include "gui/WizardEquation.h"
#include "gui/WizardMatrix.h"
#include "gui/WizardCatalog.h"
#include "gui/WizardAlgo.h"
#include "gui/CentralTabWidget.h"
#include "gui/FormalLine.h"
#include "gui/FormalLineWidgets.h"
#include <QToolButton>
#include <QTime>
#include <QTranslator>
#include <QLocale>

//#include "gui/FormalSheet.h"
//#include <Qsci/qsciscintilla.h>
//#include <Qsci/qscilexerjava.h>

#include <QUrl>
#include <QGroupBox>
# include <QDomDocument>
# include <QDomText>
#include "output.h"
#include "config.h"

#include "CasManager.h"
#include <QCompleter>

#include <giac/giac.h>
#include "gui/prefdialog.h"

#include "qt/gui/plotfunctiondialog.h"
/** MainWindow constructor

  **/

#if QT_VERSION < 0x050000
#ifdef Q_WS_MACX
#define STRCTRL "Cmd"
#else
#define STRCTRL "Ctrl"
#endif
#else
#ifdef Q_OS_MACX
#define STRCTRL "Cmd"
#else
#define STRCTRL "Ctrl"
#endif
#endif


MainWindow::MainWindow(){

    displayTimeAfterProcess=true;
    time=new QTime;

    lastgiacmessage="";

    history=new QStringList();

    isevaluatingall=false;

    commandInfo=new CommandInfo;
    taskProperties.firstPrintMessage=true;

    QString lang = QLocale::system().name().left(2);
    lang=lang.toLower();
    QStringList availablemenulang;
    availablemenulang << "fr" << "en" << "el" << "es" << "sp" << "zh";

    // if the language has no translation we default to en
    if (!availablemenulang.contains(lang)){
      lang="en";
    }

    Config::GiacHtmlLanguage=lang;
    //   qInfo()<<lang;
    translator =new QTranslator(0);


    if (lang == "en") {
        translator->load(QString(":/lang/qcas_")+lang);
        qApp->installTranslator(translator);
        Config::language=Config::ENGLISH;
	Config::giaclanguage=2;
    }
    if ((lang == "es")||(lang == "sp")) {
        translator->load(QString(":/lang/qcas_")+lang);
        qApp->installTranslator(translator);
        Config::language=Config::SPANISH;
	Config::giaclanguage=3;
    }
    if (lang == "el") {
        translator->load(QString(":/lang/qcas_")+lang);
        qApp->installTranslator(translator);
        Config::language=Config::GREEK;
	Config::giaclanguage=4;
    }
    if (lang == "zh") {
        translator->load(QString(":/lang/qcas_")+lang);
        qApp->installTranslator(translator);
        Config::language=Config::CHINESE;
	Config::giaclanguage=8;
    }


    ui.setupUi(this);
    createAction();
    createMenus();
    createContextMenu();
    createToolBars();

    readSettings();

    setWindowIcon(QIcon(":/images/icon.png"));

    setCurrentFile("");


    createGui();
    wizardList->setCurrentRow(2);
    (qobject_cast<FormalWorkSheet*>(tabPages->currentWidget()))->setFocus(Qt::OtherFocusReason);
    initAutoSave();
}

//To Start an empty MainWindow.
MainWindow::MainWindow(int startingtabmode ){

    displayTimeAfterProcess=true;
    time=new QTime;

    lastgiacmessage="";

    history=new QStringList();

    isevaluatingall=false;

 
    commandInfo=new CommandInfo;
    taskProperties.firstPrintMessage=true;
    createAction();
    createMenus();
    createContextMenu();
    createToolBars();

    readSettings();

    setWindowIcon(QIcon(":/images/icon.png"));

    setCurrentFile("");
    createGui();
    tabPages->removeTab(0);
}

void MainWindow::changeEvent(QEvent *event, QString *language){
    if(event->type() == QEvent::LanguageChange)
        {
	  ui.retranslateUi(this);
           // retranslate();
        }
    QMainWindow::changeEvent(event);
}

void MainWindow::retranslateInterface(int configlanguage){
    //QString lang = language->remove(2, language->length());
    QString lang="en";
    switch(configlanguage){
    case 0: {
      lang="fr";
      Config::giaclanguage=1;
      break;
    }
    case 1: {
      lang="en";
      Config::giaclanguage=2;
      break;
    }
    case 2: {
      lang="es";
      Config::giaclanguage=3;
      break;
    }
    case 3: {
      lang="el";
      Config::giaclanguage=4;
      break;
    }
    case 4: {
      lang="zh";
      Config::giaclanguage=8;
      break;
    }
    default:
      Config::giaclanguage=1;
    }


    /*
      The giac html doc is non empty only in a couple of language.
      We default to en.
      */
    QStringList supportedgiachtmldoc;
    supportedgiachtmldoc << "fr" << "en" << "el";
    if(supportedgiachtmldoc.contains(lang))
        Config::GiacHtmlLanguage=lang;
    else
        Config::GiacHtmlLanguage="en";

    //lang = lang.toLower();
    lang = ":/lang/qcas_" + lang + ".qm";
    if ( translator )
    qApp->removeTranslator( translator );
    if(configlanguage !=0){
        translator->load( lang);
        qApp->installTranslator( translator );
     }
        //
        retranslateMenus();
        retranslateAction();
        retranslateGui();

}

void MainWindow::createAction(){
    newAction=new QAction("",this);
    newAction->setIcon(QIcon(":/images/document-new.png"));
    connect(newAction,SIGNAL(triggered()),this,SLOT(newFile()));

    openAction=new QAction("",this);
    openAction->setIcon(QIcon(":/images/open.png"));
    connect(openAction,SIGNAL(triggered()),this,SLOT(open()));

    appendfAction=new QAction("",this);
    appendfAction->setIcon(QIcon(":/images/open.png"));
    connect(appendfAction,SIGNAL(triggered()),this,SLOT(appendf()));

    saveAction=new QAction("",this);
    saveAction->setIcon(QIcon(":/images/document-save.png"));
    connect(saveAction,SIGNAL(triggered()),this,SLOT(save()));

    saveAsAction=new QAction("",this);
    saveAsAction->setIcon(QIcon(":/images/document-saveas.png"));
    connect(saveAsAction,SIGNAL(triggered()),this,SLOT(saveAs()));

    for (int i=0;i<MaxRecentFiles;++i){
        recentFileActions[i]= new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i],SIGNAL(triggered()),this,SLOT(openRecentFile()));
    }

    exitAction=new QAction("",this);
    exitAction->setIcon(QIcon(":/images/exit.png"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    copyAction=new QAction("",this);
    copyAction->setIcon(QIcon(":/images/edit-copy.png"));
    connect(copyAction,SIGNAL(triggered()),this,SLOT(copy()));

    cutAction=new QAction("",this);
    cutAction->setIcon(QIcon(":/images/edit-cut.png"));
    connect(cutAction,SIGNAL(triggered()),this,SLOT(cut()));

    pasteAction=new QAction("",this);
    pasteAction->setIcon(QIcon(":/images/edit-paste.png"));
    connect(pasteAction,SIGNAL(triggered()),this,SLOT(paste()));

    undoAction=new QAction("",this);
    undoAction->setIcon(QIcon(":/images/edit-undo.png"));
    connect(undoAction,SIGNAL(triggered()),this,SLOT(undo()));

    redoAction=new QAction("",this);
    redoAction->setIcon(QIcon(":/images/edit-redo.png"));
    connect(redoAction,SIGNAL(triggered()),this,SLOT(redo()));

    insertlineAction=new QAction("",this);
    insertlineAction->setIcon(QIcon(":/images/add.png"));
    connect(insertlineAction,SIGNAL(triggered()),this,SLOT(insertline()));

    deleteLevelAction=new QAction("",this);
    deleteLevelAction->setIcon(QIcon(":/images/delete.png"));
    connect(deleteLevelAction,SIGNAL(triggered()),this,SLOT(deleteSelectedLevels()));

    sendLeveltointerAction=new QAction("",this);
    sendLeveltointerAction->setIcon(QIcon(":/images/tangent.png"));
//    connect(sendLeveltointerAction,SIGNAL(triggered()),this,SLOT(sendSelectedLevels()));
    connect(sendLeveltointerAction,SIGNAL(triggered()),this,SLOT(sendCurrentLine()));

    evaluateAction=new QAction("",this);
    evaluateAction->setIcon(QIcon(":/images/evaluate.png"));
    connect(evaluateAction,SIGNAL(triggered()),this,SLOT(evaluate()));

    evaluateallAction=new QAction("",this);
    evaluateallAction->setIcon(QIcon(":/images/evaluate.png"));
    connect(evaluateallAction,SIGNAL(triggered()),this,SLOT(evaluateall()));

    /*
    stopallAction=new QAction("",this);
    stopallAction->setIcon(QIcon(":/images/stop.png"));
    connect(stopallAction,SIGNAL(triggered()),this,SLOT(killThread()));
    */

    htmlhelpAction=new QAction("",this);
    connect(htmlhelpAction,SIGNAL(triggered()),this,SLOT(htmlhelp()));

    aboutAction=new QAction("",this);
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));

    stopButton=new QToolButton;
    connect(stopButton,SIGNAL(clicked()),this,SLOT(killThread()));
    stopButton->setIcon(QIcon(":/images/stop.png"));

    prefAction=new QAction("",this);
    prefAction->setIcon(QIcon(":/images/configure.png"));
    connect(prefAction,SIGNAL(triggered()),this,SLOT(pref()));

    hintAction=new QAction("",this);
    hintAction->setIcon(QIcon(":/images/ampoule.png"));
    connect(hintAction,SIGNAL(triggered()),this,SLOT(astuces()));


    retranslateAction();
    //QT5.2 Problem: the keyboard shortcuts from the Fichier menu are broken.
    addAction(openAction);
    addAction(saveAction);
    addAction(exitAction);
    //end QT5.2 Fix
}

void MainWindow::retranslateAction(){
    newAction->setText(tr("&Nouveau"));
    //newAction->setShortcut(tr("Ctrl+N"));
    newAction->setStatusTip(tr("Créer un nouvel espace de travail"));

    openAction->setText(tr("Ouvrir..."));
    openAction->setShortcut(tr("Ctrl+O"));
    //openAction->setShortcut(QKeySequence(QKeySequence::Open));
    openAction->setStatusTip(tr("Ouvrir un nouveau fichier"));

    appendfAction->setText(tr("Ajouter un Fichier"));
    appendfAction->setToolTip(tr("Ajoute un fichier à la suite de la session courante"));

    saveAction->setText(tr("Enregistrer"));
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip(tr("Enregistrer le fichier courant"));

    saveAsAction->setText(tr("&Enregistrer sous..."));
    //saveAsAction->setShortcut(tr("Ctrl+Maj+S"));//Gives a conflict with Ctrl+S
    saveAsAction->setStatusTip(tr("Enregistrer sous un nouveau nom de fichier"));

    exitAction->setText(tr("&Quitter"));
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Quitter"));

    copyAction->setText(tr("&Copier"));
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setStatusTip(tr("Copier"));

    cutAction->setText(tr("Co&uper"));
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setStatusTip(tr("Couper"));

    pasteAction->setText(tr("C&oller"));
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setStatusTip(tr("Coller"));

    undoAction->setText(tr("&Annuler"));
    undoAction->setShortcut(tr("Ctrl+Z"));
    undoAction->setStatusTip(tr("Annuler"));

    redoAction->setText(tr("&Rétablir"));
    redoAction->setShortcut(tr("Shift+Ctrl+Z"));
    redoAction->setStatusTip(tr("Rétablir"));

    insertlineAction->setText(tr("&Nouvelle Entrée"));
    insertlineAction->setShortcut(tr("Ctrl+N"));
    insertlineAction->setStatusTip(tr("Nouvelle Entrée"));

    deleteLevelAction->setText(tr("&Supprimer les lignes cochées"));
    deleteLevelAction->setShortcut(tr("Ctrl+BackSpace"));
    deleteLevelAction->setStatusTip(tr("Détruit les niveaux sélectionnés"));

    //sendLeveltointerAction->setText(tr("Evaluer la ligne courante en mode géométrie intéractive"));
    sendLeveltointerAction->setText(tr("Evalue la ligne courante en mode géométrie intéractive<center>(Expérimental et limité)</center>Les objets entre crochets séparé par des virgules seront considérés comme un seul objet( exemple [point(1),point(2)]. Les intersections  inter()  seront considérées comme des listes et ne seront pas selectionnables à la souris. Utilisez  inter_unique si vous souhaitez pouvoir selectionner un point d'intersection pour une construction."));

    evaluateallAction->setText(tr("Evaluer toutes les feuilles depuis le début."));

    hintAction->setText((   "<center><h2>"+tr("Racourcis")+"</h2></center>"
                                                          "<hr>"
                                                          "<ul>"
                                                          "<li><b>"+STRCTRL+"+"+QKeySequence::QKeySequence(Qt::Key_Space).toString()+":</b><center>"+tr("donne les complétions possibles de la chaine saisie")+"</center></li>"
                                                 "<li><b>"+STRCTRL+"+"+tr("flèche haut ou bas</b><center>Insère une commande de l'historique</center></li>")+
                                                 "<li><b>"+STRCTRL+"+BackSpace"+tr("</b><center>Supprime les lignes sélectionnées</center></li>")+
                                                 tr("<li><b>F1</b>(Lorsque l'on laisse le pointeur sur un mot clef)<center>F1 affiche l'aide détaillée correspondante dans le widget d'aide (à gauche)</center></li>")+
                                                 "</ul>"
                                                 "<center><h2>"+tr("Astuces")+"</h2></center>"
                                                 "<ul>"
                                                 "<li>"+tr("En poussant la barre du milieu fort à gauche on peut cacher les widgets de gauche</li>")+
                                                          "</ul><hr>"
                                                          ));

    evaluateAction->setText(tr("&Evaluer"));
    evaluateAction->setShortcut(tr("Shift+Entrée"));
    evaluateAction->setStatusTip(tr("Evaluer"));

    htmlhelpAction->setText(tr("&Aide Html"));
    htmlhelpAction->setStatusTip(tr("Aide html de Giac/Qcas"));

    aboutAction->setText(tr("&A propos"));
    aboutAction->setStatusTip(tr("Principales informations concernant QCAS"));


    stopButton->setToolTip(tr("Interrompre le calcul en cours"));
    stopButton->setStatusTip(tr("Interrompre le calcul en cours"));


    prefAction->setText(tr("&Préférences..."));
    prefAction->setStatusTip(tr("Afficher la fenêtre des préférences"));
}
void MainWindow::createToolBars(){
    toolBar=new QToolBar;
    toolBar->addAction(hintAction);
    toolBar->addAction(copyAction);
    toolBar->addAction(cutAction);
    toolBar->addAction(pasteAction);
    toolBar->addAction(undoAction);
    toolBar->addAction(redoAction);
    toolBar->addAction(insertlineAction);
    toolBar->addAction(sendLeveltointerAction);
    toolBar->addAction(evaluateallAction);
    //toolBar->addAction(stopallAction);
    toolBar->setOrientation(Qt::Vertical);

}
void MainWindow::createMenus(){
    fileMenu=menuBar()->addMenu("");
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(appendfAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    separatorAction=fileMenu->addSeparator();
    for (int i=0;i<MaxRecentFiles;++i){
        fileMenu->addAction(recentFileActions[i]);
    }
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu=menuBar()->addMenu("");
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addAction(insertlineAction);
    editMenu->addAction(deleteLevelAction);

    optionsMenu=menuBar()->addMenu("");
    optionsMenu->addAction(prefAction);

    helpMenu=menuBar()->addMenu("");
    helpMenu->addAction(htmlhelpAction);
    helpMenu->addAction(aboutAction);
    retranslateMenus();
}
void MainWindow::retranslateMenus(){
    fileMenu->setTitle(tr("&Fichier"));
    editMenu->setTitle(tr("&Edition"));
    optionsMenu->setTitle(tr("&Options"));
    helpMenu->setTitle(tr("&Aide"));
}

void MainWindow::createContextMenu(){


}

/**
 * @brief MainWindow::clearWorkspace
 *        Purge all variable in Giac.
 *        Delete all tabs.
 *
 */
void MainWindow::clearWorkspace(){
    MainSheet* sheet=0;
    for (int i=tabPages->count()-2;i>=0;--i){
        tabPages->closeTabwithoutWarning(i);
    }

    delete cas;
    cas=new CasManager(this);
    tabPages->addFormalSheet();
}


void MainWindow::newFile(){
    if (okToContinue()){
        clearWorkspace();
        setCurrentFile("");
    }


}
bool MainWindow::okToContinue(){
    if (isWindowModified()){
        /* PB: gives OK, NO, YES and never understand save or cancel.
        int r=QMessageBox::warning(this,tr("Espace de travail"),
                                   tr("Le document a été modifié.\n"
                                      "Voulez-vous sauvegarder les modifications?"),
                                   QMessageBox::Yes|QMessageBox::Default,
                                   QMessageBox::No,
                                   QMessageBox::Cancel||QMessageBox::Escape);
        if (r==QMessageBox::Yes){
            return save();
        */
        QMessageBox msgBox;
        msgBox.setWindowTitle("Espace de travail");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Le document a été modifié."));
        msgBox.setInformativeText(tr("Voulez-vous sauvegarder les modifications?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int r = msgBox.exec();
        if (r==QMessageBox::Save){
            return MainWindow::save();
        }
        else if (r==QMessageBox::Cancel){
            return false;
        }
    }
    return true;
}

void MainWindow::open(){
    if (okToContinue()){
        clearWorkspace();
        QString fileName=QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier"),".qcas",tr("QCAS or Giac/Xcas files (*.qcas *.cas *.xws)"));
        if (!fileName.isEmpty()){
        loadFile(fileName);
    }
    }
}

void MainWindow::appendf(){
        QString fileName=QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier"),".qcas",tr("QCAS or Giac/Xcas files (*.qcas *.cas *.xws)"));
        if (!fileName.isEmpty()){
        appendFile(fileName);
    }
}

bool MainWindow::loadFile(const QString &fileName){

      if(tabPages->count()>0){tabPages->closeTabwithoutWarning(0);}

      if (!fileName.isEmpty()){
      if (fileName.endsWith(".cas")||(fileName.endsWith(".xws"))){
        return loadGiacFile(fileName);
      }
      else
        return loadQcasFile(fileName);
      }
      return;
}

bool MainWindow::appendFile(const QString &fileName){

      if (!fileName.isEmpty()){
      if (fileName.endsWith(".cas")||(fileName.endsWith(".xws"))){
        return loadGiacFile(fileName);
      }
      else
        return loadQcasFile(fileName);
      }
      return;
}

bool MainWindow::loadQcasFile(const QString &fileName){
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)){
#if QT_VERSION < 0x050000
        qDebug()<<"Failed to open: "<<fileName;
#else
        qInfo()<<"Failed to open: "<<fileName;
#endif
        return false;
    }
    QDomDocument doc("xml");

    QDataStream dataIn(&file);
    QByteArray compByteArray;
    dataIn >> compByteArray;
    QByteArray xmlByteArray = qUncompress(compByteArray);
    QString xmlString =QString::fromUtf8(xmlByteArray.data(), xmlByteArray.size());
    //qInfo()<<xmlString;//too slow with big files
    if (!doc.setContent(xmlString)){
        file.close();
        return false;
    }
    file.close();


     QDomElement root = doc.documentElement();
     if (root.tagName()!="qcas") return false;

    int tabBeforeLoad=tabPages->count();

     QDomNode node = root.firstChild();
     while(!node.isNull()){
        QDomElement sheet = node.toElement(); // try to convert the node to an element.
         if(!sheet.isNull()) {
             if (sheet.tagName()=="formal"){
                tabPages->addFormalSheet();
                FormalWorkSheet *f=qobject_cast<FormalWorkSheet*>(tabPages->widget(tabPages->count()-2));
                QDomNode first=sheet.firstChild();
                bool goToNextLine=true;

                while(!first.isNull()){
                    QDomElement element=first.toElement();
                    if (!element.isNull()) {
                        QString tag=element.tagName();
                        if (tag=="command"){
                            if (goToNextLine) goToNextLine=false;
                            else {
                                f->goToNextLine();
                            }
                            f->sendText(element.text());

                       }
                        else if (tag=="formula"){
                            giac::gen g(element.text().toStdString(),getContext());
                            FormulaWidget* formWidget=new FormulaWidget(g,getContext());
                              f->displayResult(f->getCurrent(),formWidget);
                              goToNextLine=true;
                        }
                        else if (tag=="graph2d"){
                            GraphWidget* graph=new GraphWidget(getContext(),false,this);
                            graph->loadXML(element);
                            graph->updateAllCategories();

                            f->displayResult(f->getCurrent(),graph);

                            goToNextLine=true;

                        }
                    }
                    first=first.nextSibling();
                }
             }
             else if (sheet.tagName()=="interactive2d"){
                 tabPages->addG2dSheet();
                 GraphWidget *f=qobject_cast<GraphWidget*>(tabPages->widget(tabPages->count()-2));
                 f->loadInteractiveXML(sheet);
             }
             else if(sheet.tagName()=="settings"){
                 QDomNode first=sheet.firstChild();
                 while(!first.isNull()){
                     QDomElement element=first.toElement();
                     if (!element.isNull()) {
                         QString tag=element.tagName();
                         if (tag=="cas") cas->loadXML(element);
                         else {
			   if (tag=="general") { cas->loadGeneralXML(element);}
			   else{ if(tag=="context"){cas->loadXML(element,true);}//load giac context
			   }
			 }
		     }
                     first=first.nextSibling();
                 }
             }
         }
         node= node.nextSibling();
     }

    setCurrentFile(fileName);
    return true;
}

bool MainWindow::loadGiacFile(const QString &fileName){
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    QTextStream dataIn(&file);
    dataIn.setCodec("UTF-8");//NB: there is a nasty latin1 symbol for newlines
    if (fileName.endsWith(".xws")){
    //xcas .xws file
	QString xcasline = dataIn.readLine();
	QString fltktag="newformal";//with this flag, a formal sheet must be opened before sending a formal line
	FormalWorkSheet *f=0;//
	GraphWidget * g2d=0;
    int currentformalindex=0;
	while (!xcasline.isNull()&&!xcasline.startsWith("// context")) {
      //qInfo()<<xcasline;
	  if(xcasline.startsWith("// fltk")){
	    if((xcasline.contains("_Tile"))||(xcasline.contains("History"))){
		  //drop those cases.
		  xcasline=dataIn.readLine();//drop the bracket
	    }
	    if((xcasline.contains("Equation"))||(xcasline.contains("Output"))||(xcasline.contains("Mouse"))||(xcasline.contains("Button"))||(xcasline.contains("History"))||(xcasline.contains("Menu"))||(xcasline.contains("Scroll"))||(xcasline.contains("N4xcas7Graph3dE"))||(xcasline.contains("N4xcas7Graph2dE"))){
		  //drop those cases.
		  xcasline=dataIn.readLine();//drop the nextline (results)
	    }
	    if(xcasline.contains("N4xcas6FigureE")){
	      //Open Interactive 2D geometry
          fltktag="Geo2D";
          /*tabPages->addG2dSheet();
          g2d=qobject_cast<GraphWidget*>(tabPages->widget(tabPages->count()-2));
          *///g2d interactive not ready, convert to formal.
          //tabPages->addFormalSheet();
          //f=qobject_cast<FormalWorkSheet*>(tabPages->widget(tabPages->count()-2));
        }
	    if(xcasline.contains("N4xcas5Geo2dE")){
	      //quit geo 2d later.
	      xcasline=dataIn.readLine();//drop the next line
          //fltktag="newformal";
          fltktag="quitgeo2d";
	      //fin
	    }
	    //Fixme: add the spreadsheet and other non formal case before new formal.

	    if(xcasline.contains("Comment")){  
          //dataIn.setCodec("ISO 8859-1");
          xcasline=dataIn.readLine();
	      xcasline.prepend("/* ");
	      xcasline.append(" */");
          //xcasline=xcasline.replace(QChar(0x00A3),"\n");
          xcasline=xcasline.replace(QChar(65533),"\n");
          //xcasline=xcasline.to();//pb with greek chineese...
          //qInfo()<<xcasline;
          dataIn.setCodec("");
	      if(fltktag=="newformal"){
            fltktag="";
            tabPages->addFormalSheet();
            f=qobject_cast<FormalWorkSheet*>(tabPages->widget(tabPages->count()-2));
            currentformalindex=tabPages->currentIndex();
	      }
	      f->sendText(xcasline);
	      f->goToNextLine();
	    }
	    if((xcasline.contains("N4xcas7EditeurE"))||(xcasline.contains("Xcas_Text_EditorE"))){
          int nbytes=(dataIn.readLine()).remove(",").toInt();//number of octets of the string
          if(nbytes>0){
          xcasline=dataIn.read(nbytes);
          /*
            nbytes is the number of octets of the string in the text editor.
            but we have read nbytes characters. So if some are non 8bits we have read
            too many characters: diff
            so we correct this.
            */
          QByteArray byteline=xcasline.toUtf8();
          int diff=byteline.size()-nbytes;
          if(diff>0){
              xcasline=xcasline.left(nbytes-diff);
              dataIn.seek(dataIn.pos()-diff);
          }
		if(fltktag=="Geo2D"){
          //g2d->sendText(xcasline);
            if (! xcasline.contains(QRegExp(";\\s*$"))){ xcasline=xcasline.append(";"); }
            f->sendText(xcasline.append("\n"));
		}
		else{
		  if(fltktag=="newformal"){
		    fltktag="";
            //qInfo()<<xcasline;
		    tabPages->addFormalSheet();
		    f=qobject_cast<FormalWorkSheet*>(tabPages->widget(tabPages->count()-2));
            currentformalindex=tabPages->currentIndex();
		  }
		  f->sendText(xcasline);
		  f->goToNextLine();
		}
	      }
	    }
	  }
	  else{
	    if((fltktag=="")||(fltktag=="newformal")){
	      if((xcasline!="]")&&(xcasline!="[")&&(xcasline!=",")&&(xcasline!="")){
        if(fltktag=="newformal"){
		  fltktag="";
          //qInfo()<<xcasline;
		  tabPages->addFormalSheet();
		  f=qobject_cast<FormalWorkSheet*>(tabPages->widget(tabPages->count()-2));
          currentformalindex=tabPages->currentIndex();
		}
        xcasline=xcasline.replace(QChar(65533),"\n");
        if(xcasline=="// Qcas new Formal sheet TAG. Please dont't modify this line."){
            fltktag="newformal";
        }
        else{
            f->sendText(xcasline);
            f->goToNextLine();
        }
	      }
	    }
	    if(fltktag=="Geo2D"){
            if(f==0){
                tabPages->addFormalSheet();
                f=qobject_cast<FormalWorkSheet*>(tabPages->widget(tabPages->count()-2));
                currentformalindex=tabPages->currentIndex();
              }

          if((xcasline!="]")&&(xcasline!="[")&&(xcasline!=",")&&(xcasline!=(""))){
              /* if (!(xcasline.contains(QRegExp(";\s*$")))){
                    xcasline.append(";");
                 }
                 g2d->sendText(xcasline);
              */
              if (! xcasline.contains(QRegExp(";\\s*$"))){ xcasline=xcasline.append(";"); }
              f->sendText(xcasline.append("\n"));
          }
	    }
        if(fltktag=="quitgeo2d"){
            tabPages->setCurrentIndex(currentformalindex);
            sendCurrentLine();
            fltktag="";
            f->goToNextLine();
        }
	  }
	  xcasline = dataIn.readLine();
	}


      }
    else{
    //giac .cas file
        QString giacline = dataIn.readLine();
	tabPages->addFormalSheet();
	FormalWorkSheet *f=qobject_cast<FormalWorkSheet*>(tabPages->widget(tabPages->count()-2));
	while (!giacline.isNull()) {
	  f->sendText(giacline);
	  f->goToNextLine();
	  giacline = dataIn.readLine();
	}
      }
    file.close();
    //setCurrentFile(fileName.replace(".cas",".qcas"));PB si le fichier existe deja.
    return true;
}

bool MainWindow::askforXcasarchive(){
    if(!curFile.isEmpty()){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Archive Xcas");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Enregistrer une copie pour Xcas avant de quitter?"));
        msgBox.setInformativeText(tr("Il est conseillé de garder un fichier dans un format lisible par Xcas (.xws)"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int r = msgBox.exec();
        if (r==QMessageBox::Save){
            return MainWindow::saveToGiacFile(curFile.append("toxcas.xws"));
        }
        else if (r==QMessageBox::Cancel){
            return true;
        }
    }
    else{
        return true;
    }
  return;
}


//for giacpy
void MainWindow::loadgiacgen(const giac::gen & g, giac::context * ct){
  tabPages->addG2dSheet(ct);
  GraphWidget * g2d=qobject_cast<GraphWidget*>(tabPages->widget(tabPages->count()-2));
  g2d->sendgiacgen(g);
  return;
}
//for giacpy
void MainWindow::loadinteractivegiacgen(const giac::gen & g, giac::context * ct){
  tabPages->addG2dSheet();
  GraphWidget * g2d=qobject_cast<GraphWidget*>(tabPages->widget(tabPages->count()-2));
  //g2d->sendgiacgen(g);
  g2d->sendinteractivegiacgen(g);
  return;
}


bool MainWindow::save(){
  if ((curFile.isEmpty())||!(curFile.endsWith(".qcas")))
        return saveAs();
    else
        return saveFile(curFile);
}
bool MainWindow::saveFile(const QString &fileName){

    QDomDocument doc;

    QDomElement root=doc.createElement("qcas");
    if(cas->isRunning()){
        QMessageBox::warning(this,tr("Avertissement"),tr("Vous essayez d'enregistrer pendant un calcul. Recommencez plus tard."));
        return false;
    }
    // write cas configuration
    cas->toXML(root,false);// the true option save also the  giac context. FIXME: it is often too big.
    for (int i=0;i<tabPages->count()-1;++i){
        MainSheet* sheet=dynamic_cast<MainSheet*>(tabPages->widget(i));
        switch(sheet->getType()){
        case MainSheet::FORMAL_TYPE:
            {FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->widget(i));
            form->toXML(root);
        }
            break;
        case MainSheet::SPREADSHEET_TYPE:
            break;
        case MainSheet::PROGRAMMING_TYPE:
            break;
        case MainSheet::G2D_TYPE:
        {GraphWidget *graph=qobject_cast<GraphWidget*>(tabPages->widget(i));
        graph->toInteractiveXML(root);
    }
         break;

        }

   }
    doc.appendChild(root);
//    qInfo()<<root.toString();
//    qInfo()<<doc.toString();
//    qInfo()<<"Nom du fichier"<<fileName;
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QString xmlString;
        QTextStream textOut(&xmlString);
        doc.save(textOut,1);
        QByteArray xmlByteArray = qCompress(xmlString.toUtf8());
        QDataStream dataOut(&file);
        dataOut.writeBytes(xmlByteArray.data(), xmlByteArray.size());

//   QTextStream out(&file);
//   doc.save(out, 4);

    file.close();
/*    -------------------------------------


    Uncompressing xml-Data to QDomDocument doc:
    ===========================================
    QDataStream dataIn(&inFile);
    QByteArray compByteArray;
    dataIn >> compByteArray;
    QByteArray xmlByteArray = qUncompress(compByteArray);
    QString xmlString =
        QString::fromUtf8(xmlByteArray.data(), xmlByteArray.size());
    doc.setContent(xmlString, &errorMsg, &errorLine, &errorColumn);


    ------------------------------
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    QFile out("test.gz");
         file.open(QIODevice::WriteOnly);
         QDataStream out(&amp;file);
         out << qCompress(ba);
  */


    }
    else
    return(false);





    setCurrentFile(fileName);

    return true;
}
bool MainWindow::saveToGiacFile(const QString &fileName){

    QDomElement rootold;
    QString root;
    // TODO: write cas configuration
    for (int i=0;i<tabPages->count()-1;++i){
        MainSheet* sheet=dynamic_cast<MainSheet*>(tabPages->widget(i));
        switch(sheet->getType()){
        case MainSheet::FORMAL_TYPE:
            {FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->widget(i));
            if(fileName.endsWith(".cas"))
	      form->toGIAC(root);
        else{
            if(i>0)
                root.append("// Qcas new Formal sheet TAG. Please dont't modify this line.\n");
            form->toXCAS(root);
            }
        }
            break;
        case MainSheet::SPREADSHEET_TYPE:
            break;
        case MainSheet::PROGRAMMING_TYPE:
            break;
        case MainSheet::G2D_TYPE:
        {GraphWidget *graph=qobject_cast<GraphWidget*>(tabPages->widget(i));
	      if(fileName.endsWith(".xws"))
	          graph->toInteractiveXCAS2D(root);

	}
         break;

        }

    }
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
	out<<root;
	file.close();
    }
    else
    return(false);

    //setCurrentFile(fileName);
    setWindowModified(true);

    return true;
}

void MainWindow::initAutoSave(){

    QDir curD=QDir::current();
    //We use the current dir only if it is under home
    if(!curD.absolutePath().startsWith(QDir::homePath()))
        curD=QDir::home();
    QStringList filter;
    QString curDpath=curD.path();
    if(!curDpath.endsWith("/"))
        curDpath.append("/");

    QString tmp=QDateTime::currentDateTime().toString("yy:MM:dd:hh:mm:ss");
    tmp.remove(":");
    if(tmp.isEmpty())
        tmp="99998877";
    autosaveFileName=curDpath+"xcas_auto_"+tmp+".xws";

    filter<<"xcas_auto*.xws";
    //curD.setNameFilters(filter);
    curD.setSorting(QDir::Time);
    QStringList autofound=curD.entryList(filter);
    QFile delfile;
    int i;

    if(! autofound.isEmpty()){
#if QT_VERSION < 0x050000
        qDebug()<<"Found automatic saving file"<<autofound;
#else
        qInfo()<<"Found automatic saving file"<<autofound;
#endif
        QMessageBox msgBox;
        msgBox.setWindowTitle("AutoSave");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Des fichiers de sauvegarde automatique d'Xcas ou Qcas ont été trouvés."));
        msgBox.setInformativeText(tr("Voulez vous charger le plus récent?\n Ou alors ouvrir la liste des fichiers, ou  détruire ces archives (Reset)."));
        msgBox.setStandardButtons(QMessageBox::Ignore | QMessageBox::Yes | QMessageBox::Open | QMessageBox::Reset);
        msgBox.setDefaultButton(QMessageBox::Ignore);
        int r = msgBox.exec();
        if (r==QMessageBox::Yes){

             if(loadGiacFile(curDpath+autofound.at(0))){
                     autosaveFileName=curDpath+autofound.at(0);
                     setWindowModified(true);
             }
            return;
        }
        else if (r==QMessageBox::Open){
            QString fileName=QFileDialog::getOpenFileName(this,tr("auto_save"), curDpath ,tr("QCAS or Giac/Xcas files (xcas_auto_*.xws)"));
            if (!fileName.isEmpty()){
                loadFile(fileName);
            }
            return;
        }
        else if (r== QMessageBox::Reset){
            for(i=0;i<autofound.size();i++){
                delfile.setFileName(curDpath+autofound.at(i));
                delfile.remove();
            }
        }
        else{
            return;
        }
    }
}
void MainWindow::cleanautoSaveFiles(){
    QFile todel;
    todel.setFileName(autosaveFileName);
    todel.remove();
}

bool MainWindow::autoSave(){
    if(!saveToGiacFile(autosaveFileName)){
        QString hp=QDir::homePath();
        if(!hp.endsWith("/")){
            hp.append("/");
        }
        autosaveFileName=hp+autosaveFileName;
        saveToGiacFile(autosaveFileName);
    }
  return;
}

bool MainWindow::saveAs(){

  QFileDialog rep(this,tr("Enregistrer sous..."),"",tr("QCAS files (*.qcas);;XCAS files (*.xws);;GIAC files (*.cas)"));
  rep.setAcceptMode(QFileDialog::AcceptSave);

  if(rep.exec()){

     QString fileName = rep.selectedFiles()[0];
     if((fileName.endsWith(".xws"))||(fileName.endsWith(".cas")))
           return saveToGiacFile(fileName);
     else{
        if(fileName.endsWith(".qcas"))
	   return saveFile(fileName);
        else{
	  if (rep.selectedNameFilter() == tr("fichiers XCAS (*.xws)"))
	    return saveToGiacFile(fileName.append(".xws"));
	  if (rep.selectedNameFilter() == tr("fichiers GIAC (*.cas)"))
	    return saveToGiacFile(fileName.append(".cas"));
	  else
	    return saveFile(fileName.append(".qcas"));
	}
     }
  }
  else
    return false;
}

bool MainWindow::saveAsgiacxcas(){
    QString fileName=QFileDialog::getSaveFileName(this,tr("Exporter vers..."),".xws",tr("fichiers XCAS  (*.xws);;fichiers GIAC  (*.cas)"));
    if (fileName.isEmpty()){
        return false;
    }
    return saveToGiacFile(fileName);
}

void MainWindow::setCurrentFile(const QString &fileName){
    curFile=fileName;
    setWindowModified(false);
    QString shownName=tr("Sans titre");

    if (!curFile.isEmpty()){
        shownName=strippedName(curFile);
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);
        updateRecentFileActions();
        setWindowTitle(tr("%1[*] - %2").arg(shownName).arg("QCAS"));

    }
    else setWindowTitle("QCAS[*]");
}

QString MainWindow::strippedName(const QString &fullFileName){
  return QFileInfo(fullFileName).fileName();

}

void MainWindow::updateRecentFileActions(){
    QMutableStringListIterator i(recentFiles);
    while(i.hasNext()){
        if (!QFile::exists(i.next()))
            i.remove();
    }
    for (int j=0;j<MaxRecentFiles;++j){
        if (j<recentFiles.size()){
            QString text=tr("&%1 %2").arg(j+1).arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        }
        else{
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible((!recentFiles.isEmpty()));
}

void MainWindow::openRecentFile(){
    if (okToContinue()){
        QAction *action=qobject_cast<QAction *>(sender());
            if (action)
                loadFile(action->data().toString());
    }
}

void MainWindow::createGui(){

    cas=new CasManager(this);//also setup xcasroot for help so put it before createWizards.
    createWizards();

    tabPages=new MainTabWidget(this);
    QSplitter *vertSplit=new QSplitter(Qt::Horizontal);

    leftPanel=new QWidget;

    QHBoxLayout *h=new QHBoxLayout;
    h->addWidget(wizardList);
    h->addWidget(wizardPages,1);
    wizardList->setMinimumHeight(400);
    //wizardList->setMaximumHeight(800);
    wizardList->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    giacMessages=new QPlainTextEdit;
    giacMessages->setReadOnly(true);
    QPalette p=giacMessages->palette();
    p.setColor(QPalette::Base,QColor::fromRgb(251,251,113,128));
    giacMessages->setPalette(p);
    giacMessages->setMaximumBlockCount(1000);

    giacPanel=new QGroupBox;

    QVBoxLayout *giacLayout=new QVBoxLayout;
    giacLayout->addWidget(giacMessages);
    giacPanel->setLayout(giacLayout);
    giacPanel->setMinimumHeight(100);
    giacPanel->setMaximumHeight(250);
    QVBoxLayout *leftLayout=new QVBoxLayout;
    leftLayout->addLayout(h);
    //leftLayout->addStretch(1);
    leftLayout->addWidget(giacPanel);
    leftPanel->setLayout(leftLayout);
    leftPanel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Expanding);



/*    QsciScintilla *text=new QsciScintilla;

    text->setLexer(new QsciLexerJava);
    text->setFolding(QsciScintilla::BoxedFoldStyle,2);

    text->setAutoIndent(true);
    text->setBraceMatching(QsciScintilla::SloppyBraceMatch);
*/
    rightPanel=new QWidget(this);

    QHBoxLayout *rightLayout=new QHBoxLayout;

/*  QGridLayout *right=new QGridLayout;
  right->addWidget(mainWidgetList,0,0,Qt::AlignTop);
    right->addWidget(mainWidgetPages,1,0,Qt::AlignTop);
    right->addWidget(toolBar,1,1,Qt::AlignTop);*/

    rightLayout->addWidget(tabPages);
    rightLayout->addWidget(toolBar);
 //  rightLayout->addStretch(1);
    rightPanel->setLayout(rightLayout);

    setCentralWidget(vertSplit);

    vertSplit->addWidget(leftPanel);
    vertSplit->addWidget(rightPanel);

//    vertSplit->setStretchFactor(0,1);
    vertSplit->setStretchFactor(1,8);

    labelStatus=new QLabel;
    labelStatus->setAlignment(Qt::AlignRight);
    labelStatus->setIndent(20);
    labelStatus->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    statusBar()->addWidget(labelStatus,1);
    labelStatus->show();


 /*   warningFirstEvaluation=new QLabel(tr("<b><font color=\"red\">Shift+Entrée pour évaluer</font></b>"));
    warningFirstEvaluation->setIndent(20);
    warningFirstEvaluation->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    statusBar()->addWidget(warningFirstEvaluation,1);
    warningFirstEvaluation->show();


    warningStop=new QLabel(tr("<b><font color=\"red\">Tentative d'interruption...</font></b>"));
    warningStop->setAlignment(Qt::AlignRight);
    warningStop->setIndent(20);
    warningStop->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    warningStop->setParent(0);
*/

    prefDialog=new PrefDialog(this);
//fred    cas=new CasManager(this);
//    connect(&ev,SIGNAL(finished()),this,SLOT(displayResult()));

    retranslateGui();

}
void MainWindow::retranslateGui(){
    giacPanel->setTitle(tr("Messages Giac"));
    displayInStatusBar(tr("Shift+Entrée pour évaluer"),"red");

    matrixItem->setText(tr("Matrices"));
    equationItem->setText(tr("Résoudre"));
    catalogItem->setText(tr("Catalogue"));
    algoItem->setText(tr("Prog/Giac"));
    algoscolItem->setText(tr("Prog/Algo"));
    //fred
}
void MainWindow::printHeader(){
    if (cas->getGiacDisplay().isEmpty()&&!displayTimeAfterProcess) return;

    if (taskProperties.firstPrintMessage) {
        QString title=tr("Feuille ")+QString::number(taskProperties.currentSheet+1)+tr(", ligne ")+QString::number(taskProperties.currentLine+1)+":";
        giacMessages->appendHtml("<b><u><font color=\"#40A497\">"+title+"</font></u></b>");
        taskProperties.firstPrintMessage=false;
    }

}
void MainWindow::displayInStatusBar(const QString & text, const QString & color){
    labelStatus->setText(QString("<b><font color=\"%1\">%2</font></b>").arg(color,text));
}
void MainWindow::updateInterface(MainSheet::sheetType type){
    switch(type){
        case MainSheet::G2D_TYPE:    {
            giac::decimal_digits(3,getContext());
            hintAction->setVisible(false);
            copyAction->setVisible(false);
            cutAction->setVisible(false);
            pasteAction->setVisible(false);
            redoAction->setVisible(true);
            undoAction->setVisible(true);
            //evaluateallAction->setVisible(false);
            evaluateallAction->setVisible(true);
            //stopallAction->setVisible(false);
            insertlineAction->setVisible(false);
            deleteLevelAction->setVisible(false);
            sendLeveltointerAction->setVisible(false);
            leftPanel->hide();
        }
        break;
        case MainSheet::FORMAL_TYPE:
        default:{
            giac::decimal_digits(decimalDigits,getContext());
            hintAction->setVisible(true);
            copyAction->setVisible(true);
            cutAction->setVisible(true);
            pasteAction->setVisible(true);
            redoAction->setVisible(true);
            undoAction->setVisible(true);
            insertlineAction->setVisible(true);
            deleteLevelAction->setVisible(true);
            evaluateallAction->setVisible(true);
            //stopallAction->setVisible(false);
            sendLeveltointerAction->setVisible(true);
            leftPanel->show();
        }
    }
}
void MainWindow::setRedoButton(bool b){
    redoAction->setEnabled(b);
}
void MainWindow::setUndoButton(bool b){
    undoAction->setEnabled(b);
}
int MainWindow::getDecimalDigit()const{
    return decimalDigits;
}
void MainWindow::setDecimalDigits(const int &a){
    decimalDigits=a;
}

void MainWindow::displayGiacMessages(){
    printHeader();
    lastgiacmessage="";
    QStringList list=cas->getGiacDisplay();
    for (int  i=0;i<list.size();++i){
        giacMessages->appendHtml(list.at(i));
        lastgiacmessage.append(list.at(i));
        lastgiacmessage.append("\n");
    }
    giacMessages->appendHtml(tr("<br><font color=\"gray\">Temps mis:")+QString::number(time->elapsed())+" ms</font><br>");
    giacMessages->verticalScrollBar()->setValue(giacMessages->verticalScrollBar()->maximum());
    cas->clearGiacDisplay();
}


void MainWindow::createWizards(){
    wizardList=new QListWidget;
    wizardList->setViewMode(QListView::IconMode);
    wizardList->setIconSize(QSize(50,50));
    wizardList->setSpacing(10);
    wizardList->setResizeMode(QListView::Adjust);
    wizardList->setMovement(QListView::Static);
    wizardList->setFixedWidth(90);

    wizardPages=new QStackedWidget;
    wizardPages->addWidget(new WizardMatrix(this));
    wizardPages->addWidget(new WizardEquation(this));
    wizardPages->addWidget(new WizardCatalog(this));
    wizardPages->addWidget(new WizardAlgo(this,false));
    wizardPages->addWidget(new WizardAlgo(this,true));
    wizardPages->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    matrixItem=new QListWidgetItem(QIcon(":/images/matrix.png"),"",wizardList);
    matrixItem->setTextAlignment(Qt::AlignHCenter);
    matrixItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    equationItem=new QListWidgetItem(QIcon(":/images/equation.png"),"",wizardList);
    equationItem->setTextAlignment(Qt::AlignHCenter);
    equationItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    catalogItem=new QListWidgetItem(QIcon(":/images/book.png"),"",wizardList);
    catalogItem->setTextAlignment(Qt::AlignHCenter);
    catalogItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    algoItem=new QListWidgetItem(QIcon(":/images/programming.png"),"",wizardList);
    algoItem->setTextAlignment(Qt::AlignHCenter);
    algoItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    algoscolItem=new QListWidgetItem(QIcon(":/images/prog-scol100.png"),"",wizardList);
    algoscolItem->setTextAlignment(Qt::AlignHCenter);
    algoscolItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    connect(wizardList,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(changeWizard(QListWidgetItem*,QListWidgetItem*)));

}

void MainWindow::changeWizard(QListWidgetItem *current, QListWidgetItem *previous){
    if (!current)
        current=previous;
    wizardPages->setCurrentIndex(wizardList->row(current));
}
void MainWindow::about(){
    QMessageBox::about(this,tr("A propos de QCAS"),tr("<center><h1>  QCAS</h1></center>"
                                                      "<hr>"
                                                      "<ul>"
                                                      "<li><b>Licence:</b> GPL v3</li>"
                                                      "<li><b>Version:</b> %1 </li>"
                                                      "<li><b>Sources de QCAS: </b><center><a href=\"http://git.tuxfamily.org/?p=qcas/qcas.git\">http://git.tuxfamily.org/?p=qcas/qcas.git</a></center><br></li>"
                              "<li><b>Site Web de QCAS: </b><center><a href=\"http://www.imj-prg.fr/~frederic.han/qcas\">http://www.imj-prg.fr/~frederic.han/qcas</a></center><br></li>"
                                                      "<li><b>Auteur de QCAS: </b>Loïc Le Coq  (2012) </li>"
                              "<li><b>Maintenance et développement de QCAS: </b>(2013- )<center>Han Frédéric <a href=\"mailto:frederic.han@imj-prg.fr\"> frederic.han@imj-prg.fr</a></center></li>"
                                                      "<li><b>Développeur de Giac/Xcas: </b>Bernard Parisse</li>"
                                                      "<li><b> Site Web de XCas:  <center><a href=\"http://www-fourier.ujf-grenoble.fr/~parisse/giac_fr.html\">http://www-fourier.ujf-grenoble.fr/~parisse/giac_fr.html</a></center><nr></li>"
                                                      "</ul><hr>"
                                                      ).arg(Config::QcasVersion).append(QDate::currentDate().toString()));

}
void MainWindow::astuces(){
    QMessageBox::about(this,tr("Astuces"),hintAction->text());

}

void MainWindow::htmlhelp(){
    giac::system_browser_command("doc/index.html");
}
void MainWindow::closeEvent(QCloseEvent *event){
    if (okToContinue()){
        writeSettings();
        cleanautoSaveFiles();
        askforXcasarchive();
        event->accept();
    }
    else{
        event->ignore();
    }
}

void MainWindow::pref(){
    prefDialog->initValue();
    prefDialog->setVisible(true);

}
void MainWindow::readSettings(){
    QSettings settings("qcas.tuxfamily.org","QCAS");
    QRect rect=settings.value("geometry",QRect(50,50,1000,700)).toRect();
    move(rect.topLeft());
    resize(rect.size());

    recentFiles=settings.value("recentFiles").toStringList();
    int siz=settings.value("mmlfontsize").toInt();
    if(siz>9 && siz<21){
        Config::mml_fontsize=siz;
    }
    updateRecentFileActions();
}

void MainWindow::writeSettings(){
    QSettings settings("qcas.tuxfamily.org","QCAS");
    settings.setValue("geometry",geometry());
    settings.setValue("recentFiles",recentFiles);
    settings.setValue("mmlfontsize",Config::mml_fontsize);

}

void MainWindow::copy(){
   MainSheet* sheet=dynamic_cast<MainSheet*>(tabPages->currentWidget());
   switch(sheet->getType()){
   case MainSheet::FORMAL_TYPE:
       {FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->currentWidget());
           form->copy();}
       break;
   case MainSheet::SPREADSHEET_TYPE:
   case MainSheet::G2D_TYPE:
   case MainSheet::PROGRAMMING_TYPE:
       break;
   }
   }
void MainWindow::cut(){
    MainSheet* sheet=dynamic_cast<MainSheet*>(tabPages->currentWidget());
    switch(sheet->getType()){
    case MainSheet::FORMAL_TYPE:
        {FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->currentWidget());
            form->cut();}
        break;
    case MainSheet::SPREADSHEET_TYPE:
    case MainSheet::G2D_TYPE:
    case MainSheet::PROGRAMMING_TYPE:
        break;
    }
}
void MainWindow::paste(){
    MainSheet* sheet=dynamic_cast<MainSheet*>(tabPages->currentWidget());
    switch(sheet->getType()){
    case MainSheet::FORMAL_TYPE:
        {FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->currentWidget());
        form->paste();}
        break;
    case MainSheet::SPREADSHEET_TYPE:
    case MainSheet::G2D_TYPE:
    case MainSheet::PROGRAMMING_TYPE:
        break;
    }

}
void MainWindow::undo(){
    MainSheet* sheet=dynamic_cast<MainSheet*>(tabPages->currentWidget());
    switch(sheet->getType()){
    case MainSheet::FORMAL_TYPE:
        {

        FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->currentWidget());
        form->undo();}
        break;
    case MainSheet::SPREADSHEET_TYPE:
        break;
    case MainSheet::PROGRAMMING_TYPE:
        break;
     case MainSheet::G2D_TYPE:
    {
        GraphWidget* graph=qobject_cast<GraphWidget*>(tabPages->currentWidget());
        graph->undo();
    }
     break;
    }
}

void MainWindow::redo(){
    MainSheet* sheet=dynamic_cast<MainSheet*>(tabPages->currentWidget());
    switch(sheet->getType()){
    case MainSheet::FORMAL_TYPE:
        {FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->currentWidget());
        form->redo();}
        break;
    case MainSheet::SPREADSHEET_TYPE:
        break;
    case MainSheet::PROGRAMMING_TYPE:
        break;
     case MainSheet::G2D_TYPE:
    {
        GraphWidget* graph=qobject_cast<GraphWidget*>(tabPages->currentWidget());
        graph->redo();
    }
     break;
    }
}
void MainWindow::insertline(){
    MainSheet* sheet=dynamic_cast<MainSheet*>(tabPages->currentWidget());
    switch(sheet->getType()){
    case MainSheet::FORMAL_TYPE:
    {FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->currentWidget());
            form->insertline();}
    break;
    case MainSheet::SPREADSHEET_TYPE:
    case MainSheet::G2D_TYPE:
    case MainSheet::PROGRAMMING_TYPE:
    break;
    }
}
void MainWindow::deleteSelectedLevels(){
    FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->currentWidget());
    if (form==0) return;
    form->deleteSelectedLevels();
}

void MainWindow::sendSelectedLevels(){
     FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->currentWidget());
     if (form==0) return;
     if (form->selectedLevels.size()==0){
         QMessageBox msgBox;
         msgBox.setWindowTitle("Remarque");
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setText(tr("Vous n'avez coché aucune ligne!<br>Il n'y a donc aucune commande à envoyer en géomérie inéractive."));
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
         return;
     }
     QString titre="(F";
     titre.append(QString::number(tabPages->currentIndex()+1));
     titre.append("L");
     titre.append(QString::number(form->getCurrent()+1));
     titre.append(")");
     tabPages->addG2dSheetfromLine(titre);
     GraphWidget *g2d=qobject_cast<GraphWidget*>(tabPages->widget(tabPages->count()-2));
     form->sendSelectedLevels(g2d);
}
void MainWindow::sendCurrentLine(){
     //send a line from a formal sheet to G2D
     FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->currentWidget());
     if (form==0) return;
     QString titre="(F";
     titre.append(QString::number(tabPages->currentIndex()+1));
     titre.append("L");
     titre.append(QString::number(form->getCurrent()+1));
     titre.append(")");
     tabPages->addG2dSheetfromLine(titre);
     GraphWidget *g2d=qobject_cast<GraphWidget*>(tabPages->widget(tabPages->count()-2));
     form->sendCurrentLine(g2d);
}
void MainWindow::killThread(){

    cas->killThread();
    setEvaluatingAll(false);
}
void MainWindow::evaluate(const QString &formula){
/*    if (warningFirstEvaluation!=NULL) {
        statusBar()->removeWidget(warningFirstEvaluation);
        delete warningFirstEvaluation;
        warningFirstEvaluation=NULL;
    }*/
    if(formula == ""){ return;}
    autoSave();
    setWindowModified(true);
    displayInStatusBar("","black");
    taskProperties.firstPrintMessage=true;
    taskProperties.currentSheet=tabPages->currentIndex();
    taskProperties.currentLine=-1;
    MainSheet* sheet=dynamic_cast<MainSheet*>(tabPages->currentWidget());
    switch(sheet->getType()){
    case MainSheet::FORMAL_TYPE:
        {FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->currentWidget());
            //qInfo() << "form " << form;
            form->getCurrentLine()->addStopButton(stopButton);
            //qInfo()<<"current line in ev" << form->getCurrentLine();

            if(cas->isRunning()){
                //qInfo()<<"starting buisy loop in ev";
                cas->buisyloop->exec();
                //qInfo()<<"finished; I can compute:";
            }

            taskProperties.currentLine=form->getCurrentLine()->getId();
            CasManager::warning id=cas->initExpression(&formula);
            if (id==CasManager::Warning){
                QString s("<font color=\"red\"");
                s.append(tr("Attention! <br> Pour affecter une valeur à une variable, vous devez utiliser le symbole :="));
                s.append("<br><u>Exemple</u>: a:=2 ou f(x):=x^2");
                s.append("</font>");
                printHeader();
                giacMessages->appendHtml(s);
                form->getCurrentLine()->getTextInput()->setFocus();

            }
            history->prepend(form->getCurrentLine()->getTextInput()->toPlainText());
            if(history->size()>30){ //max history is 30 entries.
                history->removeLast();
            }
            cas->evaluate();
            time->start();

        }
        break;
    case MainSheet::SPREADSHEET_TYPE:
    case MainSheet::G2D_TYPE:
    case MainSheet::PROGRAMMING_TYPE:
        break;
    }
}
void MainWindow::evaluateall(){

    if(isEvaluatingAll()){
        return;
     }
    autoSave();
    setWindowModified(true);
    displayInStatusBar("","black");
    //qInfo()<<"evaluateall ";
    taskProperties.firstPrintMessage=true;
    taskProperties.currentSheet=tabPages->currentIndex();
    taskProperties.currentLine=-1;
    MainSheet* sheet=0;
  for(int j=0;j<tabPages->count()-1;j++){
    //MainSheet* sheet=dynamic_cast<MainSheet*>(tabPages->currentWidget());
    //qInfo()<< "tabPages: " << j;
    tabPages->setCurrentIndex(j);
    sheet=dynamic_cast<MainSheet*>(tabPages->currentWidget());
    switch(sheet->getType()){
    case MainSheet::FORMAL_TYPE:
        {FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->currentWidget());

        //evaluateallAction->setVisible(false);
        //stopallAction->setVisible(true);
        evaluateallAction->setIcon(QIcon(":/images/evaluate-buisy.png"));
        setEvaluatingAll(true);
        for(int i=0;i< form->getLinesSize() ;i++){


            //be sure that previous computation is finished and displayed before a new output.
            if(cas->isRunning()){
                //qInfo()<<"starting buisyloop" << "i="<<i;
                cas->buisyloop->exec();
                //qInfo()<<"finished; I can compute:"
            }
            if(! isEvaluatingAll()){break;} // external stop button
            //if(){qInfo() <<"PB: display not done";}
            form->setCurrent(i);
            taskProperties.currentLine=i;
            //qInfo()<<"line i" << form->getLineAt(i) <<"i" <<i << " id:" <<form->getLineAt(i)->getId();
            const QString formula=form->getLineAt(i)->getTextInput()->toPlainText();
            //qInfo()<<"formula "<<formula<<" index i="<<i;
            form->getLineAt(i)->evaluate(formula);

        }
        evaluateallAction-> setIcon(QIcon(":/images/evaluate.png"));
        setEvaluatingAll(false);
        //evaluateallAction->setVisible(true);
        }
        break;
    case MainSheet::SPREADSHEET_TYPE:
    case MainSheet::G2D_TYPE:
        {
            GraphWidget * g2d =qobject_cast<GraphWidget*>(tabPages->currentWidget());
            QStringList displaycommands;
            g2d->getDisplayCommands(displaycommands);
            //g2d->clearcanvas();
            int i=tabPages->currentIndex();
            QString titre=tabPages->tabText(i);
            delete g2d;
            tabPages->insertG2dSheet(i,titre);
            g2d=qobject_cast<GraphWidget*>(tabPages->currentWidget());
            for(int i=0;i< displaycommands.size();i++){
                //qInfo()<<"display commands "<<displaycommands.at(i);
                g2d->sendText(displaycommands.at(i));
            }
            g2d->updateAllCategories();
            g2d->repaint();
        break;
        }
    //evaluateallAction->setVisible(false);
    case MainSheet::PROGRAMMING_TYPE:
        break;
    }
 }//fin for tabpages
}
void MainWindow::evaluateforinsertion(const QString &formula){
    if(formula == ""){ return;}
    autoSave();
    setWindowModified(true);
    displayInStatusBar("","black");
    taskProperties.firstPrintMessage=true;
    taskProperties.currentSheet=tabPages->currentIndex();
    taskProperties.currentLine=-1;
    MainSheet* sheet=dynamic_cast<MainSheet*>(tabPages->currentWidget());
    switch(sheet->getType()){
    case MainSheet::FORMAL_TYPE:
        {FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->currentWidget());
            form->getCurrentLine()->addStopButton(stopButton);

            if(cas->isRunning()){
                cas->buisyloop->exec();
                //qInfo()<<"finished; I can compute:"
            }

            CasManager::warning id=cas->initExpression(&formula);

            taskProperties.currentLine=form->getCurrentLine()->getId();
            cas->evaluateforinsertion();
            time->start();

        }
        break;
    case MainSheet::SPREADSHEET_TYPE:
    case MainSheet::G2D_TYPE:
    case MainSheet::PROGRAMMING_TYPE:
        break;
    }
}

void MainWindow::displayResult(){
    //qInfo()<<"currentLine in displayresult"<<taskProperties.currentLine;
    displayGiacMessages();
    tabPages->setCurrentIndex(taskProperties.currentSheet);
    // Formal Sheet
    if (taskProperties.currentLine>-1){
        FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->widget(taskProperties.currentSheet));

        form->removeStop(taskProperties.currentLine);
        stopButton->setParent(this);

//        return form->getLineAt(taskProperties.currentLine)->getOuputWidget();
        form->displayResult(taskProperties.currentLine,cas->createDisplay()); 
    }
        //qInfo()<<"end displayresult cur line" << taskProperties.currentLine;
        cas->setdisplaydone();
}

void MainWindow::insertResult(){
    displayGiacMessages();
    tabPages->setCurrentIndex(taskProperties.currentSheet);
    // Formal Sheet
    if (taskProperties.currentLine>-1){
        FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->widget(taskProperties.currentSheet));

        form->removeStop(taskProperties.currentLine);
        stopButton->setParent(this);

        //form->displayResult(taskProperties.currentLine,cas->createDisplay());
        QTextCursor cs=form->getCurrentLine()->getTextInput()->textCursor();

        int pos=cs.selectionStart();
        //form->getCurrentLine()->getTextInput()->cut();
        cs.beginEditBlock();
        cs.insertText(QString::fromStdString(giac::print(cas->getAnswer(),cas->getContext())));
        cs.setPosition(pos,QTextCursor::KeepAnchor);
        cs.endEditBlock();
        form->getCurrentLine()->getTextInput()->setTextCursor(cs);

    }
    cas->setdisplaydone();
}
QToolButton* MainWindow::getStopButton() const{
    return stopButton;
}


void MainWindow::evaluate(){
    MainSheet* sheet=dynamic_cast<MainSheet*>(tabPages->currentWidget());
    switch(sheet->getType()){
    case MainSheet::FORMAL_TYPE:
        {FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->currentWidget());
                evaluate(form->getCurrentLine()->getTextInput()->toPlainText());
        }
        break;
    case MainSheet::SPREADSHEET_TYPE:
    case MainSheet::G2D_TYPE:
    case MainSheet::PROGRAMMING_TYPE:
        break;
    }

}

void MainWindow::displayHelp(const QString & keyWord) const{
   wizardList->setCurrentRow(2);
   leftPanel->show();
   (qobject_cast<WizardCatalog*>(wizardPages->currentWidget()))->displayPage(QUrl(keyWord));
}

void MainWindow::displayXcasHelp(const QString & keyWord) const{
   wizardList->setCurrentRow(2);
   //CasManager::warning id=cas->initExpression(&keyWord);

   giac::gen expression(keyWord.remove("?").toStdString(),giac::context0);

   QString rep=cas->xcashtmlHelp(expression);

   if (! rep.isEmpty()){
       while(rep.contains("/")){
           rep.remove(QRegExp("^.*/"));
       }
       //qInfo()<<"found xcas html doc:"<<rep;
       displayHelp(rep);
   }

}
void MainWindow::displayHome() const{
   wizardList->setCurrentRow(2);
   (qobject_cast<WizardCatalog*>(wizardPages->currentWidget()))->displayHome();
}
void MainWindow::sendText(const QString & s){
    MainSheet* sheet=dynamic_cast<MainSheet*>(tabPages->currentWidget());
    switch(sheet->getType()){
    case MainSheet::FORMAL_TYPE:
        {FormalWorkSheet *form=qobject_cast<FormalWorkSheet*>(tabPages->currentWidget());
        form->sendText(s);}
        break;
    case MainSheet::SPREADSHEET_TYPE:
        break;
    case MainSheet::G2D_TYPE:
      {GraphWidget * g2d=qobject_cast<GraphWidget*>(tabPages->currentWidget());
	 g2d->sendText(s);}
       break;
    case MainSheet::PROGRAMMING_TYPE:
        break;
    }
}

bool MainWindow::isEvaluating(){
    return cas->isRunning();
}

bool MainWindow::isEvaluatingAll(){
    return isevaluatingall;
}

void MainWindow::setEvaluatingAll(bool status){
    isevaluatingall=status;
}

giac::context* MainWindow::getContext() const{
    return cas->getContext();
}

CommandInfo* MainWindow::getCommandInfo()const{
return commandInfo;
}
void MainWindow::displayStopWarning(){

        displayInStatusBar(tr("Tentative d'interruption..."),"red");
//    statusBar()->addWidget(warningStop,1);
//   warningStop->show();
}
void MainWindow::removeStopWarning(){
    displayInStatusBar("","black");
/*    if (warningStop->parent()!=0){
       statusBar()->removeWidget(warningStop);
        warningStop->setParent(0);
    }*/
}
void MainWindow::displayCrashWarning(){
    QMessageBox::warning(this,tr("Avertissement!"),tr("Risque de crash important lors de l'interruption requise."),QMessageBox::Ok,QMessageBox::NoButton);
    emit(hideCrashWarning());
}


CommandInfo::CommandInfo(){
    listAllCommands();
    commandList.prepend("");
    completer=new QCompleter(commandList);

}
void CommandInfo::listAllCommands(){
    QFile file(":/aide_cas");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString line;
    // Only take command with alpanumeric characters
    // For example %{ %} is excluded.

    QRegExp expr("([a-z]|[A-Z]|[_]|[0-9])+");

    while(!stream.atEnd()){
        line=stream.readLine();
        if (line.startsWith("#")) {
            QStringList list=line.remove(0,2).split(" ",QString::SkipEmptyParts);
            for(int i=0;i<list.size();++i){
                QString s(list.at(i));
                if (expr.exactMatch(s))
                    commandList.append(list.at(i));
            }
        }
    }

    commandList.sort();
}

QString CommandInfo::minimaltoHtml(const QString & source) const{
    QString tmp=source;
    tmp.replace("&","&amp;");
    tmp.replace("<","&lt;");
    tmp.replace(">","&gt;");
    return tmp;
}

QString CommandInfo::displayPage(const QString& keyWord) const{
    QFile file(":/aide_cas");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    QString command,description;
    QStringList synonym,seeAlso,examples;
    QString line;
    QString kwdsearch;

    while(!stream.atEnd()){
        line=stream.readLine();
        if (line.startsWith("#")&& (line.contains(" "+keyWord+" ",Qt::CaseSensitive)||line.endsWith(" "+keyWord,Qt::CaseSensitive))) {
            QStringList list=line.split(" ",QString::SkipEmptyParts);
            for(int i=0;i<list.size();++i){
                if (i==1) command=list.at(1);
                else if (i>1) synonym.append(list.at(i));
            }
            kwdsearch=command; // on sauve le nom de la commande
            do{
                line=stream.readLine();
                if (line.startsWith("#")) break;
                if (line.startsWith("0")){
                    list=line.split(" ",QString::SkipEmptyParts);
                    if (list.size()>1){
                        command.append("(");
                        for(int i=1;i<list.size();++i){
                            command.append(list.at(i));
                        }
                        command.append(")");
                    }
                }
		//else if (line.startsWith("1")){
                else if (line.startsWith(QString::number(Config::giaclanguage))){  //giac lang number
                    description=line.remove(0,2);
                }
                else if (line.startsWith("-")){
                    list=line.split(" ",QString::SkipEmptyParts);
                    if (list.size()>1) seeAlso.append(list.at(1));
                }
                else if (line.length()>0 ){
                    QChar c=line.at(0);
                    if (!QString("123456789").contains(c))
                     examples.append(line);
                }
            }  while (!stream.atEnd());
            break;
        }
        }
        line.clear();
        line.append("<h3><font color=\"#40A497\">").append(minimaltoHtml(command)).append("</font> <a href=\"!"+command.split("(").at(0)+"\">-></a></h3>");
        if (!synonym.isEmpty()){
            line.append("<b>").append(QObject::tr("Synonyme(s):")).append("</b>");
            for( int i=0;i<synonym.size();++i){
                line.append(" ").append(minimaltoHtml(synonym.at(i)));
            }
        }
        line.append("<hr>");
        line.append(minimaltoHtml(description));
        //
#if QT_VERSION < 0x050000
        qDebug()<<command<<"fin de commande\n";
#else
        qInfo()<<command<<"fin de commande\n";
#endif

        //kwdsearch="?"+command.split(QRegExp("[\s(]")).at(0);
        kwdsearch.prepend("?");
#if QT_VERSION < 0x050000
                qDebug()<<kwdsearch;
#else
                qInfo()<<kwdsearch;
#endif
        if(kwdsearch !="?"){
        line.append(" <a href=\"").append(kwdsearch).append("\">").append(QObject::tr("(Plus de détails)</a>"));
        //
        }
        line.append("<br>");
        if (!examples.isEmpty()){
            line.append("<br><b>").append(QObject::tr("Exemples:")).append("</b><br>");
            for( int i=0;i<examples.size();++i){
                if(!examples.at(i).contains('"'))
                    line.append(minimaltoHtml(examples.at(i))).append("  <a href=\"!"+examples.at(i)+" \">-></a><br>");
                else
                    line.append(minimaltoHtml(examples.at(i))).append("<br>");

            }
        }
        if (!seeAlso.isEmpty()){
            line.append(QObject::tr("Voir aussi: ")).append("<ul>");
            for( int i=0;i<seeAlso.size();++i){
                line.append("<li><a href=\"").append(seeAlso.at(i)).append("\">").append(minimaltoHtml(seeAlso.at(i))).append("</a></li>");
            }
            line.append("</ul>");
        }
        return(line);
    }

QString CommandInfo::seekForKeyword(const QString & inputkeyWord) const{

        //consider if the keyword can be translated by giac.
        QString keyWord=QString::fromStdString(giac::unlocalize(inputkeyWord.toStdString()));
        //qInfo()<<"traduction par giac:"<<keyWord;

        QFile file(":/aide_cas");
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        QString currentCommand;
        QString html;
        QString html1;
        QString html2;
        QString line;

        while(!stream.atEnd()){
            line=stream.readLine();
            if (line.startsWith("#")) currentCommand=line.remove(0,2);
            else if (line.startsWith(QString::number(Config::giaclanguage))&&  // giac lang
                     (line.contains(keyWord,Qt::CaseInsensitive)||currentCommand.contains(keyWord,Qt::CaseInsensitive))){
                if(currentCommand.startsWith(keyWord+" ") || currentCommand==keyWord){
                    html.append("<a href=\"");
                    html.append(currentCommand);
                    html.append("\">");
                    html.append(minimaltoHtml(currentCommand));
                    html.append("</a> ");
                    if(keyWord != inputkeyWord){
                        html.append(" <b>("+inputkeyWord+")</b><br>\n");
                    }
                    else{
                        html.append("<br>\n");
                    }
                    html.append(minimaltoHtml(line.remove(0,2)));
                    html.append("<br><br>");
                }
                else if(currentCommand.contains(keyWord)){
                    html1.append("<a href=\"");
                    html1.append(currentCommand);
                    html1.append("\">");
                    html1.append(minimaltoHtml(currentCommand));
                    html1.append("</a><br>\n");
                    html1.append(line.remove(0,2));
                    html1.append("<br><br>");
                    }
                    else{
                    html2.append("<a href=\"");
                    html2.append(currentCommand);
                    html2.append("\">");
                    html2.append(minimaltoHtml(currentCommand));
                    html2.append("</a><br>\n");
                    html2.append(line.remove(0,2));
                    html2.append("<br><br>");
                }
            }
        }
        html.append(html1);
        html.append(html2);
        if (html.isEmpty()) {
            return(QObject::tr("Aucun résultat"));
        }
        else return(html);

    }
bool CommandInfo::isCommand(const QString &st) const{
    QString keyWord=QString::fromStdString(giac::unlocalize(st.toStdString()));
    return (commandList.contains(keyWord));
}
QCompleter* CommandInfo::getCompleter() const{
    return completer;
}
QStringList CommandInfo::getCommands(){
    return commandList;
}
