#include "dialog1.h"
#include "ui_dialog1.h"
#include "qcustomplot.h"
#include "freq2pitch.h"
#include <string>
#include <QtGui>


#define Plot_DotColor QColor(5,189,251)
#define Plot_LineColor QColor(41,138,220)
#define Plot_LineColor2 QColor(220,138,41)
#define Plot_BGColor QColor(41,138,220,80)

#define Plot_Count 200
#define Plot_Data_Count 180
#define Plot_MaxY 800

#define TextWidth 1
#define LineWidth 2
#define DotWidth 10
#define TextColor QColor(255,255,255)
#define Plot_NoColor QColor(0,0,0,0)

int major=1;
Dialog1::Dialog1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog1)
{
    ui->setupUi(this);


    timerthread = new TimerThread();
    songthread = new SongThread();

    songthread->timethread = timerthread;

    recent_pitch=0;

    this->initPlot();

    this->loadPlot();

    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);

    //connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    connect(timerthread,SIGNAL(TimerSignal(double)),this,SLOT(timerUpdate(double)));
    connect(songthread,SIGNAL(SongSignal(double)),this,SLOT(songUpdate(double)));
    timerthread->start();

}

Dialog1::~Dialog1()
{
    delete ui;

}

void Dialog1::on_pushButton_3_clicked()
{
    this->close();
}

void Dialog1::initPlot()
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

    plot->graph(0)->setLineStyle((QCPGraph::LineStyle)QCPGraph::lsLine);
//    plot->graph(0)->setLineStyle((QCPGraph::LineStyle)QCPGraph::lsNone);
//    plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
 //   plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 6));

    //设置静态曲线的横坐标范围及自适应横坐标
    plot->xAxis->setRange(0, Plot_Count, Qt::AlignLeft);
    for (int i = 0; i < Plot_Data_Count; i++) {
       plot_keys.append(i);
    }
    //add graph 1
    plot->addGraph();
    plot->graph(1)->setName("音调");
    plot->graph(1)->setPen(QPen(Plot_LineColor2, LineWidth));

    plot->graph(1)->setLineStyle((QCPGraph::LineStyle)QCPGraph::lsLine);
//    plot->graph(1)->setLineStyle((QCPGraph::LineStyle)QCPGraph::lsNone);
//    plot->graph(1)->setScatterStyle(QCPScatterStyle(QC  PScatterStyle::ssNone));
    plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 6));

    for (int i = 0; i < Plot_Data_Count; i++) {
       plot2_keys.append(i);
    }
}

void Dialog1::loadPlot()
{
    QCustomPlot* plot=ui->customPlot;
    plot_values.clear();
    for (int i = 0; i < Plot_Data_Count; i++) {

//        plot_values.append(this->recent_pitch);
        plot_values.append(0);
    }
    plot->graph(0)->setData(plot_keys, plot_values);
    //plot->graph(0)->rescaleAxes();


//    QCustomPlot* plot2=ui->customPlot_2;
    plot2_values.clear();
    for (int i = 0; i < Plot_Data_Count; i++) {

        plot2_values.append(0);
//        plot2_values.append(this->recent_pitch);
    }
//    plot2->graph(0)->setData(plot2_keys, plot2_values);
    plot->graph(1)->setData(plot_keys,plot_values);

    plot->replot();
    //plot->graph(0)->rescaleAxes();
//    plot2->replot();
}

void Dialog1::timerUpdate(double a)
{
   //  QCustomPlot *plot = ui->customPlot;
  //用freq2pitch来进行频率至音高的转化
//    int pitch=freq2pitch(a,major);
    this->recent_pitch=a;

}
void Dialog1::songUpdate(double a)
{
    //QCustomPlot *plot = ui->customPlot_2;
    //int pitch=freq2pitch(a,major);
    QCustomPlot *plot = ui->customPlot;

if(count_150 < Plot_Data_Count){

plot_values.replace(count_150,this->recent_pitch);
plot2_values.replace(count_150,a);
count_150++;

}else{
    plot_values.append((double)recent_pitch);
//    plot_values.append((double)recent_pitch);
    plot_values.pop_front();
//    plot_values.pop_front();

//    plot2_values.append((double)a);
    plot2_values.append((double)a);
//    plot2_values.pop_front();
    plot2_values.pop_front();

   // plot->graph(0)->setData(plot2_keys, plot2_values);

}
   if(timerthread->flag == true){//改变下方按钮的文字显示
     ui->pushButton_2->setText("清空");
    }else{
     ui->pushButton_2->setText("停止");
   }
    //plot->graph(0)->rescaleAxes();
    ui->customPlot->graph(1)->setData(plot2_keys, plot2_values);
    ui->customPlot->graph(0)->setData(plot_keys, plot_values);
    ui->customPlot->replot();
    plot->replot();
//    this->update();
}


void Dialog1::on_pushButton_clicked()
{



    timerthread->filepathd=pathFile;
    timerthread->pathFlag = 1;
 //   timerthread->flag=false;
    /* 之前的实时监测会后台先开始，然后在乐曲读取完后再显示实时的波形，这样子不利于滤波器进行迭代的。
     * 现在将实时监测的触发信号放在了songthread.cpp里，保证了图形的显示和滤波器的迭代能基本同时开始。
     * 确保了滤波器的工作正常。
     */

    songthread->filePath=pathFile;
    songthread->timeThreadUsed = true;//因为离线打分功能调用的也是songthread，所以增加一个标记位，以防在离线打分的时候打开了实时录音。
    songthread->start();

}

void Dialog1::on_pushButton_2_clicked()
{
      if(timerthread->flag == false){
          timerthread->flag=true;
          songthread->timeThreadUsed = false;
      }else{
//          plot_values.clear();
//          plot2_values.clear();
//          for (int i = 0; i < Plot_Data_Count; i++) {
//              plot_values.append(0);
//              plot2_values.append(0);
//          }

          count_150 = 0;
          this->loadPlot();


      }

}

void Dialog1::on_pushButton_4_clicked()
{

    pathFile =QFileDialog::getOpenFileName(this, tr("Open File"), "G:/pitchDetector_for_XCOM/music", tr("WAV音频文件(*.wav)"));
    if(!pathFile.isEmpty()) {

           ui->filePathLabel->setAlignment(Qt::AlignTop);
           ui->filePathLabel->setText(pathFile);
           ui->pushButton->setVisible(true);
           ui->pushButton_2->setVisible(true);

           QFile file(pathFile);
           if (!file.open(QIODevice::ReadOnly)) {
               QMessageBox::warning(this, tr("Read File"), tr("无法打开文件:\n%1").arg(pathFile));
               return;
           }
       } else {
           QMessageBox::warning(this, tr("文件路径"), tr("您未选择任何文件"));
       }


}
