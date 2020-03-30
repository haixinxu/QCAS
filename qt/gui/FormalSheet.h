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

#ifndef FORMALSHEET_H
#define FORMALSHEET_H

#include <QVector>
#include <QScrollArea>
#include "CentralTabWidget.h"
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QDomElement>
#include "output.h"

class OutputWidget;
class QCheckBox;
class QGridLayout;
class QVBoxLayout;
class QSyntaxHighlighter;
class QTextDocument;
class MainWindow;
class QCompleter;
class Line;
class CommandInfo;


struct DelimiterInfo
{
    QChar character;
    int position;
};
class TextBlockData : public QTextBlockUserData
{
public:
    TextBlockData();
    DelimiterInfo* infoAt(const int);
    void insert(DelimiterInfo *info);
    int size() const;
private:
    QVector<DelimiterInfo *> delimiters;
};


/**
  This class is used to highlight QTextEdit
  it supports
  - Comments  //...  or / * ... * /
  - quotation "..."
  - keyword highlighting
  **/

class Highlighter:public QSyntaxHighlighter{
    Q_OBJECT
public:
    Highlighter(QTextDocument * parent=0,CommandInfo* cas=0);
//    static bool isQuotationFormat(const QTextCharFormat &) ;

protected:
    void highlightBlock(const QString &text);
private:
    enum style{
     keyword=1,comment=2,
     quotation=3
    };
    CommandInfo* commandInfo;

    QTextCharFormat quotationFormat;
    QTextCharFormat keywordFormat;
    QTextCharFormat commentFormat;

};



class FormalWorkSheet: public QScrollArea, public MainSheet{
    Q_OBJECT

public:
    FormalWorkSheet(MainWindow *parent=0);
    ~FormalWorkSheet();
    MainWindow* getApp();
    void goToNextLine();
    void goToNextExistingLine();
    void goToPreviousExistingLine();
    void gotoFirstLine();
    void setCurrent(const int);
    int getCurrent() const;
    Line* getCurrentLine();
    Line* getLineAt(int);
    int FormalWorkSheet::getLinesSize();
    void setFocus(Qt::FocusReason);
    void copy();
    void cut();
    void paste();
    void undo();
    void redo();
    void insertline();
    void deleteSelectedLevels();
    void sendSelectedLevels(GraphWidget *);
    void sendCurrentLine(GraphWidget *);
    void sendText(const QString &);
    void displayResult(int line,OutputWidget* );
    void addSelectedLevel(int);
    void removeStop(int);
    void toXML(QDomElement & root);
    void toGIAC(QString &);
    void toXCAS(QString &);

    QVector<int>selectedLevels;

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:
    bool shift;
    QWidget *mainPanel;
    int current;
    QVector <Line*>*lines;
    QVBoxLayout* vLayout;
    MainWindow *mainWindow;
};

#endif // FORMALSHEET_H
