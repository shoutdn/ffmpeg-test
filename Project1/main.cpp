#include <iostream>
#include <thread>
using namespace std;
extern "C" {
#include <libavformat/avformat.h>
}


int main(int argc, char* argv[])
{
	const char* inputFileUrl = "C://Users//liu//Desktop//filebuffer//MP4//逆行2.mp4";

	//解封装
	AVFormatContext* inputContext = nullptr;
	avformat_open_input(&inputContext, inputFileUrl, NULL, NULL);
	avformat_find_stream_info(inputContext, NULL);

	//打印视频封装信息
	av_dump_format(inputContext, 0, inputFileUrl, 0);

	//分离音频流和视频流
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

	//重封装
	const char* outputFileUrl = "test_output.mp4";
	AVFormatContext* outputContext = nullptr;
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
	//将输入音频流的封装配置复制至输出音频流
	/*audioOutputStream->time_base = audioInputStream->time_base;
	avcodec_parameters_copy(audioOutputStream->codecpar, audioInputStream->codecpar);*/
	//写入文件头信息
	avformat_write_header(outputContext, NULL);
	av_dump_format(outputContext, 0, outputFileUrl, 1);

	//读取输入流
	AVPacket packet;
	for (;;)
	{
		int result = av_read_frame(inputContext, &packet);
		if (result != 0) break;

		if (packet.stream_index == videoOutputStream->index)
		{
			cout << "视频:";
		}
	/*	else if (packet.stream_index == audioOutputStream->index) {
			cout << "音频:";
		}*/
		cout << packet.pts << " : " << packet.dts << " :" << packet.size << endl;

		av_interleaved_write_frame(outputContext, &packet);
	}

	av_write_trailer(outputContext);
	avformat_close_input(&inputContext);
	avio_closep(&outputContext->pb);
	avformat_free_context(outputContext);
	outputContext = nullptr;
	return 0;
}