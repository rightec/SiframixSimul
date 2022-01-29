#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "generichwchanmanager.h"


extern ChannelsBackupParam Chan[_MAX_LOAD_CHAN_];
GenericHwChanManager  *weightChan;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    weightChan = new GenericHwChanManager();
            // <dword, _WEIGHT_BUFFER_LENGTH_> (_MAX_LOAD_CHAN_, _HW_CHAN_VV_TYPE_, _24_BITS_RESOLUTION_, 10);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    siframixSimul.readLoadInLine15();
}


void MainWindow::on_m_btn_SaveWeightSample_clicked()
{
    siframixSimul.m_WeightSample[0] = ui->m_txt_WeightSample1->text().toULong();
    siframixSimul.m_WeightSample[1] = ui->m_txt_WeightSample2->text().toULong();
}


void MainWindow::on_m_btn_SaveOffsetChannel_clicked()
{
    weightChan->setOffset(0, ui->m_txt_OffsetChannel1->text().toLong());
                          // long _offset)
    weightChan->setOffset(1, ui->m_txt_OffsetChannel2->text().toLong());
}

