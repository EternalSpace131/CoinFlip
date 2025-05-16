#include "playscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include "mycoin.h"
#include "dataconfig.h"
#include <QPropertyAnimation>
#include <QSoundEffect>

// PlayScene::PlayScene(QWidget *parent)
//     : QMainWindow{parent}
// {}

PlayScene::PlayScene(int levelNum){
    // QString str = QString("进入了第%1关").arg(levelNum);
    // qDebug() << str;
    this->levelIndex = levelNum;

    //初始化游戏场景
    //设置固定大小
    setFixedSize(320,588);
    //设置图标
    setWindowIcon(QIcon(":/resource/Coin0001.png"));
    //设置标题
    setWindowTitle("翻金币");

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

    //准备返回按钮的音效
    QSoundEffect *backSound = new QSoundEffect(this);
    backSound->setSource(QUrl::fromLocalFile(":/resource/BackButtonSound.wav"));
    //翻金币的音效
    QSoundEffect *flipSound = new QSoundEffect(this);
    flipSound->setSource(QUrl::fromLocalFile(":/resource/CoinFlipSound.wav"));
    //胜利音效
    QSoundEffect *winSound = new QSoundEffect(this);
    winSound->setSource(QUrl::fromLocalFile(":/resource/LevelWinSound.wav"));

    //返回按钮
    MyPushButton *backBtn = new MyPushButton(":/resource/BackButton.png",":/resource/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());

    //点击返回
    connect(backBtn,&QPushButton::clicked,[=](){
        //播放返回按钮音效
        backSound->play();

        //延时返回
        QTimer::singleShot(500,this,[=](){
            emit this->playSceneBack();
        });
    });

    //显示当前的关卡数
    QLabel *label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str = QString("Level:%1").arg(this->levelIndex);
    //将字体设置到标签控件中
    label->setFont(font);
    label->setText(str);
    label->setGeometry(30, this->height() - 50 ,120, 50);

    //初始化每个关卡的二维数组
    dataConfig config;
    for(int i = 0 ; i < 4 ; i ++){
        for(int j = 0 ;j < 4 ; j++){
            this->gameArray[i][j]=config.mData[this->levelIndex][i][j];
        }
    }

    //胜利图片显示
    QLabel* winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/resource/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move( (this->width() - tmpPix.width())*0.5 ,-tmpPix.height());

    //显示金币背景图案
    for(int i = 0 ; i < 4;i++){
        for(int j = 0 ; j < 4; j++){
            //绘制背景图片
            QPixmap pix = QPixmap(":/resource/BoardNode.png");
            QLabel* label = new QLabel;
            label->setGeometry(0,0,pix.width(),pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(57 + i*50,200 + j*50);

            //创建金币
            QString str;
            if(this->gameArray[i][j] == 1){
                str = QString(":/resource/Coin0001.png");
            }else {
                str = QString(":/resource/Coin0008.png");
            }
            MyCoin *coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(59 + i*50,204 + j*50);

            //给金币属性赋值
            coin->pos_x = i;
            coin->pos_y = j;
            coin->flag = this->gameArray[i][j];//1正 0反

            //将金币放到金币的二维数组中以便维护
            coinBtn[i][j] = coin;

            //点击金币 进行翻转
            connect(coin,&MyCoin::clicked,[=](){
                //播放翻金币音效
                flipSound->play();

                for(int p=0;p<4;p++){
                    for(int q=0;q<4;q++){
                        coinBtn[p][q]->isWin = true;
                    }
                }

                coin->changeFlag();
                this->gameArray[i][j] = this->gameArray[i][j] == 0 ? 1 : 0;

                //延时翻转周围硬币
                QTimer::singleShot(300,this,[=](){
                    if(coin->pos_x <= 2){
                        coinBtn[coin->pos_x + 1][coin->pos_y]->changeFlag();
                        this->gameArray[coin->pos_x + 1][coin->pos_y] = this->gameArray[coin->pos_x + 1][coin->pos_y] == 0 ? 1 : 0;
                    }
                    if(coin->pos_x >= 1){
                        coinBtn[coin->pos_x - 1][coin->pos_y]->changeFlag();
                        this->gameArray[coin->pos_x - 1][coin->pos_y] = this->gameArray[coin->pos_x - 1][coin->pos_y] == 0 ? 1 : 0;
                    }
                    if(coin->pos_y <= 2){
                        coinBtn[coin->pos_x][coin->pos_y + 1]->changeFlag();
                        this->gameArray[coin->pos_x][coin->pos_y + 1] = this->gameArray[coin->pos_x][coin->pos_y + 1] == 0 ? 1 : 0;
                    }
                    if(coin->pos_y >= 1){
                        coinBtn[coin->pos_x][coin->pos_y - 1]->changeFlag();
                        this->gameArray[coin->pos_x][coin->pos_y - 1] = this->gameArray[coin->pos_x][coin->pos_y - 1] == 0 ? 1 : 0;
                    }

                    for(int p=0;p<4;p++){
                        for(int q=0;q<4;q++){
                            coinBtn[p][q]->isWin = false;
                        }
                    }

                    //判断是否胜利
                    this->isWin = true;
                    for(int p=0;p<4;p++){
                        for(int q=0;q<4;q++){
                            if(coinBtn[p][q]->flag == false){
                                this->isWin = false;
                                break;
                            }
                        }
                        if(!isWin){
                            break;
                        }
                    }
                    if(isWin){
                        //播放胜利音效
                        winSound->play();

                        // qDebug() << "游戏胜利";
                        for(int p=0;p<4;p++){
                            for(int q=0;q<4;q++){
                                coinBtn[p][q]->isWin = true;
                            }
                        }
                        //将胜利的图片移动下来
                        QPropertyAnimation *animation = new QPropertyAnimation(winLabel,"geometry");
                        //设置事件间隔
                        animation->setDuration(1000);
                        //设置开始位置
                        animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        //设置结束位置
                        animation->setEndValue(QRect(winLabel->x(),winLabel->y() + 114,winLabel->width(),winLabel->height()));
                        //设置缓和曲线
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        //执行动画
                        animation->start();

                    }
                });

            });

        }
    }
}

void PlayScene::paintEvent(QPaintEvent *){
    //加载标题
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/resource/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载背景
    pix.load(":/resource/Title.png");
    pix = pix.scaled(pix.width() * 0.5 ,pix.height() * 0.5);
    painter.drawPixmap(10,30,pix);
};


