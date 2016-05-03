#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "utilities.h"
#include "cut_image.h"
#include "online_detection.h"
#include "kim.h"
#include <iostream>

#include "Windows.h"

using namespace std;
using namespace utilities;
using namespace cv;

int main(int argc, char** argv)
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER tBegin, tEnd, tProcessBegin, tProcessEnd;
    double elapsedTime;
    QueryPerformanceFrequency(&frequency);

    const int sBloc = 32;
    Cell<const Mat> aMatrixCell(8, 12);
    int ret = cutImage(aMatrixCell, "E:/jee/cours/GEN5023/code/textile_images/Free/1.TIF", sBloc, false, false);
    
    vector<float> refSignature;
    ret = extractKimSignature(aMatrixCell.get(0, 0), sBloc, sBloc, refSignature);

    QueryPerformanceCounter(&tProcessBegin);
    for (int i = 1; i <= 26; ++i)
    {
        QueryPerformanceCounter(&tBegin);
        ostringstream stringStream; 
        stringStream << "E:/jee/cours/GEN5023/code/textile_images/Defect/" << i << ".TIF";
        
        Cell<const Mat> anotherMatrixCell(8,12);

        Mat markedImg(256, 384, aMatrixCell.get(0,0).type());
        
        ret = cutImage(anotherMatrixCell, stringStream.str(), sBloc, false, false);
    
        onlineDetection(anotherMatrixCell, sBloc, refSignature, 4, false, markedImg);

        QueryPerformanceCounter(&tEnd);
        elapsedTime = (tEnd.QuadPart - tBegin.QuadPart) * 1000.0 / frequency.QuadPart;
        printf("Elapsed time for image %d : %f\n", i, elapsedTime);
        /*
        ostringstream anotherStream;
        anotherStream << "E:/jee/cours/GEN5023/code/textile_images/results_bare/img-" << i << "-defect.tiff";
        bool success = imwrite(anotherStream.str(), markedImg);
        cout << anotherStream.str() << endl;
        printf("Success for image [%d] : %d \n", i, (int)success);
        */
    }
    QueryPerformanceCounter(&tProcessEnd);
    elapsedTime = (tProcessEnd.QuadPart - tProcessBegin.QuadPart) * 1000.0 / frequency.QuadPart;
    printf("Elapsed time for process : %f\n", elapsedTime);
    return 0;
}