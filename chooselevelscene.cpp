#include "chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include <QSoundEffect>

ChooseLevelScene::ChooseLevelScene(QWidget *parent)
    : QMainWindow{parent}
{
    //配置选择关卡的场景
    //设置固定大小
    setFixedSize(320,588);
    //设置图标
    setWindowIcon(QIcon(":/resource/Coin0001.png"));
    //设置标题
    setWindowTitle("选择关卡");

    //创建菜单栏
    QMenuBar *bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu *startMenu = bar->addMenu("开始");

    //创建退出菜单项
    QAction *quitAction = startMenu->addAction("退出");

    //点击退出，退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //准备选择关卡的音效
    QSoundEffect *chooseSound = new QSoundEffect(this);
    chooseSound->setSource(QUrl::fromLocalFile(":/resource/TapButtonSound.wav"));
    //准备返回按钮的音效
    QSoundEffect *backSound = new QSoundEffect(this);
    backSound->setSource(QUrl::fromLocalFile(":/resource/BackButtonSound.wav"));

    //返回按钮
    MyPushButton *backBtn = new MyPushButton(":/resource/BackButton.png",":/resource/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());

    //点击返回
    connect(backBtn,&QPushButton::clicked,[=](){
        // qDebug() << "点击了返回按钮";
        //主场景要监听返回按钮
        //延时返回

        //播放返回按钮音效
        backSound->play();

        QTimer::singleShot(500,this,[=](){
            emit this->chooseSceneBack();
        });
    });

    //创建选择关卡的按钮
    for(int i = 0 ;i < 20 ; i++){
        MyPushButton *menuBtn = new MyPushButton(":/resource/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move( 25 +i % 4 * 70,130 + i / 4 * 70);

        //监听每个按钮的点击事件
        connect(menuBtn,&MyPushButton::clicked,[=](){
            //播放选择关卡音效
            chooseSound->play();

            // QString str = QString("您选择的是第%1关").arg(i+1);
            // qDebug() << str;

            //进入到游戏场景
            hide();//选关场景隐藏
            play = new PlayScene(i+1);
            //设置游戏场景的初始位置
            play->setGeometry(this->geometry());

            play->show();

            connect(play,&PlayScene::playSceneBack,[=](){
                this->setGeometry(play->geometry());
                this->show();
                delete play;
                play = nullptr;
            });
        });

        QLabel *label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(),menuBtn->height());
        label->setText(QString::number(i+1));
        label->move(25 +i % 4 * 70,130 + i / 4 * 70);

        //设置label上的文字对齐方式 水平居中垂直居中
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        //设置让鼠标进行穿透
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

}

//重写paintEvent事件，画背景图
void ChooseLevelScene::paintEvent(QPaintEvent *){
    //加载标题
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/resource/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载背景
    pix.load(":/resource/Title.png");
    painter.drawPixmap(10,30,pix);
}
