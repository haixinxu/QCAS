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

#include <QKeyEvent>
#include <QToolTip>
#include <QScrollBar>
#include <QDebug>
#include <QCompleter>
#include <QHBoxLayout>
#include <QAbstractItemView>
#include "FormalSheet.h"
#include "FormalLineWidgets.h"
#include "FormalLine.h"
#include "../MainWindow.h"
#include "../config.h"
#include "qtmmlwidget.h"

TextInput::TextInput(Line *parent):QPlainTextEdit(parent){
    line=parent;
    historylevel=-1;
    // +1 devrait sufire en théorie mais problème sous KDE plasma sur mon netbook
    setFixedHeight(fontMetrics().lineSpacing()+fontMetrics().descent()+2*document()->documentMargin()+6);
    highlighter=new Highlighter(document(),line->getWorkSheet()->getApp()->getCommandInfo());
    setTabStopWidth(40);
    connect(document(),SIGNAL(contentsChange(int,int,int)),this,SLOT(addMultiLines(int,int,int)));
    connect(this,SIGNAL(cursorPositionChanged()),this,SLOT(matchDelimiters()));
}
TextInput::~TextInput(){
    delete highlighter;
}

void TextInput::matchDelimiters(){
    QList<QTextEdit::ExtraSelection> selections;
       setExtraSelections(selections);

     TextBlockData *data =static_cast<TextBlockData *>(
                textCursor().block().userData());
    if (data) {
       int pos = textCursor().block().position();
       int curPos = textCursor().position()-pos;
       for (int i = 0; i < data->size(); ++i) {
          DelimiterInfo *info = data->infoAt(i);
          // the previous character is a delimiter
          if (info->position == curPos-1){
              int index=QString("([{").indexOf(info->character);
              if (index!=-1){
                  QChar right=QString(")]}").at(index);
                  if (matchLeftDelimiter(info->character,right,textCursor().block(), i+1, 0))
                      createDelimiterSelection(pos + info->position,true);
                    else createDelimiterSelection(pos + info->position,false);
              }
              else {
                  index=QString(")]}").indexOf(info->character);
                  if (index!=-1){
                      QChar left=QString("([{").at(index);
                      if (matchRightDelimiter(left,info->character,textCursor().block(), data->size()-i, 0))
                          createDelimiterSelection(pos + info->position,true);
                        else createDelimiterSelection(pos + info->position,false);
                  }
              }
          }
         }
      }
   }


void TextInput::createDelimiterSelection(int pos,bool found)
{
    QList<QTextEdit::ExtraSelection> selections = extraSelections();

    QTextEdit::ExtraSelection selection;
    QTextCharFormat format = selection.format;
    if (found) format.setBackground(Qt::green);
    else format.setBackground(Qt::red);
    selection.format = format;

    QTextCursor cursor = textCursor();
    cursor.setPosition(pos);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    selection.cursor = cursor;

    selections.append(selection);

    setExtraSelections(selections);
}

bool TextInput::matchLeftDelimiter(QChar left,QChar right,QTextBlock currentBlock, int i, int numLeftParentheses){
    TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());
    int docPos = currentBlock.position();
    for (; i < data->size(); ++i) {
        DelimiterInfo *info = data->infoAt(i);
        if (info->character == left) {
            ++numLeftParentheses;
            continue;
        }
        if (info->character == right){
            if(numLeftParentheses == 0) {
                createDelimiterSelection(docPos+info->position,true);
                return true;
            }
            else --numLeftParentheses;
        }
    }
    currentBlock = currentBlock.next();
    if (currentBlock.isValid())
        return matchLeftDelimiter(left,right,currentBlock, 0, numLeftParentheses);
    return false;
}
bool TextInput::matchRightDelimiter(QChar left,QChar right,QTextBlock currentBlock, int i, int numRightParentheses)
{
    TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());

    int docPos = currentBlock.position();
    for (; i < data->size() ; ++i) {
        DelimiterInfo *info = data->infoAt(data->size()-i-1);
        if (info->character == right) {
            ++numRightParentheses;
            continue;
        }
        if (info->character == left){
            if (numRightParentheses == 0) {
                createDelimiterSelection(docPos + info->position,true);
                return true;
            }
            else
                --numRightParentheses;
        }
    }
    currentBlock = currentBlock.previous();
    if (currentBlock.isValid())
        return matchRightDelimiter(left,right,currentBlock, 0, numRightParentheses);

    return false;
}



void TextInput::installCompleter(){
    MainWindow * qmw=line->getWorkSheet()->getApp();
    completer=qmw->getCommandInfo()->getCompleter();
    completer->setWidget(this);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    if(giac::xcas_mode(qmw->getContext())==1)
         completer->setCaseSensitivity(Qt::CaseInsensitive);//maple_mode
    else
        completer->setCaseSensitivity(Qt::CaseSensitive);

    connect(completer,SIGNAL(highlighted(QString)),this,SLOT(helpCompletion(QString)));
    connect(completer,SIGNAL(activated(QString)),this,SLOT(insertCompletion(QString)));
}


void TextInput::keyPressEvent(QKeyEvent *e){
    if (completer&&completer->popup()->isVisible()){
        switch(e->key()){
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_ParenLeft:
            case Qt::Key_Backtab:
                e->ignore();
                return;
        default:
                break;
        }

    }
    switch(e->key()){
        // The mouse is above a command name
        // User has pressed F1 to read inline help
        case Qt::Key_F1:
        if (QToolTip::isVisible()){
            line->getWorkSheet()->getApp()->displayHelp(
                     property("myToolTip").toString());
            setFocus();
        }
        break;
        // User has pressed Space
        // Checking if Ctrl is pressed to enable autocompletion
        case Qt::Key_Space:
            if (e->modifiers()&Qt::ControlModifier){
                updateCompleter();
            }
            else QPlainTextEdit::keyPressEvent(e);
            break;
        case Qt::Key_Tab:
            //if (Config::useTabCompletions && !textUnderCursor().isEmpty()){
        if (Config::useTabCompletions && !isStartCursor()){
               updateCompleter();
            }
            else QPlainTextEdit::keyPressEvent(e);
            break;
        // User has pressed Return or Enter Key
        // If Shift is pressed, evaluate and go to next line
        // else adjust the size of the text zone
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if (e->modifiers()&Qt::ShiftModifier) {
                //prevent crash when enter is pressed during evaluation
                if(! line->getWorkSheet()->getApp()->isEvaluating() ){
                    line->evaluate(toPlainText());
                }
            }
            else {
                QPlainTextEdit::keyPressEvent(e);
                setFixedHeight(height()+fontMetrics().lineSpacing()+1);
            }
         break;

        case Qt::Key_Down:
            if (e->modifiers()&Qt::ControlModifier){

                if(historylevel>0){
                     this->undo();
                     historylevel--;
                     this->textCursor().beginEditBlock();
                     if(historylevel<line->getWorkSheet()->getApp()->history->size()){
                        this->textCursor().insertText(line->getWorkSheet()->getApp()->history->at(historylevel));

                     }
                     this->textCursor().endEditBlock();
                }

            }
            else{
                if (e->modifiers()&Qt::ShiftModifier){
                    //we stay in this line during selection
                    QPlainTextEdit::keyPressEvent(e);
                }
                else{
                    if (goDown()){
                     QPlainTextEdit::keyPressEvent(e);
                    }
                }
            }
            break;

        case Qt::Key_Up:
            if (e->modifiers()&Qt::ControlModifier){

                if(historylevel<line->getWorkSheet()->getApp()->history->size()-1){
                    historylevel++;
                }
                if(historylevel>0){this->undo();}
                if(historylevel>=0){
                    this->textCursor().beginEditBlock();
                    this->textCursor().insertText(line->getWorkSheet()->getApp()->history->at(historylevel));
                    this->textCursor().endEditBlock();
                }
            }
            else{
                if (e->modifiers()&Qt::ShiftModifier){
                    //we stay in this line during selection
                    QPlainTextEdit::keyPressEvent(e);
                }
                else{
                    if (goUp()){
                        QPlainTextEdit::keyPressEvent(e);
                    }
                }
            }
            break;
        case Qt::Key_Delete:
        case Qt::Key_Backspace:
        {
            QTextCursor cursor=this->textCursor();
            QString st;
            if (cursor.hasSelection()){
                st.append(cursor.selectedText());
            }
            else {
                int pos=cursor.position();
                QTextCursor newcursor(document());
                newcursor.setPosition(pos,QTextCursor::MoveAnchor);
                if( e->key()==Qt::Key_Delete)
                    newcursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor);
                 else
                     newcursor.movePosition(QTextCursor::Left,QTextCursor::KeepAnchor);
                st.append(newcursor.selectedText());
            }
            int count=st.count(QChar(0x2029));
            setFixedHeight(height()-count*(fontMetrics().lineSpacing()+1));
            QPlainTextEdit::keyPressEvent(e);
            if (completer->popup()->isVisible()){
               updateCompleter();
            }

            break;
       }
       default:
       {  QPlainTextEdit::keyPressEvent(e);
          if (completer->popup()->isVisible()){
               updateCompleter();
          }

       }
    }
}

void TextInput::updateCompleter(){
    QString completionPrefix=textUnderCursor();
    if (completionPrefix !=completer->completionPrefix()){
        completer->setCompletionPrefix(completionPrefix);
        completer->popup()->setCurrentIndex(completer->completionModel()->index(0,0));
    }
    QRect cr=cursorRect();
    cr.setWidth(completer->popup()->sizeHintForColumn(0)
                +completer->popup()->verticalScrollBar()->sizeHint().width());
    completer->complete(cr);
}

void TextInput::keyReleaseEvent(QKeyEvent *e){
    if (!( e->modifiers()&Qt::ControlModifier)){
        historylevel=-1;
    }
}

void TextInput::focusInEvent(QFocusEvent *e){
    matchDelimiters();
    installCompleter();
//    line->getWorkSheet()->getHighlighter()->setDocument(document());
    line->getWorkSheet()->setCurrent(line->getId());
    QPlainTextEdit::focusInEvent(e);
}
void TextInput::focusOutEvent(QFocusEvent *e){
 //   line->getWorkSheet()->getHighlighter()->setDocument(0);
    QList<QTextEdit::ExtraSelection> selections;
    setExtraSelections(selections);

    QObject::disconnect(completer,0,this,0);

    QPlainTextEdit::focusOutEvent(e);
}
/**
  This method returns true if the user is able to go down with the "Down" Key"
    in the text zone
  **/
bool TextInput::goDown(){
    QTextCursor newCursor(document());
    newCursor.setPosition(textCursor().position());

    newCursor.movePosition(QTextCursor::EndOfLine);
    int pos=newCursor.position();
    newCursor.movePosition(QTextCursor::End);
    if (newCursor.position()==pos){
        line->getWorkSheet()->goToNextExistingLine();
        return false;
    }
    return true;
}
void TextInput::insertAnswer(const QString &s){
    this->goDown();
    line->getWorkSheet()->getCurrentLine()->getTextInput()->insertIndentedString(s);
}
void TextInput::insertAnswerInNew(const QString &s){
    this->goDown();
    line->getWorkSheet()->insertline();
    line->getWorkSheet()->getCurrentLine()->getTextInput()->insertIndentedString(s);
}
/**
  This method returns true if the user is able to go Up with the "Down" Key"
    in the text zone
  **/
bool TextInput::goUp(){
    QTextCursor newCursor(document());
    newCursor.setPosition(textCursor().position());

    newCursor.movePosition(QTextCursor::StartOfLine);
    if (newCursor.position()==0){
        line->getWorkSheet()->goToPreviousExistingLine();
        return false;
    }
    return true;
}


bool TextInput::event(QEvent* event){
        if (event->type() == QEvent::ToolTip&&hasFocus())
        {
        QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
        QTextCursor cursor=cursorForPosition(helpEvent->pos());
        cursor.select(QTextCursor::WordUnderCursor);
        QString st=cursor.selectedText();
        if (!st.isEmpty()&&
            line->getWorkSheet()->getApp()->getCommandInfo()->isCommand(st)) {
            setProperty("myToolTip",st);
            st.prepend(QString("<u>%1</u> &nbsp;&nbsp;<b><font color=#0000ff>").arg(tr("Mot-clé:")));
            st.append("</font></b><br>");
            st.prepend("<center><img src=\":/images/f1.png\" align=\"middle\" height=\"30\"></center><hr>");

/*            QString s;
            s.setNum(2*QToolTip::font().pointSize());
            st.append(s);
            st.append("\">");
*/
            QToolTip::showText(helpEvent->globalPos(), st);
        }
        else
                QToolTip::hideText();
        return true;
        }
        return QPlainTextEdit::event(event);
}

void TextInput::addMultiLines(int pos,int removed,int charsAdded){
    //User paste a text in the editor
    if (charsAdded-removed>1){
        QTextCursor newcursor(document());
        newcursor.setPosition(pos,QTextCursor::MoveAnchor);
        newcursor.setPosition(pos+charsAdded-removed,QTextCursor::KeepAnchor);
       QString s=newcursor.selectedText();
       int count=s.count(QChar(0x2029));
       setFixedHeight(height()+count*(fontMetrics().lineSpacing()+1));


    }
    //user cut a text:
    if (charsAdded-removed<1){
        int count=this->toPlainText().count("\n");
        setFixedHeight((2+count)*(fontMetrics().lineSpacing()+1));
    }
}

QString TextInput::textUnderCursor() const{
    QTextCursor tc=textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

QString TextInput::selectedText() const{
    QTextCursor tc=textCursor();
    return tc.selectedText();
}

bool TextInput::isStartCursor() const{
    QTextCursor tc=textCursor();

    int pos=tc.positionInBlock();
    tc.select(QTextCursor::LineUnderCursor);
    QString ret=(tc.selectedText());
    ret=ret.left(pos);
    return ret.remove("\t").isEmpty();
}

void TextInput::insertIndentedString(const QString &s){
    QString idts=s;
    QTextCursor tc=textCursor();
    int pos=tc.positionInBlock();
    tc.beginEditBlock();
    tc.select(QTextCursor::LineUnderCursor);
    QString ret=(tc.selectedText());
    ret=ret.left(pos);
    QString test=ret;
    if(test.remove("\t").isEmpty())
        idts.replace("\n","\n"+ret);
    insertPlainText(idts);
    tc.endEditBlock();
}

void TextInput::helpCompletion(const QString &completion){
    if(!completion.isEmpty()){
       line->getWorkSheet()->getApp()->displayHelp(completion);
    }
}

void TextInput::insertCompletion(const QString &completion){
    QTextCursor tc=textCursor();
    tc.movePosition(QTextCursor::EndOfWord);
    tc.movePosition(QTextCursor::StartOfWord,QTextCursor::KeepAnchor);
    tc.removeSelectedText();
    tc.insertText(completion);
    setTextCursor(tc);
}


TextEditOutput::TextEditOutput(Line *parent):TextInput(parent){
    setMinimumHeight(fontMetrics().lineSpacing()+fontMetrics().descent()+2*document()->documentMargin()+6);
    //setFrameStyle(QFrame::NoFrame);
    setToolTip(tr("<li><b>Entrée</b> pour insérer la réponse ou la sélection dans la ligne suivante</li>\
                  <li><b>Maj Entrée</b> pour insérer la réponse ou la sélection dans une nouvelle ligne</li>\
                  <li><b>+</b> ou <b>-</b> pour agrandir ou réduire</li>"));
 }

TextEditOutput::~TextEditOutput(){
}

void TextEditOutput::adjustHeight(){
    int nblignes=document()->size().height();
    if(nblignes>30)
        nblignes=30;
    this->setFixedHeight(fontMetrics().lineSpacing()+fontMetrics().descent()+\
                             2*document()->documentMargin()+6\
                             +fontMetrics().lineSpacing()*(nblignes)\
                             );
}

void TextEditOutput::keyPressEvent(QKeyEvent *e){
    switch(e->key())
    {
        case Qt::Key_C : {
            if(e->modifiers()&Qt::ControlModifier)
                 QPlainTextEdit::keyPressEvent(e);
        break;
        }
        case Qt::Key_A : {
            if(e->modifiers()&Qt::ControlModifier){
                QPlainTextEdit::keyPressEvent(e);
            }
            break;

        }
        case Qt::Key_Left :
            QPlainTextEdit::keyPressEvent(e);break;
        case Qt::Key_Right :
            QPlainTextEdit::keyPressEvent(e);break;
        case Qt::Key_Down: {
            QPlainTextEdit::keyPressEvent(e);
            /*
            if (e->modifiers()&Qt::ShiftModifier){
                //we stay in this line during selection
                QPlainTextEdit::keyPressEvent(e);
            }
            else{
                if (goDown()){
                    QPlainTextEdit::keyPressEvent(e);
                }
            }*/
            break;
        }

        case Qt::Key_Up: {
            /*
             if (e->modifiers()&Qt::ShiftModifier){
                //we stay in this line during selection
                QPlainTextEdit::keyPressEvent(e);
             }
             else{
                if (goUp()){
                     QPlainTextEdit::keyPressEvent(e);
                }
            }*/
            QPlainTextEdit::keyPressEvent(e);
            break;
        }
        case Qt::Key_Return:{
            QString s=this->selectedText();
            if(s=="")
                s=this->toPlainText();

            if(e->modifiers()&Qt::ShiftModifier){
                this->insertAnswerInNew(s);
            }
            else{
                this->insertAnswer(s);
            }
            break;
        }
    case Qt::Key_Plus:{
        int h=this->height();
        if(h<35*fontMetrics().lineSpacing())
            this->setMinimumHeight(h+fontMetrics().lineSpacing());
        break;
    }
    case Qt::Key_Minus:{
        int h=this->height();
        if(h>2*fontMetrics().lineSpacing())
            this->setMinimumHeight(h-fontMetrics().lineSpacing());
        break;
    }

        default:
            e->ignore();
        }
}


