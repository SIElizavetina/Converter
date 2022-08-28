#include "converter.h"

#include <QApplication>
#include <QtPlugin>

//QT_BEGIN_NAMESPESE
//Q_IMPORT_PLUGIN(qico)
//QT_END_NAMESPASE


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Converter w;
    w.show();
    return a.exec();
}
