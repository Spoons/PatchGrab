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
            const size_t dim_array[3]={y,x,n};
            m_rgb_ = mxCreateNumericArray(3, dim_array, mxUINT8_CLASS, mxREAL);
            rgb_ = (uint8_t*)mxGetData(m_rgb_);
        }

        Image(const mxArray *input) {
            const size_t * dims = mxGetDimensions(input);
            y_ = dims[0];
            x_ = dims[1];
            n_ = dims[2];
            m_rgb_ = input;
            rgb_ = (uint8_t*)mxGetData(input);

        }

        inline void SetPixel(int x, int y, const Image &other, int x_other, int y_other) {

            int this_frame_size = x_ * y_;
            int this_pixel_position = (x * y_) + y;

            int other_frame_size = other.x_ * other.y_;
            int other_pixel_position = x_other*(other.y_) + y_other;

            rgb_[this_pixel_position] = other.rgb_[other_pixel_position];
            rgb_[this_pixel_position + this_frame_size] =
                other.rgb_[other_pixel_position + other_frame_size];
            rgb_[this_pixel_position + 2*this_frame_size] =
                other.rgb_[other_pixel_position + 2*other_frame_size];
        }
};

//! WorkOrder class represents a list of patches to be grabbed from a specific
//frame. It is always initialized from an mxarray pointer to the workorder
//matlab structure.
class WorkOrder {
    public:
       int num_patches_;
       bool completed;
       int *x_, *y_, *xsize, *ysize, *theta_;
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

            m_ptr = mxGetField(input,0,"xsize");
            xsize_ = (int*)mxGetData(m_ptr);


            m_ptr = mxGetField(input,0,"ysize");
            ysize_ = (int*)mxGetData(m_ptr);
            
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

int grabPatches(Image frame, const WorkOrder &work, Results &results) {

    int num_patches = work.num_patches_;

    int i;


    //uint8_t * frame_rgb = frame.rgb_;
    //int x_frame_size = frame.x_;
    //int y_frame_size = frame.y_;

    for (int i = 0; i < num_patches; ++i) {
        double theta = work.theta_[i]*(M_PI/180);
        int x_size_half = work.xsize_[i]/2;
        int y_size_half = work.ysize_[i]/2;

        int x_size = work.xsize_[i];
        int y_size = work.ysize_[i];

        //int x_pos_half = work.x_[i]/2;
        //int y_pos_half = work.y_[i]/2;

        int x_pos = work.x_[i];
        int y_pos = work.y_[i];

        //uint8_t *patch_rgb = results.patches[i].rgb_;

        for (int y = y_pos-y_size_half; y < y_pos+y_size_half; ++y) {
            for(int x = x_pos-x_size_half; x < x_pos+x_size_half; ++x) {
                int tx = x;
                int ty = y;

                int xdif = x_pos - x;
                int ydif = y_pos - y;

                int rtx = x_pos + (-xdif * cos(theta) - ydif * sin(theta));
                int rty = y_pos + (-ydif * cos(theta) + xdif * sin(theta));

                tx = tx - x_pos + x_size_half;
                ty = ty - y_pos + y_size_half;

                if(rtx < 0 || rty < 0 || ty < 0 || tx < 0 || tx >= x_size || ty
                        >= y_size || rtx >= x_frame_size || rty >= y_frame_size) {
                    continue;

                }
                

                results.patches[i].SetPixel(rtx, rty, frame, tx, ty);

            }

        }

    }

    return 0;
}

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
    //! Creates an image object from a matlab image
    Image frame(prhs[0]);

    //! Initialized the workorder object using the workorder struct from matlab
    WorkOrder work(prhs[1]);

    //! Inializing results with the workload object will allocate memory in the
    //results vector patches
    Results result(work);

    //This method actually perfoms the patch grabbing
    grabPatches(frame, work, result);
}


