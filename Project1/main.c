
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <libavformat/avformat.h>

int main(int argc, char** argv) {
	char* inputFileUrl = "D:/vsProject/Project1/GK88_mpeg4.mp4";

	//解封装
	AVFormatContext* inputContext = NULL;
	avformat_open_input(&inputContext, inputFileUrl, NULL, NULL);
	avformat_find_stream_info(inputContext, NULL);

	//打印视频封装信息
	av_dump_format(inputContext, 0, inputFileUrl, 0);

	//分离音频流和视频流
	AVStream* audioInputStream = NULL;
	AVStream* videoInputStream = NULL;

	for (int i = 0; i < inputContext->nb_streams; i++)
	{
		if (inputContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			audioInputStream = inputContext->streams[i];
		else if (inputContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			videoInputStream = inputContext->streams[i];
		else continue;
	}

	//重封装
	char* outputFileUrl = "test_output.mp4";
	AVFormatContext* outputContext = NULL;
	//根据输出文件后缀推测封装格式
	avformat_alloc_output_context2(&outputContext, NULL, NULL, outputFileUrl);
	//添加音频流和视频流
	AVStream* videoOutputStream = avformat_new_stream(outputContext, NULL);
	//AVStream* audioOutputStream = avformat_new_stream(outputContext, NULL);
	//打开IO流
	avio_open(&outputContext->pb, outputFileUrl, AVIO_FLAG_WRITE);
	//将输入视频流的封装配置复制至输出视频流
	videoOutputStream->time_base = videoInputStream->time_base;
	avcodec_parameters_copy(videoOutputStream->codecpar, videoInputStream->codecpar);
	// //将输入音频流的封装配置复制至输出音频流
	// audioOutputStream->time_base = audioInputStream->time_base;
	// avcodec_parameters_copy(audioOutputStream->codecpar, audioInputStream->codecpar);
	//写入文件头信息
	avformat_write_header(outputContext, NULL);
	av_dump_format(outputContext, 0, outputFileUrl, 1);


	int read_num = 0;
	int frame_num = 0;
	bool hasKey = false;
	
	for (;;)
	{
		AVPacket packet_tmp, packet_tmp2;
		int result = av_read_frame(inputContext, &packet_tmp2);
		//packet_tmp = packet_tmp2;
		av_packet_copy_props(&packet_tmp, &packet_tmp2);
		
		if (result != 0) break;
		if (++frame_num < 100) continue;
		if (frame_num >= 10000) break;
		if (!hasKey) {
			if (!(packet_tmp.flags & AV_PKT_FLAG_KEY)) {
				continue;
			}
			else {
				hasKey = true;
			}
		}

		av_interleaved_write_frame(outputContext, &packet_tmp);
	}

	av_write_trailer(outputContext);
	avio_closep(&outputContext->pb);
	avformat_free_context(outputContext);
	outputContext = NULL;
	avformat_close_input(&inputContext);


	return 0;
}