/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#ifndef __DEF_FFTS__
#define __DEF_FFTS__

#if (__cplusplus <= 199711L)
#define noexcept
#define nullptr NULL
#endif

#include "../../Ffts/include/ffts.h"
#include <Accelerate/Accelerate.h>

namespace ffts
{
    
    class FftsContext
    {
    private:
        ffts_plan_t* m_forward_plan;
        ffts_plan_t* m_backward_plan;
        size_t       m_size;
        
        void allocate(const size_t lenght)
        {
            m_forward_plan = ffts_init_1d_real(lenght, NEGATIVE_SIGN);
            m_backward_plan = ffts_init_1d_real(lenght, POSITIVE_SIGN);
        }
        
        void deallocate() noexcept
        {
            if(m_forward_plan)
            {
                ffts_free(m_forward_plan);
            }
            if(m_backward_plan)
            {
                ffts_free(m_backward_plan);
            }
        }
        
    public:
        
        inline FftsContext() noexcept : m_forward_plan(nullptr), m_backward_plan(nullptr), m_size(0ul) {}
        
        inline FftsContext(const size_t lenght) noexcept : m_forward_plan(nullptr), m_backward_plan(nullptr), m_size(0ul) {allocate(lenght);}
        
        inline ~FftsContext() noexcept { deallocate(); }
        
        inline void resize(const size_t lenght)
        {
            deallocate();
            allocate(lenght);
        }
        
        inline void forward(const float* input, float* complex) const noexcept
        {
            ffts_execute(m_forward_plan, input, complex);
        }
        
        inline void backward(const float* complex, float* output) const noexcept
        {
            ffts_execute(m_backward_plan, complex, output);
        }
    };
}

class FftaaContext
{
private:
    size_t m_size;
    size_t m_power_of_two;
    FFTSetup m_fft_Setup;
    
    void allocate(const size_t lenght)
    {
        m_size          = lenght;
        m_power_of_two = 0;
        while ((1 << m_power_of_two) < m_size)
        {
            ++m_power_of_two;
        }
        m_fft_Setup = vDSP_create_fftsetup(m_power_of_two, FFT_RADIX2);
    }
    
    void deallocate() noexcept
    {
        if(m_fft_Setup)
        {
            vDSP_destroy_fftsetup(m_fft_Setup);
        }
        m_size = 0;
        m_power_of_two = 0;
        m_fft_Setup = nullptr;
    }
    
public:
    inline FftaaContext() noexcept :  m_size(0ul), m_power_of_two(0ul), m_fft_Setup(nullptr) {}
    
    inline FftaaContext(const size_t lenght) noexcept :  m_size(0ul), m_power_of_two(0ul), m_fft_Setup(nullptr) {allocate(lenght);}
    
    inline  ~FftaaContext() noexcept { deallocate(); }
    
    inline void resize(const size_t lenght)
    {
        deallocate();
        allocate(lenght);
    }
    /*
    inline void forward(const float* input, float* complex) const noexcept
    {
        const size_t size2 = m_size / 2;
        DSPSplitComplex splitComplex;
        splitComplex.realp = complex;
        splitComplex.imagp = complex+size2;
        vDSP_ctoz(reinterpret_cast<const COMPLEX*>(input), 2, &splitComplex, 1, size2);
        vDSP_fft_zrip(m_fft_Setup, &splitComplex, 1,m_power_of_two, FFT_FORWARD);
        const float factor = 0.5f;
        vDSP_vsmul(re, 1, &factor, re, 1, size2);
        vDSP_vsmul(im, 1, &factor, im, 1, size2);
        re[size2] = im[0];
        im[0] = 0.0f;
        im[size2] = 0.0f;
    }
    
    inline void backward(const float* complex, float* output) const noexcept
    {
        const size_t size2 =m_size / 2;
        ::memcpy(_re.data(), re, size2 * sizeof(float));
        ::memcpy(_im.data(), im, size2 * sizeof(float));
        _im[0] = re[size2];
        DSPSplitComplex splitComplex;
        splitComplex.realp = _re.data();
        splitComplex.imagp = _im.data();
        vDSP_fft_zrip(m_fft_Setup, &splitComplex, 1,m_power_of_two, FFT_INVERSE);
        vDSP_ztoc(&splitComplex, 1, reinterpret_cast<COMPLEX*>(output), 2, size2);
        const float factor = 1.0f / static_cast<float>(_size);
        vDSP_vsmul(data, 1, &factor, output, 1,m_size);
    }
     */
};

#endif
