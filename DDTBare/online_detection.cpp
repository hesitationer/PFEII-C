//--------------------------------------------------
// implementation file of online detection functions
//--------------------------------------------------

//----------------------------
// INCLUDES
//----------------------------
#include "online_detection.h"
#include "kim.h"
#include <opencv2/core/core.hpp>
//----------------------------
// NAMESPACES
//----------------------------
using namespace cv;
using namespace std;
using namespace utilities;

//----------------------------
// IMPLEMENTATION
//----------------------------

//****************************
// See header file for details
//****************************
int onlineDetection(MatrixRefCell& imageBlocks, int blockSize, vector<float>& refSign, float eta, bool overlap, Mat& markedImage)
{
    // overlap not implemented yet
    (void)overlap;

    int ret = 0;
    int blockRows = imageBlocks.rows;
    int blockCols = imageBlocks.cols;
    Mat_<float> distanceMat(blockRows, blockCols);

    // compute block signature and
    // calcul distance between ref
    // signature and block signature
    for (int i = 0; i < blockRows; ++i)
    {
        if (ret == 0)
        {
            for (int j = 0; j < blockCols; ++j)
            {
                const Mat& aBlock = imageBlocks.get(i, j);
                vector<float> aSign;
                extractKimSignature(aBlock, blockSize, blockSize, aSign);
            
                // compute euclididan distance
                if (aSign.size() == refSign.size())
                {
                    float signDistance = 0;
                    for (int n = 0; n < refSign.size(); ++n)
                    {
                        signDistance += abs(refSign[n] - aSign[n]);
                    }
                    distanceMat(i,j) = signDistance;
                }
                else // signatures sizes mismatch, error
                {
                    ret = 1;
                    break;
                }
            }
        }
        else
            break;
    }

    // compute alpha treshold
    Scalar distMean = mean(distanceMat);
    Cell<float> distCell(distanceMat);
    vector<float> distVector = distCell.getContent();

    float alpha = distMean.val[0] + (eta*iqr<float>(distVector));

    // mark defectious blocks
    for (int i = 0; i < blockRows; ++i)
    {
        for (int j = 0; j < blockCols; ++j)
        {
            if (distanceMat(i, j) > alpha)
            {
                const Mat& aBlock = imageBlocks.get(i, j);
                aBlock.copyTo( markedImage(Range(i*blockSize, (i + 1)*blockSize), Range(j*blockSize, (j + 1)*blockSize)));
            }
        }
    }

    return ret;
}