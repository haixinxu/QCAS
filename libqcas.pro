# #####################################################################
# build of qcas against  libgiac
# #####################################################################
QT += core \
     gui \
     xml \
     svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qcas
TEMPLATE = lib
#CONFIG+=staticlib

QMAKE_CFLAGS_DEBUG += -fno-strict-aliasing -Wno-unused-parameter
QMAKE_CFLAGS_RELEASE += -fno-strict-aliasing -Wno-unused-parameter

QMAKE_CXXFLAGS_DEBUG += \
    -fno-strict-aliasing \
    -Wno-unused-parameter \
    -DGIAC_GENERIC_CONSTANTS -fpermissive -fPIC
QMAKE_CXXFLAGS_RELEASE += \
    -fno-strict-aliasing \
    -Wno-unused-parameter \
    -DGIAC_GENERIC_CONSTANTS -fpermissive -fPIC
DEPENDPATH += . \
    qt
INCLUDEPATH += . \
    qt
win32{
    RC_FILE=qcas.rc
    CONFIG+=rtti
    QMAKE_CXXFLAGS+=-D_GLIBCXX_USE_CXX11_ABI=0 -D__MINGW_H -DGIAC_MPQS -UHAVE_CONFIG_H -DIN_GIAC  -fexceptions
    win32:LIBS+=-lgiac -lgmp -lpthread
    # ce test ne marche qu avec QT5 ??
    win32:contains(QMAKE_HOST.arch, i386):{
        message("x86 build")
   } else {
        message("x86_64 build")
        QMAKE_LFLAGS+=-L./win64/bin
        INCLUDEPATH += ./win64/include

   }
}

unix{
    #QMAKE_CXXFLAGS+=-DHAVE_CONFIG_H
    #QMAKE_CXXFLAGS+=-DSIZEOF_VOID_P=__SIZEOF_POINTER__ #for amd64 system. define SMARTPTR64 in <giac/first.h>
    #CONFIG+=staticlib
    LIBS += -ldl -lgiac  -lgmp
}
macx{
    LIBS += -lintl
    # path to giac and gmp libraries/headers
    LIBS+=-L/opt/local/lib
    INCLUDEPATH+=/opt/local/include
    #with libgiac from the xcas installer
    #INCLUDEPATH+=/Applications/usr/include
    #LIBS+=-L/Applications/usr/64/local/lib -L/Applications/usr/lib
#
}


# Input
HEADERS +=  qt/MainWindow.h \
    qt/output.h \
    qt/CasManager.h \
    qt/geometry.h \
    qt/config.h \
    qt/giacpy.h \
    qt/gui/WizardMatrix.h \
    qt/gui/WizardEquation.h \
    qt/gui/WizardCatalog.h \
    qt/gui/WizardAlgo.h \
    qt/gui/spreadsheet.h \
    qt/gui/qtmmlwidget.h \
    qt/gui/FormalSheet.h \
    qt/gui/FormalLineWidgets.h \
    qt/gui/FormalLine.h \
    qt/gui/CentralTabWidget.h \
    qt/gui/prefdialog.h \
    qt/gui/plotfunctiondialog.h \
    qt/sizeof_void_p.h


SOURCES +=     qt/MainWindow.cpp \
    qt/output.cpp \
    qt/main.cpp \
    qt/config.cpp \
    qt/giacpy.cpp \
    qt/CasManager.cpp \
    qt/gui/WizardMatrix.cpp \
    qt/gui/WizardEquation.cpp \
    qt/gui/WizardCatalog.cpp \
    qt/gui/WizardAlgo.cpp \
    qt/gui/spreadsheet.cpp \
    qt/gui/qtmmlwidget.cpp \
    qt/gui/FormalSheet.cpp \
    qt/gui/FormalLineWidgets.cpp \
    qt/gui/FormalLine.cpp \
    qt/gui/CentralTabWidget.cpp \ # qt/gui/src/qtmmlwidget.cpp \
    qt/gui/prefdialog.cpp \
    qt/geometry.cpp \
    qt/gui/plotfunctiondialog.cpp
#
OTHER_FILES += \
    qt/doc/fr/menu.html \
    qt/doc/fr/memento.html \
    qt/doc/fr/memento_algo.html \
    qt/doc/fr/xcasmenu.html \
    qt/doc/fr/transformations.html \
    qt/doc/en/menu.html \
    qt/doc/en/memento.html \
    qt/doc/en/xcasmenu.html \
    qt/doc/en/transformations.html \
    qt/doc/el/menu.html \
    qt/doc/el/memento.html \
    qt/doc/el/xcasmenu.html \
    qt/doc/el/transformations.html \
    qt/doc/es/menu.html \
    qt/doc/es/memento.html \
    qt/doc/es/xcasmenu.html \
    qt/doc/es/transformations.html \
    qt/doc/zh/menu.html \
    qt/doc/zh/memento.html \
    qt/doc/zh/xcasmenu.html \
    qt/doc/zh/transformations.html \
    qt/doc/de/menu.html \
    qt/doc/de/memento.html \
    qt/doc/de/xcasmenu.html \
    qt/doc/de/transformations.html \
    qt/images/stop.png \
    qt/images/spreadsheet.png \
    qt/images/segment.png \
    qt/images/programming.png \
    qt/images/prog-scol100.png \
    qt/images/process-stop.png \
    qt/images/previous.png \
    qt/images/point.png \
    qt/images/open.png \
    qt/images/next.png \
    qt/images/new.png \
    qt/images/midpoint.png \
    qt/images/matrix.png \
    qt/images/line.png \
    qt/images/inter.png \
    qt/images/help.png \
    qt/images/halfline.png \
    qt/images/formal.png \
    qt/images/f1.png \
    qt/images/exit.png \
    qt/images/evaluate.png \
    qt/images/evaluate-buisy.png \
    qt/images/equation.png \
    qt/images/edit-undo.png \
    qt/images/edit-redo.png \
    qt/images/edit-paste.png \
    qt/images/edit-find.png \
    qt/images/home.png \
    qt/images/edit-cut.png \
    qt/images/edit-copy.png \
    qt/images/document-save-as.png \
    qt/images/document-save.png \
    qt/images/document-new.png \
    qt/images/circle3pt.png \
    qt/images/circle2pt.png \
    qt/images/book.png \
    qt/images/add.png \
    qt/images/cas.png \
    qt/images/ampoule.png \
    qt/images/general.png \
    qt/images/french.png \
    qt/images/english.png \
    qt/images/spain.png \
    qt/images/greece.png \
    qt/images/china.png \
    qt/images/pointxy.png \
    qt/images/zoom-out.png \
    qt/images/zoom-in.png \
    qt/images/ortho.png \
    qt/images/source.png \
    qt/images/tex.png \
    qt/images/circleRadius.png \
    qt/images/right.png \
    qt/images/select.png \
    qt/images/function.png \
    qt/images/configure.png \
    qt/images/bezier.png \
    qt/images/vector.png \
    qt/images/regularpolygon.png \
    qt/images/polygon.png \
    qt/images/perpenbisector.png \
    qt/images/parallel.png \
    qt/images/openpolygon.png \
    qt/images/bot.png \
    qt/images/bisector.png \
    qt/images/perpendicular.png \
    qt/images/move.png \
    qt/images/translation.png \
    qt/images/reflection.png \
    qt/images/point-symmetry.png \
    qt/images/arc3pt.png \
    qt/images/rotation.png \
    qt/images/delete.png \
    qt/images/tangent.png \
    qt/images/similarity.png \
    qt/images/linebyequation.png \
    qt/images/angle.png \
    qt/images/numericcursor.png \
    qt/images/formalcursor.png \
    qt/images/svg.png \
    qt/images/png.png \
    qt/images/homothety.png \
    pthread-win32/libpthreadGC2.a \
    qt/images/mathml.png \
    qt/aide_cas \
    qt/images/icon.png \
    android/AndroidManifest.xml \
    android/version.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/res/values-ro/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-de/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-id/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/values-fa/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/values-ms/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/values-pl/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-es/strings.xml
RESOURCES += qt/qcas.qrc
TRANSLATIONS = qt/lang/qcas_en.ts qt/lang/qcas_es.ts qt/lang/qcas_el.ts qt/lang/qcas_zh.ts
FORMS=qt/MainWindow.ui
