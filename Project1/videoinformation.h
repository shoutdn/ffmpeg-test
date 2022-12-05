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

/* 视频有哪些信息？
 * (1) 总体
 *  - 格式            o
 *  - 编解码器
 *  - 文件大小
 *  - 时长            o
 *  - 总体码率模式
 *  - 总体码率
 *  - 日期
 * (2) 对于视频
 *  - ID             o
 *  - 格式            o
 *  - 码率            o
 *  - 最大码率
 *  - 宽度            o
 *  - 高度            o
 *  - 帧率模式
 *  - 帧率            o
 *  - 色彩空间
 *  - 色度抽样
 *  - 位深
 *  - 流大小           o
 * (3) 对于音频
 *  - 格式            o
 *  - 码率            o
 *  - 声道数           o
 *  - 采样率           o
 *  - 压缩模式
 *  - 流大小           o
 */
class VideoInformation
{
public:
    VideoInformation();

    ~VideoInformation() {
        avformat_close_input(&input_AVFormat_context_);
    }

    //@brief: 得到视频信息
    //@param: file_path: 文件路径
    //@ret  : void
    //@birth: created by LucasNan on 20210218
    void getVideoInfo(std::string file_path);

    //@brief: 得到格式
    //@param: void
    //@ret  : 视频格式
    //@birth: created by LucasNan on 20210219
    std::string getFormat() {
        return this->format_;
    }

    //@brief: 得到视频长度
    //@param: void
    //@ret  : 视频长度
    //@birth: created by LucasNan on 20210219
    std::string getDuration() {
        return this->duration_;
    }

    //@brief: 得到视频帧率
    //@param: void
    //@ret  : 视频帧率
    //@birth: created by LucasNan on 20210219
    int getFrameRate() {
        return this->frame_rate_;
    }

    //@brief: 得到视频码率
    //@param: void
    //@ret  : 视频平均码率
    //@birth: created by LucasNan on 20210219
    int getVideoAverageBitRate() {
        return this->video_average_bit_rate_;
    }

    //@brief: 得到视频宽度
    //@param: void
    //@ret  : 视频宽度
    //@birth: created by LucasNan on 20210219
    int getWidth() {
        return this->width_;
    }

    //@brief: 得到视频高度
    //@param: void
    //@ret  : 视频高度
    //@birth: created by LucasNan on 20210219
    int getHeight() {
        return this->height_;
    }

    //@brief: 得到视频流大小
    //@param: void
    //@ret  : 视频流大小
    //@birth: created by LucasNan on 20210219
    float getVideoSize() {
        return this->video_size_;
    }

    //@brief: 得到视频编码格式
    //@param: void
    //@ret  : 视频编码格式
    //@birth: created by LucasNan on 20210219
    std::string getVideoFormat() {
        return this->video_format_;
    }

    std::string getAudioFormat() {
        return this->audio_format_;
    }


    //@brief: 得到音频平均码率
    //@param: void
    //@ret  : 音频平均码率
    //@birth: created by LucasNan on 20210219
    int getAudioAverageBitRate() {
        return this->audio_average_bit_rate_;
    }

    //@brief: 得到音频通道数
    //@param: void
    //@ret  : 音频通道数
    //@birth: created by LucasNan on 20210219
    int getChannelNumbers() {
        return this->channel_nums;
    }


    //@brief: 得到音频采样率
    //@param: void
    //@ret  : 音频采样率
    //@birth: created by LucasNan on 20210219
    int getSampleRate() {
        return this->sample_rate_;
    }


    //@brief: 得到音频大小
    //@param: void
    //@ret  : 音频大小
    //@birth: created by LucasNan on 20210219
    float getAudioSize() {
        return this->audio_size_;
    }

public:
    AVFormatContext* input_AVFormat_context_;

    //流数
    unsigned int stream_numbers_;

    //视频流索引号
    unsigned int video_stream_index_;
    //音频流索引号
    unsigned int audio_stream_index_;


    //(1) 总体
    std::string format_;            //格式
    std::string duration_;                //时长


    //(2) 对于视频
    int frame_rate_;                //帧率

    int video_average_bit_rate_;    //平均码率

    int width_;                     //视频宽度
    int height_;                    //视频高度

    float video_size_;              //视频流大小

    std::string video_format_;       //视频编码格式

    //(3) 对于音频
    std::string audio_format_;      //音频编码格式

    int audio_average_bit_rate_;    //音频平均码率

    int channel_nums;               //声道数

    int sample_rate_;               //采样率

    float audio_size_;              //音频流大小

};

#endif // VIDEOINFORMATION_H

