#include "stdafx.h"
#include "changeColor.h"
#include "Utils.h"

//��ȡRGBֵ
vector<int> getRGB(IplImage* &image, int x, int y){
	vector<int> rgb;
	RgbImage  imgA(image);

	int b = imgA[x][y].b ;
	int g = imgA[x][y].g ;
	int r = imgA[x][y].r ;

	rgb.push_back(b);
	rgb.push_back(g);
	rgb.push_back(r);

	return rgb;
}

//�ı�RGBֵ
void changeColor(IplImage* &img, int x, int y, int R, int G, int B)
{
	RgbImage  imgA(img);

	imgA[x][y].b = R;
	imgA[x][y].g = G;
	imgA[x][y].r = B;



}

//��ȡ�Ҷ�ͼ��ĻҶ�ֵ
int getGray(IplImage* &image, int x, int y){

	int value = 0;

	BwImage imgA(image);
	value = imgA[x][y] ;

	return value;

}

void changeGray(IplImage* &image, int x, int y,int value){

	BwImage imgA(image);
	imgA[x][y] = value;

}