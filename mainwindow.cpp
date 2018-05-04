#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "opencv2/opencv.hpp"
#include <QFileDialog>
#include "procesadoficheros.h"

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_CameraBtn_clicked()
{
    QMessageBox msgBox;

    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        msgBox.setText("Could not initialize capturing..");
        //return -1;

    Mat edges;
    //namedWindow("edges",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        //imshow("edges", edges);
        imshow("edges", frame);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    //return 0;
    msgBox.setText("Thats all falks..");
}

void MainWindow::on_FileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Video"), "/home/ercmos/Proyectos/Audiencias/Ficheros_Audiencias/", tr("Audience Files (*.txt)"));
        ui->OpenFileBrowser->setText(fileName);

        ///home/ercmos/Proyectos/Audiencias/Ficheros_Audiencias/Test.txt

//    QString dirName = QFileDialog::getExistingDirectory(this,
//           tr("DoF Test Files (*.txt)"),"/home/ercmos",
//                       QFileDialog::ShowDirsOnly
//                       | QFileDialog::DontResolveSymlinks);

        ProcesadoFicheros fichero;
        fichero.LeeFichero(fileName.toStdString(),"/home/ercmos/Proyectos/Audiencias/Ficheros_Audiencias/");


        //Graphic_File procesa;
        //procesa.Procesa_Fichero_Video(fileName);
        //procesa.Lee_Fichero(fileName);

}

