#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSerialPort *port;
    QString read_text;
    QString send_text;
    QString direction;

private slots:
    void text_Reading();

    void radio_button();

    void on_pushButton_scan_clicked();

    void on_pushButton_connect_clicked();

    void on_pushButton_disconnect_clicked();

    void on_pushButton_apply_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
