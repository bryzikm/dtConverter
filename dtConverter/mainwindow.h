#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>

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
    void conversionStartHandle();
    void conversionEndHandle();
    void conversionProgressHandle(int value);
    void conversionSuccessHandle();
    void conversionFailedHandle();
    void errorStringHandle(QString error);

    void on_bdFile_selectionChanged();
    void on_bdPath_selectionChanged();
    void on_bdFileButton_released();
    void on_bdPathButton_released();
    void on_bdConvertButton_released();
    void on_dbFile_selectionChanged();
    void on_dbPath_selectionChanged();
    void on_dbFileButton_released();
    void on_dbPathButton_released();
    void on_dbConvertButton_released();

private:
    Ui::MainWindow *ui;

    QString getFilePath(int mode);
    QString getFolderPath(int mode);
    QProgressDialog* progress = NULL;
    int qLineMaxLength = 500;

};

#endif // MAINWINDOW_H
