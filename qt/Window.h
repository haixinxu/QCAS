/*
 * Adapted by B. Parisse from Android preliminary interface
 *  Copyright (C) 2012 Thomas Luka, 18 rue des feuilles mortes, 95 Vaureal
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef WINDOW_H
#define WINDOW_H
#include <QtGui>
#include <QtCore>
#include <QApplication>
#include "giac.h"

class MainWindow : public QMainWindow{
  Q_OBJECT
    
    public:
  
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
  public slots:
  
  void Evaluate();
  
 private:
  giac::context * contextptr;
  QVBoxLayout *m_Layout;
  QLineEdit *m_Input, * m_Output;
  QAction *m_Aide;
  QWidget * Fen;
  QSpacerItem *m_spacer;
};

#endif //WINDOW_H
