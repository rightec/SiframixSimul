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

    m_eePromCalibFactoryFiles[0] = new QFile("EEPROM_CHAN_1_FACTORY.txt");
    m_eePromCalibFactoryFiles[1] = new QFile("EEPROM_CHAN_2_FACTORY.txt");
    m_eePromCalibIdleFiles[0] = new QFile("EEPROM_CHAN_1_IDLE.txt");
    m_eePromCalibIdleFiles[1] = new QFile("EEPROM_CHAN_2_IDLE.txt");

    setYellow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    bool b_Error = false;
    setYellow();
    if (siframixSimul.readLoadInLine15() == false)
    {
        qDebug() << "readLoadInLine15() Fails";
        b_Error = true;
    } else {
        qDebug() << "readLoadInLine15() Succeeded";
        if(StatusCmd.status == STATO_CALIBRAZIONE_FABBRICA) {
            // Save in the relevant file for CALIBRAZIONE_FABBRICA
            if ( m_eePromCalibFactoryFiles[0]->open(QIODevice::ReadWrite) )
            {
                m_eePromCalibFactoryFiles[0]->reset();
                QTextStream stream( m_eePromCalibFactoryFiles[0] );
                QString s_Log;
                QString s_Adc = QString::number(_ADC1_);
                QString s_WeightFactoryGain_ADC1 = QString::number(Chan[_ADC1_].WeightFactoryGain);
                QString s_AdcTo2Kg_ADC1 = QString::number(Chan[_ADC1_].AdcTo2Kg);
                QString s_AdcTo2Kg_dx_ADC1 = QString::number(Chan[_ADC1_].AdcTo2Kg_dx);
                stream << s_Adc << "; " <<
                          s_WeightFactoryGain_ADC1 << "; " <<
                          s_AdcTo2Kg_ADC1 << "; " <<
                          s_AdcTo2Kg_dx_ADC1 << "; " <<'\n';
                s_Log = s_Adc + "; " + s_WeightFactoryGain_ADC1 + "; " + s_AdcTo2Kg_ADC1 + "; " + s_AdcTo2Kg_dx_ADC1 + '\n';
                ui->m_Txt_Log->appendPlainText(s_Log);

                m_eePromCalibFactoryFiles[0]->close();

            } else {
                qDebug() << "m_eePromCalibFactoryFiles[0] Error File";
                b_Error = true;
            }

            if ( m_eePromCalibFactoryFiles[1]->open(QIODevice::ReadWrite) )
            {
                m_eePromCalibFactoryFiles[1]->reset();
                QString s_Log;
                QTextStream stream( m_eePromCalibFactoryFiles[1] );
                QString s_Adc = QString::number(_ADC2_);
                QString s_WeightFactoryGain_ADC2 = QString::number(Chan[_ADC2_].WeightFactoryGain);
                QString s_AdcTo2Kg_ADC2 = QString::number(Chan[_ADC2_].AdcTo2Kg);
                QString s_AdcTo2Kg_dx_ADC2 = QString::number(Chan[_ADC2_].AdcTo2Kg_dx);
                stream << s_Adc << "; " <<
                          s_WeightFactoryGain_ADC2 << "; " <<
                          s_AdcTo2Kg_ADC2 << "; " <<
                          s_AdcTo2Kg_dx_ADC2 << "; " <<'\n';
                s_Log = s_Adc + "; " + s_WeightFactoryGain_ADC2 + "; " + s_AdcTo2Kg_ADC2 + "; " + s_AdcTo2Kg_dx_ADC2 + '\n';
                ui->m_Txt_Log->appendPlainText(s_Log);

                m_eePromCalibFactoryFiles[1]->close();

            } else {
                qDebug() << "m_eePromCalibFactoryFiles[1] Error File";
                b_Error = true;
            }
        } else {
            // Save in the relevant file for IDLE
            if ( m_eePromCalibIdleFiles[0]->open(QIODevice::ReadWrite) )
            {
                /*_ADC1_, Chan[_ADC1_].Weightgain
                 * */
                m_eePromCalibIdleFiles[0]->reset();
                QString s_Log;
                QTextStream stream( m_eePromCalibIdleFiles[0] );
                QString s_Adc = QString::number(_ADC1_);
                QString s_WeightGain_ADC1 = QString::number(Chan[_ADC1_].Weightgain);
                stream << s_Adc << "; " <<
                          s_WeightGain_ADC1 << "; " << '\n';

                s_Log = s_Adc + "; " + s_WeightGain_ADC1  + '\n';
                ui->m_Txt_Log->appendPlainText(s_Log);

                m_eePromCalibIdleFiles[0]->close();

            } else {
                qDebug() << "m_eePromCalibIdleFiles[0] Error File";
                b_Error = true;
            }

            if ( m_eePromCalibIdleFiles[1]->open(QIODevice::ReadWrite) )
            {
                m_eePromCalibIdleFiles[1]->reset();
                QString s_Log;
                QTextStream stream( m_eePromCalibIdleFiles[1] );
                QString s_Adc = QString::number(_ADC2_);
                QString s_WeightGain_ADC2 = QString::number(Chan[_ADC2_].Weightgain);
                stream << s_Adc << "; " <<
                          s_WeightGain_ADC2 << "; " << '\n';
                s_Log = s_Adc + "; " + s_WeightGain_ADC2  + '\n';
                ui->m_Txt_Log->appendPlainText(s_Log);

                m_eePromCalibIdleFiles[1]->close();

            } else {
                qDebug() << "m_eePromCalibIdleFiles[1] Error File";
                b_Error = true;
            }
        }
    }

    if (b_Error == true){
        setRed();
    } else {
        setGreen();
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

void MainWindow::on_pushButton_2_clicked()
{
    ui->m_Txt_Log->clear();
    setYellow();
}

void MainWindow::setYellow()
{
    ui->m_lbl_green->setStyleSheet("Background-color: white;");
    ui->m_lbl_yellow->setStyleSheet("Background-color: yellow;");
    ui->m_lbl_red->setStyleSheet("Background-color: white;");
}

void MainWindow::setRed()
{
    ui->m_lbl_green->setStyleSheet("Background-color: white;");
    ui->m_lbl_yellow->setStyleSheet("Background-color: white;");
    ui->m_lbl_red->setStyleSheet("Background-color: red;");
}

void MainWindow::setGreen()
{
    ui->m_lbl_green->setStyleSheet("Background-color: green;");
    ui->m_lbl_yellow->setStyleSheet("Background-color: white;");
    ui->m_lbl_red->setStyleSheet("Background-color: white;");
}
