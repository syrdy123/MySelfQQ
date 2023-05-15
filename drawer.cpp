#include "drawer.h"

#include <QGroupBox>
#include <QVBoxLayout>


Drawer::Drawer(QWidget* parent,Qt::WindowFlags f):
    QToolBox (parent,f)
{
    setWindowTitle(tr("MySelfQQ 2023"));
    QPixmap pix;
    pix.load(":/images/qq.png");
    setWindowIcon(pix);

    toolBtn1 = new QToolButton;
    toolBtn1->setText(tr("感觉不如原神"));
    pix.load(":/images/spqy.png");
    toolBtn1->setIcon(pix);
    toolBtn1->setIconSize(pix.size());
    toolBtn1->setAutoRaise(true); //当鼠标移开时，按钮自动恢复成弹起状态
    toolBtn1->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //点击按钮显示窗口
    connect(toolBtn1,SIGNAL(clicked()),this,SLOT(show_chat_widget1()));

    toolBtn2 = new QToolButton;
    toolBtn2->setText(tr("顶针"));
    pix.load(":/images/ymrl.png");
    toolBtn2->setIcon(pix);
    toolBtn2->setIconSize(pix.size());
    toolBtn2->setAutoRaise(true);
    toolBtn2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn2,SIGNAL(clicked()),this,SLOT(show_chat_widget2()));

    toolBtn3 = new QToolButton;
    toolBtn3->setText(tr("再看一眼就会爆炸"));
    pix.load(":/images/qq.png");
    toolBtn3->setIcon(pix);
    toolBtn3->setIconSize(pix.size());
    toolBtn3->setAutoRaise(true);
    toolBtn3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn3,SIGNAL(clicked()),this,SLOT(show_chat_widget3()));

    toolBtn4 = new QToolButton;
    toolBtn4->setText(tr("cherry"));
    pix.load(":/images/Cherry.png");
    toolBtn4->setIcon(pix);
    toolBtn4->setIconSize(pix.size());
    toolBtn4->setAutoRaise(true);
    toolBtn4->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn4,SIGNAL(clicked()),this,SLOT(show_chat_widget4()));

    toolBtn5 = new QToolButton;
    toolBtn5->setText(tr("止境"));
    pix.load(":/images/dr.png");
    toolBtn5->setIcon(pix);
    toolBtn5->setIconSize(pix.size());
    toolBtn5->setAutoRaise(true);
    toolBtn5->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn5,SIGNAL(clicked()),this,SLOT(show_chat_widget5()));

    toolBtn6 = new QToolButton;
    toolBtn6->setText(tr("娇娇"));
    pix.load(":/images/jj.png");
    toolBtn6->setIcon(pix);
    toolBtn6->setIconSize(pix.size());
    toolBtn6->setAutoRaise(true);
    toolBtn6->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn6,SIGNAL(clicked()),this,SLOT(show_chat_widget6()));

    toolBtn7 = new QToolButton;
    toolBtn7->setText(tr("平静止水"));
    pix.load(":/images/lswh.png");
    toolBtn7->setIcon(pix);
    toolBtn7->setIconSize(pix.size());
    toolBtn7->setAutoRaise(true);
    toolBtn7->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn7,SIGNAL(clicked()),this,SLOT(show_chat_widget7()));

    toolBtn8 = new QToolButton;
    toolBtn8->setText(tr("孙笑川"));
    pix.load(":/images/qmnn.png");
    toolBtn8->setIcon(pix);
    toolBtn8->setIconSize(pix.size());
    toolBtn8->setAutoRaise(true);
    toolBtn8->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn8,SIGNAL(clicked()),this,SLOT(show_chat_widget8()));

    toolBtn9 = new QToolButton;
    toolBtn9->setText(tr("东雪莲"));
    pix.load(":/images/wy.png");
    toolBtn9->setIcon(pix);
    toolBtn9->setIconSize(pix.size());
    toolBtn9->setAutoRaise(true);
    toolBtn9->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn9,SIGNAL(clicked()),this,SLOT(show_chat_widget9()));

    QGroupBox * groupBox = new QGroupBox;
    QVBoxLayout * layout = new QVBoxLayout(groupBox);

    layout->setMargin(20);
    layout->setAlignment(Qt::AlignLeft);

    layout->addWidget(toolBtn1);
    layout->addWidget(toolBtn2);
    layout->addWidget(toolBtn3);
    layout->addWidget(toolBtn4);
    layout->addWidget(toolBtn5);
    layout->addWidget(toolBtn6);
    layout->addWidget(toolBtn7);
    layout->addWidget(toolBtn8);
    layout->addWidget(toolBtn9);
    layout->addStretch();  //插入一个占位符

    this->addItem((QWidget*)groupBox,tr("群成员"));
}

void Drawer::show_chat_widget1(){
    chat_widget1 = new Widget(0,toolBtn1->text());
    chat_widget1->setWindowTitle(toolBtn1->text());
    chat_widget1->setWindowIcon(toolBtn1->icon());
    chat_widget1->show();
}

void Drawer::show_chat_widget2(){
    chat_widget2 = new Widget(0,toolBtn2->text());
    chat_widget2->setWindowTitle(toolBtn2->text());
    chat_widget2->setWindowIcon(toolBtn2->icon());
    chat_widget2->show();
}

void Drawer::show_chat_widget3(){
    chat_widget3 = new Widget(0,toolBtn3->text());
    chat_widget3->setWindowTitle(toolBtn3->text());
    chat_widget3->setWindowIcon(toolBtn3->icon());
    chat_widget3->show();
}

void Drawer::show_chat_widget4(){
    chat_widget4 = new Widget(0,toolBtn4->text());
    chat_widget4->setWindowTitle(toolBtn4->text());
    chat_widget4->setWindowIcon(toolBtn4->icon());
    chat_widget4->show();
}

void Drawer::show_chat_widget5(){
    chat_widget5 = new Widget(0,toolBtn5->text());
    chat_widget5->setWindowTitle(toolBtn5->text());
    chat_widget5->setWindowIcon(toolBtn5->icon());
    chat_widget5->show();
}

void Drawer::show_chat_widget6(){
    chat_widget6 = new Widget(0,toolBtn6->text());
    chat_widget6->setWindowTitle(toolBtn6->text());
    chat_widget6->setWindowIcon(toolBtn6->icon());
    chat_widget6->show();
}

void Drawer::show_chat_widget7(){
    chat_widget7 = new Widget(0,toolBtn7->text());
    chat_widget7->setWindowTitle(toolBtn7->text());
    chat_widget7->setWindowIcon(toolBtn7->icon());
    chat_widget7->show();
}

void Drawer::show_chat_widget8(){
    chat_widget8 = new Widget(0,toolBtn8->text());
    chat_widget8->setWindowTitle(toolBtn8->text());
    chat_widget8->setWindowIcon(toolBtn8->icon());
    chat_widget8->show();
}

void Drawer::show_chat_widget9(){
    chat_widget9 = new Widget(0,toolBtn9->text());
    chat_widget9->setWindowTitle(toolBtn9->text());
    chat_widget9->setWindowIcon(toolBtn9->icon());
    chat_widget9->show();
}
