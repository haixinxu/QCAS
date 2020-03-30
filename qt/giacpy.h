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

#include "sizeof_void_p.h"
#include <giac/giac.h>
#include <QApplication>
#include <QTextCodec>
#include "MainWindow.h"
#include "output.h"
#include <locale.h>

#ifndef QCAS_GIACPY
#define QCAS_GIACPY



int externalqcas( giac::gen &  , giac::context *);
int externalinteractiveqcas( giac::gen & c , giac::context * ct, std::string s);



#endif
