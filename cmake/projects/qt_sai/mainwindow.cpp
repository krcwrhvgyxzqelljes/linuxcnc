#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "interpreter_intf.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parse(){

    std::string filename = "gcode.ngc";
    std::ofstream outfile;
    outfile.open(filename, std::ios::out);
    outfile << ui->textEdit->toPlainText().toStdString();
    outfile.close();
    interpreter_intf("gcode.ngc");
}

void MainWindow::on_pushButton_pressed()
{
    parse();
}

