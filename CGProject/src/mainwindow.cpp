#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

// R1
void MainWindow::on_horizontalSlider_9_valueChanged(int value)
{
	this->ui->openGLWidget->changeRGBofTF("R1", value);
}

// G1
void MainWindow::on_horizontalSlider_7_valueChanged(int value)
{
	this->ui->openGLWidget->changeRGBofTF("G1", value);
}

// B1
void MainWindow::on_horizontalSlider_8_valueChanged(int value)
{
	this->ui->openGLWidget->changeRGBofTF("B1", value);
}

// R2
void MainWindow::on_horizontalSlider_6_valueChanged(int value)
{
	this->ui->openGLWidget->changeRGBofTF("R2", value);
}

// G2
void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
	this->ui->openGLWidget->changeRGBofTF("G2", value);
}

// B2
void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
	this->ui->openGLWidget->changeRGBofTF("B2", value);
}

// R3
void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
	this->ui->openGLWidget->changeRGBofTF("R3", value);
}

// G3
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
	this->ui->openGLWidget->changeRGBofTF("G3", value);
}

// B3
void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
	this->ui->openGLWidget->changeRGBofTF("B3", value);
}

void MainWindow::on_horizontalSlider_10_valueChanged(int value)
{
	this->ui->openGLWidget->changeMidColorPosTF("MidColorPos", value);
}
