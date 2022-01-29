#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "generichwchanmanager.h"

extern ChannelsBackupParam Chan[_MAX_LOAD_CHAN_];
extern STRUCTSTATUSCMD StatusCmd;
GenericHwChanManager  *weightChan;
extern ChannelsBackupParam Chan[_MAX_LOAD_CHAN_];


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    weightChan = new GenericHwChanManager();
            // <dword, _WEIGHT_BUFFER_LENGTH_> (_MAX_LOAD_CHAN_, _HW_CHAN_VV_TYPE_, _24_BITS_RESOLUTION_, 10);

    ui->m_cmb_CalibState->addItem("STATO_IDLE");
    ui->m_cmb_CalibState->addItem("STATO_CALIBRAZIONE_FABBRICA");

    eePromCalibFactoryFiles[0].setFileName("EEPROM_CHAN_1_FACTORY.txt");
    eePromCalibFactoryFiles[1].setFileName("EEPROM_CHAN_2_FACTORY.txt");
    eePromCalibIdleFiles[0].setFileName("EEPROM_CHAN_1_IDLE.txt");
    eePromCalibIdleFiles[1].setFileName("EEPROM_CHAN_2_IDLE.txt");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (siframixSimul.readLoadInLine15() == false)
    {
        qDebug() << "readLoadInLine15() Fails";
    } else {
        qDebug() << "readLoadInLine15() Succeeded";
        if(StatusCmd.status == STATO_CALIBRAZIONE_FABBRICA) {
            // Save in the relevant file for CALIBRAZIONE_FABBRICA
            // Saving the following on file
            /* _ADC1_,
             * Chan[_ADC1_].WeightFactoryGain,
             * Chan[_ADC1_].AdcTo2Kg,
             * Chan[_ADC1_].AdcTo2Kg_dx
             * */
            if ( eePromCalibFactoryFiles[0].open(QIODevice::ReadWrite) )
            {
                QTextStream stream( &eePromCalibFactoryFiles[0] );
                stream << "something" << '\n';
            }
        } else {
            // Save in the relevant file for IDLE
        }
    }

}


void MainWindow::on_m_btn_SaveWeightSample_clicked()
{
    siframixSimul.m_WeightSample[0] = ui->m_txt_WeightSample1->text().toULong();
    siframixSimul.m_WeightSample[1] = ui->m_txt_WeightSample2->text().toULong();
}


void MainWindow::on_m_btn_SaveOffsetChannel_clicked()
{
    QString temp = ui->m_txt_OffsetChannel1->text();
    bool bTemp = false;
    long tempLomg = temp.toLong(&bTemp,10);
    weightChan->setOffset(0, ui->m_txt_OffsetChannel1->text().toLong());
                          // long _offset)
    weightChan->setOffset(1, ui->m_txt_OffsetChannel2->text().toLong());
}

void MainWindow::on_m_cmb_CalibState_currentTextChanged(const QString &arg1)
{
    if (arg1 == "STATO_CALIBRAZIONE_FABBRICA"){
        StatusCmd.status = STATO_CALIBRAZIONE_FABBRICA;
    } else {
        StatusCmd.status = STATO_IDLE;
    }
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == 0){
        Chan[0].AreCalibrate = false;
    } else {
        Chan[0].AreCalibrate = true;
    }
}


void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    if (arg1 == 0){
        Chan[1].AreCalibrate = false;
    } else {
        Chan[1].AreCalibrate = true;
    }
}

