#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    port = new QSerialPort();

    on_pushButton_scan_clicked();       //프로그램 시작시, 사용가능한 포트 확인용.

    ui->pushButton_disconnect->setDisabled(1);
    ui->pushButton_apply->setDisabled(1);

    //도트매트릭스 상태표시 라인에디터 사용 불가. 및 초기값 설정.
    ui->lineEdit_dir->setDisabled(1);
    ui->lineEdit_text->setDisabled(1);

    ui->lineEdit_dir->setText("RIGHT");
    ui->lineEdit_text->setText("HELLO");

    ui->groupBox_setting->setDisabled(1);

    //UART read 시그널, 라디오버튼 시그널 처리용 connect.
    QObject::connect(port, SIGNAL(readyRead()), this, SLOT(text_Reading()));
    connect(ui->radioButton_left, SIGNAL(clicked()), this, SLOT(radio_button()));
    connect(ui->radioButton_right, SIGNAL(clicked()), this, SLOT(radio_button()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//UART read 동작
void MainWindow::text_Reading(){
    QByteArray read_data;

    read_data = port->readAll();
    read_text += QString(read_data);
    qDebug() << read_text;
    ui->textEdit->clear();

    if(strchr(read_data.data(), '\n')){
        ui->textEdit->append(read_text);
        ui->textEdit->show();
        read_text = "";
    }
}

void MainWindow::radio_button(){
    //라디오 버튼 선택 시, 방향정보를 전달.
    if(ui->radioButton_left->isChecked()){
        direction = "LEFT\n";
        port->write(direction.toUtf8());
        qDebug() << "\nradio Button checked : left\n";
    }
    if(ui->radioButton_right->isChecked()){
        direction = "RIGHT\n";
        port->write(direction.toUtf8());
        qDebug() << "\nradio Button checked : right\n";
    }
    //라디오 버튼 선택 후, 텍스트 전송 전까지 다시 선택 불가능.
    ui->radioButton_left->setDisabled(1);
    ui->radioButton_right->setDisabled(1);
    //라디오 버튼 선택 후, 텍스트 전송 가능.
    ui->pushButton_apply->setDisabled(0);
}

void MainWindow::on_pushButton_scan_clicked()
{
    ui->comboBox_port->clear();
    foreach(const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()){
        ui->comboBox_port->addItem(serial_port_info.portName());
    }
}

void MainWindow::on_pushButton_connect_clicked()
{
    port->setPortName(ui->comboBox_port->currentText());
    port->setBaudRate(QSerialPort::Baud115200);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);

    if(!port->open(QIODevice::ReadWrite)){
        qDebug() << "\n Serial Port Open Error : " << ui->comboBox_port->currentText() << endl;
        exit(1);
    }
    else{       //시리얼 포트 연결 성공.
        qDebug() << "\n Serial Port Open Success! : " << ui->comboBox_port->currentText() << endl;
        ui->pushButton_connect->setDisabled(1);
        ui->pushButton_disconnect->setDisabled(0);
        ui->groupBox_setting->setDisabled(0);
    }
}

void MainWindow::on_pushButton_disconnect_clicked()
{
    port->close();
    qDebug() << "\n Serial Port Close Success!" << endl;
    ui->pushButton_connect->setDisabled(0);
    ui->pushButton_disconnect->setDisabled(1);
    ui->groupBox_setting->setDisabled(1);
}

void MainWindow::on_pushButton_apply_clicked()
{
    QByteArray send_data;

    send_text = ui->lineEdit_text_2->text();
    send_data = send_text.toUtf8().left(30);
    send_data.append('\n');

    qDebug() << " send text : " << send_data << endl;
    port->write(send_data.data());

    ui->radioButton_left->setDisabled(0);
    ui->radioButton_right->setDisabled(0);
    ui->pushButton_apply->setDisabled(1);

    //추가 :: 라디오버튼 초기화 코드
    ui->lineEdit_text_2->setText("");

    ui->lineEdit_dir->setText(direction);
    ui->lineEdit_text->setText(send_text);
}
