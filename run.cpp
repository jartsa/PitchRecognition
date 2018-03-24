#include <iostream>
#include <fstream>
#include <string.h>
#include<math.h>
#include<cmath>
#include<stdlib.h>
#include <bitset>
#include <iomanip>
#include<string>
#include "Gist.h"
#include "wav2pitch.h"
//#include "windows.h"
//#include "mmsystem.h"
//#pragma comment(lib,"winmm.lib")


#define LONG_ARRAY_COUNT 4410
#define SHORT_ARRAY_COUNT 2205

//要在int main()的前面加上函数的声明，因为你的函数写在main函数的后面

//string hex_to_binary(char* szHex);
using namespace std;


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


int Wav2Pitch(vector<double> &timeAx, vector<double> &pitchData,QString path)
{
    //为了加快处理速度，我们将文件处理一次后的数据保存在音频文件的根目录下，下次调用的时候直接读取处理好的数据
    //而不是再一次处理音频，从而提高效率
    QFileInfo fileinfo(path);
    QString fileRootPath = fileinfo.path();
    QString fileName = fileinfo.fileName();
    int fileNameLength = fileName.length();
    QString fileNameCutted = fileName.left(fileNameLength-4);
    QString txtFileNameFull = fileRootPath+"/"+fileNameCutted+".txt";//此处获得了数据保存的路径

    QFile dataTxt(txtFileNameFull);

    QByteArray ba = txtFileNameFull.toLatin1();
    const char *txtFileName = ba.data();

    int emitMark = 0;//用作发射

    if(dataTxt.exists()){
        string line;
        ifstream in(txtFileName);

//        播放声音
//        const wchar_t * filePath = reinterpret_cast<const wchar_t *>(path.utf16());
//        PlaySound(filePath,NULL, SND_FILENAME | SND_ASYNC);

        while (getline (in, line)) // line中不包括每行的换行符
        {
            char* ch = const_cast<char*>(line.c_str());
            const char *splitMark = ",";
            char *p;
            char *nextBuf = NULL;
            p = strtok_s(ch, splitMark,&nextBuf);

            while (p)
            {
                double submitData;
                submitData = atof(p);
                if(emitMark == 0){
                    timeAx.push_back(submitData);
                    emitMark = 1;
                }else if(emitMark == 1){
                    pitchData.push_back(submitData);
                    emitMark = 0;
                }

                p = strtok_s(NULL, splitMark,&nextBuf);
            }
        }
        return 0;
    }else{
        fstream fs;
        wav_struct WAV;
//        int mark_2048 = 0;
        int mark_shortArr = 0;
        int mark_3 = 0;
        int mark_2array = 0;
        //int freqOfPitch = 0;

        double data_2048[LONG_ARRAY_COUNT] = { 0 };
        double data_1024_0[SHORT_ARRAY_COUNT] = { 0 }, data_1024_1[SHORT_ARRAY_COUNT] = { 0 }, data_1024_2[SHORT_ARRAY_COUNT] = { 0 };

        FILE * txtData;

        printf("start\n");

        const wchar_t * filePath = reinterpret_cast<const wchar_t *>(path.utf16());

        fs.open(filePath, ios::binary | ios::in);
        //    fs.seekg(0x04);                //从文件数据中获取文件大小
        //    fs.read((char*)&WAV.file_size,sizeof(WAV.file_size));
        //    WAV.file_size+=8;

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

        //cout << "文件大小为  ：" << WAV.file_size << endl;
        //cout << "音频通道数  ：" << WAV.channel << endl;
        //cout << "采样频率    ：" << WAV.frequency << endl;
        //cout << "Byte率      ：" << WAV.Bps << endl;
        //cout << "样本位数    ：" << WAV.sample_num_bit << endl;
        //cout << "音频数据大小：" << WAV.data_size << endl;
        //cout << "最后10个数据：" << endl;




        fopen_s(&txtData, txtFileName, "w");


        int frameSize = LONG_ARRAY_COUNT;
        int sampleRate = WAV.frequency;

        Gist<double> gist(frameSize, sampleRate);
        printf("enter loop\n");

//        if(WAV.channel == 1){
//        播放声音
//        PlaySound(filePath,NULL, SND_FILENAME | SND_ASYNC);

        for (unsigned long i = 0; i<WAV.data_size; i = i + 2)
                //for (unsigned long i = 0; i<20; i = i + 2)
            {
                //右边为大端
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
                double float_data = (double)(data_complement / (double)32768);
                //printf("%f ", float_data);

                /*-------------------------------------以下为数据处理部分------------------------------------------------*/
                if (mark_shortArr == SHORT_ARRAY_COUNT)//放满一个数组以后进入
                {
                    mark_2array++;
                    if (mark_2array>=2)//当有两个数组被新的数据放满后
                    {
                        if (mark_3 % 3 == 0)
                        {
                            for (int j = 0; j < SHORT_ARRAY_COUNT; j++){
                                data_2048[j] = data_1024_1[j];
                            }
                            for (int j = 0; j < SHORT_ARRAY_COUNT; j++){
                                data_2048[j+SHORT_ARRAY_COUNT] = data_1024_0[j];
                            }

                            gist.processAudioFrame(data_2048, LONG_ARRAY_COUNT);
                            double pitch = gist.pitch();
                            //printf("%fHz\n", pitch);

                            if(pitch>1000) pitch = pitchData.back();

                            double timeprint = ((double)i - (double)SHORT_ARRAY_COUNT) / ((double)WAV.frequency *(double)2*(double)WAV.channel);
                            fprintf_s(txtData, "%f,%f\n", timeprint, pitch);
                            timeAx.push_back(timeprint);
                            pitchData.push_back(pitch);

                        }else if(mark_3 % 3 == 1){
                            for (int j = 0; j < SHORT_ARRAY_COUNT; j++){
                                data_2048[j] = data_1024_2[j];
                            }
                            for (int j = 0; j < SHORT_ARRAY_COUNT; j++){
                                data_2048[j + SHORT_ARRAY_COUNT] = data_1024_1[j];
                            }

                            gist.processAudioFrame(data_2048, LONG_ARRAY_COUNT);
                            double pitch = gist.pitch();

                            if(pitch>1000) pitch = pitchData.back();

                            //printf("%fHz\n", pitch);
                            double timeprint = ((double)i - (double)SHORT_ARRAY_COUNT) / ((double)WAV.frequency *(double)2*(double)WAV.channel);
                            fprintf_s(txtData, "%f,%f\n", timeprint, pitch);
                            timeAx.push_back(timeprint);
                            pitchData.push_back(pitch);

                        }
                        else if (mark_3 % 3 == 2){
                            for (int j = 0; j < SHORT_ARRAY_COUNT; j++){
                                data_2048[j] = data_1024_0[j];
                            }
                            for (int j = 0; j < SHORT_ARRAY_COUNT; j++){
                                data_2048[j + SHORT_ARRAY_COUNT] = data_1024_2[j];
                            }

                            gist.processAudioFrame(data_2048, LONG_ARRAY_COUNT);
                            double pitch = gist.pitch();

                           if(pitch>1000) pitch = pitchData.back();

                            //printf("%fHz\n", pitch);
                            double timeprint = ((double)i - (double)SHORT_ARRAY_COUNT) / ((double)WAV.frequency *(double)2*(double)WAV.channel);
                            fprintf_s(txtData, "%f,%f\n", timeprint, pitch);
                            timeAx.push_back(timeprint);
                            pitchData.push_back(pitch);

                        }
                        mark_2array--;//为防止音乐过长导致mark_2array整数溢出，每次在减一下使其值维持在2。
                    }
                    mark_shortArr = 0;
                    mark_3++;
                    mark_3 = mark_3 % 3;//同样为防止溢出
                }
                if (mark_shortArr < SHORT_ARRAY_COUNT)//为满足未来实时性要求，设置三个数组，将数据循环放入，完成滑窗效果
                {
                    if (mark_3%3==0){
                        data_1024_0[mark_shortArr] = float_data;
                        mark_shortArr++;
                    }else if (mark_3%3==1){
                        data_1024_1[mark_shortArr] = float_data;
                        mark_shortArr++;
                    } else if(mark_3%3==2){
                        data_1024_2[mark_shortArr] = float_data;
                        mark_shortArr++;
                    }
                }

            }


        printf("end loop\n");
        fs.close();
        fclose(txtData);
        delete[] WAV.data;

        return 0;
    }



}




/*
string hex_to_binary(char* szHex)
{
int len = 2;
string result;
for (int i = 0; i < len;i++)
}
*/
