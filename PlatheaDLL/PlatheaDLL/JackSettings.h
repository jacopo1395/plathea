#pragma once

#include <vector>
#include <iostream>
#include <cstdint>
#include <cstring>

#include <list>

#include "jpeglib.h"

#include "video_output.h"
#include <LeoWindowsOpenCV.h>
#include "CommonHeader.h"

#include <mutex>          // std::mutex




extern int IDroom;


class StreamsVideo {
	std::vector<unsigned char>  left, background, rawforeground, foreground, disparity, edge, occupancy, height;
	//std::list<unsigned char>  left, background, rawforeground, foreground, disparity, edge, occupancy, height;
	//std::vector<unsigned char> buf;

	std::mutex mtxleft, mtxbackground, mtxrawforeground, mtxforeground, mtxdisparity, mtxedge, mtxoccupancy, mtxheight;           // mutex for critical section
	int leftcount = 0;
	int backgroundcount = 0;
	int rawforegroundcount = 0;
	int foregroundcount = 0;
	int disparitycount = 0;
	int occupancycount = 0;
	int heightcount = 0;
	int edgecount = 0;

public:

	bool ipl2jpeg(IplImage *frame, unsigned char **outbuffer, unsigned long*outlen)
	{
		IplImage *img = new IplImage;
		memcpy(img, frame, frame->nSize);
		unsigned char *outdata = (uchar *)img->imageData;
		struct jpeg_compress_struct cinfo = { 0 };
		struct jpeg_error_mgr jerr;
		JSAMPROW row_ptr[1];
		int row_stride;

		*outbuffer = NULL;
		*outlen = 0;

		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_compress(&cinfo);
		jpeg_mem_dest(&cinfo, outbuffer, outlen);

		cinfo.image_width = frame->width;
		cinfo.image_height = frame->height;
		cinfo.input_components = frame->nChannels;
		cinfo.in_color_space = JCS_RGB;

		jpeg_set_defaults(&cinfo);
		jpeg_start_compress(&cinfo, TRUE);
		system("pause");
		row_stride = frame->width * frame->nChannels;


		while (cinfo.next_scanline < cinfo.image_height)
		{
			row_ptr[0] = &outdata[cinfo.next_scanline * row_stride];
			jpeg_write_scanlines(&cinfo, row_ptr, 1);
		}

		jpeg_finish_compress(&cinfo);
		jpeg_destroy_compress(&cinfo);

		return true;

	}
	
	StreamsVideo();
	
	

	void addFrame(IplImage* frame) {
		mtxleft.lock();
		cv::Mat image = cv::cvarrToMat(frame);
		//cv::threshold(image, image, 100, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);	
		cv::imencode(".jpg", image, left);
		//std::list<unsigned char> list;
		//std::copy(buf.begin(), buf.end(), std::back_inserter(list));
		//printf("add frame\n");
		//printf("tobytes: %d\n", list.size());
		//left.push_back(list);
		//if(left.size()>1)left.erase(left.begin());
		//leftcount++;
		mtxleft.unlock();
	}
	std::vector<unsigned char> getFrame() {
		mtxleft.lock();
		
		/*
		//printf("nframe: %d\n", left.size());
		if (left.empty()) {
			std::list<unsigned char> r;
			mtxleft.unlock();
			return r;
		}	
		*/
		std::vector<unsigned char> ret = left;
		//left.erase(left.begin());
		
		//std::list<unsigned char> ret = left;
		mtxleft.unlock();
		return ret;
	}

	
	void addFrameBackground(IplImage* frame) {
		mtxbackground.lock();
		cv::Mat image = cv::cvarrToMat(frame);
		cv::imencode(".jpg", image, background);
		mtxbackground.unlock();
	}
	std::vector<unsigned char> getFrameBackground() {
		mtxbackground.lock();
		std::vector<unsigned char> ret = background;
		mtxbackground.unlock();
		return ret;
	}

	void addFrameRawforeground(IplImage* frame) {
		mtxrawforeground.lock();
		cv::Mat image = cv::cvarrToMat(frame);
		cv::imencode(".jpg", image, rawforeground);
		mtxrawforeground.unlock();

	}
	std::vector<unsigned char> getFrameRawforeground() {
		mtxrawforeground.lock();
		std::vector<unsigned char> ret = rawforeground;
		mtxrawforeground.unlock();
		return ret;
	}

	void addFrameForeground(IplImage* frame) {
		mtxforeground.lock();
		cv::Mat image = cv::cvarrToMat(frame);
		cv::imencode(".jpg", image, foreground);
		mtxforeground.unlock();
	}
	std::vector<unsigned char> getFrameForeground() {
		mtxforeground.lock();

		std::vector<unsigned char> ret = foreground;

		mtxforeground.unlock();
		return ret;
	}


	void addFrameDisparity(IplImage* frame) {
		mtxdisparity.lock();
		cv::Mat image = cv::cvarrToMat(frame);
		cv::imencode(".jpg", image, disparity);
		mtxdisparity.unlock();
	}
	std::vector<unsigned char> getFrameDisparity() {
		mtxdisparity.lock();
		std::vector<unsigned char> ret = disparity;
		mtxdisparity.unlock();
		return ret;
	}

	void addFrameEdge(IplImage* frame) {
		mtxedge.lock();
		cv::Mat image = cv::cvarrToMat(frame);
		cv::imencode(".jpg", image, edge);
		mtxedge.unlock();
	}
	std::vector<unsigned char> getFrameEdge() {
		mtxedge.lock();
		std::vector<unsigned char> ret = edge;
		mtxedge.unlock();
		return ret;
	}


	void addFrameOccupancy(IplImage* frame) {
		mtxoccupancy.lock();
		cv::Mat image = cv::cvarrToMat(frame);
		cv::imencode(".jpg", image, occupancy);
		mtxoccupancy.unlock();
	}
	std::vector<unsigned char> getFrameOccupancy() {
		mtxoccupancy.lock();
		std::vector<unsigned char> ret = occupancy;
		mtxoccupancy.unlock();
		return ret;
	}

	void addFrameHeight(IplImage* frame) {
		mtxheight.lock();
		cv::Mat image = cv::cvarrToMat(frame);
		cv::imencode(".jpg", image, height);
		mtxheight.unlock();
	}
	std::vector<unsigned char> getFrameHeight() {
		mtxheight.lock();
		std::vector<unsigned char> ret = height;
		mtxheight.unlock();
		return ret;
	}
	

private:
	std::list<unsigned char> toBytes(IplImage img)
	{
		IplImage* image = &img;
		int img_sz = image->width * image->height * image->nChannels;
		unsigned char* data = (unsigned char*)malloc(img_sz);
		//memcopy(data, image->imageData, img_sz);
		std::list<unsigned char> bytes;
		for (int i = 0; i<img_sz; i++)
		{
			bytes.push_back((unsigned char)image->imageData[i]);
		}
		return bytes;
	}

};





extern StreamsVideo* streamsVideo;