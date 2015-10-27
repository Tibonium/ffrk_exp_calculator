#include "ffrk_exp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ffrk_exp w;
    w.show();

    return a.exec();
}
