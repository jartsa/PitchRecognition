#ifndef DIALOG1_H
#define DIALOG1_H

#include <QDialog>
#include "qcustomplot.h"
#include "timerthread.h"
#include "songthread.h"
#include "qstring.h"

namespace Ui {
class Dialog1;
}

class Dialog1 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog1(QWidget *parent = 0);
    ~Dialog1();

private slots:
    void loadPlot();
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void timerUpdate(double a);
    void songUpdate(double a);

    void on_pushButton_4_clicked();

private:
    Ui::Dialog1 *ui;
    QTimer *timer;
    TimerThread *timerthread;
    SongThread *songthread;

    QVector<double> plot_keys;
    QVector<double> plot_values;

    QVector<double> plot2_keys;
    QVector<double> plot2_values;
    double recent_pitch;
    QString pathFile;
    void initPlot();
    int count_150 = 0;
};

#endif // DIALOG1_H
