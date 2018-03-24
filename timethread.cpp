#include "timerthread.h"
#include "songthread.h"
#include <stdio.h>
#include <QMainWindow>

//========================================================

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include<math.h>
#include<cmath>
#include<stdlib.h>
#include <bitset>
#include <iomanip>
#include<string>
#pragma comment(lib, "winmm.lib")
#include "Gist.h"
#define freqRate 44100

#define LONG_ARRAY_COUNT 4410
#define SHORT_ARRAY_COUNT 2205


//LMS滤波器参数初始化
double w_n[4410] = {0};
//mu应该取0至1/(4410*0.1)之间的一个数字，这里取这个范围的中间值，具体计算方法见论文《语音降噪--LMS算法》，3.2.1章节
//double mu = 1/(4410*0.1*2);
double mu = 0.000001;

void lmsFilter(double *inputArr, double standardData);

static int hex_char_value(char ss);
static int hex_to_decimal(const char* s);
FILE * coordinatesdata;


using namespace std;
int mark_long = 0;
int mark_short = 0;
int mark_3 = 0;
int mark_2array = 0;
unsigned long filemark = 0;//取文件数据时候的一个标记位


double data_long[LONG_ARRAY_COUNT] = { 0 };
double data_short_0[SHORT_ARRAY_COUNT] = { 0 }, data_short_1[SHORT_ARRAY_COUNT] = { 0 }, data_short_2[SHORT_ARRAY_COUNT] = { 0 };

double data_long_d[LONG_ARRAY_COUNT] = { 0 };
double data_short_0_d[SHORT_ARRAY_COUNT] = { 0 }, data_short_1_d[SHORT_ARRAY_COUNT] = { 0 }, data_short_2_d[SHORT_ARRAY_COUNT] = { 0 };



int frameSize = LONG_ARRAY_COUNT;
int sampleRate = freqRate;
Gist<double> gist(frameSize, sampleRate);

HWAVEIN hWaveIn;  //输入设备
WAVEFORMATEX waveform; //采集音频的格式，结构体
BYTE *pBuffer1;//采集音频时的数据缓存
WAVEHDR wHdr1; //采集音频时包含数据缓存的结构体



TimerThread::TimerThread(QObject *parent) :
    QThread(parent)
{
}

void TimerThread::run()
{

    flag=true;
    HANDLE wait;
    waveform.wFormatTag = WAVE_FORMAT_PCM;//声音格式为PCM
    waveform.nSamplesPerSec = freqRate;//采样率，16000次/秒
    waveform.wBitsPerSample = 16;//采样比特，16bits/次
    waveform.nChannels = 1;//采样声道数，2声道
    waveform.nAvgBytesPerSec = freqRate*2;//每秒的数据率，就是每秒能采集多少字节的数据
    waveform.nBlockAlign = 2;//一个块的大小，采样bit的字节数乘以声道数
    waveform.cbSize = 0;//一般为0

    wait = CreateEvent(NULL, 0, 0, NULL);
    //使用waveInOpen函数开启音频采集
    waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);

    //建立两个数组（这里可以建立多个数组）用来缓冲音频数据
    DWORD bufsize = 1024 * 100;//每次开辟10k的缓存存储录音数据
    //int i = 10000;
    //fopen_s(&pf, "录音测试.pcm", "wb");
    //fopen_s(&coordinatesdata, "C:\\Users\\Junbo\\Desktop\\data.txt", "w");




    fstream fs;
    wav_struct WAV;

    while (1)
    {
        while(flag)sleep(0);

        if(flag == false && pathFlag == 1){

            const wchar_t * pathfile = reinterpret_cast<const wchar_t *>(filepathd.utf16());
            fs.open(pathfile, ios::binary | ios::in);


            fs.seekg(0, ios::end);        //用c++常用方法获得文件大小
            WAV.file_size = fs.tellg();

            fs.seekg(0x16);
            fs.read((char*)&WAV.channel, sizeof(WAV.channel));

            fs.seekg(0x18);
            fs.read((char*)&WAV.frequency, sizeof(WAV.frequency));

            fs.seekg(0x1c);
            fs.read((char*)&WAV.Bps, sizeof(WAV.Bps));

            fs.seekg(0x22);
            fs.read((char*)&WAV.sample_num_bit, sizeof(WAV.sample_num_bit));

            fs.seekg(0x28);
            fs.read((char*)&WAV.data_size, sizeof(WAV.data_size));

            WAV.data = new unsigned char[WAV.data_size];

            fs.seekg(0x2c);
            fs.read((char *)WAV.data, sizeof(char)*WAV.data_size);

            pathFlag = 0;
        }

        pBuffer1 = new BYTE[bufsize];
        wHdr1.lpData = (LPSTR)pBuffer1;
        wHdr1.dwBufferLength = bufsize;
        wHdr1.dwBytesRecorded = 0;
        wHdr1.dwUser = 0;
        wHdr1.dwFlags = 0;
        wHdr1.dwLoops = 1;
        waveInPrepareHeader(hWaveIn, &wHdr1, sizeof(WAVEHDR));//准备一个波形数据块头用于录音
        waveInAddBuffer(hWaveIn, &wHdr1, sizeof(WAVEHDR));//指定波形数据块为录音输入缓存
        waveInStart(hWaveIn);//开始录音
        msleep(50);//等待声音录制0.05s
        waveInReset(hWaveIn);//停止录音
        //printf("%x\n", pBuffer1);
        //fwrite(pBuffer1, 1, wHdr1.dwBytesRecorded, pf);

    int data_size = (int)wHdr1.dwBytesRecorded;


    for (unsigned long i = 0; i < data_size; i = i + 2)
        //for (unsigned long i = 0; i<20; i = i + 2)
    {
        //右边为大端
        unsigned long data_low = pBuffer1[i];
        unsigned long data_high = pBuffer1[i + 1];
        double data_true = data_high * 256 + data_low;
        //printf("%d ",data_true);
        long data_complement = 0;
        //取大端的最高位（符号位）
        int my_sign = (int)(data_high / 128);
        //printf("%d ", my_sign);
        if (my_sign == 1)
        {
            data_complement = data_true - 65536;
        }
        else
        {
            data_complement = data_true;
        }
        //printf("%d ", data_complement);
        setprecision(4);
        double float_data = (double)(data_complement / (double)32768);


        double float_file_data = 0;
        if(filemark < WAV.data_size){
            unsigned long data_low = WAV.data[i];
            unsigned long data_high = WAV.data[i + 1];
            double data_true = data_high * 256 + data_low;
            //printf("%d ",data_true);
            long data_complement = 0;
            //取大端的最高位（符号位）
            int my_sign = (int)(data_high / 128);
            //printf("%d ", my_sign);
            if (my_sign == 1)
            {
                data_complement = data_true - 65536;
            }
            else
            {
                data_complement = data_true;
            }
            //printf("%d ", data_complement);
            setprecision(4);
            float_file_data = (double)(data_complement / (double)32768);

            filemark += 2;
        }

        /*-------------------------------------以下为数据处理部分------------------------------------------------*/
        if (mark_short == SHORT_ARRAY_COUNT)//放满一个数组以后进入
        {
            mark_2array++;
            if (mark_2array>=2)//当有两个数组被新的数据放满后
            {
                if (mark_3 % 3 == 0)
                {
                    for (int j = 0; j < SHORT_ARRAY_COUNT; j++){
                        data_long[j] = data_short_1[j];
                        data_long_d[j] = data_short_1_d[j];
                    }
                    for (int j = 0; j < SHORT_ARRAY_COUNT; j++){
                        data_long[j+SHORT_ARRAY_COUNT] = data_short_0[j];
                        data_long_d[j+SHORT_ARRAY_COUNT] = data_short_0_d[j];
                    }

                 //   lmsFilter(data_long,data_long_d);

                    gist.processAudioFrame(data_long, LONG_ARRAY_COUNT);
                    double pitch = gist.pitch();
                    //printf("%fHz\n", pitch);
                    emit TimerSignal(pitch);
        /*			double timeprint = ((double)i - (double)SHORT_ARRAY_COUNT) / ((double)44100 *(double)2);
                    fprintf_s(coordinatesdata, "%f,%f\n", timeprint, pitch);*/

                }else if(mark_3 % 3 == 1){
                    for (int j = 0; j < SHORT_ARRAY_COUNT; j++){
                        data_long[j] = data_short_2[j];
                        data_long_d[j] = data_short_2_d[j];
                    }
                    for (int j = 0; j < SHORT_ARRAY_COUNT; j++){
                        data_long[j + SHORT_ARRAY_COUNT] = data_short_1[j];
                        data_long_d[j + SHORT_ARRAY_COUNT] = data_short_1_d[j];
                    }

                //    lmsFilter(data_long,data_long_d);

                    gist.processAudioFrame(data_long, LONG_ARRAY_COUNT);
                    double pitch = gist.pitch();
                    //printf("%fHz\n", pitch);
                    emit TimerSignal(pitch);
                    //double timeprint = ((double)i - (double)SHORT_ARRAY_COUNT) / ((double)44100 *(double)2);
                    //fprintf_s(coordinatesdata, "%f,%f\n", timeprint, pitch);

                }
                else if (mark_3 % 3 == 2){
                    for (int j = 0; j < SHORT_ARRAY_COUNT; j++){
                        data_long[j] = data_short_0[j];
                        data_long_d[j] = data_short_0_d[j];
                    }
                    for (int j = 0; j < SHORT_ARRAY_COUNT; j++){
                        data_long[j + SHORT_ARRAY_COUNT] = data_short_2[j];
                        data_long_d[j + SHORT_ARRAY_COUNT] = data_short_2_d[j];
                    }

                  //  lmsFilter(data_long,data_long_d);

                    gist.processAudioFrame(data_long, LONG_ARRAY_COUNT);
                    double pitch = gist.pitch();
                    //printf("%fHz\n", pitch);
                    emit TimerSignal(pitch);
                    //double timeprint = ((double)i - (double)SHORT_ARRAY_COUNT) / ((double)44100 *(double)2);
                    //fprintf_s(coordinatesdata, "%f,%f\n", timeprint, pitch);

                }
                mark_2array--;//为防止音乐过长导致mark_2array整数溢出，每次在减一下使其值维持在2。
            }
            mark_short = 0;
            mark_3++;
            mark_3 = mark_3 % 3;//同样为防止溢出
        }
        if (mark_short < SHORT_ARRAY_COUNT)//为满足未来实时性要求，设置三个数组，将数据循环放入，完成滑窗效果
        {
            if (mark_3%3==0){
                data_short_0[mark_short] = float_data;
                data_short_0_d[mark_short] = float_file_data;
                mark_short++;
            }else if (mark_3%3==1){
                data_short_1[mark_short] = float_data;
                data_short_1_d[mark_short] = float_file_data;
                mark_short++;
            } else if(mark_3%3==2){
                data_short_2[mark_short] = float_data;
                data_short_2_d[mark_short] = float_file_data;
                mark_short++;
            }
        }

    }

    //fclose(coordinatesdata);
    delete pBuffer1;

    }


    //fclose(pf);
    fs.close();
    delete[] WAV.data;
    waveInClose(hWaveIn);
}

//void lmsFilter(double *inputArr, double standardData){

//    double xw_n = 0;

//    for(int m=0;m<4410;m++) xw_n += inputArr[m]*w_n[m];

//    for(int m=0;m<4410;m++) w_n[m] +=  mu * inputArr[m] * (standardData - xw_n[m]);


//}


//int hex_char_value(char c)
//{
//    if (c >= '0' && c <= '9')
//        return c - '0';
//    else if (c >= 'a' && c <= 'f')
//        return (c - 'a' + 10);
//    else if (c >= 'A' && c <= 'F')
//        return (c - 'A' + 10);
//    //assert(0);
//    return 0;
//}
//int hex_to_decimal(char* szHex)
//{
//    int len = 2;
//    int result = 0;
//    for (int i = 0; i < len; i++)
//    {
//        result += (int)pow((float)16, (int)len - i - 1) * hex_char_value(szHex[i]);
//    }
//    return result;
//}


/*
string hex_to_binary(char* szHex)
{
int len = 2;
string result;
for (int i = 0; i < len;i++)
}
*/

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//void TimerThread::run()
//{
//    flag=true;

//    //调取录音信号并触发信号
//    while(true){
//        while(flag)sleep(0);
//        msleep(100);
//        emit TimerSignal(40.0);
//        msleep(100);
//        emit TimerSignal(20.0);
//    }


//}
