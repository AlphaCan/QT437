#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace QtConcurrent;
ec_control J1;
ec_control J2;
ec_control J3;
ec_control J4;
bool foreward,back,hp,lp;
void controlcheck(void);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(timer,SIGNAL(timeout()),this,SLOT(showtime()));
    timer->start(500);
    ui->pushButton_highspeed->setAutoRepeat(true);// continue
    ui->pushButton_forward->setAutoRepeat(true);
    ui->pushButton_back->setAutoRepeat(true);
    ui->pushButton_lowspeed->setAutoRepeat(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_LED1_clicked()
{
    ui->label->setText(">> open LED1");
    system("echo 1 > /sys/class/leds/myc:blue:cpu0/brightness");
}

void MainWindow::on_pushButton_LED2_clicked()
{
    ui->label->setText(">> open LED2");
    system("echo 1 > /sys/class/leds/myd:blue:mmc1/brightness");
}

void MainWindow::on_pushButton_LED3_clicked()
{
    ui->label->setText(">> open LED3");
    system("echo 1 > /sys/class/leds/myd:blue:heartbeat/brightness");
}

void MainWindow::on_pushButton_LED4_clicked()
{
    ui->label->setText(">> open LED4");
    system("echo 1 > /sys/class/leds/myd:blue:usr3/brightness");
}

void MainWindow::on_pushButton_close_clicked()
{
    ui->label->setText(">> close all LED");
    system("echo 0 > /sys/class/leds/myc:blue:cpu0/brightness");
    system("echo 0 > /sys/class/leds/myd:blue:mmc1/brightness");
    system("echo 0 > /sys/class/leds/myd:blue:heartbeat/brightness");
    system("echo 0 > /sys/class/leds/myd:blue:usr3/brightness");
}

void MainWindow::on_pushButton_restart_clicked()
{
    ui->label->setText(">> shut down and restart Immediately");
    system("shutdown -r now");
}

void MainWindow::on_pushButton_shutdown_clicked()
{
    ui->label->setText(">> shut down Immediately");
    system("shutdown -h now");
}

void MainWindow::showtime()
{
    QTime time = QTime::currentTime();
    QString txttime = time.toString("hh:mm:ss");
    ui->lcdNumber->display(txttime);
}

void MainWindow::on_pushButton_mount_clicked()
{
    ui->label->setText(">> Mount the file system");
    system("mount -t nfs -o nolock 10.0.20.55:/ARM/rootnfs /mnt");
}

void MainWindow::on_pushButton_umount_clicked()
{
    ui->label->setText(">> Unmount the file system");
    system("umount /mnt");
}

QRectF MainWindow::textRectF(double radius, int pointSize, double angle)
{
    QRectF rectF;
    rectF.setX(radius*qCos(angle*M_PI/180.0) - pointSize*2);
    rectF.setY(radius*qSin(angle*M_PI/180.0) - pointSize/2.0);
    rectF.setWidth(pointSize*4);
    rectF.setHeight(pointSize);
    return rectF;
}

void MainWindow::paintEvent(QPaintEvent *)

{
    static const QPoint hourHand[3] = {
        QPoint(3, 8),
        QPoint(-3, 8),
        QPoint(0, -40)
    };
    static const QPoint minuteHand[3] = {
        QPoint(3, 8),
        QPoint(-3, 8),
        QPoint(0, -70)
    };
    static const QPoint secondHand[3] = {
        QPoint(3, 8),
        QPoint(-3, 8),
        QPoint(0, -90)
    };

    QColor hourColor(0, 0, 0);
    QColor minuteColor(0, 0, 255,191);
    QColor secondColor(248, 9, 27,200);

//    int side = qMin(width(), height());
    QTime time = QTime::currentTime();

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(240 / 2, 240 / 2);//system by the vector (dx,dy)

    painter.scale(1, 1);//suo fang

    painter.setPen(Qt::NoPen);

    painter.setBrush(hourColor);

    painter.save();

    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();

    painter.setPen(hourColor);

    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }

    int nHour = 0;
        for (int i = 0; i < 12; ++i) {
            nHour = i + 3;
            if (nHour > 12)
                nHour -= 12;
            painter.drawText(textRectF(100*0.8, 80, i * 30), Qt::AlignCenter, QString::number(nHour));
        }

    painter.setPen(Qt::NoPen);

    painter.setBrush(minuteColor);

    painter.save();
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();

    painter.setPen(minuteColor);

    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }

    painter.setPen(Qt::NoPen);

    painter.setBrush(secondColor);

    painter.save();
    painter.rotate(6.0*time.second());
    painter.drawConvexPolygon(secondHand, 3);
    painter.restore();

    painter.setPen(minuteColor);

    for (int j = 0; j < 360; ++j) {
        if ((j % 5) != 0)
           painter.drawLine(95, 0, 96, 0);
           painter.rotate(6.0);
     }

}
uint8_t* q;
uint8_t* p = NULL;

int shmid;
uint8_t iomap[256],first[256];
pthread_mutex_t create_mutex;

void createsharedmemory(void)
{
    pthread_mutex_lock(&create_mutex);
    q = (uint8_t*)shmat(shmid,NULL,0);
    if(q == (void *)-1)
    {
        qDebug("shmat err");
    }
    memcpy(first,q,256);
    shmdt(q);
    pthread_mutex_unlock(&create_mutex);
    qDebug("%d %d\n",first[120],first[124]);



    while(1)
    {
        pthread_mutex_lock(&create_mutex);

        p = (uint8_t*)shmat(shmid,NULL,0);
        if(p == (void *)-1)
        {
            qDebug("shmat2 err");
           // return errno;
            break;
        }

        memcpy(p,iomap,256);

        shmdt(p);
        pthread_mutex_unlock(&create_mutex);
        usleep(6000);
    }

}

int MainWindow::on_pushButton_serial_connct_clicked()
{
//   int ret = 0;
   memset(iomap,0,sizeof(iomap));
    shmid = shmget(0x456789, 256, IPC_CREAT|0666);
    if (shmid ==-1)
    {
        ui->label->setText("shmget err");
        return errno;
    }
    QString s = QString::number(shmid,10);
    ui->label->setText(s);
    QFuture<void> f1 = run(createsharedmemory);
    QFuture<void> f2 = run(controlcheck);

    return 0;
}

void MainWindow::on_pushButton_serial_disconnect_clicked()
{
    int ret = 0;
    ret = shmctl(shmid, IPC_RMID, NULL);
    if (ret < 0)
    {
         qDebug("rmerror\n");
    }

}

void control_J1(void)
{
    uint8_t i;
    if(foreward)
    {
        for(i = 0;i < 4;i++)
            iomap[2 + i] = first[90 + i] + (J1.FRD.tab[i]&0xff);
    }
    if(back)
    {
        for(i = 0 ;i < 4 ;i++)
            iomap[2 + i] = first[90 + i] - (J1.INV.tab[i]&0xff);
    }
    if(hp)
    {
        for(i = 0 ;i < 4 ;i++)
            iomap[10 + i] = first[94 + i] + (J1.hSP.tab[i]&0xff);
    }
    if(lp)
    {
        for(i = 0 ;i < 4 ;i++)
            iomap[10 + i] = first[94 + i] - (J1.LSP.tab[i]&0xff);
    }

    qDebug("J1:%d %d %d %d\n",iomap[2],iomap[3],iomap[10],iomap[11]);
}

void control_J2(void)
{
    uint8_t i;
    if(foreward)
    {
        for(i = 0;i < 4;i++)
            iomap[24 + i] = first[100 + i] + (J2.FRD.tab[i]&0xff);
    }
    if(back)
    {
        for(i = 0 ;i < 4 ;i++)
            iomap[24 + i] = first[100 + i] + (J2.INV.tab[i]&0xff);
    }
    if(hp)
    {
        for(i = 0 ;i < 4 ;i++)
            iomap[32 + i] = first[104 + i] + (J2.hSP.tab[i]&0xff);
    }
    if(lp)
    {
        for(i = 0 ;i < 4 ;i++)
            iomap[32 + i] = first[104 + i] + (J2.LSP.tab[i]&0xff);
    }
    qDebug("J2:%d %d %d %d\n",iomap[24],iomap[25],iomap[32],iomap[33]);
}

void control_J3(void)
{
    uint8_t i;
    if(foreward)
    {
        for(i = 0;i < 4;i++)
            iomap[46 + i] = first[110 + i] + (J3.FRD.tab[i]&0xff);
    }

    if(back)
    {
        for(i = 0 ;i < 4 ;i++)
            iomap[46 + i] = first[110 + i] + (J3.INV.tab[i]&0xff);
    }
    if(hp)
    {
        for(i = 0 ;i < 4 ;i++)
            iomap[54 + i] = first[114 + i] + (J3.hSP.tab[i]&0xff);
    }
    if(lp)
    {
        for(i = 0 ;i < 4 ;i++)
            iomap[54 + i] = first[114 + i] + (J3.LSP.tab[i]&0xff);
    }
    qDebug("J3:%d %d %d %d\n",iomap[46],iomap[47],iomap[54],iomap[55]);

}

void control_J4(void)
{
    uint8_t i;
    if(foreward)
    {
        for(i = 0;i < 4;i++)
            iomap[68 + i] = first[120 + i] + (J4.FRD.tab[i]&0xff);
    }
    if(back)
    {
        for(i = 0 ;i < 4 ;i++)
            iomap[68 + i] = first[120 + i] + (J4.INV.tab[i]&0xff);
    }
    if(hp)
    {
        for(i = 0 ;i < 4 ;i++)
            iomap[76 + i] = first[124 + i] + (J4.hSP.tab[i]&0xff);
    }
    if(lp)
    {
        for(i = 0 ;i < 4 ;i++)
            iomap[76 + i] = first[124 + i] + (J4.LSP.tab[i]&0xff);
    }
    qDebug("J4:%d %d %d %d\n",iomap[68],iomap[69],iomap[76],iomap[77]);
}


void controlcheck(void)
{
    while(1)
    {
        if(J1.J1_check)
            control_J1();
        if(J2.J2_check)
            control_J2();
        if(J3.J3_check)
            control_J3();
        if(J4.J4_check)
            control_J4();
        usleep(10000);
//        qDebug("123\n");
    }


}



void MainWindow::on_pushButton_Enable_clicked()
{
    int i ;
    for(i = 0 ;i < 0x42 ;i += 0x16)
        iomap[i] = 6;
    usleep(100000);

    for(i = 0 ;i < 0x42 ;i += 0x16)
        iomap[i] = 7;
    usleep(100000);

    for(i = 0 ;i < 0x42 ;i += 0x16)
        iomap[i] = 15;
    usleep(100000);

}

void MainWindow::on_pushButton_forward_clicked()
{
    if(ui->pushButton_forward->isDown())
        foreward = true;
    else
        foreward = false;

    if(ui->checkBox_J1->isChecked())
        J1.FRD.forewared_position++;
    if(ui->checkBox_J2->isChecked())
        J2.FRD.forewared_position++;
    if(ui->checkBox_J3->isChecked())
        J3.FRD.forewared_position++;
    if(ui->checkBox_J4->isChecked())
        J4.FRD.forewared_position++;


}

void MainWindow::on_pushButton_back_clicked()
{
    if(ui->pushButton_back->isDown())
        back =true;
    else
        back =false;

    if(ui->checkBox_J1->isChecked())
        J1.INV.invsersion_position++;
    if(ui->checkBox_J2->isChecked())
        J2.INV.invsersion_position++;
    if(ui->checkBox_J3->isChecked())
        J3.INV.invsersion_position++;
    if(ui->checkBox_J4->isChecked())
        J4.INV.invsersion_position++;

}

void MainWindow::on_pushButton_highspeed_clicked()
{
    if(ui->pushButton_highspeed->isDown())
        hp =true;
    else
        hp = true;

    if(ui->checkBox_J1->isChecked())
        J1.hSP.high_speed++;
    if(ui->checkBox_J2->isChecked())
        J2.hSP.high_speed++;
    if(ui->checkBox_J3->isChecked())
        J3.hSP.high_speed++;
    if(ui->checkBox_J4->isChecked())
        J4.hSP.high_speed++;

}

void MainWindow::on_pushButton_lowspeed_clicked()
{
    if(ui->pushButton_lowspeed->isDown())
        lp = true;
    else
        lp = false;

    if(ui->checkBox_J1->isChecked())
        J1.LSP.low_speed++;
    if(ui->checkBox_J2->isChecked())
        J2.LSP.low_speed++;
    if(ui->checkBox_J3->isChecked())
        J3.LSP.low_speed++;
    if(ui->checkBox_J4->isChecked())
        J4.LSP.low_speed++;

}

void MainWindow::on_checkBox_J1_clicked()
{
    if(ui->checkBox_J1->isChecked())
        J1.J1_check = true;
    else
        J1.J1_check = false;
//    QString s = QString::number(J1.FRD.forewared_position);

}

void MainWindow::on_checkBox_J2_clicked()
{
    if(ui->checkBox_J2->isChecked())
        J2.J2_check = true;
    else
        J2.J2_check = false;

}

void MainWindow::on_checkBox_J3_clicked()
{
    if(ui->checkBox_J3->isChecked())
        J3.J3_check = true;
    else
        J3.J3_check = false;

}

void MainWindow::on_checkBox_J4_clicked()
{
    if(ui->checkBox_J4->isChecked())
        J4.J4_check = true;
    else
        J4.J4_check = false;

}
