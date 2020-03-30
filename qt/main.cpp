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
#include <QTextCodec>
#include "MainWindow.h"
#include "config.h"
#include <locale.h>

int main(int argc, char * argv []){
    QApplication app(argc, argv);
    setlocale(LC_NUMERIC,"POSIX");

#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif

    MainWindow win;
    if(argc>1){
      if(win.loadFile(argv[1])){win.show();}
      else{return 1;}
    }
    else{win.show();}

    return app.exec();

}
