#include "loadMnist.h"
#include <fstream>
#include <opencv2/opencv.hpp>
using namespace std;

size_t ucharToInt(unsigned char* str)
{
	return (str[0] << 24)|(str[1] << 16)|(str[2] << 8)|(str[3]);
}
arma::mat loadMnistData(const char* filename)
{
	arma::mat data;
	ifstream input;
	try
	{
		input.open(filename, ios::binary);
	}
	catch (exception* e)
	{
		throw new exception(e->what());
	}
	unsigned char fourUchar[4];
	int imgsize;
	int rows, cols;
	input.read((char*)fourUchar, 4);//magic num
	input.read((char*)fourUchar, 4);//size of img
	imgsize = ucharToInt(fourUchar);
	input.read((char*)fourUchar, 4);//rows of img
	rows = ucharToInt(fourUchar);
	input.read((char*)fourUchar, 4);//cols of img
	cols = ucharToInt(fourUchar);
	if (imgsize <= 0 || rows <= 0 || cols <= 0)
	{
		throw new exception("error: read image size should be greater than 0");
	}
	vector<unsigned char> cvec(rows * cols);
	arma::vec temp(rows * cols);
	for (int i = 0; i < imgsize; ++i)
	{
		try
		{
			input.read((char*)&cvec[0], sizeof(unsigned char)* rows * cols);
		}
		catch (exception* e)
		{
			throw runtime_error(e->what());
		}
		for (int j = 0; j < rows * cols; ++j)
		{
			temp[j] = cvec[j];
		}
#if 0
		cv::Mat img(32, 32, CV_8UC1);
		for (int y = 0; y < 32; y++) {
			for (int x = 0; x < 32; x++) {
				img.at<uchar>(y, x) = temp[y * cols + x];
			}
		}
		cv::namedWindow("img", cv::WINDOW_AUTOSIZE);
		cv::imshow("img", img);
		cv::waitKey(0);
#endif
		data = join_rows(data, temp);
	}
	input.close();
	return data;
}

arma::vec loadMnistLabel(const char* filename)
{
	arma::vec labels;
	ifstream input;
	try
	{
		input.open(filename, ios::binary);
	}
	catch (exception* e)
	{
		throw runtime_error(e->what());
	}
	unsigned char fourUchar[4];
	int imgsize;
	input.read((char*)fourUchar, 4);//read magic num
	input.read((char*)fourUchar, 4);//read image size
	imgsize = ucharToInt(fourUchar);
	if (imgsize <= 0)
	{
		throw runtime_error("error: imgage size should not be 0 or negtive!\n");
	}
	vector<unsigned char> label(imgsize);
	input.read((char*)&label[0], imgsize);
	labels = arma::vec(imgsize);
	for (int i = 0; i < imgsize; ++i)
	{
		labels[i] = label[i];
	}
	input.close();
	return labels;
}
