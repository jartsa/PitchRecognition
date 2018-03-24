#include "dialog0.h"
#include "ui_dialog0.h"
#include"qcustomplot.h"
#include "timerthread.h"
#include "songthread.h"
#include "freq2pitch.h"
#include <string>

#define Plot_DotColor QColor(5,189,251)
#define Plot_LineColor QColor(41,138,220)
#define Plot_LineColor2 QColor(220,138,41)
#define Plot_BGColor QColor(41,138,220,80)

#define Plot_Count 100
#define Plot_Half_Count Plot_Count/2
#define Plot_MaxY 36

#define TextWidth 1
#define LineWidth 2
#define DotWidth 10
#define TextColor QColor(255,255,255)
#define Plot_NoColor QColor(0,0,0,0)


dialog0::dialog0(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog0)
{
    ui->setupUi(this);

        this->initPlot();


}

dialog0::~dialog0()
{
    delete ui;
}

void dialog0::initPlot()
{
    QCustomPlot* plot=ui->customPlot;
    plot->yAxis->setLabel("音调");
    plot->yAxis->setRange(0, Plot_MaxY);
    plot->yAxis->setLabelColor(TextColor);
    plot->xAxis->setTickLabelColor(TextColor);
    plot->yAxis->setTickLabelColor(TextColor);
    plot->xAxis->setBasePen(QPen(TextColor, TextWidth));
    plot->yAxis->setBasePen(QPen(TextColor, TextWidth));
    plot->xAxis->setTickPen(QPen(TextColor, TextWidth));
    plot->yAxis->setTickPen(QPen(TextColor, TextWidth));
    plot->xAxis->setSubTickPen(QPen(TextColor, TextWidth));
    plot->yAxis->setSubTickPen(QPen(TextColor, TextWidth));
    //设置画布背景色
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    plot->setBackground(plotGradient);
    //设置坐标背景色
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    plot->axisRect()->setBackground(axisRectGradient);
    //设置图例提示位置及背景色
    plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
    plot->legend->setBrush(QColor(255, 255, 255, 200));
    plot->replot();
    plot->addGraph();
    plot->graph(0)->setName("音调");
    plot->graph(0)->setPen(QPen(Plot_LineColor, LineWidth));
    //设置静态曲线的横坐标范围及自适应横坐标
    plot->xAxis->setRange(0, Plot_Count, Qt::AlignLeft);
    for (int i = 0; i < Plot_Count; i++) {
       plot_keys.append(i);
    }
    //add graph 1
    plot->addGraph();
    plot->graph(1)->setName("音调");
    plot->graph(1)->setPen(QPen(Plot_LineColor2, LineWidth));

    for (int i = 0; i < Plot_Count; i++) {
       plot2_keys.append(i);
    }
}

void dialog0::on_pushButton_clicked()
{
    this->close();
}

void dialog0::on_pushButton_2_clicked()
{
    pathFileSample =QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("WAV音频文件(*.wav)"));
    if(!pathFileSample.isEmpty()) {

           ui->sampleLabel->setAlignment(Qt::AlignTop);
           ui->sampleLabel->setText(pathFileSample);

           QFile file(pathFileSample);
           if (!file.open(QIODevice::ReadOnly)) {
               QMessageBox::warning(this, tr("Read File"), tr("无法打开文件:\n%1").arg(pathFileSample));
               return;
           }
       } else {
           QMessageBox::warning(this, tr("文件路径"), tr("您未选择任何文件"));
       }
}

void dialog0::on_pushButton_3_clicked()
{
    pathFileHomework =QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("WAV音频文件(*.wav)"));
    if(!pathFileHomework.isEmpty()) {

           ui->homeworkLabel->setAlignment(Qt::AlignTop);
           ui->homeworkLabel->setText(pathFileHomework);


           QFile file(pathFileHomework);
           if (!file.open(QIODevice::ReadOnly)) {
               QMessageBox::warning(this, tr("Read File"), tr("无法打开文件:\n%1").arg(pathFileHomework));
               return;
           }
       } else {
           QMessageBox::warning(this, tr("文件路径"), tr("您未选择任何文件"));
       }
}
