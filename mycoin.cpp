#include "mycoin.h"
#include <QDebug>
#include <QPixmap>
#include <QTimer>

// MyCoin::MyCoin(QWidget *parent)
//     : QPushButton{parent}
// {}

//参数代表传入的金币/银币路径
MyCoin::MyCoin(QString btnImg){
    QPixmap pix;
    bool jud = pix.load(btnImg);
    if(!jud){
        QString str = QString("图片%1加载失败").arg(btnImg);
        qDebug() << str;
        return;
    }
    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));

    //初始化定时器对象
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    //监听正面翻反面的信号，并翻转硬币
    connect(timer1,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str = QString(":/resource/Coin000%1").arg(this->min++);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
        //如果翻完了，重置min的值
        if(this->min > this->max){
            this->min = 1;
            isAnimation = false;//结束做动画
            timer1->stop();
        }
    });

    //监听反面翻正面的信号，并翻转硬币
    connect(timer2,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str = QString(":/resource/Coin000%1").arg(this->max--);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
        //如果翻完了，重置max的值
        if(this->min > this->max){
            this->max = 8;
            isAnimation = false;//结束做动画
            timer2->stop();
        }
    });

};

//重写 按下 事件
void MyCoin::mousePressEvent(QMouseEvent *e){
    if(this->isAnimation || this->isWin){
        return;
    }else{
        QPushButton::mousePressEvent(e);
    }
};

//改变标志的方法
void MyCoin::changeFlag(){
    //如果是正面，翻转成反面
    if(this->flag){
        //开启正-反计时器
        timer1->start(30);
        isAnimation = true;//开始做动画
        flag = false;
    }else {
        //开启反-正计时器
        timer2->start(30);
        isAnimation = true;//开始做动画
        flag = true;
    }
};
