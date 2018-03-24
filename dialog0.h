#ifndef DIALOG0_H
#define DIALOG0_H

#include <QDialog>
#include "qcustomplot.h"
#include "timerthread.h"
#include "songthread.h"
#include "qstring.h"

namespace Ui {
class dialog0;
}

class dialog0 : public QDialog
{
    Q_OBJECT

public:
    explicit dialog0(QWidget *parent = 0);
    ~dialog0();

private slots:
    void on_pushButton_clicked();
  //  void loadPlot();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::dialog0 *ui;
    QTimer *timer;

    TimerThread *timerthread;
    SongThread *songthread;

    QVector<double> plot_keys;
    QVector<double> plot_values;

    QVector<double> plot2_keys;
    QVector<double> plot2_values;
    int recent_pitch;
    QString pathFileSample;
    QString pathFileHomework;

    void initPlot();

};

#endif // DIALOG0_H
