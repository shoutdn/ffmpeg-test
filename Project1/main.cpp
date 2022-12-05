#include "videoinformation.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libpostproc/postprocess.h>
#include <libswresample/swresample.h>
};

#include <iostream>
#include <string>
#include <fstream>

std::string name("D:\\userData\\dataSet\\videos\\highSpeed\\GK88_mpeg4.mp4");
int test() {
	AVFormatContext* avFormatContext = avformat_alloc_context();
	
	if (avformat_open_input(&avFormatContext, name.c_str(), nullptr, nullptr) < 0) {
		std::cout << "打开视频失败" << std::endl;
		return -1;
	}

	if (avformat_find_stream_info(avFormatContext, NULL) < 0) {
		std::cout << "获取内容失败" << std::endl;
		return -1;
	}

	AVCodecParameters* origin_par = NULL;
	int mVideoStreamIdx = -1;
	mVideoStreamIdx = av_find_best_stream(avFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);

	if (mVideoStreamIdx < 0) {
		std::cout << "获取视频流失败" << std::endl;
		return -1;
	}

	AVCodec* mVcodec = nullptr;
	AVCodecContext* mAvContext = nullptr;
	mVcodec = avcodec_find_decoder(avFormatContext->data_codec_id);
	mAvContext = avcodec_alloc_context3(mVcodec);

	if (!mVcodec || !mAvContext) {
		std::cout << " 寻找编解码或上下文失败" << std::endl;
		return -1;
	}

	if (avcodec_parameters_to_context(mAvContext, origin_par) < 0) {
		std::cout << " 初始化解码器上下文失败" << std::endl;
		return -1;
	}

	if (avcodec_open2(mAvContext, mVcodec, NULL) != 0) {
		std::cout << "打开解码器失败" << std::endl;
		return -1;
	}


	AVPacket* packet = av_packet_alloc();


	AVFrame* frame = av_frame_alloc();



	uint8_t* byte_buffer = NULL;

	int byte_buffer_size = av_image_get_buffer_size(mAvContext->pix_fmt, mAvContext->width, mAvContext->height, 32);
	//int byte_buffer_size = avpicture_get_size(mAvContext->pix_fmt, mAvContext->width, mAvContext->height);
	byte_buffer = (uint8_t*)av_malloc(byte_buffer_size);
	if (!byte_buffer) {
		av_log(NULL, AV_LOG_ERROR, "Can't allocate buffer\n");
		return AVERROR(ENOMEM);
	}




	std::ofstream fp_yuv;
	fp_yuv.open("yuv.data", std::ios::app | std::ios::binary);

	while (1)
	{
		int ret = av_read_frame(avFormatContext, packet);
		if (ret != 0) {
			char buf[256];
			memset(buf, 0, 256);
			av_strerror(ret, buf, sizeof(buf));
			printf("%s\n", buf);
			av_packet_unref(packet);
			break;
		}

		if (ret >= 0 && packet->stream_index != mVideoStreamIdx) {
			av_packet_unref(packet);
			continue;
		}

		{
			// 发送待解码包
			int result = avcodec_send_packet(mAvContext, packet);
			av_packet_unref(packet);
			if (result < 0) {
				av_log(NULL, AV_LOG_ERROR, "Error submitting a packet for decoding\n");
				continue;
			}

			// 接收解码数据
			while (result >= 0) {
				result = avcodec_receive_frame(mAvContext, frame);
				if (result == AVERROR_EOF)
					break;
				else if (result == AVERROR(EAGAIN)) {
					result = 0;
					break;
				}
				else if (result < 0) {
					av_log(NULL, AV_LOG_ERROR, "Error decoding frame\n");
					av_frame_unref(frame);
					break;
				}

				int number_of_written_bytes = av_image_copy_to_buffer(byte_buffer, byte_buffer_size,
					(const uint8_t* const*)frame->data, (const int*)frame->linesize,
					mAvContext->pix_fmt, mAvContext->width, mAvContext->height, 1);
				if (number_of_written_bytes < 0) {
					av_log(NULL, AV_LOG_ERROR, "Can't copy image to buffer\n");
					av_frame_unref(frame);
					continue;
				}

				// 写文件保存视频数据
				fp_yuv.write((const char*)byte_buffer, number_of_written_bytes);
				fp_yuv.flush();

				av_frame_unref(frame);
			}
		}

	}


	//释放
	fp_yuv.close();
	av_frame_unref(frame);
	av_packet_unref(packet);
	avcodec_close(mAvContext);
	avformat_free_context(avFormatContext);
}






int main() {
	VideoInformation videoinfo;
	videoinfo.getVideoInfo(name);
	std::string format = videoinfo.getFormat();
	std::string video_format = videoinfo.getVideoFormat();
}