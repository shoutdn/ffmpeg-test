extern "C" {
//#include <libavcodec/avcodec.h>
//#include <libavutil/imgutils.h>
//#include <libavutil/samplefmt.h>
//#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
//#include <libavutil/fifo.h>
}

#include <queue>
#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
struct IMG {
	unsigned size;
	AVPacket packet;
};

std::queue<AVPacket> packet_queue;
//std::mutex packet_mutex;
//std::condition_variable packet_con;


//void writeVideo(AVFormatContext* outputContext) {
//
//	//读取输入流
//	bool hasKey = false;
//
//	int frame_num = 0;
//	for (;;) {
//		std::unique_lock<std::mutex> lkk(packet_mutex);
//		bool go = packet_con.wait_for(lkk, std::chrono::milliseconds(100),[&]{ return packet_queue.size() > 0; });
//		if (!go) {
//			continue;
//		}
//
//		IMG img = packet_queue.front();
//		packet_queue.pop();
//		lkk.unlock();
//
//		//if (++frame_num < 100) continue;
//		if (++frame_num > 10000) break;
//		if (!hasKey) {
//			if (!(img.packet.flags & AV_PKT_FLAG_KEY)) {
//				continue;
//			}
//			else {
//				hasKey = true;
//			}
//		}
//
//		std::cout << "packet_size: " << img.size << std::endl;
//		av_interleaved_write_frame(outputContext, &img.packet);
//	}
//
//	av_write_trailer(outputContext);
//	avio_closep(&outputContext->pb);
//	avformat_free_context(outputContext);
//	outputContext = nullptr;
//}


void testDemuxer() {
	std::string inputFileUrl = "D:/vsProject/Project1/GK88_mpeg4.mp4";

	//解封装
	AVFormatContext* inputContext = nullptr;
	avformat_open_input(&inputContext, inputFileUrl.c_str(), NULL, NULL);
	avformat_find_stream_info(inputContext, NULL);

	//打印视频封装信息
	av_dump_format(inputContext, 0, inputFileUrl.c_str(), 0);

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
	std::string outputFileUrl = "test_output.mp4";
	AVFormatContext* outputContext = nullptr;
	//根据输出文件后缀推测封装格式
	avformat_alloc_output_context2(&outputContext, NULL, NULL, outputFileUrl.c_str());
	//添加音频流和视频流
	AVStream* videoOutputStream = avformat_new_stream(outputContext, NULL);
	//AVStream* audioOutputStream = avformat_new_stream(outputContext, NULL);
	//打开IO流
	avio_open(&outputContext->pb, outputFileUrl.c_str(), AVIO_FLAG_WRITE);
	//将输入视频流的封装配置复制至输出视频流
	videoOutputStream->time_base = videoInputStream->time_base;
	avcodec_parameters_copy(videoOutputStream->codecpar, videoInputStream->codecpar);
	// //将输入音频流的封装配置复制至输出音频流
	// audioOutputStream->time_base = audioInputStream->time_base;
	// avcodec_parameters_copy(audioOutputStream->codecpar, audioInputStream->codecpar);
	//写入文件头信息
	avformat_write_header(outputContext, NULL);
	av_dump_format(outputContext, 0, outputFileUrl.c_str(), 1);

	
	/*std::thread* write_thread = new std::thread(writeVideo, outputContext);*/
	
	int read_num = 0;
	int frame_num = 0;
	bool hasKey = false;
	for (;;)
	{
		AVPacket packet_tmp;
		AVPacket packet;
		

		/*AVPacket packet_tmp2;
		AVPacket packet_tmp3;*/
		int result = av_read_frame(inputContext, &packet_tmp);
		if (result == 0) {
			packet = packet_tmp;
			/*av_packet_ref(&packet_tmp2, &packet_tmp);
			av_packet_ref(&packet_tmp3, &packet_tmp);*/
			//av_packet_unref(&packet_tmp);



			if (++read_num > 10000) {
				break;
			}
		}
		
		/*AVPacket packet_write;
		av_packet_ref(&packet_write,&packet_queue.front());
		av_packet_unref(&packet_queue.front());
		packet_queue.pop();
		av_packet_unref(&packet_queue.front());*/
		if (result != 0) break;
		if (++frame_num < 100) continue;
		if (frame_num >= 10000) break;
		if (!hasKey) {
			if ((packet.flags & AV_PKT_FLAG_KEY)) {
				hasKey = true;
			}
			else {
				continue;
			}
		}

		av_interleaved_write_frame(outputContext, &packet);
	}



	av_write_trailer(outputContext);
	avio_closep(&outputContext->pb);
	avformat_free_context(outputContext);
	outputContext = nullptr;
	avformat_close_input(&inputContext);
}

int main()
{
	testDemuxer();
}