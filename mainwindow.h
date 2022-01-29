#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
