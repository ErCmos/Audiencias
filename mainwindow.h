#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMessageBox>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

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
    void on_CameraBtn_clicked();

    void on_FileButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
