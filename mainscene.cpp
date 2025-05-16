#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QSoundEffect>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //配置主场景

    //设置固定大小
    setFixedSize(320,588);
    //设置图标
    setWindowIcon(QIcon(":/resource/Coin0001.png"));
    //设置标题
    setWindowTitle("快乐翻金币");
    //退出按钮的实现
    connect(ui->actionquit,&QAction::triggered,[=](){
        this->close();
    });

    //准备开始按钮的音效
    QSoundEffect *startSound = new QSoundEffect(this);
    startSound->setSource(QUrl::fromLocalFile(":/resource/TapButtonSound.wav"));

    //开始按钮
    MyPushButton *startBtn = new MyPushButton(":/resource/MenuSceneStartButton");
    startBtn->setParent(this);
    startBtn->move((this->width() - startBtn->width()) * 0.5 , this->height() * 0.7);

    //实例化选择关卡的场景
    chooseScene = new ChooseLevelScene;

    //监听选择关卡的返回按钮信号
    connect(chooseScene, &ChooseLevelScene::chooseSceneBack,[=](){
        this->setGeometry(chooseScene->geometry());
        chooseScene->hide();//选择关卡场景隐藏
        this->show() ;//重新显示主场景
    });

    //点击开始按钮
    connect(startBtn,&QPushButton::clicked,[=](){
        // qDebug() << "点击了开始按钮";

        //播放开始音效
        startSound->play();

        //做弹起特效
        startBtn->zoom1();
        startBtn->zoom2();

        //延时进入关卡特效
        QTimer::singleShot(500,this,[=](){
            //设置chooseScene场景的位置
            chooseScene->setGeometry(this->geometry());

            //进入到选择关卡的场景中
            //自身隐藏
            this->hide();
            //显示选择关卡的场景
            chooseScene->show();
        });
    });
}

//重写paintEvent事件，画背景图
void MainScene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/resource/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //画背景图上的图标
    pix.load(":/resource/Title.png");
    pix = pix.scaled(pix.width() * 0.5 ,pix.height() * 0.5);
    painter.drawPixmap(10,30,pix);
}

MainScene::~MainScene()
{
    delete ui;
}
