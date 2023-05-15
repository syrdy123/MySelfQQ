#ifndef DRAWER_H
#define DRAWER_H

#include <QToolBox>
#include <QToolButton>

#include "widget.h"

class Drawer : public QToolBox
{
    Q_OBJECT
public:
    Drawer(QWidget* parent = 0,Qt::WindowFlags f = 0);;

    //人物头像
    QToolButton * toolBtn1;
    QToolButton * toolBtn2;
    QToolButton * toolBtn3;
    QToolButton * toolBtn4;
    QToolButton * toolBtn5;
    QToolButton * toolBtn6;
    QToolButton * toolBtn7;
    QToolButton * toolBtn8;
    QToolButton * toolBtn9;

private slots:
    void show_chat_widget1();
    void show_chat_widget2();
    void show_chat_widget3();
    void show_chat_widget4();
    void show_chat_widget5();
    void show_chat_widget6();
    void show_chat_widget7();
    void show_chat_widget8();
    void show_chat_widget9();

private:
   Widget * chat_widget1;
   Widget * chat_widget2;
   Widget * chat_widget3;
   Widget * chat_widget4;
   Widget * chat_widget5;
   Widget * chat_widget6;
   Widget * chat_widget7;
   Widget * chat_widget8;
   Widget * chat_widget9;
};

#endif // DRAWER_H
