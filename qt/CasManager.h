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

#ifndef CASMANAGER_H
#define CASMANAGER_H
#include <streambuf>
#include <QThread>
#include <QDomElement>
#include <QStringList>
//#include "global.h"
#include "sizeof_void_p.h"
#include <giac/gen.h>
#include "MainWindow.h"

class OutputWidget;
class MonitorThread:public QThread{
    Q_OBJECT
public:
    MonitorThread(giac::context *c);
protected:
    void run();
private:
    giac::context* contextptr;

};
class StopThread:public QThread{
    Q_OBJECT
public:
    StopThread(giac::context *);
protected:
    void run();
private:
    bool go;
    giac::context* contextptr;

public slots:
    void setContinueTrue();
signals:
    void startDirtyInterrupt();
};


/**
 the class mybuf is used to redirect the standard std::cout
 We can catch print("....") commands, and all comment for libgiac
  **/

class mybuf: public std::streambuf{
 private:
   void    put_buffer(void);
   void    put_char(int);
   CasManager *cas;

   protected:
   int    overflow(int);
   int    sync();
   public:
   mybuf(CasManager *,int = 0);
   // ~mybuf();
 };
class MyStream: public std::ostream{
 public:
   MyStream(CasManager*,int = 0);
 };

void callback(const giac::gen & ,void * );

/**
  The main class which handles with the libGiac
  This is an interface for main Window to deal with the libGiac (run command, add to history ...)
  **/
class CasManager :QObject{
public:
    enum warning{No_warning,Warning,Error};
    ~CasManager();
    CasManager(MainWindow *main );
    void evaluate();
    void evaluateforinsertion();
    bool isRunning() const;
    void setdisplaydone();
    void killThread();
    giac::context* getContext() const;
    giac::gen getAnswer() const;
    CasManager::warning initExpression(const QString *str);
    OutputWidget* createDisplay();
    void appendPrintCache(const QChar& );
    QStringList& getGiacDisplay() ;
    void clearGiacDisplay();
    void toXML(QDomElement &, const bool &archivecontext=false);
    void loadXML(const QDomElement &, const bool &archivecontext=false);
    void loadGeneralXML(const QDomElement &);
    QEventLoop * buisyloop; //buisyloop->exec() will wait the end of display
    QString xcashtmlHelp(const giac::gen &);

private:
    static giac::gen answer;
    static bool displaydone;
    MainWindow* mainWindow;
    MonitorThread* monitor;
    StopThread* stopThread;
    QString printCache;
    QStringList fullDisplay;
    static void callback(const giac::gen & ,void * );
    QString gen2mathml(const giac::gen &);
    bool testExpression(const giac::gen& );
    giac::gen expression;
    giac::context *context;
    void info(giac::gen &, int decal ) const;
     OutputWidget* graph2Widget(const giac::gen&);
    QString displayType(int c) const;
    QString displaySubType(int c) const;





 };





#endif // CASMANAGER_H
