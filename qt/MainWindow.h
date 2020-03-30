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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "sizeof_void_p.h"
#include "gui/CentralTabWidget.h"
#include <QMainWindow>
#include "ui_MainWindow.h"
#include <QGroupBox>
#include <giac/giac.h>
class QAction;
class QListWidget;
class QStackedWidget;
class QMenu;
class QListWidgetItem;
class QToolBar;
class CasManager;
class MainTabWidget;
class QPlainTextEdit;
class QLabel;
class QCompleter;
class QToolButton;
class QTime;
class QTranslator;
class PrefDialog;
struct TaskProperties{
    bool firstPrintMessage;
    int currentLine;
    int currentSheet;
};
class CommandInfo{
public:
    CommandInfo();
    QString seekForKeyword(const QString &) const;
    QString displayPage(const QString &) const;
    QCompleter* getCompleter() const;
    QStringList getCommands();
    QString minimaltoHtml(const QString &) const;
    bool isCommand(const QString &) const;
private:
    QCompleter *completer;
    void listAllCommands();
    QStringList commandList;
};

class MainWindow :public QMainWindow {
    Q_OBJECT
 public:
    MainWindow();
    MainWindow(int startingtabmode); //Ex: giacpy
    void changeEvent(QEvent*, QString*);
    void retranslateInterface(int);
    void displayHelp(const QString &) const;
    void displayXcasHelp(const QString &) const;
    void displayHome() const;
    void sendText(const QString &);
    bool isEvaluating();
    bool isEvaluatingAll();
    void setEvaluatingAll(bool);
    CommandInfo* getCommandInfo() const;
    void evaluate(const QString & formula);
    void evaluateforinsertion(const QString & formula);
    QToolButton* getStopButton() const;
    giac::context * getContext() const;
    void displayStopWarning();
    void displayInStatusBar(const QString &,const QString&);
    void updateInterface(MainSheet::sheetType);
    void setRedoButton(bool);
    void setUndoButton(bool);
    int getDecimalDigit() const;
    void setDecimalDigits(const int & a);
    bool loadFile(const QString &fileName);
    bool appendFile(const QString &fileName);
    bool autoSave();
    void cleanautoSaveFiles();
    bool askforXcasarchive();
    void loadgiacgen( const giac::gen & g, giac::context * ct); //cf giacpy
    void loadinteractivegiacgen( const giac::gen & g, giac::context * ct); //cf giacpy
    QStringList *history;
    QString lastgiacmessage;


protected:
    void closeEvent(QCloseEvent *event);

private:
    void clearWorkspace();
    void createGui();
    void retranslateGui();
    void createWizards();
    void createAction();
    void retranslateAction();
    void createToolBars();
    void createMenus();
    void retranslateMenus();
    void createContextMenu();
    void readSettings();
    void writeSettings();
    void initAutoSave();
    bool okToContinue();
    bool isevaluatingall;
    bool loadQcasFile(const QString &fileName);
    bool loadGiacFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    bool saveToGiacFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString curFile;
    QStringList recentFiles;
    QString strippedName(const QString & fullFileName);
    void displayGiacMessages();
    enum{MaxRecentFiles=5};
    QAction* recentFileActions[MaxRecentFiles];
    QAction* separatorAction;
    QLabel* labelStatus;
    Ui::MainWindow ui;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *selectSubMenu;
    QMenu *toolsMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAction;
    QAction *openAction;
    QAction *appendfAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction * hintAction;
    QAction *insertlineAction;
    QAction *deleteLevelAction;
    QAction *sendLeveltointerAction;
    QAction *evaluateAction;
    QAction *evaluateallAction;
    QAction *stopallAction;
    QAction *prefAction;
    QAction *htmlhelpAction;
    QAction *aboutAction;
    QGroupBox * giacPanel;

    QAction *stopAction;
    QToolButton * stopButton;

    QWidget* leftPanel; QWidget*rightPanel;
    QListWidget *wizardList;
    QStackedWidget *wizardPages;

    PrefDialog *prefDialog;
    MainTabWidget *tabPages;
    QPlainTextEdit* giacMessages;
    QToolBar* toolBar;

    QListWidgetItem *matrixItem;
    QListWidgetItem *equationItem;
    QListWidgetItem *catalogItem;
    QListWidgetItem *algoItem;
    QListWidgetItem *algoscolItem;


//    QLabel* warningFirstEvaluation;
//    QLabel* warningStop;
    CasManager* cas;
    TaskProperties taskProperties;
    CommandInfo* commandInfo;
    QTime* time;
    QTranslator * translator;
    bool displayTimeAfterProcess;
    void printHeader();
    int decimalDigits;

    QString autosaveFileName;

public slots:
    void displayCrashWarning();
    void removeStopWarning();

private slots:
    void newFile();
    void open();
    void appendf();
    bool save();
    bool saveAs();
    bool saveAsgiacxcas();
    void about();
    void astuces();
    void pref();
    void copy();
    void htmlhelp();
    void cut();
    void paste();
    void undo();
    void redo();
    void insertline();
    void deleteSelectedLevels();
    void sendSelectedLevels();
    void sendCurrentLine();
    void evaluate();
    void evaluateall();
    void openRecentFile();
    void changeWizard(QListWidgetItem*,QListWidgetItem*);
    void displayResult();
    void insertResult();
    void killThread();
signals:
    void hideCrashWarning();
};
#endif // MAINWINDOW_H
