#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void imgProc(Mat& desMat, string imgPath, string imgPathChild) //结果，原图路径，子原图路径
{
    /*
        TM_SQDIFF           //平方差匹配
        TM_SQDIFF_NORMED
        TM_CCORR            //乘数匹配
        TM_CCORR_NORMED
        TM_CCOEFF           //相关匹配
        TM_CCOEFF_NORMED
    */
    int METHOD = TM_CCOEFF;                 //使用相关匹配算法，相比前两个匹配精准度最优
    Mat imgSrc = imread(imgPath);           //读原图
    Mat imgTmp = imread(imgPathChild);      //读匹配子图
              
    Mat imgRes;        //比较结果的映射图像
    Mat imgDisplay;    //处理后图片
    imgSrc.copyTo(imgDisplay);
    int rescols = imgSrc.cols - imgTmp.cols + 1; //imgRes的尺寸
    int resrows = imgSrc.rows - imgTmp.rows + 1;
    imgRes.create(rescols, resrows, CV_32FC1);          //创建输出结果的矩阵，32位浮点型图像
    matchTemplate(imgSrc, imgTmp, imgRes, METHOD);      //进行匹配
    normalize(imgRes, imgRes, 0, 1, NORM_MINMAX, -1, Mat());//进行标准化
    double minVal;
    double maxVal;
    Point minLoc;
    Point maxLoc;
    Point matchLoc;
    minMaxLoc(imgRes, &minVal, &maxVal, &minLoc, &maxLoc, Mat());//通过函数 minMaxLoc 定位最匹配的位置
    //对于方法 SQDIFF 和 SQDIFF_NORMED, 越小的数值代表更高的匹配结果. 而对于其他方法, 数值越大匹配越好
    if (METHOD == TM_SQDIFF || METHOD == TM_SQDIFF_NORMED) {
        matchLoc = minLoc;
    }
    else {
        matchLoc = maxLoc;
    }
    //绘制矩形
    rectangle(imgDisplay, matchLoc, Point(matchLoc.x + imgTmp.cols, matchLoc.y + imgTmp.rows), Scalar(0,255,255), 2, 8, 0);
   // rectangle(imgRes, matchLoc, Point(matchLoc.x + imgTmp.cols, matchLoc.y + imgTmp.rows), Scalar::all(0), 2, 8, 0);
    desMat = imgDisplay;
}

int main(int argc, char** argv) {
    string imgPath = "C:/Users/lenovo/Pictures/examMid/WeldingProcessImage/t_3.jpg";        //原图路径
    string imgPathChild = "C:/Users/lenovo/Pictures/examMid/1.png";                         //匹配子图路径
    Mat des; //处理后的图片
    imgProc(des, imgPath, imgPathChild); //匹配飞溅
    namedWindow("输入窗口", WINDOW_FREERATIO); //创建窗口
	imshow("输入窗口", des);
    string stuNum = "114514";
    string nameIndex = imgPath.substr(imgPath.length() - 5, 1);
    string desPath = "C:/Users/lenovo/Pictures/examMid/WeldingProcessImageDetectionResults/"+ stuNum + "_" + nameIndex + ".png"; //保存处理后的图片
    imwrite(desPath, des);

	waitKey(0); 
	destroyAllWindows();
	return 0;
}