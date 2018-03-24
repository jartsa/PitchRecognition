#ifndef TIMERTHREAD
#define TIMERTHREAD
#include <QThread>
#include <qstring.h>

class TimerThread : public QThread
{
    Q_OBJECT
public:
    explicit TimerThread(QObject *parent = 0);
    bool flag;
    QString filepathd;
    int pathFlag = 0;
    bool stopMark;

protected:
    void run();

signals:
    void TimerSignal(double a);

private:
    int msg;



    struct wav_struct
    {
        unsigned long file_size;        //文件大小
        unsigned short channel;            //通道数
        unsigned long frequency;        //采样频率
        unsigned long Bps;                //Byte率
        unsigned short sample_num_bit;    //一个样本的位数
        unsigned long data_size;        //数据大小
        unsigned char *data;            //音频数据 ,这里要定义什么就看样本位数了，我这里只是单纯的复制数据
    };
};
#endif // TIMERTHREAD

