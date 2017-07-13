#include "mex.h"
#include <cstdint>

class Image {
    public:
        int x_, y_, n_;
        uint8_t * rgb_;
        Image(const int x, const int y, const int n, const uint8_t & rgb) {
            x_ = x;
            y_ = y;
            n_ = n;
            rgb_ = &rgb;
        }


        Image(const int x, const int y, const int n) {
            x_ = x;
            y_ = y;
            n_ = n;
        }

        Image(const mxArray *input) {
            mxArray *m_ptr = mxGetField(input,0,"x");
            x_ = *(int)mxGetData(m_ptr);
            m_ptr = mxGetField(input,0,"y");
            y_ = *(int)mxGetData(m_ptr);
            m_ptr = mxGetField(input,0,"n");
            n_ = *(int)mxGetData(m_ptr);
            m_ptr = mxGetField(input,0,"rgb");
            rgb_ = *(int)mxGetData(m_ptr);
        }
}

class WorkOrder {
    public:
       int num_patches_;
       bool completed;
       int * x_, int * y_, int * theta_;
       Image * frame_;

       WorkOrder(const int n, const int * x, const int * y, const int * theta,
               const Image &in)
       {
           num_patches_ = n;
           x_ = x;
           y_ = y;
           theta_ = theta;
           frame_ = &in;
           completed = false;
       }

       WorkOrder(mxArray * in) {
            mxArray *m_ptr = mxGetField(input,0,"num_patches");
            num_patches_ = *(int)mxGetData(m_ptr);
            m_ptr = mxGetField(input,0,"x");
            x_ = *(int)mxGetData(m_ptr);
            m_ptr = mxGetField(input,0,"y");
            y_ = *(int)mxGetData(m_ptr);
            m_ptr = mxGetField(input,0,"n");
            n_ = *(int)mxGetData(m_ptr);
            m_ptr = mxGetField(input,0,"theta");
            theta_ = *(int)mxGetData(m_ptr);
       }
}

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )

    Image frame(prhs[0]);
    WorkOrder work(prhs[1]);


    
}
