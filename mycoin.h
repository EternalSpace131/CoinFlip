#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QTimer>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    // explicit MyCoin(QWidget *parent = nullptr);

    //参数代表传入的金币/银币路径
    MyCoin(QString btnImg);

    //金币属性
    int pos_x;//x坐标位置
    int pos_y;//y坐标位置
    bool flag;//正反标识

    //改变标志的方法
    void changeFlag();
    QTimer *timer1;//正-反
    QTimer *timer2;//反-正
    int min = 1;
    int max = 8;

    //执行动画的标志
    bool isAnimation = false;

    //重写 按下 事件
    void mousePressEvent(QMouseEvent *e);

    //是否胜利
    bool isWin = false;

signals:
};

#endif // MYCOIN_H
