#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressBar>
#include <QCoreApplication>
#include <QThread>

#include <SDL.h>
#include <vector>
#include <iostream>

#include "Converter.h"
#include "Picture.h"
#include "FileWriter.h"
#include "Compressor.h"

#include "decoder.h"
#include "reader.h"
#undef SDL_main


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowTitle("DTConverter"); //ustawienie tytułu głównego okna
    ui->setupUi(this);

    ui->bdFile->setReadOnly(true);
    ui->bdPath->setReadOnly(true);

    ui->dbFile->setReadOnly(true);
    ui->dbPath->setReadOnly(true);

}

QString MainWindow::getFilePath(int mode) {

    QString selectedFilePath = QFileDialog::getOpenFileName(this,
        tr("Otwórz obraz"), "", tr((mode == 0) ? "Image Files (*.bmp)" : "Image Files (*.dt)" ));

    int index = selectedFilePath.lastIndexOf('/')+1;
    QString fileName = selectedFilePath.mid(index, selectedFilePath.size()- index);

    if(mode == 0) {
        ui->bdFile->setMaxLength(this->qLineMaxLength);
        ui->bdFile->setText(selectedFilePath);
        fileName.replace(".bmp", "");
        ui->bdFileName->setText(fileName);

    } else {
        ui->dbFile->setMaxLength(this->qLineMaxLength);
        ui->dbFile->setText(selectedFilePath);
        fileName.replace(".dt", "");
        ui->dbFileName->setText(fileName);
    }

    return selectedFilePath;
}

QString MainWindow::getFolderPath(int mode) {
    QString selectedFolderPath =  QFileDialog::getExistingDirectory(this, tr("Wskaż folder"),
         "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(mode == 0) {
        ui->bdPath->setMaxLength(this->qLineMaxLength);
        ui->bdPath->setText(selectedFolderPath);
    } else {
        ui->dbPath->setMaxLength(this->qLineMaxLength);
        ui->dbPath->setText(selectedFolderPath);
    }
    return selectedFolderPath ;
}

void MainWindow::on_bdFile_selectionChanged()
{
    if((ui->bdFile->text()).isEmpty()) {
        QString filePath = this->getFilePath(0);
    }
}

void MainWindow::on_bdFileButton_released()
{
    QString filePath = this->getFilePath(0);
}

void MainWindow::on_bdPath_selectionChanged()
{
    if((ui->bdPath->text()).isEmpty()) {
        QString folderPath = this->getFolderPath(0);
    }
}

void MainWindow::on_bdPathButton_released()
{
    QString folderPath = this->getFolderPath(0);
}

void MainWindow::on_bdConvertButton_released()
{
    string openPath = ui->bdFile->text().toStdString();
    string savePath = ui->bdPath->text().toStdString();
    string saveName = ui->bdFileName->text().toStdString();

    if(!openPath.empty() && !savePath.empty() && !saveName.empty()) {


        int colorType;
        if(ui->greyScale->checkState()) {
            colorType = 1;
        } else {
            colorType = 0;
        }

        QThread* thread = new QThread();
        Converter* converter = new Converter(openPath, savePath, saveName, colorType);
        converter->moveToThread(thread);

        connect(converter, SIGNAL(conversionStart()), this, SLOT(conversionStartHandle()));
        connect(converter, SIGNAL(conversionEnd()), this, SLOT(conversionEndHandle()));
        connect(converter, SIGNAL(conversionSuccess()), this, SLOT(conversionSuccessHandle()));
        connect(converter, SIGNAL(conversionFailed()), this, SLOT(conversionFailedHandle()));
        connect(converter, SIGNAL(conversionProgress(int)), this, SLOT(conversionProgressHandle(int)));
        connect(converter, SIGNAL(error(QString)), this, SLOT(errorStringHandle(QString)));
        connect(thread, SIGNAL(started()), converter, SLOT(run()));
        connect(converter, SIGNAL(finished()), thread, SLOT(quit()));
        connect(converter, SIGNAL(finished()), converter, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();\

    } else {
        QMessageBox::information( this, "Błąd", "Wypełnij wszystkie pola", QMessageBox::Ok, 0 );
    }
}

void MainWindow::errorStringHandle(QString error) {
    qDebug()<<error;
}

void MainWindow::conversionStartHandle() {
    this->progress = new QProgressDialog("Trwa konwersja", "", 0,0, NULL);
    this->progress->setWindowModality(Qt::WindowModal);
    this->progress->setWindowTitle("Konwersja");
    this->progress->setCancelButton(0);
    this->progress->setFixedSize(250, 100);
    this->progress->setRange(0,100);
    this->progress->show();
}

void MainWindow::conversionEndHandle() {
    this->progress->close();
}

void MainWindow::conversionProgressHandle(int value) {
    this->progress->setValue(value);
}

void MainWindow::conversionSuccessHandle() {
    QMessageBox::information( this, "Koniec", "Plik został przekonwertowany poprawnie!", QMessageBox::Ok, 0 );
}

void MainWindow::conversionFailedHandle() {
    QMessageBox::critical( this, "Błąd", "Konwersja nie powiodła się. Spróbuj ponownie.", QMessageBox::Ok, 0 );
}

void MainWindow::on_dbFile_selectionChanged()
{
    if((ui->dbFile->text()).isEmpty()) {
        QString filePath = this->getFilePath(1);
    }
}

void MainWindow::on_dbPath_selectionChanged()
{
    if((ui->dbPath->text()).isEmpty()) {
        QString folderPath = this->getFolderPath(1);
    }
}

void MainWindow::on_dbFileButton_released()
{
     QString filePath = this->getFilePath(1);

}

void MainWindow::on_dbPathButton_released()
{
     QString folderPath = this->getFolderPath(1);

}

void MainWindow::on_dbConvertButton_released()
{
    if(!(ui->dbFile->text()).isEmpty() && !(ui->dbPath->text()).isEmpty() && !(ui->dbFileName->text().isEmpty()) ) {

        Reader reader;
        string openFile = ui->dbFile->text().toStdString();
        string savePath = ui->dbPath->text().toStdString();
        string saveName = ui->dbFileName->text().toStdString();
        string save_name = savePath +"/"+saveName;
        reader.open(openFile,save_name);
        QMessageBox::information( this, "Koniec", "Plik został zapisany poprawnie!", QMessageBox::Ok, 0 );
        SDL_Quit();
    }
        else
        QMessageBox::information( this, "Błąd", "Wypełnij wszystkie pola", QMessageBox::Ok, 0 );


}

MainWindow::~MainWindow()
{
    delete ui;
}
