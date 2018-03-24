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

//Ҫ��int main()��ǰ����Ϻ�������������Ϊ��ĺ���д��main�����ĺ���

//string hex_to_binary(char* szHex);
using namespace std;


struct wav_struct
{
	unsigned long file_size;        //�ļ���С
	unsigned short channel;            //ͨ����
    unsigned long frequency;        //����Ƶ��
	unsigned long Bps;                //Byte��
	unsigned short sample_num_bit;    //һ��������λ��
	unsigned long data_size;        //���ݴ�С
	unsigned char *data;            //��Ƶ���� ,����Ҫ����ʲô�Ϳ�����λ���ˣ�������ֻ�ǵ����ĸ�������
};


int Wav2Pitch(vector<double> &timeAx, vector<double> &pitchData,QString path)
{
    //Ϊ�˼ӿ촦���ٶȣ����ǽ��ļ�����һ�κ�����ݱ�������Ƶ�ļ��ĸ�Ŀ¼�£��´ε��õ�ʱ��ֱ�Ӷ�ȡ����õ�����
    //��������һ�δ�����Ƶ���Ӷ����Ч��
    QFileInfo fileinfo(path);
    QString fileRootPath = fileinfo.path();
    QString fileName = fileinfo.fileName();
    int fileNameLength = fileName.length();
    QString fileNameCutted = fileName.left(fileNameLength-4);
    QString txtFileNameFull = fileRootPath+"/"+fileNameCutted+".txt";//�˴���������ݱ����·��

    QFile dataTxt(txtFileNameFull);

    QByteArray ba = txtFileNameFull.toLatin1();
    const char *txtFileName = ba.data();

    int emitMark = 0;//��������

    if(dataTxt.exists()){
        string line;
        ifstream in(txtFileName);

//        ��������
//        const wchar_t * filePath = reinterpret_cast<const wchar_t *>(path.utf16());
//        PlaySound(filePath,NULL, SND_FILENAME | SND_ASYNC);

        while (getline (in, line)) // line�в�����ÿ�еĻ��з�
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
        //    fs.seekg(0x04);                //���ļ������л�ȡ�ļ���С
        //    fs.read((char*)&WAV.file_size,sizeof(WAV.file_size));
        //    WAV.file_size+=8;

        fs.seekg(0, ios::end);        //��c++���÷�������ļ���С
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

        //cout << "�ļ���СΪ  ��" << WAV.file_size << endl;
        //cout << "��Ƶͨ����  ��" << WAV.channel << endl;
        //cout << "����Ƶ��    ��" << WAV.frequency << endl;
        //cout << "Byte��      ��" << WAV.Bps << endl;
        //cout << "����λ��    ��" << WAV.sample_num_bit << endl;
        //cout << "��Ƶ���ݴ�С��" << WAV.data_size << endl;
        //cout << "���10�����ݣ�" << endl;




        fopen_s(&txtData, txtFileName, "w");


        int frameSize = LONG_ARRAY_COUNT;
        int sampleRate = WAV.frequency;

        Gist<double> gist(frameSize, sampleRate);
        printf("enter loop\n");

//        if(WAV.channel == 1){
//        ��������
//        PlaySound(filePath,NULL, SND_FILENAME | SND_ASYNC);

        for (unsigned long i = 0; i<WAV.data_size; i = i + 2)
                //for (unsigned long i = 0; i<20; i = i + 2)
            {
                //�ұ�Ϊ���
                unsigned long data_low = WAV.data[i];
                unsigned long data_high = WAV.data[i + 1];
                double data_true = data_high * 256 + data_low;
                //printf("%d ",data_true);
                long data_complement = 0;
                //ȡ��˵����λ������λ��
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

                /*-------------------------------------����Ϊ���ݴ�����------------------------------------------------*/
                if (mark_shortArr == SHORT_ARRAY_COUNT)//����һ�������Ժ����
                {
                    mark_2array++;
                    if (mark_2array>=2)//�����������鱻�µ����ݷ�����
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
                        mark_2array--;//Ϊ��ֹ���ֹ�������mark_2array���������ÿ���ڼ�һ��ʹ��ֵά����2��
                    }
                    mark_shortArr = 0;
                    mark_3++;
                    mark_3 = mark_3 % 3;//ͬ��Ϊ��ֹ���
                }
                if (mark_shortArr < SHORT_ARRAY_COUNT)//Ϊ����δ��ʵʱ��Ҫ�������������飬������ѭ�����룬��ɻ���Ч��
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
