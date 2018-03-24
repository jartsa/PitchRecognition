#include "songthread.h"

#include <stdio.h>

#include<vector>
#include <QMainWindow>
#include "wav2pitch.h"
using namespace std;

SongThread::SongThread(QObject *parent) :
    QThread(parent)
{

}

void SongThread::run()
{
  //  flag=true;
    vector<double> *timeAx = new vector<double>();
    vector<double> *pitchData = new vector<double>();
    Wav2Pitch(*timeAx,*pitchData,filePath);
    int num=timeAx->size();
    double timecount=0.0;

    if(timeThreadUsed == true){
        timethread->flag = false;
}
        //调取录音信号并触发信号
    for(int i=1;i<num-1;){
        msleep(50);
        timecount+=0.050; 
        while(timeAx->at(i)<timecount){
            i++;
            emit SongSignal(pitchData->at(i-1));
        }

    }

    if(timethread->flag==false && timeThreadUsed == true){

       timethread->flag=true;
       timeThreadUsed = false;

    }
  quit ();
}

