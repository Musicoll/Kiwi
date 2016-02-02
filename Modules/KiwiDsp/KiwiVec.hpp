/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#ifndef __DEF_OSP_VEC__
#define __DEF_OSP_VEC__

#if (__cplusplus <= 199711L)
#define noexcept
#define nullptr NULL
#define constexpr
#endif

#include <algorithm>

namespace osp
{
    // ==================================================================================== //
    //                                          VEC                                         //
    // ==================================================================================== //
    //! @brief The methods.
    //! @details The class offers a set of optimized operations for vectors of samples.
    template < typename T > class Vec
    {
    public:
        //! @brief Allocates a vector.
        //! @details Allocates an aligned vector of samples and clears its content.
        //! @param size  The size of the vector.
        //! @return A pointer to a vector or a null pointer if the allocation failed.
        static inline T* allocate(const size_t size) noexcept
        {
#ifdef __APPLE__
            T* vec = (T *)std::malloc(size * sizeof(T));
#elif _WINDOWS
            T* vec = (T *)_aligned_malloc(size * sizeof(T), (size_t)pow(float(2), int(sizeof(T))));
#else
            T* vec = (T *)memalign((size_t)pow(2, sizeof(T)), size * sizeof(T));
#endif
            if(vec) {clear(size, vec);}
            return vec;
        }
        
        //! @brief Frees a vector.
        //! @details Frees a vector and returns a null pointer.
        //! @param vec A pointer to a vector.
        //! @return A null pointer.
        static inline T* release(T* vec) noexcept
        {
            if(vec)
#ifdef __APPLE__
            {std::free(vec);}
#elif _WINDOWS
            {_aligned_free(vec);}
#else
            {std::free(vec);}
#endif
            return nullptr;
        }
        
        //! @brief Clears a vector.
        //! @details Sets all the content of a vector to zero.
        //! @param size The size of the vector.
        //! @param vec  The pointer to a vector.
        static inline void clear(size_t const size, T* vec) noexcept
        {
            memset(vec, 0, size * sizeof(T));
        }
        
        //! @brief Fills a vector with a value.
        //! @details Sets all the content of a vector to a value.
        //! @param  size  The size of the vector.
        //! @param value The value.
        //! @param in    The input vector.
        static inline void fill(size_t const size, T const& value, T* in) noexcept
        {
            const T vec[8] = {value, value, value, value, value, value, value, value};
            for(size_t i = size>>3; i; --i, in += 8)
            {
                memcpy(in, vec, 8 * sizeof(T));
            }
            if(size&7)
            {
                memcpy(in, vec, (size&7) * sizeof(T));
            }
        }
        
        //! @brief Swaps two vectors.
        //! @details Swaps the content of two vectors. The size must be inferior or equal to
        //! @details the size of both vectors.
        //! @param size The size of the vectors.
        //! @param in1  The first vector.
        //! @param in2  The second vector.
        static inline void swap(const size_t size, T* in1, T* in2) noexcept
        {
            std::swap_ranges(in1, in1+size, in2);
        }
        
        //! @brief Copies a vector into another.
        //! @details Copies the content of a vector into another. The size must be inferior or
        //! @details equal to the size of both vectors.
        //! @param size The size of the vectors.
        //! @param in   The input vector.
        //! @param out  The output vector.
        static inline void copy(const size_t size, const T* in, T* out) noexcept
        {
            std::copy(in, in+size, out);
        }
        
        //! @brief Adds a vector to another.
        //! @details Adds the content of a vector to another vector. The size must be inferior
        //! @details or equal to the size of both vectors.
        //! @param size The size of the vectors.
        //! @param in   The input vector.
        //! @param out  The output vector.
        static inline void add(const size_t size, const T* in, T* out) noexcept
        {
            for(size_t i = size>>3; i; --i, in += 8, out += 8)
            {
                out[0] += in[0]; out[1] += in[1]; out[2] += in[2]; out[3] += in[3];
                out[4] += in[4]; out[5] += in[5]; out[6] += in[6]; out[7] += in[7];
            }
            for(size_t i = size&7; i; --i, in++, out++)
            {
                out[0] += in[0];
            }
        }
        
        //! @brief Adds two vectors and leaves the result in another.
        //! @details Adds the contents of two vectors and leaves the result in another. The
        //! @details size must be inferior or equal to the size of all vectors.
        //! @param size The size of the vectors.
        //! @param in1  The first input vector.
        //! @param in2  The second input vector.
        //! @param out  The output vector.
        static inline void add(const size_t size, const T* in1, const T* in2, T* out) noexcept
        {
            for(size_t i = size>>3; i; --i, in1 += 8, in2 += 8, out += 8)
            {
                const T f0 = in1[0], f1 = in1[1], f2 = in1[2], f3 = in1[3];
                const T f4 = in1[4], f5 = in1[5], f6 = in1[6], f7 = in1[7];
                out[0] = f0 + in2[0]; out[1] = f1 + in2[1];
                out[2] = f2 + in2[2]; out[3] = f3 + in2[3];
                out[4] = f4 + in2[4]; out[5] = f5 + in2[5];
                out[6] = f6 + in2[6]; out[7] = f7 + in2[7];
            }
            for(size_t i = size&7; i; --i, in1++, in2++, out++)
            {
                out[0] = in1[0] + in2[0];
            }
        }
        
        //! @brief Adds a scalar to a vector.
        //! @details Adds a scalar to the content of a vector.
        //! @param size  The size of the vector.
        //! @param value The value.
        //! @param in    The vector.
        static inline void add(const size_t size, T const& value, T* in) noexcept
        {
            for(size_t i = size>>3; i; --i, in += 8)
            {
                in[0] += value; in[1] += value; in[2] += value; in[3] += value;
                in[4] += value; in[5] += value; in[6] += value; in[7] += value;
            }
            for(size_t i = size&7; i; --i, in++)
            {
                in[0] += value;
            }
        }
        
        //! @brief Adds a scalar to a vector and leaves the result in another.
        //! @details Adds a scalar to the content of a vector and leaves the result in
        //! @details another. The size must be inferior or equal to the size of both vectors.
        //! @param size  The size of the vectors.
        //! @param value The value.
        //! @param in    The vector.
        //! @param out   The output vector.
        static inline void add(const size_t size, T const& value, const T* in, T* out) noexcept
        {
            for(size_t i = size>>3; i; --i, in += 8, out += 8)
            {
                out[0] = value + in[0]; out[1] = value + in[1];
                out[2] = value + in[2]; out[3] = value + in[3];
                out[4] = value + in[4]; out[5] = value + in[5];
                out[6] = value + in[6]; out[7] = value + in[7];
                
            }
            for(size_t i = size&7; i; --i, in++, out++)
            {
                out[0] = value + in[0];
            }
        }
        
        //! @brief Subtracts a vector to another.
        //! @details Subtracts the content of a vector to another vector. The size must be
        //! @details inferior or equal to the size of both vectors.
        //! @param size The size of the vectors.
        //! @param in   The input vector.
        //! @param out  The output vector.
        static inline void sub(const size_t size, const T* in, T* out) noexcept
        {
            for(size_t i = size>>3; i; --i, in += 8, out += 8)
            {
                out[0] -= in[0]; out[1] -= in[1]; out[2] -= in[2]; out[3] -= in[3];
                out[4] -= in[4]; out[5] -= in[5]; out[6] -= in[6]; out[7] -= in[7];
            }
            for(size_t i = size&7; i; --i, in++, out++)
            {
                out[0] -= in[0];
            }
        }
        
        //! @brief Subtracts two vectors and leaves the result in another.
        //! @details Subtracts the contents of two vectors and leaves the result in another.
        //! @details The size must be inferior or equal to the size of all vectors.
        //! @param size The size of the vectors.
        //! @param in1  The first input vector.
        //! @param in2  The second input vector.
        //! @param out  The output vector.
        static inline void sub(const size_t size, const T* in1, const T* in2, T* out) noexcept
        {
            for(size_t i = size>>3; i; --i, in1 += 8, in2 += 8, out += 8)
            {
                const T f0 = in1[0], f1 = in1[1], f2 = in1[2], f3 = in1[3];
                const T f4 = in1[4], f5 = in1[5], f6 = in1[6], f7 = in1[7];
                out[0] = f0 - in2[0]; out[1] = f1 - in2[1];
                out[2] = f2 - in2[2]; out[3] = f3 - in2[3];
                out[4] = f4 - in2[4]; out[5] = f5 - in2[5];
                out[6] = f6 - in2[6]; out[7] = f7 - in2[7];
            }
            for(size_t i = size&7; i; --i, in1++, in2++, out++)
            {
                out[0] = in1[0] - in2[0];
            }
        }
        
        //! @brief Subtracts a scalar to a vector.
        //! @details Subtracts a scalar to the content of a vector.
        //! @param size  The size of the vector.
        //! @param value The value.
        //! @param in    The vector.
        static inline void sub(const size_t size, T const& value, T* in) noexcept
        {
            for(size_t i = size>>3; i; --i, in += 8)
            {
                in[0] -= value; in[1] -= value; in[2] -= value; in[3] -= value;
                in[4] -= value; in[5] -= value; in[6] -= value; in[7] -= value;
            }
            for(size_t i = size&7; i; --i, in++)
            {
                in[0] -= value;
            }
        }
        
        //! @brief Subtracts a scalar to a vector and leaves the result in another.
        //! @details Subtracts a scalar to the content of a vector and leaves the result in
        //! @details another. The size must be inferior or equal to the size of both vectors.
        //! @param size  The size of the vectors.
        //! @param value The value.
        //! @param in    The vector.
        //! @param out   The output vector.
        static inline void sub(const size_t size, T const& value, const T* in, T* out) noexcept
        {
            for(size_t i = size>>3; i; --i, in += 8, out += 8)
            {
                out[0] = in[0] - value; out[1] = in[1] - value;
                out[2] = in[2] - value; out[3] = in[3] - value;
                out[4] = in[4] - value; out[5] = in[5] - value;
                out[6] = in[6] - value; out[7] = in[7] - value;
                
            }
            for(size_t i = size&7; i; --i, in++, out++)
            {
                out[0] = in[0] - value;
            }
        }
        
        //! @brief Multiplies a vector to another.
        //! @details Multiplies the content of a vector to another vector. The size must be
        //! @details inferior or equal to the size of both vectors.
        //! @param size The size of the vectors.
        //! @param in   The input vector.
        //! @param out  The output vector.
        static inline void mul(const size_t size, const T* in, T* out) noexcept
        {
            for(size_t i = size>>3; i; --i, in += 8, out += 8)
            {
                out[0] *= in[0]; out[1] *= in[1]; out[2] *= in[2]; out[3] *= in[3];
                out[4] *= in[4]; out[5] *= in[5]; out[6] *= in[6]; out[7] *= in[7];
            }
            for(size_t i = size&7; i; --i, in++, out++)
            {
                out[0] *= in[0];
            }
        }
        
        //! @brief Multiplies two vectors and leaves the result in another.
        //! @details Multiplies the contents of two vectors and leaves the result in another.
        //! @details The size must be inferior or equal to the size of all vectors.
        //! @param size The size of the vectors.
        //! @param in1  The first input vector.
        //! @param in2  The second input vector.
        //! @param out  The output vector.
        static inline void mul(const size_t size, const T* in1, const T* in2, T* out) noexcept
        {
            for(size_t i = size>>3; i; --i, in1 += 8, in2 += 8, out += 8)
            {
                const T f0 = in1[0], f1 = in1[1], f2 = in1[2], f3 = in1[3];
                const T f4 = in1[4], f5 = in1[5], f6 = in1[6], f7 = in1[7];
                out[0] = f0 * in2[0]; out[1] = f1 * in2[1];
                out[2] = f2 * in2[2]; out[3] = f3 * in2[3];
                out[4] = f4 * in2[4]; out[5] = f5 * in2[5];
                out[6] = f6 * in2[6]; out[7] = f7 * in2[7];
            }
            for(size_t i = size&7; i; --i, in1++, in2++, out++)
            {
                out[0] = in1[0] * in2[0];
            }
        }
        
        //! @brief Multiplies a scalar to a vector.
        //! @details Multiplies a scalar to the content of a vector.
        //! @param size  The size of the vector.
        //! @param value The value.
        //! @param in    The vector.
        static inline void mul(const size_t size, T const& value, T* in) noexcept
        {
            for(size_t i = size>>3; i; --i, in += 8)
            {
                in[0] *= value; in[1] *= value; in[2] *= value; in[3] *= value;
                in[4] *= value; in[5] *= value; in[6] *= value; in[7] *= value;
            }
            for(size_t i = size&7; i; --i, in++)
            {
                in[0] *= value;
            }
        }
        
        //! @brief Multiplies a scalar to a vector and leaves the result in another.
        //! @details Multiplies a scalar to the content of a vector and leaves the result in
        //! @details another. The size must be inferior or equal to the size of both vectors.
        //! @param size  The size of the vectors.
        //! @param value The value.
        //! @param in    The vector.
        //! @param out   The output vector.
        static inline void mul(const size_t size, T const& value, const T* in, T* out) noexcept
        {
            for(size_t i = size>>3; i; --i, in += 8, out += 8)
            {
                out[0] = in[0] * value; out[1] = in[1] * value;
                out[2] = in[2] * value; out[3] = in[3] * value;
                out[4] = in[4] * value; out[5] = in[5] * value;
                out[6] = in[6] * value; out[7] = in[7] * value;
                
            }
            for(size_t i = size&7; i; --i, in++, out++)
            {
                out[0] = in[0] * value;
            }
        }
        
        //! @brief Divides a vector to another.
        //! @details Divides the content of a vector by another vector. The size must be
        //! @details inferior or equal to the size of both vectors.
        //! @param size The size of the vectors.
        //! @param in   The input vector.
        //! @param out  The output vector.
        static inline void div(const size_t size, const T* in, T* out) noexcept
        {
            for(size_t i = size>>3; i; --i, in += 8, out += 8)
            {
                out[0] /= in[0]; out[1] /= in[1]; out[2] /= in[2]; out[3] /= in[3];
                out[4] /= in[4]; out[5] /= in[5]; out[6] /= in[6]; out[7] /= in[7];
            }
            for(size_t i = size&7; i; --i, in++, out++)
            {
                out[0] /= in[0];
            }
        }
        
        //! @brief Divides two vectors and leaves the result in another.
        //! @details Divides the contents of two vectors and leaves the result in another.
        //! @details The size must be inferior or equal to the size of all vectors.
        //! @param size The size of the vectors.
        //! @param in1  The first input vector.
        //! @param in2  The second input vector.
        //! @param out  The output vector.
        static inline void div(const size_t size, const T* in1, const T* in2, T* out) noexcept
        {
            for(size_t i = size>>3; i; --i, in1 += 8, in2 += 8, out += 8)
            {
                const T f0 = in1[0], f1 = in1[1], f2 = in1[2], f3 = in1[3];
                const T f4 = in1[4], f5 = in1[5], f6 = in1[6], f7 = in1[7];
                out[0] = f0 / in2[0]; out[1] = f1 / in2[1];
                out[2] = f2 / in2[2]; out[3] = f3 / in2[3];
                out[4] = f4 / in2[4]; out[5] = f5 / in2[5];
                out[6] = f6 / in2[6]; out[7] = f7 / in2[7];
            }
            for(size_t i = size&7; i; --i, in1++, in2++, out++)
            {
                out[0] = in1[0] / in2[0];
            }
        }
        
        //! @brief Divides a scalar to a vector.
        //! @details Divides a scalar to the content of a vector.
        //! @param size  The size of the vector.
        //! @param value The value.
        //! @param in    The vector.
        static inline void div(const size_t size, T const& value, T* in) noexcept
        {
            for(size_t i = size>>3; i; --i, in += 8)
            {
                in[0] /= value; in[1] /= value; in[2] /= value; in[3] /= value;
                in[4] /= value; in[5] /= value; in[6] /= value; in[7] /= value;
            }
            for(size_t i = size&7; i; --i, in++)
            {
                in[0] /= value;
            }
        }
        
        //! @brief Divides a scalar to a vector and leaves the result in another.
        //! @details Divides a scalar to the content of a vector and leaves the result in
        //! @details another. The size must be inferior or equal to the size of both vectors.
        //! @param size  The size of the vectors.
        //! @param value The value.
        //! @param in    The vector.
        //! @param out   The output vector.
        static inline void div(const size_t size, T const& value, const T* in, T* out) noexcept
        {
            for(size_t i = size>>3; i; --i, in += 8, out += 8)
            {
                out[0] = in[0] / value; out[1] = in[1] / value;
                out[2] = in[2] / value; out[3] = in[3] / value;
                out[4] = in[4] / value; out[5] = in[5] / value;
                out[6] = in[6] / value; out[7] = in[7] / value;
                
            }
            for(size_t i = size&7; i; --i, in++, out++)
            {
                out[0] = in[0] / value;
            }
        }
        
        //! @brief Computes the dot product of two vectors.
        //! @details Computes the dot product of the content of two vectors. The size must be
        //! @details inferior or equal to the size of both vectors.
        //! @param   size   The size of the vectors.
        //! @param   in1    The first vector.
        //! @param   in2    The second vector.
        //! @return The dot product of the two vectors.
        static inline T dot(const size_t size, const T* in1, const T* in2) noexcept
        {
            T result = 0;
            for(size_t i = size>>3; i; --i, in1 += 8, in2 += 8)
            {
                result += in1[0] * in2[0]; result += in1[1] * in2[1];
                result += in1[2] * in2[2]; result += in1[3] * in2[3];
                result += in1[4] * in2[4]; result += in1[5] * in2[5];
                result += in1[6] * in2[6]; result += in1[7] * in2[7];
            }
            for(size_t i = size&7; i; --i, in1++, in2++)
            {
                result += in1[0] * in2[0];
            }
            return result;
        }
    };
}

#if (__cplusplus <= 199711L)
#undef noexcept
#undef nullptr NULL
#undef constexpr
#endif

#endif
