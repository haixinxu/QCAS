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
#include "output.h"
#include <QPainter>
#include <QTreeWidgetItem>
#include <QDebug>
#include "geometry.h"

MyItem::MyItem(Canvas2D *graph){
    g2d=graph;
    visible=true;
    highLighted=false;
    angleLegend=-1;
    fromInter=false;
    level=-1;
    movable=false;
    purgeable=true;
    undef=false;
    traceActive=false;
}
MyItem::~MyItem(){
//    qDebug()<<var<<"MyItem deleted";

}

void MyItem::setHighLighted(const bool &b){
    highLighted=b;
}
giac::gen &MyItem::getValue(){
    return value;

}
QString MyItem::getVar() const{
    return var;
}
void MyItem::setVar(const QString & c){
    var=c;
}

void MyItem::setValue(const giac::gen & s){
    value=s;
}
QString  MyItem::getDisplayValue(){
    return QString("");
}
void MyItem::updateValueFrom(MyItem *){

}
bool MyItem::isUndef() const{
    return undef;
}
void MyItem::setUndef(const bool& b){
    undef=b;
}
bool MyItem::isMovable() const {
    return movable;
}
void MyItem::setMovable(const bool & b){
    movable=b;
}
bool MyItem::isPurgeable() const {
    return purgeable;
}
void MyItem::setPurgeable(const bool & b){
    purgeable=b;
}

bool MyItem::isFromInter() const {
    return fromInter;
}
void MyItem::setFromInter(const bool & b){
    fromInter=b;
}
bool MyItem::isTraceActive() const {
    return traceActive;
}
void MyItem::setTraceActive(const bool & b){
    traceActive=b;
}
void MyItem::toXML(QDomElement & top){
    top.setAttribute("attributes",attributes);
    QDomElement legendNode=top.ownerDocument().createElement("legend");
    QDomText text=top.ownerDocument().createTextNode(getLegend());
    legendNode.appendChild(text);
    top.appendChild(legendNode);
    top.setAttribute("fillable",isFillable());

}
bool MyItem::hasChildren() const{
    return !children.isEmpty();
}
void MyItem::addChild(MyItem * item){
  int id=children.indexOf(item);//check if this item isn't already a child
  if (id==-1 && item != this){
    children.append(item);
  }
}
QVector<MyItem*> MyItem::getChildren(){
    return children;
}

MyItem* MyItem::getChildAt(const int& id){
    return children.at(id);
}
void MyItem::deleteChild(MyItem* item){
    int index=children.indexOf(item);
    while (index!=-1){
      //in case the same item appears at several indexes. (Cf listItem)
      children.remove(index);
      index=children.indexOf(item);
    }
}
bool MyItem::hasParents() const{
    return !parents.isEmpty();
}
void MyItem::addParent(MyItem * item){
    int index=parents.indexOf(item);
    if (index==-1 && item != this) {
      parents.append(item);
    }
}
QVector<MyItem*> MyItem::getParents(){
    return parents;
}
MyItem* MyItem::getParentAt(const int& id){
    return parents.at(id);
}

bool MyItem::isPoint() const{
    return false;
}
bool MyItem::isPointElement() const{
    return false;
}
bool MyItem::isLine() const{
    return false;
}
bool MyItem::isSegment() const{
    return false;
}
bool MyItem::isVector() const{
    return false;
}

bool MyItem::isHalfLine() const{
    return false;
}
bool MyItem::isCurve() const{
    return false;
}
bool MyItem::isMultiCurve() const{
    return false;
}
bool MyItem::isGroupedItem() const{
    return false;
}
bool MyItem::isCursorItem() const{
    return false;
}

bool MyItem::isBezierCurve() const{
    return false;
}

bool MyItem::isInter() const{
    return false;
}
bool MyItem::isList() const{
    return false;
}
bool MyItem::isAngleItem() const{
    return false;
}
bool MyItem::isCircle() const{
    return false;
}
bool MyItem::isPixel() const{
        return false;
}
bool MyItem::isLegendItem()const{
    return false;
}
bool MyItem::isFillable() const{
    return false;
}
void MyItem::setWidth(const int w){
     attributes=(attributes & 0xfff8ffff)+ (w<< 16);
}
QString MyItem::getLegend() const{
    return legend;
}
void MyItem::setLegend(const QString &s){
  /*if(s.startsWith(Config::GeoVarPrefix)){
    legend=s.right(s.length()-(Config::GeoVarPrefix).length());
  }
  else{*/
    legend=s;
  //}
}
void MyItem::setLevel(const int & i){
    level=i;
}
int MyItem::getLevel() const{
    return level;
}

bool MyItem::isVisible() const{
    return visible;
}
void MyItem::setVisible(const bool b){
    visible=b;
}
void MyItem::setLegendVisible(const bool b){
    if (b) {
        attributes=(attributes & 0x7fffffff);
    }
    else attributes=(attributes & 0x7fffffff)+(1<<31);
}

void MyItem::setPointStyle(const int c){
    attributes=(attributes& 0xf1ffffff)+(c<<25);
}

bool MyItem::isUnderMouse(const QRectF& p) const{
    return false;
}
void MyItem::drawTrace(QPainter *){
}

void MyItem::setStyle(const int c) {
    attributes=(attributes& 0xfe3fffff)+(c<<22);

}
void MyItem::setFilled(const bool b){
    if (b) attributes=(attributes& 0xbfffffff)+(1<<30);
    else attributes=(attributes& 0xbfffffff);
}
int MyItem::getQuadrant() const{
    return ((attributes & 0x30000000)>>28);
}
double MyItem::getAngleLegend() const{
    if (angleLegend==-1){
        return getQuadrant()*3.14159/2+3.14159/4;
    }
    else return angleLegend;
}
void MyItem::setLegendPos(const int & a){
    attributes=(attributes & 0xcfffffff)+(a<<28);
//    qDebug()<<"Attributs"<<legendVisible()<<getQuadrant();

}

int MyItem::getStyle(){
    return (attributes & 0x01c00000)>>22;
}
bool MyItem::isFilled()const{
    if (1==((attributes & 0x40000000)>>30)) return true;
    return false;
}

Qt::PenCapStyle MyItem::getPenStyle(){
   int type_line=((attributes & 0x01c00000));
   if (type_line==giac::_CAP_FLAT_LINE) return Qt::FlatCap;
   else if (type_line==giac::_CAP_SQUARE_LINE) return Qt::SquareCap;
   return Qt::RoundCap;

}



Qt::PenStyle MyItem::getLineType(){
    int type_line=((attributes & 0x01c00000));
//    qDebug()<<" type ligne"<<type_line;
    if (type_line==giac::_DASH_LINE) return Qt::DashLine;
    else if (type_line==giac::_DOT_LINE) return Qt::DotLine;
    else if (type_line==giac::_DASHDOT_LINE) return Qt::DashDotLine;
    else if (type_line==giac::_DASHDOTDOT_LINE) return Qt::DashDotDotLine;

    return Qt::SolidLine;

}
int MyItem::getPenWidth() const{
    return ((attributes & 0x00070000) >> 16);
}
bool MyItem::legendVisible()const{
    if ((attributes>>31)==1) return false;
    return true;
}
int MyItem::getAttributes() const{
    return attributes;
}
void MyItem::setAttributes(const int& c){
    attributes=(unsigned int)c;
    if (isFilled()){
        QColor c=getColor();
        c.setAlpha(36*4);
        setColor(c);
    }
}

QColor MyItem::getFltkColor(int& c) const{
    if (c<16){
        switch(c){
           case 0:
            {return Qt::black;}
            break;
            case 1:
            {return Qt::red;}
            break;
            case 2:
            {return Qt::green;}
            break;
            case 3:
            {return Qt::yellow;}
            break;
            case 4:
            {return Qt::blue;}
            break;
            case 5:
            {return Qt::magenta;}
            break;
            case 6:
            {return Qt::cyan;}
            break;
            case 7:
            {return Qt::white;}
            break;
            case 8:
             {return QColor(85,85,85);} // 1/3 gray
             break;
        case 9:
         {return QColor(198,113,113);}// salmon? pale red?
         break;
        case 10:
         {return QColor(113,198,113);}// pale green
         break;
        case 11:
         {return QColor(142,142,56);}// khaki
         break;
        case 12:
         {return QColor(113,113,198);}// pale blue
         break;
        case 13:
         {return QColor(142,56,142);}// purple, orchid, pale magenta
         break;
        case 14:
         {return QColor(56,142,142);}// cadet blue, aquamarine, pale cyan
         break;
        case 15:
         {return QColor(170,170,170);}// cadet blue, aquamarine, pale cyan
         break;
        }
    }
    else if (c<32){
        return QColor(85,85,85);
    }
    else if (c<56){
        // initial algorith to populate the table (from fltk source)
        //
           /*

        #define FL_GRAY_RAMP 32
        #define FL_NUM_GRAY  24
        #define FL_GRAY 49

        if (!r) r = 1; else if (r==255) r = 254;
        double powr = log(254/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0));
        if (!g) g = 1; else if (g==255) g = 254;
        double powg = log(g/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0));
        if (!b) b = 1; else if (b==255) b = 254;
        double powb = log(b/255.0)/log((FL_GRAY-FL_GRAY_RAMP)/(FL_NUM_GRAY-1.0));
        for (int i = 0; i < FL_NUM_GRAY; i++) {
          double gray = i/(FL_NUM_GRAY-1.0);
          cmap[i+FL_GRAY_RAMP][0] = uchar(pow(gray,powr)*255+.5);
          cmap[i+FL_GRAY_RAMP][1] = uchar(pow(gray,powg)*255+.5);
          cmap[i+FL_GRAY_RAMP][2] = uchar(pow(gray,powb)*255+.5);
        }
        */
        if (c==39) return QColor(85,85,85);
        else if(c==47) return QColor(170,170,170);
        else{
            int r = 170;
            double powr = log(r/255.0)/log(17/23);
            int g=170;
            double powg = log(g/255.0)/log(17/23);
            int b=170;
            double powb = log(b/255.0)/log(17/23);
            c-=32;
            return QColor(uchar(pow(c/23.0,powr)*255+.5),uchar(pow(c/23.0,powg)*255+.5),uchar(pow(c/23.0,powb)*255+.5));
        }
    }
    else if (c<256){
       // initial algorith to populate the table (from fltk source)
       /*
        i = 56;
         for (b=0; b<5; b++)
           for (r=0; r<5; r++)
             for (g=0; g<8; g++) {
           cmap[i][0] = r*255/4;
           cmap[i][1] = g*255/7;
           cmap[i][2] = b*255/4;
           i++;
             }*/
        c-=56;
        int b=c/40;
        int r=(c%40)/5;
        int g=(c%40)%8;
//        qDebug()<<"couleurs composante"<<r<<g<<b;
        return QColor(255*r/4,255*g/7,255*b/4);

    }
  return;
}

QColor arcenciel(int k){
    int r,g,b;
    k+=21;
    k%=126;
    if (k<0) k+=126;
    if (k<21){
        r=251;g=0;b=12*k;
    }
    if (k>=21 && k<42){
        r=251-(12*(k-21)); g=0; b=251;
      }
      if (k>=42 && k<63){
        r=0; g=(k-42)*12; b=251;
      }
      if (k>=63 && k<84){
        r=0; g=251; b=251-(k-63)*12;
      }
      if (k>=84 && k<105){
        r=(k-84)*12; g=251; b=0;
      }
      if (k>=105 && k<126){
        r=251; g=251-(k-105)*12; b=0;
      }
      return QColor(r,g,b);
}
void MyItem::setColor(const QColor &c ){
    int r=c.red()/16;
    int g=c.green()/16;
    int b=c.blue()/16;
    int alpha=c.alpha()/36;

    attributes=(0xffff0000&attributes)+(1<<15)+(alpha<<12)+(r<<8)+(g<<4)+b;
}

QColor MyItem::getColor() const{
    int color=(attributes & 0x0000ffff);
    if (0==(color>>15)){
        if (color<256){
            QColor tmp=getFltkColor(color);
          //  setColor(tmp);
            return tmp;
        }
        else if (color<0x17e ){
            QColor tmp=arcenciel(color);
            //setColor(tmp);
            return tmp;

        }
    }
    int r=((color& 0x0f00)>>8)*16;
    int g=((color&0x00f0)>>4)*16;
    int b=(color&0x000f)*16;
    int alpha=((color& 0x7000)>>12)*36; //(color&0x000f)*16;
    return QColor(r,g,b,alpha);
}
Point::Point(Canvas2D* graph):MyItem(graph){

}

Point::Point(const giac::gen & g,Canvas2D* graph):MyItem(graph){
    setValue(g);
}
int Point::getPenWidth() const{
    return ((attributes& 0x00380000) >> 19);
}
double Point::getXScreen() const{
    return xScreen;
}
double Point::getYScreen() const{
    return yScreen;
}

QString Point::getDisplayValue(){
    if (undef){
        QString mml("<math mode=\"display\">\n");
        mml.append("<text>undef</text>");
        mml.append("\n</math>") ;
        return mml;
    }
//    qDebug()<<QString::fromStdString(print(value,g2d->getContext()));
    giac::gen re=giac::_simplify(giac::re(value,g2d->getContext()),g2d->getContext());
    giac::gen im=giac::_simplify(giac::im(value,g2d->getContext()),g2d->getContext());
    giac::gen tmp(giac::makevecteur(re,im));
    tmp.subtype=giac::_SEQ__VECT;
    QString mml("<math mode=\"display\">\n");
    mml.append(QString::fromStdString(giac::gen2mathml(tmp,g2d->getContext())));
    mml.append("\n</math>");
    return mml;
}

void Point::toXML(QDomElement &top){
    QDomElement point=top.ownerDocument().createElement("point");
    QDomElement affix=top.ownerDocument().createElement("value");
    QDomText text=top.ownerDocument().createTextNode(QString::fromStdString(giac::print(value,g2d->getContext())));
    affix.appendChild(text);
    point.appendChild(affix);

    MyItem::toXML(point);
    top.appendChild(point);
}
void Point::setValue(const giac::gen & g){
   value=g;
   x=giac::evalf_double(giac::re(value,g2d->getContext()),1,g2d->getContext()).DOUBLE_val();
   y=giac::evalf_double(giac::im(value,g2d->getContext()),1,g2d->getContext()).DOUBLE_val();

}
bool Point::isUnderMouse(const QRectF& p) const{
    return recSel.intersects(p);
}
QString Point::getType() const{
    return QObject::tr("Point");
}
void Point::setTraceActive(const bool & b){
    MyItem::setTraceActive(b);
    if (!b) tracePoints.clear();
}
void Point::drawTrace(QPainter * painter){
    for (int i=0;i<tracePoints.size();++i){
        painter->setPen(Qt::blue);
        painter->setBrush(QBrush(Qt::blue,Qt::SolidPattern));
        double a,b;
        g2d->toScreenCoord(tracePoints.at(i).x(),tracePoints.at(i).y(),a,b);
        painter->drawEllipse(QPointF(a,b),2,2);
    }
}
void Point::updateValueFrom(MyItem * item){

    if (item->isUndef()){
        value=giac::undef;
        undef=true;
        return;
    }
    undef=false;
    if (!item->isPoint()) return;
    setValue(item->getValue());
     updateScreenCoords(true);
}

bool Point::isPoint()  const{
    return true;
}

void Point::updateScreenCoords(const bool compute){
    if (compute) {
        g2d->toScreenCoord(x,y,xScreen,yScreen);

    }
    int width=getPenWidth()+3;
    recSel=QRectF(xScreen,yScreen,width,width);
    recSel.adjust(-width/2,-width/2,-width/2,-width/2);
    if (isTraceActive()){
         QPointF p(x,y);
        tracePoints.append(p);
    }

}

void Point::draw(QPainter * painter) const{
    if (!isVisible()) return;
    int width=getPenWidth()+3;
    QColor color=getColor();

    if (highLighted) {
        width+=4;
        color.setAlpha(100);
    }
//    qDebug()<<attributes <<legend;
    painter->setPen(QPen(color,width/2.0,Qt::SolidLine,Qt::RoundCap));


    switch(getPointStyle()){
    case giac::_POINT_LOSANGE:
    {
        painter->setBrush(QBrush());
        QPainterPath p;
        p.moveTo(xScreen,yScreen-width);
        p.lineTo(xScreen+width,yScreen);
        p.lineTo(xScreen,yScreen+width);
        p.lineTo(xScreen-width,yScreen);
        p.closeSubpath();
        painter->drawPath(p);}
        break;
    case giac::_POINT_CARRE:
    {
        painter->setBrush(QBrush());
        QPainterPath p;
        width*=0.707;
        p.moveTo(xScreen-width,yScreen-width);
        p.lineTo(xScreen-width,yScreen+width);
        p.lineTo(xScreen+width,yScreen+width);
        p.lineTo(xScreen+width,yScreen-width);
        p.closeSubpath();
        painter->drawPath(p);


    }
        break;
    case giac::_POINT_ETOILE:
 {
        painter->setBrush(QBrush());
        painter->drawLine(QPointF(xScreen,yScreen-width),QPointF(xScreen,yScreen+width));
        painter->drawLine(QPointF(xScreen+0.866*width,yScreen-width/2),QPointF(xScreen-0.866*width,yScreen+width/2));
        painter->drawLine(QPointF(xScreen-0.866*width,yScreen-width/2),QPointF(xScreen+0.866*width,yScreen+width/2));

    }


        break;
    case giac::_POINT_PLUS:
    {
        painter->setBrush(QBrush());
        painter->drawLine(QPointF(xScreen+width,yScreen),QPointF(xScreen-width,yScreen));
        painter->drawLine(QPointF(xScreen,yScreen+width),QPointF(xScreen,yScreen-width));
    }
        break;
    case giac::_POINT_TRIANGLE:
    {
        painter->setBrush(QBrush());
        QPainterPath p;
        p.moveTo(xScreen,yScreen-width);
        p.lineTo(xScreen+width*0.866,yScreen+width/2);
        p.lineTo(xScreen-width*0.866,yScreen+width/2);
        p.closeSubpath();
        painter->drawPath(p);
    }
        break;
    case giac::_POINT_POINT:
    {
        painter->setBrush(QBrush(color,Qt::SolidPattern));
        painter->drawEllipse(QPointF(xScreen,yScreen),width-2,width-2);
    }

        break;
    case giac::_POINT_INVISIBLE:
        break;
        // CROSS_POINT
    default:
       {
        painter->setBrush(QBrush());
        width*=0.707;
        painter->drawLine(QPointF(xScreen+width,yScreen+width),QPointF(xScreen-width,yScreen-width));
        painter->drawLine(QPointF(xScreen-width,yScreen+width),QPointF(xScreen+width,yScreen-width));

    }
        break;

    }

    if (legendVisible()){
        if (legend.trimmed().isEmpty()) return;
        int h=painter->fontMetrics().ascent();
        int w=painter->fontMetrics().width(legend);
        double angle=getAngleLegend();

        QPointF p(xScreen+10*std::cos(angle),yScreen-10*sin(angle));
        if ((0<angle)&&(angle<3.14159/2)){
            painter->drawText(p.x(),p.y(),legend);
        }
        else if ((3.14159/2<=angle)&&(angle<3.14159)){
            painter->drawText(p.x()-w,p.y(),legend);

        }
        else if ((3.14159<=angle)&&(angle<3*3.14159/2)){
            painter->drawText(p.x()-w,p.y()+h,legend);

        }
        else{
            painter->drawText(p.x(),p.y()+h,legend);

        }

    }
}


int Point::getPointStyle() const{
    return (attributes & 0x0e000000);

}
void Point::setWidth(const int w){
     attributes=(attributes & 0xffc7ffff)+ (w<< 19);
}

LineItem::LineItem(const QPointF &p1, const QPointF &p2,Canvas2D *graph):MyItem(graph){
    startPoint=p1;
    endPoint=p2;
}
void LineItem::updateValueFrom(MyItem * item){
    if (item->isUndef()) {
        value=giac::undef;
        undef=true;
        return;
    }
    undef=false;
    if (!item->isLine()) return;
    LineItem*  p=dynamic_cast<LineItem*>(item);
    startPoint=p->getStartPoint();
    endPoint=p->getEndPoint();
    setValue(item->getValue());
    updateScreenCoords(true);
}

void LineItem::toXML(QDomElement & top){
    QDomElement line=top.ownerDocument().createElement("line");
    QDomElement equation=top.ownerDocument().createElement("value");
    QDomText text=top.ownerDocument().createTextNode(QString::fromStdString(giac::print(value,g2d->getContext())));
    equation.appendChild(text);
    line.appendChild(equation);
    MyItem::toXML(line);

    QDomElement start=top.ownerDocument().createElement("startPoint");
    start.setAttribute("x",startPoint.x());
    start.setAttribute("y",startPoint.y());
    QDomElement end=top.ownerDocument().createElement("endPoint");
    end.setAttribute("x",endPoint.x());
    end.setAttribute("y",endPoint.y());

    line.appendChild(start);
    line.appendChild(end);

    top.appendChild(line);


}


QPointF LineItem::getStartPoint() const{
    return startPoint;
}
QPointF LineItem::getEndPoint() const{
    return endPoint;
}

void LineItem::setValue(const giac::gen &g){
    value=g;
}
QString LineItem::getDisplayValue(){
    QString mml("<math mode=\"display\">\n");
    mml.append(QString::fromStdString(gen2mathml(value,g2d->getContext())));
    mml.append("\n</math>");
    return mml;
}

void LineItem::updateScreenCoords(const bool compute){
    if (compute) {
    QPointF startScreen, endScreen;
    if (startPoint.x()==endPoint.x()){
        double a,b;
        g2d->toScreenCoord(startPoint.x(),g2d->getYmin(),a,b);
        startScreen=QPointF(a,b);
        g2d->toScreenCoord(startPoint.x(),g2d->getYmax(),a,b);
        endScreen=QPointF(a,b);
    }
    else {
        double tmpx,tmpy;
        double a=(endPoint.y()-startPoint.y())/(endPoint.x()-startPoint.x());
        double b=endPoint.y()-a*endPoint.x();

        double y=a*g2d->getXmax()+b;

        if (y<g2d->getYmin()){
            g2d->toScreenCoord((g2d->getYmin()-b)/a,g2d->getYmin(),tmpx,tmpy);
            endScreen=QPointF(tmpx,tmpy);
        }
        else if (y>g2d->getYmax()){
            g2d->toScreenCoord((g2d->getYmax()-b)/a,g2d->getYmax(),tmpx,tmpy);
            endScreen=QPointF(tmpx,tmpy);

        }
        else {
            g2d->toScreenCoord(g2d->getXmax(),y,tmpx,tmpy);
            endScreen=QPointF(tmpx,tmpy);
        }

        y=a*g2d->getXmin()+b;

        if (y<g2d->getYmin()){
            g2d->toScreenCoord((g2d->getYmin()-b)/a,g2d->getYmin(),tmpx,tmpy);
            startScreen=QPointF(tmpx,tmpy);
        }
        else if (y>g2d->getYmax()){
            g2d->toScreenCoord((g2d->getYmax()-b)/a,g2d->getYmax(),tmpx,tmpy);
            startScreen=QPointF(tmpx,tmpy);

        }
        else {
            g2d->toScreenCoord(g2d->getXmin(),y,tmpx,tmpy);
            startScreen=QPointF(tmpx,tmpy);
        }
    }
    p=QPainterPath();
    p.moveTo(startScreen);
    p.lineTo(endScreen);

    }
    QPainterPathStroker stroke;
    stroke.setCapStyle(Qt::FlatCap);
    stroke.setDashPattern(getLineType());
    stroke.setWidth(getPenWidth()+1);
    envelop=stroke.createStroke(p);
}
bool LineItem::isLine() const{
    return true;
}

QString LineItem::getType() const{
    return QObject::tr("Droite");
}
void LineItem::draw(QPainter* painter) const{
    if (!isVisible()) return;

    QColor color=getColor();
    int width=1;
    if (highLighted){
        color.setAlpha(100);
        width=3;
    }
    painter->setPen(QPen(color,width,  Qt::SolidLine));
    painter->setBrush(QBrush(color,Qt::SolidPattern));
    painter->drawPath(envelop);
}

bool LineItem::isUnderMouse(const QRectF &r) const{
    return envelop.intersects(r);
}

HalfLineItem::HalfLineItem(const QPointF &p1, const QPointF &p2,Canvas2D *graph):MyItem(graph){
    startPoint=p1;
    endPoint=p2;
}
void HalfLineItem::updateValueFrom(MyItem * item){
    if (item->isUndef()) {
        value=giac::undef;
        undef=true;
        return;
    }
        undef=false;
    if (!item->isHalfLine()) return;
    HalfLineItem*  p=dynamic_cast<HalfLineItem*>(item);
    startPoint=p->getStartPoint();
    endPoint=p->getEndPoint();
    setValue(item->getValue());
    updateScreenCoords(true);
}
QPointF HalfLineItem::getStartPoint() const{
    return startPoint;
}
QPointF HalfLineItem::getEndPoint() const{
    return endPoint;
}

void HalfLineItem::toXML(QDomElement & top){
    QDomElement halfline=top.ownerDocument().createElement("halfline");
    QDomElement equation=top.ownerDocument().createElement("value");
    QDomText text=top.ownerDocument().createTextNode(QString::fromStdString(giac::print(value,g2d->getContext())));
    equation.appendChild(text);
    halfline.appendChild(equation);

    QDomElement start=top.ownerDocument().createElement("startPoint");
    start.setAttribute("x",startPoint.x());
    start.setAttribute("y",startPoint.y());
    QDomElement end=top.ownerDocument().createElement("endPoint");
    end.setAttribute("x",endPoint.x());
    end.setAttribute("y",endPoint.y());

    halfline.appendChild(start);
    halfline.appendChild(end);
    MyItem::toXML(halfline);

    top.appendChild(halfline);

}

void HalfLineItem::setValue(const giac::gen &g){
    value=g;
}

QString HalfLineItem::getDisplayValue(){
    QString mml("<math mode=\"display\">\n<mfenced open=\"{\" close=\"\">\n<mtable>\n<mtr><mtd>");
    mml.append(QString::fromStdString(gen2mathml(value,g2d->getContext())));
    mml.append("</mtd></mtr>\n<mtr><mtd>\n <mrow>");
    if (startPoint.x()!=endPoint.x()){
        if (startPoint.x()<endPoint.x()){
            mml.append("<mi>x</mi><mo>&gt;</mo>");
        }
        else if(startPoint.x()>endPoint.x()){
            mml.append("<mi>x</mi><mo>&lt;</mo>");
        }
        mml.append(QString::number(startPoint.x()));
        mml.append("</mrow>");
    }
    else {
        if (startPoint.y()<endPoint.y()){
            mml.append("<mi>y</mi><mo>&gt;</mo>");
        }
        else {
            mml.append("<mi>y</mi><mo>&lt;</mo>");
        }
        mml.append(QString::number(startPoint.y()));
        mml.append("</mrow>");
   }
   mml.append("</mtd></mtr></mtable></mfenced>\n</math>");
    return mml;
}
void HalfLineItem::updateScreenCoords(const bool compute){
    if (compute){
    QPointF startScreen,endScreen;
    if (startPoint.x()==endPoint.x()){
        double a,b;
        g2d->toScreenCoord(startPoint.x(),startPoint.y(),a,b);
        startScreen=QPointF(a,b);
        if (endPoint.y()>startPoint.y()){
            g2d->toScreenCoord(startPoint.x(),g2d->getYmax(),a,b);
        }
        else
            g2d->toScreenCoord(startPoint.x(),g2d->getYmin(),a,b);

        endScreen=QPointF(a,b);
    }
    else {
        double tmpx,tmpy,y;
        g2d->toScreenCoord(startPoint.x(),startPoint.y(),tmpx,tmpy);
        startScreen=QPointF(tmpx,tmpy);

        double a=(endPoint.y()-startPoint.y())/(endPoint.x()-startPoint.x());
        double b=endPoint.y()-a*endPoint.x();


        if (endPoint.x()>startPoint.x()){
            y=a*g2d->getXmax()+b;

            if (y<g2d->getYmin()){
                g2d->toScreenCoord((g2d->getYmin()-b)/a,g2d->getYmin(),tmpx,tmpy);
                endScreen=QPointF(tmpx,tmpy);
            }
            else if (y>g2d->getYmax()){
                g2d->toScreenCoord((g2d->getYmax()-b)/a,g2d->getYmax(),tmpx,tmpy);
                endScreen=QPointF(tmpx,tmpy);

            }
            else {
                g2d->toScreenCoord(g2d->getXmax(),y,tmpx,tmpy);
                endScreen=QPointF(tmpx,tmpy);
            }

        }
        else{

            y=a*g2d->getXmin()+b;

            if (y<g2d->getYmin()){
                g2d->toScreenCoord((g2d->getYmin()-b)/a,g2d->getYmin(),tmpx,tmpy);
                endScreen=QPointF(tmpx,tmpy);
            }
            else if (y>g2d->getYmax()){
                g2d->toScreenCoord((g2d->getYmax()-b)/a,g2d->getYmax(),tmpx,tmpy);
                endScreen=QPointF(tmpx,tmpy);

            }
            else {
                g2d->toScreenCoord(g2d->getXmin(),y,tmpx,tmpy);
                endScreen=QPointF(tmpx,tmpy);
            }

        }
    }
    p=QPainterPath();
    p.moveTo(startScreen);
    p.lineTo(endScreen);

    }
    QPainterPathStroker stroke;
    stroke.setWidth(getPenWidth()+1);
    stroke.setCapStyle(Qt::FlatCap);
    stroke.setDashPattern(getLineType());
    envelop=stroke.createStroke(p);
}
bool HalfLineItem::isHalfLine() const{
    return true;
}

bool HalfLineItem::isUnderMouse(const QRectF &r) const{
    return envelop.intersects(r);
}

void HalfLineItem::draw(QPainter* painter) const{
    if (!isVisible()) return;

    QColor color=getColor();
    int width=1;
    if (highLighted){
        color.setAlpha(100);
        width=3;
    }

    painter->setPen(QPen(color,width,  Qt::SolidLine));
    painter->setBrush(QBrush(color,Qt::SolidPattern));

    painter->drawPath(envelop);
}
QString HalfLineItem::getType() const{
    return QObject::tr("Demie-droite");
}



Curve::Curve(const QPainterPath &p,Canvas2D *graph):MyItem(graph){
    vector=false;
    polygon=false;
    path=p;
    fillable=false;
}
void Curve::updateValueFrom(MyItem * item){
    if (item->isUndef()) {
        value=giac::undef;
        undef=true;
        return;
    }
    undef=false;
    if (!item->isCurve()) return;
    Curve* curve=dynamic_cast<Curve*>(item);
    if (curve->isPolygon()) polygon=true;
    else if (curve->isVector()) vector=true;
    if (curve->isFillable()) fillable=true;
    value=item->getValue();
    path=curve->getPath();
    updateScreenCoords(true);

}
QPainterPath Curve::getPath() const{
    return path;
}

QString Curve::getDisplayValue(){
//    qDebug()<<QString::fromStdString(giac::print(value,g2d->getContext()));
    QString mml("<math mode=\"display\">\n");
    mml.append(QString::fromStdString(gen2mathml(value,g2d->getContext())));
    mml.append("</math>");
    return mml;

}
void Curve::draw(QPainter *painter) const{

    if (!isVisible()) return;
    int width=1;
    QColor color=getColor();
    if (highLighted){
        color.setAlpha(100);
        width=3;
    }

    if ((isFilled()&&isFillable()&&(!highLighted))){
        QColor f(color);
        f.setAlpha(255);
        painter->setBrush(QBrush(color,Qt::SolidPattern));
        painter->setPen(QPen(f,getPenWidth(),  Qt::SolidLine));
        painter->drawPath(pathScreen);
    }
    else {
        if (vector){
            painter->setBrush(QBrush(color,Qt::SolidPattern));
            painter->setPen(QPen(color,getPenWidth(),  Qt::SolidLine,Qt::FlatCap,Qt::MiterJoin));
            painter->drawPath(pathArrow);
        }
        painter->setPen(QPen(color,width,  Qt::SolidLine));
        painter->setBrush(QBrush(color,Qt::SolidPattern));
        painter->drawPath(envelop);

    }
}
bool Curve::isCurve() const{
    return true;
}
bool Curve::isUnderMouse(const QRectF &p) const{
    return envelop.intersects(p);
}

void Curve::setVector(const bool & b){
    vector=b;
}

void Curve::updateScreenCoords(const bool compute){
    if (compute){
        double xtmp,ytmp;
        pathScreen=QPainterPath();
        QPointF save;
        bool saveDef=false;
        bool moveTo=true;
        for (int i=0;i<path.elementCount();++i){
            g2d->toScreenCoord(path.elementAt(i).x,path.elementAt(i).y,xtmp,ytmp);
            if ((xtmp>0)&&(xtmp<g2d->width())&&(ytmp>0)&&(ytmp<g2d->height()))
            {
                if (moveTo) {
                    if (saveDef){
                    pathScreen.moveTo(save);
                    pathScreen.lineTo(xtmp,ytmp);
                    }
                    else pathScreen.moveTo(xtmp,ytmp);
                    moveTo=false;
                }
                else pathScreen.lineTo(xtmp,ytmp);
                saveDef=false;
            }
            else  {
                save=QPointF(xtmp,ytmp);
                moveTo=true;
                saveDef=true;
            }
        }
        if (vector){
            pathArrow=QPainterPath();

            double x1=pathScreen.elementAt(0).x;
            double y1=pathScreen.elementAt(0).y;

            double x2=pathScreen.elementAt(1).x;
            double y2=pathScreen.elementAt(1).y;

            double l=std::sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
            int w=6;
            int h=12;
//        QPointF control(x2+(x1-x2)*h/l,y2+(y1-y2)*h/l);
            QPointF second(x2+(x1-x2)*h/l+(y2-y1)*w/l,y2+(y1-y2)*h/l+(x1-x2)*w/l);
            QPointF third(x2+(x1-x2)*h/l-(y2-y1)*w/l,y2+(y1-y2)*h/l-(x1-x2)*w/l);

            pathArrow.moveTo(x2,y2);
            pathArrow.lineTo(second);
            pathArrow.lineTo(third);
            pathArrow.lineTo(x2,y2);
        }
    }
    int width=getPenWidth()+1;
    stroke.setCapStyle(Qt::FlatCap);
    stroke.setDashPattern(getLineType());
    stroke.setWidth(width);
    envelop=stroke.createStroke(pathScreen);
}

bool Curve::isVector() const{
    return vector;
}

bool Curve::isSegment() const{
    if (vector){
        return false;
    }
    else return (path.elementCount()==2);
}

void Curve::toXML(QDomElement & top){
    QDomElement curve=top.ownerDocument().createElement("curve");
    curve.setAttribute("isVector",vector);
    MyItem::toXML(curve);
    QDomElement valueNode=top.ownerDocument().createElement("value");
    QDomText text=top.ownerDocument().createTextNode(QString::fromStdString(giac::print(value,g2d->getContext())));
    valueNode.appendChild(text);
    curve.appendChild(valueNode);


    QDomElement lineTo;

    for (int i=0;i<path.elementCount();++i){

        if (path.elementAt(i).isMoveTo()) {
            lineTo= top.ownerDocument().createElement("moveto");
        }
        else lineTo= top.ownerDocument().createElement("lineto");

        QString s(QString::number(path.elementAt(i).x));
        lineTo.setAttribute("x",s);
        s=QString::number(path.elementAt(i).y);
        lineTo.setAttribute("y",s);

        curve.appendChild(lineTo);
    }
    top.appendChild(curve);

}
QString Curve::getType() const{
    if (vector) return QObject::tr("Vecteur");
    else if (isSegment()) return QObject::tr("Segment");
    else if (polygon) return QObject::tr("Polygône");
    return QObject::tr("Courbe");
}
bool Curve::isPolygon() const{
    return polygon;
}
bool Curve::isFillable() const{
    return (polygon&&fillable);
}
void Curve::setFillable(const bool & b){
    fillable=b;
}

void Curve::setPolygon(const bool & b){
    polygon=b;
}
MultiCurve::MultiCurve(const QList<MyItem *> & l, Canvas2D * c):ListItem(l,c){
}
bool MultiCurve::isList() const{
    return false;
}
bool MultiCurve::isMultiCurve() const{
    return true;
}
QString MultiCurve::getDisplayValue(){
        QString mml("<math mode=\"display\">\n");
        mml.append(QString::fromStdString(gen2mathml(value,g2d->getContext())));
        mml.append("</math>");
        return mml;
}

BezierCurve::BezierCurve(const QList<QPointF> &p,Canvas2D *graph):MyItem(graph){
    points=p;
    int rem=(points.size()-1)%3;
    for (int i=0;i<rem;++i){
        points.append(points.last());
    }
    fillable=false;

}
QList<QPointF> BezierCurve::getControlPoints() const{
    return points;
}
void BezierCurve::updateValueFrom(MyItem * item){
/*    if (item->isUndef()) {
        value=giac::undef;
        undef=true;
        return;
    }*/
    undef=false;

    if (!item->isBezierCurve()) return;
    BezierCurve* curve=dynamic_cast<BezierCurve*>(item);
    if (curve->isFillable()) fillable=true;
    points=curve->getControlPoints();
    updateScreenCoords(true);
}
/*QPainterPath Curve::getPath() const{
    return path;
}*/

QString BezierCurve::getDisplayValue(){
        QString mml("<math mode=\"display\">\n");
        mml.append("<text>");
        mml.append(QObject::tr("Courbe de bézier"));
        mml.append("</text>");
        mml.append("</math>");
        return mml;
}
void BezierCurve::draw(QPainter *painter) const{
    if (!isVisible()) return;

    int width=1;
    QColor color=getColor();
    if (highLighted){
        color.setAlpha(100);
        width=3;
    }

    if ((isFilled()&&isFillable()&&(!highLighted))){
        QColor f(color);
        f.setAlpha(255);
        painter->setBrush(QBrush(color,Qt::SolidPattern));
        painter->setPen(QPen(f,getPenWidth(),  Qt::SolidLine));
//        painter->drawPath(pointsScreen);
    }
    else {

        painter->setPen(QPen(color,width,  Qt::SolidLine));
        painter->setBrush(QBrush(color,Qt::SolidPattern));
        painter->drawPath(envelop);

    }

}
bool BezierCurve::isBezierCurve() const{
    return true;
}

bool BezierCurve::isUnderMouse(const QRectF &p) const{
    return false;
}


void BezierCurve::updateScreenCoords(const bool compute){
    if (compute){
        double xtmp,ytmp;
        pointsScreen.clear();
        for (int i=0;i<points.size();++i){
            g2d->toScreenCoord(points.at(i).x(),points.at(i).y(),xtmp,ytmp);
            pointsScreen.append(QPointF(xtmp,ytmp));
        }
        bezierCurve=QPainterPath();
        QList<QPointF> v;
        if (pointsScreen.size()==0) return;
        bezierCurve.moveTo(pointsScreen.at(0));
        for(int i=1;i<pointsScreen.size();++i){
            v.append(pointsScreen.at(i));
            if (v.size()==3){
                bezierCurve.cubicTo(v.at(0),v.at(1),v.at(2));
                v.clear();
            }
        }
    }
//    qDebug()<<points;
    int width=getPenWidth()+1;
    stroke.setCapStyle(Qt::FlatCap);
    stroke.setDashPattern(getLineType());
    stroke.setWidth(width);
    envelop=stroke.createStroke(bezierCurve);

}

QString BezierCurve::getType() const{
    return QObject::tr("Courbe de Bézier");
}
void BezierCurve::toXML(QDomElement & top){
    QDomElement bezier=top.ownerDocument().createElement("bezier");
    MyItem::toXML(bezier);
    QDomElement control= top.ownerDocument().createElement("control");

    for (int i=0;i<points.size();++i){
        control.setAttribute("x",QString::number(points.at(i).x()));
        control.setAttribute("y",QString::number(points.at(i).y()));
        bezier.appendChild(control);
        control= top.ownerDocument().createElement("control");
    }
    top.appendChild(bezier);
}

bool BezierCurve::isFillable() const{
    return (fillable);
}
void BezierCurve::setFillable(const bool & b){
    fillable=b;
}


Circle::Circle(const QPointF &p, const double &d, const double &st, const double &end, Canvas2D *graph):MyItem(graph){
    center=p;
    diametre=d;
    startAngle=st*180/3.14159;
    endAngle=end*180/3.14159;
    isArc=true;
    if (endAngle-startAngle>6.28) isArc=false;
}
QString Circle::getDisplayValue(){
    QString mml("<math mode=\"display\">\n");
    mml.append(QString::fromStdString(giac::gen2mathml(value,g2d->getContext())));
    mml.append("\n</math>");
    return mml;
}
bool Circle::isCircle() const{
    return true;
}
QPointF Circle::getCenter() const{
    return center;
}
double Circle::getDiametre() const{
    return diametre;
}
double Circle::getStartAngle() const{
    return startAngle;
}
double Circle::getEndAngle() const{
    return endAngle;
}
void Circle::toXML(QDomElement & top){
    QDomElement circle=top.ownerDocument().createElement("circle");

    circle.setAttribute("diametre",diametre);
    circle.setAttribute("startAngle",startAngle/180*3.14159);
    circle.setAttribute("endAngle",endAngle/180*3.14159);

    QDomElement equation=top.ownerDocument().createElement("value");
    QDomText text=top.ownerDocument().createTextNode(QString::fromStdString(giac::print(value,g2d->getContext())));
    equation.appendChild(text);
    circle.appendChild(equation);

    QDomElement centerNode=top.ownerDocument().createElement("center");
    centerNode.setAttribute("x",center.x());
    centerNode.setAttribute("y",center.y());
    circle.appendChild(centerNode);
    MyItem::toXML(circle);

    top.appendChild(circle);

}
void Circle::updateValueFrom(MyItem* item){
    if (item->isUndef()) {
        value=giac::undef;
        undef=true;
        return;
    }
    undef=false;
    if (!item->isCircle()) return;
    Circle* circle=dynamic_cast<Circle*>(item);
    value=item->getValue();
    center=circle->getCenter();
    diametre=circle->getDiametre();
    startAngle=circle->getStartAngle();
    endAngle=circle->getEndAngle();
    updateScreenCoords(true);
}
void Circle::updateScreenCoords(const bool compute){

    if (compute){
        double a,b;
        double r=diametre/2;
        g2d->toScreenCoord(center.x()-r,center.y()+r,a,b);
        QPointF leftUp(a,b);
        g2d->toScreenCoord(center.x()+r,center.y()-r,a,b);
        QPointF rightBottom(a,b);
        g2d->toScreenCoord(center.x()+r*std::cos(startAngle/180*3.14159),center.y()+r*std::sin(startAngle/180*3.14159),a,b);
        QPointF start(a,b);

        p=QPainterPath();
        p.moveTo(start);
        p.arcTo(QRectF(leftUp,rightBottom),startAngle,endAngle-startAngle);
        if (isFilled()){
            g2d->toScreenCoord(center.x(),center.y(),a,b);
            p.lineTo(a,b);
            p.closeSubpath();
        }
    }
    QPainterPathStroker stroke;
    stroke.setWidth(getPenWidth()+1);
    stroke.setCapStyle(Qt::FlatCap);
    stroke.setDashPattern(getLineType());
    envelop=stroke.createStroke(p);
}


void Circle::draw(QPainter* painter) const{
//    qDebug()<<undef;
    if (!isVisible()) return;
    QColor color=getColor();
    int width=1;
    if (highLighted){
        color.setAlpha(100);
        width=3;
    }
    if (isFilled()&&(!highLighted)){
        QColor f(color);
        f.setAlpha(255);
        painter->setBrush(QBrush(color,Qt::SolidPattern));
        painter->setPen(QPen(f,width,  Qt::SolidLine));
        painter->drawPath(p);
    }
    else{
        painter->setPen(QPen(color,width,  Qt::SolidLine));
        painter->setBrush(QBrush(color,Qt::SolidPattern));
        painter->drawPath(envelop);
    }
}
bool Circle::isFillable() const{
    return true;
}
QString Circle::getType() const{
    return QObject::tr("Cercle");
}

bool Circle::isUnderMouse(const QRectF &p) const{
    return envelop.intersects(p);
}
Pixel::Pixel(const QPointF &p, Canvas2D * parent):MyItem(parent){
    pixelScreen=p;
    double x,y;
    g2d->toXY(p.x(),p.y(),x,y);
    pixel=QPointF(x,y);
}
bool Pixel::isUnderMouse(const QRectF &p) const{
    return false;
}
bool Pixel::isFillable()const{
    return true;
}
QString Pixel::getType() const{
    return QObject::tr("Pixel");
}
bool Pixel::isPixel() const{
    return true;
}

void Pixel::toXML(QDomElement & top){
    QDomElement pixelNode=top.ownerDocument().createElement("pixel");
    pixelNode.setAttribute("x",pixel.x());
    pixelNode.setAttribute("y",pixel.y());
    top.appendChild(pixelNode);
}
void Pixel::updateScreenCoords(const bool compute){
    if (compute){
        double x,y;
        g2d->toScreenCoord(pixel.x(),pixel.y(),x,y);
        pixelScreen=QPointF(x,y);

    }
}

void Pixel::draw(QPainter * p) const{
    if (!isVisible()) return;
    p->setPen(getColor());
    p->drawPoint((int)pixelScreen.x(),(int)pixelScreen.y());

}

LegendItem::LegendItem(const QPoint & p,const QString & l, Canvas2D *parent):MyItem(parent){
    legend=l;
    pos=p;
}

bool LegendItem::isUnderMouse(const QRectF& p) const{
    return false;
}
void LegendItem::updateScreenCoords(const bool){}

void LegendItem::draw(QPainter* painter) const{
    if (!isVisible()) return;

    QColor color=getColor();

    painter->setPen(QPen(color,2.0,Qt::SolidLine,Qt::RoundCap));



    if (legend.trimmed().isEmpty()) return;
    int h=painter->fontMetrics().ascent();
    int w=painter->fontMetrics().width(legend);
    double angle=getAngleLegend();

    QPointF p(pos.x()+10*std::cos(angle),pos.y()-10*sin(angle));
    if ((0<angle)&&(angle<3.14159/2)){
        painter->drawText(p.x(),p.y(),legend);
    }
    else if ((3.14159/2<=angle)&&(angle<3.14159)){
        painter->drawText(p.x()-w,p.y(),legend);

    }
    else if ((3.14159<=angle)&&(angle<3*3.14159/2)){
        painter->drawText(p.x()-w,p.y()+h,legend);

    }
    else{
        painter->drawText(p.x(),p.y()+h,legend);

    }

}
void LegendItem::toXML(QDomElement & top){
    QDomElement legendNode=top.ownerDocument().createElement("legend");
    legendNode.setAttribute("x",pos.x());
    legendNode.setAttribute("y",pos.y());
    top.appendChild(legendNode);
}

bool LegendItem::isLegendItem() const{
    return true;
}
QString LegendItem::getType() const{
    return QObject::tr("Légende");
}
ListItem::ListItem(const QList<MyItem *> &v , Canvas2D* p):MyItem(p){
    list=v;
}
ListItem::~ListItem(){
    qDeleteAll(list);
}
bool ListItem::isUnderMouse(const QRectF& p) const{
    bool b=false;
    for (int i=0;i<list.size();++i){
        b=list.at(i)->isUnderMouse(p);
        if (b) break;
    }
    return b;

}
void ListItem::updateScreenCoords(const bool b){

    for (int i=0;i<list.size();++i){
        list.at(i)->updateScreenCoords(b);
    }
}
bool ListItem::isFillable() const{
    for (int i=0;i<list.size();++i){
        if (list.at(i)->isFillable()) return true;
    }
    return false;
}

void ListItem::toXML(QDomElement & top){
    for (int i=0;i<list.size();++i){
        list.at(i)->toXML(top);
    }

}
void ListItem::draw(QPainter* painter) const{
    for (int i=0;i<list.size();++i){
        list.at(i)->setHighLighted(highLighted);
        list.at(i)->setAttributes(attributes);
        list.at(i)->draw(painter);
    }
}
void ListItem::setAttributes(const int & a){
    for (int i=0;i<list.size();++i){
        list.at(i)->setAttributes(a);
    }
    MyItem::setAttributes(a);
}
bool ListItem::isList() const{
    return true;
}
QString ListItem::getType() const{
    return QObject::tr("Liste");
}
void ListItem::updateValueFrom(const QList<MyItem*> & v){
  
    if (v.size()==0){
        value=giac::undef;
        undef=true;
        return;
    }
    undef=false;
    list=v;
    updateScreenCoords(true);
}
GroupedItem::GroupedItem(const QList<MyItem *> & l, Canvas2D * c):ListItem(l,c){
}
bool GroupedItem::isGroupedItem() const{
    return true;
}
bool GroupedItem::isList() const{
    return true;
}
/*
void GroupedItem::toXML(QDomElement & top){
    for (int i=0;i<list.size();++i){
        list.at(i)->toXML(top);
    }

}
*/
UndefItem::UndefItem(Canvas2D* p):MyItem(p){
    value=giac::undef;
}
bool UndefItem::isUnderMouse(const QRectF& p) const{
    return false;
}
void UndefItem::updateScreenCoords(const bool){}
void UndefItem::draw(QPainter*) const{}
bool UndefItem::isUndef() const{
    return true;
}
QString UndefItem::getDisplayValue(){

    QString mml("<math mode=\"display\">\n<mtext>undef</mtext>\n</math>");

    return QString(mml);
}

QString UndefItem::getType() const{
    return QString("undef");
}
InterItem::InterItem(const bool &isTangent, Canvas2D* p):MyItem(p){
tangent=isTangent;
}
bool InterItem::isUnderMouse(const QRectF& p) const{
    return false;

}
void InterItem::updateScreenCoords(const bool){}
void InterItem::draw(QPainter*) const{}
bool InterItem::isInter() const{return true;}
QString InterItem::getType() const{
    if (tangent) return QString("Tangent");
    return QString("Intersection");
}

/*QPointF PointElement::getOriginScreen() const{
    return originScreen;
}*/
QPointF PointElement::getOrigin() const{
    return origin;
}

void PointElement::setOrigin(Point* p){
    origin=QPointF(p->x,p->y);
}
PointElement::PointElement(Point *p, Canvas2D *graph ):Point(graph){
    double a,b;
    origin=QPointF(p->x,p->y);
  //  qDebug()<<"origin"<<p->x<<p->y;
    g2d->toScreenCoord(p->x,p->y,a,b);
    originScreen=QPointF(a,b);
}
QString PointElement::getTranslation(const QPointF& p){
    double a,b;
    double xO,yO;
    g2d->toScreenCoord(0,0,xO,yO);
//    qDebug()<<"translation"<<p.x()<<p.y()<<origin.x()<<origin.y();
//    qDebug()<<"translation"<<p.x()-origin.x()<<origin.y()-p.y()<<p.x()-origin.x()+xO<<p.y()-origin.y()+yO;
    g2d->toXY(p.x()-originScreen.x()+xO,p.y()-originScreen.y()+yO,a,b);
    QString s;
    s.append(QString::number(a));
    s.append("+i*");
    s.append(QString::number(b));
    return s;
}
void PointElement::updateScreenCoords(const bool compute){
    if (compute){
        double a,b;
        g2d->toScreenCoord(origin.x(),origin.y(),a,b);
        originScreen=QPointF(a,b);
    }
    Point::updateScreenCoords(compute);
}
bool PointElement::isPointElement() const{
    return true;
}
AngleItem::AngleItem( Canvas2D* g2d):MyItem(g2d){
    arc=0;
    curve=0;
 }
bool AngleItem::isUnderMouse(const QRectF& p) const{
    return arc->isUnderMouse(p)||curve->isUnderMouse(p);
}
void AngleItem::updateScreenCoords(const bool b){
    arc->updateScreenCoords(b);
    curve->updateScreenCoords(b);
}
void AngleItem::draw(QPainter* painter) const{
    arc->setAttributes(attributes);
    curve->setAttributes(attributes);
    arc->draw(painter);
    curve->draw(painter);
}
bool AngleItem::isAngleItem() const{
    return true;
}
QString AngleItem::getType() const{
    return QObject::tr("Angle");
}
void AngleItem::setCurve(MyItem *c){
    curve=c;
}
void AngleItem::setCircle(MyItem * c){
    arc=c;
}
MyItem* AngleItem::getCircle(){
    return arc;
}
MyItem* AngleItem::getCurve(){
    return curve;
}
QString AngleItem::getDisplayValue(){
   return QString::fromStdString(giac::gen2mathml(value,g2d->getContext()));
}
void AngleItem::toXML(QDomElement & top){
    QDomElement angle=top.ownerDocument().createElement("angle");
    QDomElement valueNode=top.ownerDocument().createElement("value");
    QDomText text=top.ownerDocument().createTextNode(QString::fromStdString(giac::print(value,g2d->getContext())));
    valueNode.appendChild(text);
    angle.appendChild(valueNode);
    arc->toXML(angle);
    curve->toXML(angle);
    top.appendChild(angle);
}

CursorItem::CursorItem(const bool & b, Canvas2D * p):MyItem(p){
    isNumeric=b;
}
void CursorItem::draw(QPainter *) const{}
void CursorItem::updateScreenCoords(const bool){}
QString CursorItem::getType() const{
    return(QObject::tr("Curseur"));
}
void CursorItem::toXML(QDomElement & top){
    QDomElement cursor=top.ownerDocument().createElement("cursor");
    //cur

}

CursorItem::~CursorItem(){
    delete cursorPanel;
}
CursorPanel* CursorItem::getCursorPanel(){
    return cursorPanel;
}
void CursorItem::setCursorPanel(CursorPanel* p){
    cursorPanel=p;
}
bool CursorItem::isCursorItem()const{
    return true;
}
bool CursorItem::isFormal(){
    return !isNumeric;
}
