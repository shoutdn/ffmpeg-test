
#define __STDC_CONSTANT_MACROS
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
#include <libavutil/fifo.h>
}

#include <iostream>
#include <string>

void testDemuxer() {
	const char* inputFileUrl = "D:/vsProject/Project1/GK88_mpeg4.mp4";

	//���װ
	AVFormatContext* inputContext = nullptr;
	avformat_open_input(&inputContext, inputFileUrl, NULL, NULL);
	avformat_find_stream_info(inputContext, NULL);

	//��ӡ��Ƶ��װ��Ϣ
	av_dump_format(inputContext, 0, inputFileUrl, 0);

	//������Ƶ������Ƶ��
	AVStream* audioInputStream = nullptr;
	AVStream* videoInputStream = nullptr;

	for (int i = 0; i < inputContext->nb_streams; i++)
	{
		if (inputContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			audioInputStream = inputContext->streams[i];
		else if (inputContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			videoInputStream = inputContext->streams[i];
		else continue;
	}

	//�ط�װ
	const char* outputFileUrl = "test_output.mp4";
	AVFormatContext* outputContext = nullptr;
	//��������ļ���׺�Ʋ��װ��ʽ
	avformat_alloc_output_context2(&outputContext, NULL, NULL, outputFileUrl);
	//�����Ƶ������Ƶ��
	AVStream* videoOutputStream = avformat_new_stream(outputContext, NULL);
	//AVStream* audioOutputStream = avformat_new_stream(outputContext, NULL);
	//��IO��
	avio_open(&outputContext->pb, outputFileUrl, AVIO_FLAG_WRITE);
	//��������Ƶ���ķ�װ���ø����������Ƶ��
	videoOutputStream->time_base = videoInputStream->time_base;
	avcodec_parameters_copy(videoOutputStream->codecpar, videoInputStream->codecpar);
	// //��������Ƶ���ķ�װ���ø����������Ƶ��
	// audioOutputStream->time_base = audioInputStream->time_base;
	// avcodec_parameters_copy(audioOutputStream->codecpar, audioInputStream->codecpar);
	//д���ļ�ͷ��Ϣ
	avformat_write_header(outputContext, NULL);
	av_dump_format(outputContext, 0, outputFileUrl, 1);

	//��ȡ������
	AVPacket packet;
	int frame_num = 0;
	bool hasKey = false;
	for (;;)
	{
		int result = av_read_frame(inputContext, &packet);
		if (result != 0) break;
		if (++frame_num < 100) continue;
		if (frame_num >= 10000) break;
		if (!hasKey) {
			if (!(packet.flags & AV_PKT_FLAG_KEY)) {
				continue;
			}
			else {
				hasKey = true;
			}
		}



		if (packet.stream_index == videoOutputStream->index)
		{
			std::cout << "��Ƶ:";
		}
		// else if (packet.stream_index == audioOutputStream->index) {
		// 	std::cout << "��Ƶ:";
		// }
		std::cout << packet.pts << " : " << packet.dts << " :" << packet.size << std::endl;

		av_interleaved_write_frame(outputContext, &packet);
	}
	std::cout << "frame sum :" << frame_num << std::endl;
	av_write_trailer(outputContext);
	avformat_close_input(&inputContext);
	avio_closep(&outputContext->pb);
	avformat_free_context(outputContext);
	outputContext = nullptr;
}

int main(int argc, char** argv)
{
	//testYUV();
	//demuxer_decode();
	//encode_video();
	testDemuxer();
	return 0;
}