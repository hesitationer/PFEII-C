#ifndef H_UTILITIES
#define H_UTILITIES
#include <opencv2/core/core.hpp>

//***********************************************************
// Utility classes/sctructures used to make some Matlab magic
//***********************************************************
//----------------------
// DEFINES
//----------------------
// 10 digits Pi
#define ddtpi 3.1415926535f

// Pi/2N coeff for 2D DCT 8x8 
#define ROW_COEF ddtpi/16.0f
#define COL_COEF ddtpi/16.0f 

// 2D DCT Matrix dims
#define ROW_NUMBER 8
#define COL_NUMBER 8
// 2D DCT Matrix size (8x8)
#define DCT_MATRIX_SIZE 64

namespace utilities
{
    //********************************
    // Interquartile range of a vector
    //********************************
    template <typename T>
    T iqr(std::vector<T>& src);

    //********************************
    // 2D DCT of a 8x8 matrix
    //********************************
    float computeDCTCoef(int u, int v, const cv::Mat_<float>& aMatrix);

    void computeDCT2(const cv::Mat_<float>& srcMatrix, cv::Mat_<float>& dstMatrix);

    float alpha(int i);

    //**********************************************
    // Cell class is lightweight and simple 2D 
    // container indexed by integers, like a matrix.
    // data are holded by a std vector
    //**********************************************
	template <class T>
	class Cell
	{
	public:
		// constructor
		Cell<T>(int rows, int cols);

        // copy fro cv::Mat constructor
        Cell<T>(cv::Mat& src);

		// destructor, clear
		~Cell();

		// (i,j) cell accessor
		const T& operator()(int rowIdx, int colIdx) const;

		// common cell accessor
		const T& get(int rowIdx, int colIdx) const;

        // set value of a cell
        void set(int rowIdx, int colIdx, const T& data);

        int rows; //<! rows number
        int cols; //<! cols number
        const std::vector<T>& getContent() const {return content;} //<! a raw reference to content

	private:
		int size; //<! number of elements in cell (cols*rows)
		std::vector<T> content; //<! cell content holder
    };
} // namespace utilities
#endif //H_UTILITIES