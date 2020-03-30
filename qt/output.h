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

#ifndef OUTPUT_H
#define OUTPUT_H
#include "sizeof_void_p.h"
#include "geometry.h"
#include <giac/gen.h>
#include <QWidget>
#include <QDialog>
#include <QVector>
#include <QDomElement>
#include <QPixmap>
#include <QPainterPath>
#include <QScrollArea>
#include <QHash>
#include <QGroupBox>
#if QT_VERSION < 0x050000
    #include <QWindowsStyle>
#else
    #include <QProxyStyle>
#endif
#include <QTabWidget>
#include <QList>
#include <QPoint>
#include <QUndoCommand>
#include "gui/CentralTabWidget.h"
class QMenu;
//class QLabel;
class MainWindow;
class QBoxLayout;
class Line;
class QComboBox;
class QListWidget;
class QVBoxLayout;
class QPushButton;
class QCheckBox;
class QTreeWidgetItem;
class QToolButton;
class QLineEdit;
class PanelProperties;
class DisplayProperties;
class DisplayObjectPanel;
class AxisPanel;
class QLabel;
class GridPanel;
class AxisGridPanel;
class GenValuePanel;
class QTreeWidget;
class QSlider;
class QSpinBox;
class QtMmlWidget;
class WidthPanel;
class TypePointPanel;
class TypeLinePanel;
class ColorPanel;
class LegendPanel;
class AlphaFillPanel;
class CursorPanel;

#if QT_VERSION < 0x050000
class IconSize : public QWindowsStyle
#else
class IconSize : public QProxyStyle
#endif
 {
   Q_OBJECT
public:
#if QT_VERSION < 0x050000
    explicit IconSize(QWindowsStyle *parent = 0);
#else
    explicit IconSize(QProxyStyle *parent = 0);
#endif
    int pixelMetric(PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0 ) const;
signals:
public slots:
};
struct Command{
    QString command;
    int attributes;
    MyItem* item;
    bool isCustom;
};

enum PolarAngle{
    PI_12=0,PI_8=1,PI_6=2,PI_4=3,PI_3=4,PI_2=5
};
/**
 * @brief The GridParam struct
 *
 * @variable color: Grid color
 * @variable line: line Type
 * @variable isVisible true if grid is visible
 * @variable isCartesian if grid is cartesian, false if polar
 * @variable x: step on x axis (cartesian)
 * @variable y: step on y axis (cartesian)
 * @variable r: distance from origin (polar)
 * @variable theta: angle for graduation (polar)
 *
 *
 */
struct GridParam{
    QColor color;
    int  line;
    bool isCartesian,isVisible;
    double x,y,r;
    PolarAngle theta;
};
struct AxisParam{
    QColor color;
//    int  line;
    bool isVisible;
    QString legend;
    QString unitSuffix;
    double tick,min,max;
};

class AddObjectCommand:public QUndoCommand{
public:
    AddObjectCommand(Canvas2D*);
    virtual void undo();
    virtual void redo();
private:
    bool initCall;
    Canvas2D* canvas;
    QDomDocument doc;
};

class MoveObjectCommand:public QUndoCommand{
public:
    MoveObjectCommand(int level,QPointF ,QPointF ,Canvas2D*);
    virtual void undo();
    virtual void redo();
private:
    bool initCall;
    int level;
    Canvas2D* canvas;
    QPointF oldPos;
    QPointF newPos;
};
class DeleteObjectCommand:public QUndoCommand{
public:
    DeleteObjectCommand(int l, Canvas2D *);
    virtual void undo();
    virtual void redo();
    virtual int id() const;
    virtual bool mergeWith(const QUndoCommand *other);
protected:
    QList<int> levels;
    QList<QDomDocument> docs;
private:
    Canvas2D* canvas;
};
class ModifyAttributesCommand:public QUndoCommand{
public:
    ModifyAttributesCommand(const int &, const int &, const int &,Canvas2D*, const int& levChild=-1);

    virtual void undo();
    virtual void redo();
    virtual int id() const;
    virtual bool mergeWith(const QUndoCommand *other);
protected:
    QList <int> levels;
    QList <int> levelsChild;
    QList <int>  oldAtts;
    QList <int> newAtts;

private:

    Canvas2D* canvas;
};
class ZoomCommand:public QUndoCommand{
public:
    ZoomCommand(const AxisParam&,const AxisParam&,const AxisParam&,const AxisParam&,Canvas2D*,const bool &compute=true);
    virtual void undo();
    virtual void redo();
    virtual int id() const;
    virtual bool mergeWith(const QUndoCommand *other);
protected:
    bool compute;
    AxisParam oldX,oldY,newX,newY;
    Canvas2D* canvas;
};


class DisplayObjectCommand:public QUndoCommand{
public:
    DisplayObjectCommand(const int &, const bool &, const bool &,Canvas2D*, const int& levChild=-1);
    virtual void undo();
    virtual void redo();
private:
    bool oldVisible;
    bool newVisible;
    int level;
    int levelChild;

    Canvas2D* canvas;
};

class RenameObjectCommand:public QUndoCommand{
public:
    RenameObjectCommand(const QString &, const QString &,Canvas2D*);
    virtual void undo();
    virtual void redo();
private:
    Canvas2D* canvas;
    QString oldName;
    QString newName;

};

class OutputWidget:public QWidget{
public:
    OutputWidget(QWidget* widget=0);
    void setLine(Line* );
    virtual void toXML(QDomElement&);
    virtual QString getTextOutput();
    virtual bool isFormula();

private:
    Line* line;
};
class FormulaWidget :public OutputWidget{
    Q_OBJECT
public:
    FormulaWidget(QWidget*);
    FormulaWidget(const giac::gen &, giac::context *);
    void updateFormula(const giac::gen &, giac::context *);
    void updateFormula(const QString );
    void setGen(const giac::gen &);
    void zoomIn();
    void zoomOut();
    virtual void toXML(QDomElement &);
    QString getTextOutput();
    bool isFormula();
protected:
    QSize sizeHint();
private:
    void initGui();
    giac::context* context;
    giac::gen formula;
    QtMmlWidget *mmlWidget;
    QMenu* menu;
    QAction* zoomInAction;
    QAction* zoomOutAction;
    QAction* copyAction;
    QAction* toLatexAction;
    QAction* toMathmlAction;
private slots:
    void displayMenu(QPoint);
    void copy();
    void copyToLaTeX();
    void copyToMathml();
    void zoomInslot();
    void zoomOutslot();
};


/**
 * @brief The GraphWidget class
 * This widget is used for 2D graphics rendering.
 *  It supports two modes:  normal or interactive
 *
 */
class GraphWidget:public OutputWidget,public MainSheet{
    Q_OBJECT

  public:
    GraphWidget(giac::context*, bool , MainWindow *main);
    GraphWidget(const  giac::gen &, giac::context*,bool,MainWindow*main );
    QList<MyItem*> getTreeSelectedItems();
    void removeFromTree(MyItem * );
    void clearSelection();
    bool isInteractive() const;
    void addToTree(MyItem*);
    void renameInTree(MyItem * );
    void updateAllCategories();
    void updateValueInDisplayPanel();
    void selectInTree(MyItem *);
    void addCursorPanel(CursorPanel* );
    void deleteCursorPanel(CursorPanel* );
    virtual void toXML(QDomElement &);
    void toInteractiveXML(QDomElement  &);
    void evaluate();
    void getDisplayCommands(QStringList &);
    void toInteractiveXCAS2D(QString  &);
    void sendText(const QString  &);
    void sendgiacgen(const giac::gen  &); //cf giacpy
    void sendinteractivegiacgen(const giac::gen  &); //cf giacpy
    void loadXML(QDomElement &);
    void loadInteractiveXML(QDomElement &);
    void XML2Axis(QDomElement &);
    void XML2Grid(QDomElement &);
    void undo();
    void redo();
 private:
    MainWindow* mainWindow;
    bool isInteractiveWidget;
    Canvas2D* canvas;
    PanelProperties * propPanel;

    QWidget* sliderPanel;
    QWidget* toolPanel;
    // The menu tool bar in interactive mode
    QToolButton* buttonPointer;
    QToolButton* buttonPt;
    QToolButton* buttonLine;
    QToolButton* buttonTool;
    QToolButton* buttonPlot;
    QToolButton* buttonCircle;

    QAction* select;
    QAction* zoomIn;
    QAction* zoomOut;
    QAction* singlept;
    QAction* pointxy;
    QAction * inter;
    QAction* midpoint;
    QAction* line;
    QAction* linebyequation;
    QAction* halfline;
    QAction* segment;
    QAction *bisector;
    QAction *tangent;
    QAction *perpen_bisector;
    QAction* vectorAc;
    QAction *parallel;
    QAction *perpendicular;
    QAction *pointSymmetry;
    QAction *reflection;
    QAction *rotation;
    QAction *translation;
    QAction* homothety;
    QAction* similarity;
    QAction* plotFunction;
    QAction* plotBezier;
    QAction* regularPolygon;
    QAction* polygon;
    QAction* circle2pt;
    QAction* circle3pt;
    QAction* circleRadius;
    QAction* arc3pt;
    QAction* angle;
    QAction* numericCursor;
    QAction* formalCursor;

    void initGui();
    void createToolBar();
    void XML2Circle(QDomElement &nodeCircle, const int &att);
    void XML2Curve(QDomElement &, const bool &fillable, const int &att);

 private slots:
    void selectButtonIcon(QAction*);
    void selectAction();
public slots:
    void setUndoButton(bool);
    void setRedoButton(bool);
};



// The canvas to draw 2D graphics

class Canvas2D:public QWidget{
    Q_OBJECT
public:
    enum action{SELECT,ZOOM_IN,ZOOM_OUT,SINGLEPT,POINT_XY,MIDPOINT,INTER,LINE,LINEBYEQUATION,HALFLINE,SEGMENT,BISECTOR,PERPEN_BISECTOR,VECTOR,TANGENT,PARALLEL,PERPENDICULAR,
                REFLECTION,POINT_SYMMETRY,TRANSLATION,ROTATION,HOMOTHETY,SIMILARITY,PLOT_FUNCTION,PLOT_BEZIER,REGULAR_POLYGON,POLYGON,CIRCLE2PT,CIRCLE_RADIUS
                ,CIRCLE3PT,ARC3PT,ANGLE,NUMERIC_CURSOR,FORMAL_CURSOR};

    Canvas2D(GraphWidget* g2d, giac::context*);
    ~Canvas2D();
    void createScene(const giac::gen & );
    void toScreenCoord(const double,const double,double& , double&);
    void toXY(const double,const double,double& , double&);
    void toInteractiveXCAS2D(QString  &);
    void sendText(const QString  &);
    void sendgiacgen(const giac::gen  &); //cf. giacpy
    void sendinteractivegiacgen(const giac::gen  &); //cf. giacpy
    void toInteractiveXML(QDomElement &);
    void loadInteractiveXML(QDomElement &);
    void itemToXML(Command , QDomElement &, const bool &setLevel=false);
    void axisToXML(QDomElement &);
    void gridToXML(QDomElement &);
    void toXML(QDomElement &);
    QList<MyItem*>* getPointItem();
    QList<MyItem *> *getLineItem();
    QList<MyItem*>* getFilledItem();
    void setFocusOwner(MyItem*);
    void updatePixmap(const bool &);
    void setActionTool(action);
    void setXYUnit();
    bool checkForOnlyPoints(const QList<MyItem *> *) const;
    bool checkForOnlyLines(const QList<MyItem *> *) const;
    bool checkForOnlyFillables(const QList<MyItem *> *) const;
    void getDisplayCommands(QStringList & );
    void clearallItems();
    void undo();
    void redo();
    void moveItem(MyItem*, const QPointF & );


    // Getter & Setter
    bool isInteractive() const;
    void setBounds(const double &,const double &,const double &,const double &);
    QString getXAxisLegend() const;
    QString getYAxisLegend() const;
    QString getXUnitSuffix() const;
    QString getYUnitSuffix() const;
    GridParam getGridParam() const;
    AxisParam getXAxisParam() const;
    AxisParam getYAxisParam() const;
    giac::context* getContext() const;
    void setGridParam(const GridParam &);
    void setXAxisParam(const AxisParam &);
    void setYAxisParam(const AxisParam &);

    double getXmin() const;
    double getXmax() const ;
    double getYmin() const;
    double getYmax() const;
    QList<Command>& getCommands();
    void deleteObject(MyItem*);

    void deleteSingleObject(MyItem*);
    void addToScene(QList<MyItem *> &);
    void initAfterDeleting();
    void renameSingleObject(MyItem*,const QString&);
    int findItemFromVar(const QString&,QList<MyItem*>*);
    QUndoStack * getUndoStack();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    bool event(QEvent *);
    void resizeEvent(QResizeEvent *);
    virtual QSize minimumSizeHint() const;

private:
    giac::context *context;
    giac::context *localcontext;
    giac::context* externalcontext;

    // Item which could be highlighted
    MyItem* focusOwner;
    // Item for preview
    MyItem* itemPreview;
    // vector to store Trace points
    QList<MyItem*> traceVector;
    // Command for missing point in preview
    QString missingPoint;

    // x,y range and units
    double xunit,yunit;
    bool ortho;

    GridParam gridParam;
    AxisParam xAxisParam,yAxisParam;

    // Lists to store geometry items
    QList<MyItem*> lineItems;
    QList<MyItem*> pointItems;
    QList<MyItem*> filledItems;

    //Lists to store geometry cursors
    QList<MyItem*> cursorItems;


    // A command for each line
    QList<Command> commands;

    // action selected
    action currentActionTool;
    QList<MyItem*> selectedItems;

    // History undo-redo
    QUndoStack* undoStack;


    QMenu * menuGeneral;
    QAction* exportPNG;
    QAction* exportSVG;
    QAction* exportLatex;
    QMenu* menuExport;
    QAction* zoomIn;
    QAction* zoomOut;
    QAction* orthoAction;
    QAction* axisAction;
    QAction* gridAction;
    QAction* sourceAction;
    QMenu* menuObject;
    QAction* objectTitleAction;
    QAction* displayObjectAction;
    QAction* displayLegendAction;
    QAction* deleteAction;
    QAction* renameAction;
    QAction* traceAction;

    GraphWidget *parent;

    // To draw a rectangle selection
    bool selectionRight;
    QPoint startSel;
    QPoint endSel;
    // If the user press the left button
    bool selectionLeft;
    // true if the user is moving a point on the screen
    bool isMoving;

    // The Pixmap on which all Items are drawn.
    QPixmap pixmap;
    // Variables names for points and lines.
    QString varPt;
    QString varLine;

//    QString newCommand;

    int evaluationLevel;

    void createMenuAction();
    void addToVector(const giac::gen &, QList<MyItem *> &);


    double find_tick(double);
    void drawAxes(QPainter*);
    void drawGrid(QPainter*);
    void drawPolarLine(const int &,QPainter * );
    bool isInScene(const double & x,const double & y);
//    int findPolarInter(const double &,double & , double & , double &,double &);
    void drawElements(QList<MyItem *> &, QPainter*, const bool&);
    bool checkUnderMouse(QList<MyItem *> *v, const QPointF &);
    bool checkForValidAction(MyItem *);
    void findFreeVar(QString &);
    void incrementVariable(QString &);
    void updateAllChildrenFrom(MyItem*);
    QString commandFreePoint(const QPointF&);
    void refreshFromItem(MyItem *, QList<MyItem *> &,bool evenInter=false);
    void addNewPoint(const QPointF );
    void addNewLine(const QString &,const bool&);
    void addNewCircle(const bool&);
    void addNewPolygon(const bool&, const bool &iso=false);
    void addNewArc(const bool&);
    void addNewBezier(const bool&);
    void addTransformObject(const QString &);
    void addMidpoint();
    void addNewBezierControlPoint();
    void addBisector(const bool &);
    void addPerpenBisector(const bool &);
    void addNewPointElement(const QPointF &pos);
    void addInter(const QString &);
    void addNewAngle();
    void commandTwoArgs(const QString &,const QString &,const QString &,QString  & );
    bool checkForCompleteAction();
    bool checkForPointWaiting();
    bool checkForOneMissingPoint();
    void executeMyAction(bool );
    void renameObject(MyItem*,const QString&);
    void findIDNT(giac::gen &, MyItem *);
    void importparentvalues(giac::gen &);

   void updateAllLevelsFrom(const int& );

private slots:
    void zoom_In();
    void zoom_Out();
    void make_ortho();
    void displayGrid(bool);
    void displayAxis(bool);
    void displaySource();
    void displayObject(bool);
    void exportToPNG();
    void exportToSVG();
    void displayLegend(bool);
    void deleteObject();
    void trace(bool);
    void renameObject();
    void updateAllChildrenFrom();
    void deleteCursorPanel();
};

class PanelProperties:public QWidget{
    Q_OBJECT
public:
    PanelProperties(Canvas2D *);
    void removeFromTree(MyItem * item);
    QList<MyItem*> getTreeSelectedItems();
    void clearSelection();
    void addToTree(MyItem* );
    void renameInTree(MyItem * );

    void updateAllCategories();
    void selectInTree(MyItem * );
    void updateValueInDisplayPanel();
/*protected:
    void resizeEvent(QResizeEvent *);*/

private:
    Canvas2D* parent;
    QTreeWidget* tree;
    QTreeWidgetItem* nodeAxis;
    QTreeWidgetItem* nodePoint;
    QTreeWidgetItem* nodeCurve;
    QTreeWidgetItem* nodeVector;
    QTreeWidgetItem* nodeLine;
    QTreeWidgetItem* nodeSegment;
    QTreeWidgetItem* nodeHalfLine;
    QTreeWidgetItem* nodePolygon;
    QTreeWidgetItem* nodeCircle;
    QTreeWidgetItem* nodeAngle;
    QTreeWidgetItem* nodeList;
    QTreeWidgetItem* nodeUndef;

    QHash<QTreeWidgetItem*,MyItem*> nodeLinks;
    QBoxLayout*  hbox;
    AxisGridPanel* axisGridPanel;
    DisplayProperties* displayPanel;
    bool updateCategory(QTreeWidgetItem *, const int &);
    void initGui();
private slots:
    void updateTree();

};
class DisplayProperties:public QTabWidget{
    Q_OBJECT
public:
    DisplayProperties(Canvas2D* canvas);
    void updateCanvas();
    QList<MyItem*>* getListItems() const;
    void updateDisplayPanel(QList<MyItem*>*);
    void updateValueInDisplayPanel();

private:
    Canvas2D* parent;
    QWidget * generalPanel;
    QWidget * attributesPanel;
    QVBoxLayout *vLayoutGeneral;
    QVBoxLayout *vLayoutAttributes;
    QList<MyItem*>* listItems;
    GenValuePanel* valuePanel;
    DisplayObjectPanel* displayObjectPanel;
    ColorPanel *colorPanel;
    LegendPanel*legendPanel;
    WidthPanel* widthPanel;
    TypePointPanel* typePointPanel;
    TypeLinePanel* typeLinePanel;
    AlphaFillPanel* alphaFillPanel;

    void initGui();
private slots:
    void updateAttributes(int);
    void updateVisible(bool);
};
class ColorPanel:public QWidget{
    Q_OBJECT
public:
    ColorPanel(QWidget*);
   void setColor(const QColor &);
private:
    QColor color;
    QPushButton* preview;
    void initGui();
    void updateButton();
private slots:
    void chooseColor();
signals:
    void colorSelected(int);

};
class LegendPanel:public QWidget{
    Q_OBJECT
public:
    LegendPanel(DisplayProperties *p );
    bool hasLegend() const;
    void setLegend(const bool &,const QString & s="");
    void setChecked(bool);
    void setLegendPosition(const int &);

private:
    DisplayProperties * parent;
    QWidget* legendPanel;
    QWidget* legendPosPanel;
    QCheckBox* legendCheck;
    QLineEdit *legendEdit;
    QString legend;
    QComboBox* comboPos;
    QSpinBox* spinAnglePos;
    void initGui();
private slots:
    void updateCanvas();

};

class SliderPanel:public QGroupBox{
    Q_OBJECT
public:
    SliderPanel(QWidget *p, const QString &);
    void setValue(const int);
private:
    void initGui(const QString &);
protected:
    int value;
    QSlider *slider;
signals:
    void valueChanged(int);

};
class WidthPanel:public SliderPanel{
    Q_OBJECT
public:
    WidthPanel(QWidget*,const QString &);
};
class AlphaFillPanel:public SliderPanel{
    Q_OBJECT
public:
    AlphaFillPanel(QWidget*,const QString & );
};


class DisplayObjectPanel:public QWidget{
    Q_OBJECT
public:
    DisplayObjectPanel(QWidget*);
    void setChecked(const bool);
private:
    void initGui();
    QCheckBox * displayObject;
signals:
    void visibleChanged(bool);
};

class TypePointPanel:public QWidget{
    Q_OBJECT
public:
    TypePointPanel(const int,QWidget* );
    void setStyle(int);
private:
    void initGui();
    int type;
    QComboBox *combo;
signals:
    void typePointSelected(int);

};
class TypeLinePanel:public QWidget{
    Q_OBJECT
public:
    TypeLinePanel(const int,QWidget* );
    void setStyle(const int&);
private:
    void initGui();
    int type;
    QComboBox *combo;

signals:
   void  typeLineSelected(int);
};
class GenValuePanel:public QWidget{
public:
    GenValuePanel(Canvas2D * );
    void setGenValue(const giac::gen  &g);
    void setDisplayValue(const QString);


private:
 //   QLabel* label;
    FormulaWidget *formulaWidget;
    QBoxLayout * layout;
//    QString value;
    void initGui();
    Canvas2D* parent;

//private slots:
  //  void updateCanvas(int );

};
class AxisGridPanel: public QTabWidget{
    Q_OBJECT
public:
    AxisGridPanel(Canvas2D*);
    void initValue();
//    void updateCanvas(const bool &);
private:
    void initGui();
    GridPanel* gridPanel;
    AxisPanel* xPanel;
    AxisPanel* yPanel;
    Canvas2D* parent;
    void updateAxis(AxisParam,bool,const bool&);
private slots:
    void updateGrid(GridParam);
    void updateYAxis(AxisParam,bool);
    void updateXAxis(AxisParam,bool);

};
class GridPanel:public QWidget{
    Q_OBJECT
public:
    GridPanel(QWidget* );

    void initValue(const GridParam &);
private:
    QComboBox* comboType;
    QWidget* polarPanel;
    QWidget* cartesianPanel;
    QLineEdit* editDistance;
    QComboBox* comboPolarAngle;
    QComboBox* comboX;
    QComboBox* comboY;
    QCheckBox* showGrid;
    ColorPanel* colorPanel;
    TypeLinePanel* typeLinePanel;
    void initGui();
    GridParam param;
private slots:
    void updateCanvas();
    void updateColor(int);
    void updateLineType(int);
    void displayValidPanel(int);

signals:
    void gridUpdated(GridParam);
};

class AxisPanel:public QWidget{
    Q_OBJECT
public:
    AxisPanel(QWidget* );
    void initValue(const AxisParam&,const double&,const double&);
private:
    QLineEdit* editLabel;
    QLineEdit* editUnitLabel;
    QLineEdit* editMin;
    QLineEdit* editMax;
    QLineEdit* editDistance;
    QCheckBox* showAxis;
    ColorPanel* colorPanel;
    QColor color;
    void initGui();
private slots:
    void updateCanvas();
    void updateColor(int);
signals:
    void axisUpdated(AxisParam,bool);
};
class SourceDialog:public QDialog{
    Q_OBJECT
public:
    SourceDialog(Canvas2D*);
private:
    Canvas2D* parent;
    void initGui();
    QListWidget* listWidget;
    QPushButton* deleteButton;
private slots:
    void updateCanvas();
};

class CoordsDialog:public QDialog{
public:
    CoordsDialog(Canvas2D*);
    QLineEdit* editX;
    QLineEdit* editY;

private:
    void initGui();
    QPushButton* ok;
    QPushButton* cancel;
};
class OneArgDialog:public QDialog{
public:
    OneArgDialog(Canvas2D*, const QString &, const QString &title);
    QLineEdit* editRadius;
private:
    QString type;
    void initGui();
    QPushButton* ok;
    QPushButton* cancel;
};

class CursorDialog:public QDialog{
public:
    CursorDialog(Canvas2D*);
    QString getVar() const;
    QString getMin() const;
    QString getMax() const;
    QString getStep() const;
    QString getDefault() const;
private:
    void initGui();
    QLineEdit* editMin;
    QLineEdit* editMax;
    QLineEdit* editVar;
    QLineEdit* editDefault;
    QLineEdit* editStep;
    QPushButton* ok;
    QPushButton* cancel;
};
class CursorPanel:public QWidget{
    Q_OBJECT
public:
    CursorPanel(const QString &Name,const double &Min, const double & Max, const double & Step,const double & Default, CursorItem * p);
   double getValue() const;
   CursorItem* getOwner();
   double getMin() const;
   double getMax() const;
   double getStep() const;
private:
    double min;
    double max;
    double step;
    double defaultValue;
    QString name;
    QPushButton* deleteButton;
    QSlider*  slider;
    QLabel* labMin;
    QLabel* labValue;
    QLabel* labMax;
    QLabel* labName;
    CursorItem* owner;
    void initGui();
private slots:
    void updateValue(int);
signals:
    void valueChanged();
    void deletePanel();
};
#endif // OUTPUT_H
