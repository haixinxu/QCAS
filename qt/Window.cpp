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

#include "Window.h"

MainWindow::MainWindow(QWidget *parent){
  setWindowTitle(tr("Qtgiac example"));

  contextptr = new giac::context;
  m_Input = new QLineEdit;
  m_Output = new QLineEdit;

  m_spacer = new QSpacerItem(20,40, QSizePolicy::Minimum, QSizePolicy::Expanding);

  m_Layout = new QVBoxLayout;
  m_Layout->addWidget(m_Input);
  m_Layout->addWidget(m_Output);
  m_Layout->addSpacerItem(m_spacer);

  Fen = new QWidget;
  Fen->setLayout(m_Layout);
  setCentralWidget(Fen);

  connect(m_Input, SIGNAL(returnPressed()), this, SLOT(Evaluate()));
}

void MainWindow::Evaluate(){
  QString Expression = qobject_cast<QLineEdit *>(sender())->text();
  std::string S = Expression.toStdString(); // convert to standard string
  giac::gen g(S,contextptr); // parse it 
  g = g.eval(1,contextptr); // eval it
  S = g.print(contextptr); // print answer
  m_Output->setText(QString::fromStdString(S));//and convert to qt
  m_Input->clear();
}

MainWindow::~MainWindow(){
  delete contextptr;
  delete Fen;
  /*
  delete m_Layout;
  delete m_Output;
  delete m_Input;
  delete m_spacer;
  */
}
