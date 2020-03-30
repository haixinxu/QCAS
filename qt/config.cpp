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

#include "config.h"

Config::Config(){}

//bool Config::ortho=false;
//bool Config::autoscale=true;
int Config::graph_width=400;
bool Config::gridAttraction=true;
int Config::mml_fontsize=14;//default mml size

/* We add a prefix  to automatic names of geometric objects for storing in giac to avoid unexpected 
   non free variables names in formal calculus.
*/
QString Config::GeoVarPrefix="autoG2D\_"; //obsolete, trying to remove it.

QString Config::QcasVersion="0.5.3"; //

Config::LANGUAGES Config::language=Config::FRENCH;
int Config::giaclanguage=1;
QString Config::GiacHtmlLanguage="fr"; // for html doc

bool Config::useTabCompletions=true;
QString Config::XcasRoot="";

