#include "giacpy.h"

QApplication * app=0;   
     
int externalqcas( giac::gen & c , giac::context * ct){

    char *argv[] = {"qcas", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;
    setlocale(LC_NUMERIC,"POSIX");
    app=new QApplication(argc, argv,true);
    Q_INIT_RESOURCE(qcas);
#if QT_VERSION < 0x050000
QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif

    MainWindow win(1);
    win.loadgiacgen(c,ct);
    //    qDebug("gen loaded");
    //   std::cout<<print(c,ct)<<std::endl;
    win.show();
    return app->exec();
}

int externalinteractiveqcas( giac::gen & c , giac::context * ct, std::string s){

    char *argv[] = {"qcas", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;
    setlocale(LC_NUMERIC,"POSIX");
    app=new QApplication(argc, argv,true);
    Q_INIT_RESOURCE(qcas);
#if QT_VERSION < 0x050000
QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif


    MainWindow win(1);
    win.loadinteractivegiacgen(c,ct);
    win.sendText(QString::fromStdString(s));
    //    qDebug("gen loaded");
    //   std::cout<<print(c,ct)<<std::endl;
    win.show();
    return app->exec();
}

