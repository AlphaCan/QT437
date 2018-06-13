#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QTime>
#include <QTimer>
#include <QLabel>
#include <QtWidgets>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <QCheckBox>
#include <QThread>
#include <QString>
#include <qtconcurrentrun.h>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_LED1_clicked();

    void on_pushButton_LED2_clicked();

    void on_pushButton_LED3_clicked();

    void on_pushButton_LED4_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_restart_clicked();

    void on_pushButton_shutdown_clicked();

    void showtime();

    void on_pushButton_mount_clicked();

    void on_pushButton_umount_clicked();

    int on_pushButton_serial_connct_clicked();

    void on_pushButton_serial_disconnect_clicked();

    void on_pushButton_Enable_clicked();

    void on_pushButton_forward_clicked();

    void on_pushButton_back_clicked();

    void on_pushButton_highspeed_clicked();

    void on_pushButton_lowspeed_clicked();

    void on_checkBox_J1_clicked();

    void on_checkBox_J2_clicked();

    void on_checkBox_J3_clicked();

    void on_checkBox_J4_clicked();

    void on_pushButton_Disable_clicked();

private:
    Ui::MainWindow *ui;



protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    QRectF textRectF(double radius, int pointSize, double angle);


};

typedef struct CONTROL
{
    union f_position
    {
        int forewared_position;
        char tab[4];
    }FRD;
    union b_positon
    {
        int invsersion_position;
        char tab[4];
    }INV;
    union high_speed
    {
        int high_speed;
        char tab[4];
    }hSP;
    union l_speed
    {
        int low_speed;
        char tab[4];
    }LSP;

    bool J1_check,J2_check,J3_check,J4_check;


}ec_control;



#endif // MAINWINDOW_H
