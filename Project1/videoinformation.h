#pragma once
// Copyright (c) 2021 LucasNan <nanche@mail.hfut.edu.cn> <www.kevinnan.org.cn>

// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.


#ifndef VIDEOINFORMATION_H
#define VIDEOINFORMATION_H

#include <iostream>
#include <string>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

/* ��Ƶ����Щ��Ϣ��
 * (1) ����
 *  - ��ʽ            o
 *  - �������
 *  - �ļ���С
 *  - ʱ��            o
 *  - ��������ģʽ
 *  - ��������
 *  - ����
 * (2) ������Ƶ
 *  - ID             o
 *  - ��ʽ            o
 *  - ����            o
 *  - �������
 *  - ���            o
 *  - �߶�            o
 *  - ֡��ģʽ
 *  - ֡��            o
 *  - ɫ�ʿռ�
 *  - ɫ�ȳ���
 *  - λ��
 *  - ����С           o
 * (3) ������Ƶ
 *  - ��ʽ            o
 *  - ����            o
 *  - ������           o
 *  - ������           o
 *  - ѹ��ģʽ
 *  - ����С           o
 */
class VideoInformation
{
public:
    VideoInformation();

    ~VideoInformation() {
        avformat_close_input(&input_AVFormat_context_);
    }

    //@brief: �õ���Ƶ��Ϣ
    //@param: file_path: �ļ�·��
    //@ret  : void
    //@birth: created by LucasNan on 20210218
    void getVideoInfo(std::string file_path);

    //@brief: �õ���ʽ
    //@param: void
    //@ret  : ��Ƶ��ʽ
    //@birth: created by LucasNan on 20210219
    std::string getFormat() {
        return this->format_;
    }

    //@brief: �õ���Ƶ����
    //@param: void
    //@ret  : ��Ƶ����
    //@birth: created by LucasNan on 20210219
    std::string getDuration() {
        return this->duration_;
    }

    //@brief: �õ���Ƶ֡��
    //@param: void
    //@ret  : ��Ƶ֡��
    //@birth: created by LucasNan on 20210219
    int getFrameRate() {
        return this->frame_rate_;
    }

    //@brief: �õ���Ƶ����
    //@param: void
    //@ret  : ��Ƶƽ������
    //@birth: created by LucasNan on 20210219
    int getVideoAverageBitRate() {
        return this->video_average_bit_rate_;
    }

    //@brief: �õ���Ƶ���
    //@param: void
    //@ret  : ��Ƶ���
    //@birth: created by LucasNan on 20210219
    int getWidth() {
        return this->width_;
    }

    //@brief: �õ���Ƶ�߶�
    //@param: void
    //@ret  : ��Ƶ�߶�
    //@birth: created by LucasNan on 20210219
    int getHeight() {
        return this->height_;
    }

    //@brief: �õ���Ƶ����С
    //@param: void
    //@ret  : ��Ƶ����С
    //@birth: created by LucasNan on 20210219
    float getVideoSize() {
        return this->video_size_;
    }

    //@brief: �õ���Ƶ�����ʽ
    //@param: void
    //@ret  : ��Ƶ�����ʽ
    //@birth: created by LucasNan on 20210219
    std::string getVideoFormat() {
        return this->video_format_;
    }

    std::string getAudioFormat() {
        return this->audio_format_;
    }


    //@brief: �õ���Ƶƽ������
    //@param: void
    //@ret  : ��Ƶƽ������
    //@birth: created by LucasNan on 20210219
    int getAudioAverageBitRate() {
        return this->audio_average_bit_rate_;
    }

    //@brief: �õ���Ƶͨ����
    //@param: void
    //@ret  : ��Ƶͨ����
    //@birth: created by LucasNan on 20210219
    int getChannelNumbers() {
        return this->channel_nums;
    }


    //@brief: �õ���Ƶ������
    //@param: void
    //@ret  : ��Ƶ������
    //@birth: created by LucasNan on 20210219
    int getSampleRate() {
        return this->sample_rate_;
    }


    //@brief: �õ���Ƶ��С
    //@param: void
    //@ret  : ��Ƶ��С
    //@birth: created by LucasNan on 20210219
    float getAudioSize() {
        return this->audio_size_;
    }

public:
    AVFormatContext* input_AVFormat_context_;

    //����
    unsigned int stream_numbers_;

    //��Ƶ��������
    unsigned int video_stream_index_;
    //��Ƶ��������
    unsigned int audio_stream_index_;


    //(1) ����
    std::string format_;            //��ʽ
    std::string duration_;                //ʱ��


    //(2) ������Ƶ
    int frame_rate_;                //֡��

    int video_average_bit_rate_;    //ƽ������

    int width_;                     //��Ƶ���
    int height_;                    //��Ƶ�߶�

    float video_size_;              //��Ƶ����С

    std::string video_format_;       //��Ƶ�����ʽ

    //(3) ������Ƶ
    std::string audio_format_;      //��Ƶ�����ʽ

    int audio_average_bit_rate_;    //��Ƶƽ������

    int channel_nums;               //������

    int sample_rate_;               //������

    float audio_size_;              //��Ƶ����С

};

#endif // VIDEOINFORMATION_H

