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

#include "videoinformation.h"

VideoInformation::VideoInformation()
{
    //��ȡ��̬�ڴ�
    input_AVFormat_context_ = avformat_alloc_context();
    //input_AVFormat_context_ = NULL;
}

void VideoInformation::getVideoInfo(std::string file_path) {

    if (avformat_open_input(&input_AVFormat_context_, file_path.c_str(), 0, NULL) < 0) {
        std::cout << "file open error!" << std::endl;
        return;
    }

    if (avformat_find_stream_info(input_AVFormat_context_, NULL) < 0) {
        printf("error");
        return;
    }

    //�õ�������
    this->stream_numbers_ = input_AVFormat_context_->nb_streams;

    //������Ƶ����
    int hours, mins, secs;
    secs = input_AVFormat_context_->duration / 1000000;
    mins = secs / 60;
    secs %= 60;
    hours = mins / 60;
    mins %= 60;

    //��ʽ����Ƶ����
    char duration_foramt_[128];
    sprintf(duration_foramt_, "%d:%d:%d", hours, mins, secs);
    this->duration_ = duration_foramt_;

    //av_dump_format(input_AVFormat_context_, 0, file_path.c_str(), 0);

    //�õ�������Ƶ�ķ�װ��ʽ

    std::cout << "format: " << input_AVFormat_context_->streams[0]->codecpar->format << std::endl;
    std::cout << "bit rate: " << input_AVFormat_context_->bit_rate / 1000.0 << std::endl;

    AVInputFormat* infoFormat = input_AVFormat_context_->iformat;
    this->format_ = infoFormat->name;

    //�ֱ������Ƶ����
    for (unsigned int i = 0; i < stream_numbers_; i++) {
        //ȡ��һ·��,������AVStream����
        AVStream* input_stream = input_AVFormat_context_->streams[i];
        //AVDictionaryEntry *lang = av_dict_get(input_stream->metadata, "language", NULL, 0);
        //std::cout<<"ddd: "<<lang->value<<std::endl;
       //�ж��Ƿ�Ϊ��Ƶ��
        if (input_stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {

            //avg_frame_rate -> AVRational(������),
            //avg_frame_rate.num : ����
            //avg_frame_rate.den : ĸ
            //�õ���Ƶ֡��
            this->frame_rate_ = input_stream->avg_frame_rate.num / input_stream->avg_frame_rate.den;

            //ȡ����Ƶ���еı����������, ����AVCodecParamters����
            AVCodecParameters* codec_par = input_stream->codecpar;

            //���ñ����������AVCdecParamters�õ���Ƶ��ȣ��߶ȣ����ʣ���Ƶ��С
            this->width_ = codec_par->width;
            this->height_ = codec_par->height;
            this->video_average_bit_rate_ = codec_par->bit_rate / 1000;
            this->video_size_ = this->video_average_bit_rate_ * secs / (8.0 * 1024);

            //����avcodec_paramters_to_context()��������AVCodecContext����
            //input_stream->codec�Ѿ�����̭�����Ƽ�ʹ�����ַ�ʽ����AVCodecContext
            AVCodecContext* avctx_video;
            avctx_video = avcodec_alloc_context3(NULL);
            int ret = avcodec_parameters_to_context(avctx_video, codec_par);
            if (ret < 0) {
                avcodec_free_context(&avctx_video);
                return;
            }
            //ʹ��AVCodecContext�õ���Ƶ�����ʽ�����Ƽ���
            char buf[128];
            avcodec_string(buf, sizeof(buf), avctx_video, 0);
            //ʹ��AVCodecParameters�õ���Ƶ���뷽ʽ
            this->video_format_ = avcodec_get_name((codec_par->codec_id));

            //�ж��Ƿ�Ϊ��Ƶ��
        }
        else if (input_stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {

            //����AVcodecParamters����
            AVCodecParameters* codec_par = input_stream->codecpar;
            AVCodecContext* avctx_audio;
            avctx_audio = avcodec_alloc_context3(NULL);
            int ret = avcodec_parameters_to_context(avctx_audio, codec_par);
            if (ret < 0) {
                avcodec_free_context(&avctx_audio);
                return;
            }

            this->audio_format_ = avcodec_get_name(avctx_audio->codec_id);

            this->audio_average_bit_rate_ = codec_par->bit_rate / 1000;

            this->channel_nums = codec_par->channels;

            this->sample_rate_ = codec_par->sample_rate;

            this->audio_size_ = this->audio_average_bit_rate_ * secs / (8.0 * 1024);
        }
    }
}
