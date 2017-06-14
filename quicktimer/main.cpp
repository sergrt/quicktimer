#include "stdafx.h"
#include "quicktimer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    quicktimer w;
    w.show();
    return a.exec();
}
