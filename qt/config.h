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

#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config{

public:
    Config();
//    static bool ortho,autoscale;
    static int graph_width;
/* Warning: in giac the languages are like this:  
  int string2lang(const string & s){
    if (s=="fr")
      return 1;
    if (s=="en")
      return 2;
    if (s=="sp" || s=="es")
      return 3;
    if (s=="el")
      return 4;
    if (s=="pt")
      return 5;
    if (s=="it")
      return 6;
    if (s=="tr")
      return 7;
    if (s=="zh")
      return 8;
    if (s=="de")
      return 9;
    return 0;
  }
*/
    enum LANGUAGES{
       FRENCH=0,
       ENGLISH=1,
       SPANISH=2,
       GREEK=3,
       CHINESE=4
    };
    static LANGUAGES language;
    static int giaclanguage;
    static QString GiacHtmlLanguage; // for html doc
    static bool gridAttraction;
    static bool useTabCompletions;
    static int mml_fontsize;
    static QString GeoVarPrefix;
    static QString QcasVersion;
    static QString XcasRoot;//base directory for xcas doc/
};

#endif // CONFIG_H
