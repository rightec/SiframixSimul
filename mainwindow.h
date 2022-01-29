#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include "sifra_manager.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    SIFRA_Manager siframixSimul;

private slots:
    void on_pushButton_clicked();

    void on_m_btn_SaveWeightSample_clicked();

    void on_m_btn_SaveOffsetChannel_clicked();

    void on_m_cmb_CalibState_currentTextChanged(const QString &arg1);

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QFile eePromCalibFactoryFiles[2];
    QFile eePromCalibIdleFiles[2];
};

#endif // MAINWINDOW_H
