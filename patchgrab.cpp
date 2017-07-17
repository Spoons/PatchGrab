#include "mex.h"
#include <cstdint>
#include <vector>

//! Image class represents a matlab image
// Preferred to be iniatilized using a mxArray pointer.
// Alternatively, if inited by x,y,n, then image class
// will allocate memory on the matlab stack for you.
class Image {
    public:
        int x_, y_, n_;
        uint8_t * rgb_;
        const mxArray * m_rgb_;
        Image(const int x, const int y, const int n, uint8_t * rgb) {
            x_ = x;
            y_ = y;
            n_ = n;
            rgb_ = rgb;
        }


        Image(const int x, const int y, const int n) {
            x_ = x;
            y_ = y;
            n_ = n;
            const int dim_array[3]={y,x,n};
            m_rgb_ = mxCreateNumericArray(3, dim_array, mxUINT8_CLASS, mxREAL);
            rgb_ = (uint8_t*)mxGetData(m_rgb_);
        }

        Image(const mxArray *input) {
            const int * dims = mxGetDimensions(input);
            y_ = dims[0];
            x_ = dims[1];
            n_ = dims[2];
            m_rgb_ = input;
            rgb_ = (uint8_t*)mxGetData(input);

        }
};

//! WorkOrder class represents a list of patches to be grabbed from a specific
//frame. It is always initialized from an mxarray pointer to the workorder
//matlab structure.
class WorkOrder {
    public:
       int num_patches_;
       bool completed;
       int * x_, * y_, * theta_;
       Image * frame_;

       WorkOrder(int num_patch, int * x, int * y, int * theta,
               Image &in)
       {
           num_patches_ = num_patch;
           x_ = x;
           y_ = y;
           theta_ = theta;
           frame_ = &in;
           completed = false;
       }

       WorkOrder(const mxArray * input) {
			mxArray * m_ptr = mxGetField(input,0,"x");
			num_patches_ = mxGetM(m_ptr);
            x_ = (int*)mxGetData(m_ptr);
            m_ptr = mxGetField(input,0,"y");
            y_ = (int*)mxGetData(m_ptr);
            m_ptr = mxGetField(input,0,"theta");
            theta_ = (int*)mxGetData(m_ptr);
       }

};

//! Results class represents a memory space for the returned patches to be
//stored. Always initialize results with workorder and the associated results
//class will store the results in its vector array.
class Results {
    public:
        int num_results_;
        bool ready;
        WorkOrder * work_order_; 
        std::vector<Image> patches;

        Results(WorkOrder & in) {
            work_order_ = &in;
            num_results_ = in.num_patches_;

            patches.reserve(num_results_);
            for (int i = 0; i < num_results_; ++i) {
                patches.push_back(Image(in.x_[i], in.y_[i], 3)); 
            }   
        }
};

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
    //! Creates an image object from a matlab image
    Image frame(prhs[0]);

    //! Initialized the workorder object using the workorder struct from matlab
    WorkOrder work(prhs[1]);

    //! Inializing results with the workload object will allocate memory in the
    //results vector patches
    Results patches(work);
}
