#ifndef CHOOSELEVELSCENE_H
#define CHOOSELEVELSCENE_H

#include <QMainWindow>
#include "playscene.h"

class ChooseLevelScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChooseLevelScene(QWidget *parent = nullptr);

    //重写paintEvent事件，画背景图
    void paintEvent(QPaintEvent *);
    //游戏场景对象指针
    PlayScene *play;
signals:
    //自定义信号，告诉主场景返回了
    void chooseSceneBack();
};

#endif // CHOOSELEVELSCENE_H
