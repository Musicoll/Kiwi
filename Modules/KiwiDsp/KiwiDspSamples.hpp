//
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//

#ifndef KIWI_DSP_SAMPLES_H_INCLUDED
#define KIWI_DSP_SAMPLES_H_INCLUDED

#include "KiwiDspMisc.hpp"

namespace kiwi
{
namespace dsp
{
    // ==================================================================================== //
    //                                      SAMPLES                                         //
    // ==================================================================================== //
    //! @brief A set of static methods to manage vectors and matrices of samples.
    //! @details The class offers a set of optimized operations on vector and matrices
    //! @details samples. The methods are organized according to sections : memory, move,
    //! @details algebrica, ect.
    template < typename Type > class Samples
    {
        static_assert(std::is_floating_point< Type >::value, "The type of sample_t must be floating point.");
        static_assert(!std::is_const< Type >::value, "The type of sample_tcan't be constant.");
    public:
        
        // ================================================================================ //
        //                                      TOOLS                                       //
        // ================================================================================ //
        
        //! @brief Gets if a size is a power of two.
        //! @param size The size.
        //! @return true if the size if a power of two, otherwise false.
        static inline constexpr bool isPowerOfTwo(const size_t size) noexcept
        {
            return size && ((size & (size - 1)) == 0);
        }
        
        //! @brief Gets ithe alignment of a vector.
        //! @return The alignment in bits.
        static inline constexpr size_t getAlignment() noexcept
        {
            return static_cast< size_t >(std::pow(2ul, sizeof(Type)));
        }
        
        // ================================================================================ //
        //                                      MEMORY                                      //
        // ================================================================================ //
        
        //! @brief Allocates a vector of samples.
        //! @details Allocates an aligned vector of samples.
        //! @param size  The size of the vector that must be a power of 2 and non-null.
        //! @return A pointer to a vector or a null pointer if the allocation failed.
        //! @todo Uncomment the windows version
        static Type* allocate(const size_t size)
        {
            assert("The size of a vector of samples must be non-null and a power of 2." && isPowerOfTwo(size));
#ifdef __APPLE__
            return static_cast< Type* >(std::malloc(size * sizeof(Type)));
#elif _WINDOWS
            return static_cast< Type* >(_aligned_malloc( size * sizeof(Type), std::pow(2ul, sizeof(Type)) ));
#else
            return static_cast< Type* >(memalign(std::pow(2ul, sizeof(Type)), size * sizeof(Type)));
#endif
        }
        
        //! @brief Frees a vector of samples.
        //! @details Frees a vector and returns a null pointer.
        //! @param vec A pointer to a vector.
        //! @return A null pointer.
        //! @todo Uncomment the windows version
        static Type* release(Type* vec)
        {
            if(vec)
#ifdef _WINDOWS
            {
                _aligned_free(vec);
            }
#else
            {
                std::free(vec);
            }
#endif
            return nullptr;
        }
        
        // ================================================================================ //
        //                                      INITIALIZATION                              //
        // ================================================================================ //
        
        //! @brief Clears a vector of samples.
        //! @details Sets alla vector to zero.
        //! @param size The size of the vector.
        //! @param vec  The pointer to a vector.
        static inline void clear(size_t const size, Type* vec) noexcept
        {
            assert("The vector can't be null." && vec != nullptr);
            memset(vec, 0, size * sizeof(Type));
        }
        
        //! @brief Fills a vector with a value.
        //! @details Sets alla vector to a value.
        //! @param size  The size of the vector.
        //! @param value The value.
        //! @param in    The input vector.
        static inline void fill(size_t const size, Type const& value, Type* in) noexcept
        {
            assert("The vector can't be null." && in != nullptr);
            const Type vec[8] = {value, value, value, value, value, value, value, value};
            for(size_t i = size>>3; i; --i, in += 8)
            {
                memcpy(in, vec, 8 * sizeof(Type));
            }
            for(size_t i = size&7; i; --i, in++)
            {
                in[0] = value;
            }
        }
        
        // ================================================================================ //
        //                                          MOVE                                    //
        // ================================================================================ //
        
        //! @brief Swaps two vectors.
        //! @details Swapstwo vectors. The size must be inferior or equal to the size of both
        //! @details vectors.
        //! @param size The size of the vectors.
        //! @param in1  The first vector.
        //! @param in2  The second vector.
        static inline void swap(const size_t size, Type* in1, Type* in2) noexcept
        {
            assert("The vector can't be null." && in1 != nullptr && in2 != nullptr);
            std::swap_ranges(in1, in1+size, in2);
        }
        
        //! @brief Copies a vector into another.
        //! @details Copiesa vector into another. The size must be inferior or equal to the
        //! @details size of both vectors.
        //! @param size The size of the vectors.
        //! @param in   The input vector.
        //! @param out  The output vector.
        static inline void copy(const size_t size, const Type* in, Type* out) noexcept
        {
            assert("The vector can't be null." && in != nullptr && out != nullptr);
            std::copy(in, in+size, out);
        }
        
        // ================================================================================ //
        //                                      ALGEBRA                                     //
        // ================================================================================ //
        
        //! @brief Adds a vector to another.
        //! @details Addsa vector to another vector. The size must be inferior or equal to the
        //! @details size of both vectors.
        //! @param size The size of the vectors.
        //! @param in   The input vector.
        //! @param out  The output vector.
        static inline void add(const size_t size, const Type* in, Type* out) noexcept
        {
            assert("The vector can't be null." && in != nullptr && out != nullptr);
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
        //! @details Adds two vectors and leaves the result in another. The size must be
        //! @details inferior or equal to the size of all vectors.
        //! @param size The size of the vectors.
        //! @param in1  The first input vector.
        //! @param in2  The second input vector.
        //! @param out  The output vector.
        static inline void add(const size_t size, const Type* in1, const Type* in2, Type* out) noexcept
        {
            assert("The vector can't be null." && in1 != nullptr && in2 != nullptr && out != nullptr);
            for(size_t i = size>>3; i; --i, in1 += 8, in2 += 8, out += 8)
            {
                const Type f0 = in1[0], f1 = in1[1], f2 = in1[2], f3 = in1[3];
                const Type f4 = in1[4], f5 = in1[5], f6 = in1[6], f7 = in1[7];
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
        //! @details Adds a scalar toa vector.
        //! @param size  The size of the vector.
        //! @param value The value.
        //! @param in    The vector.
        static inline void add(const size_t size, Type const& value, Type* in) noexcept
        {
            assert("The vector can't be null." && in != nullptr);
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
        //! @details Adds a scalar toa vector and leaves the result in another. The size must
        //! @details be inferior or equal to the size of both vectors.
        //! @param size  The size of the vectors.
        //! @param value The value.
        //! @param in    The vector.
        //! @param out   The output vector.
        static inline void add(const size_t size, Type const& value, const Type* in, Type* out) noexcept
        {
            assert("The vector can't be null." && in != nullptr && out != nullptr);
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
        
        //! @brief Subtracts a vector by another.
        //! @details Subtracts a vector by another vector. The size must be inferior or equal
        //! @details to the size of both vectors.
        //! @param size The size of the vectors.
        //! @param in   The input vector.
        //! @param out  The output vector.
        static inline void sub(const size_t size, const Type* in, Type* out) noexcept
        {
            assert("The vector can't be null." && in != nullptr && out != nullptr);
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
        
        //! @brief Subtracts a vector by another and leaves the result in another.
        //! @details Subtracts a vector by another and leaves the result in third one. The
        //! @details size must be inferior or equal to the size of all vectors.
        //! @param size The size of the vectors.
        //! @param in1  The first input vector.
        //! @param in2  The second input vector.
        //! @param out  The output vector.
        static inline void sub(const size_t size, const Type* in1, const Type* in2, Type* out) noexcept
        {
            assert("The vector can't be null." && in1 != nullptr && in2 != nullptr && out != nullptr);
            for(size_t i = size>>3; i; --i, in1 += 8, in2 += 8, out += 8)
            {
                const Type f0 = in1[0], f1 = in1[1], f2 = in1[2], f3 = in1[3];
                const Type f4 = in1[4], f5 = in1[5], f6 = in1[6], f7 = in1[7];
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
        
        //! @brief Subtracts a vector by a scalar.
        //! @details Subtracts a vector by a scalar. The size must be inferior or equal to the
        //! @details size of the vector.
        //! @param size  The size of the vector.
        //! @param value The value.
        //! @param in    The vector.
        static inline void sub(const size_t size, Type const& value, Type* in) noexcept
        {
            assert("The vector can't be null." && in != nullptr);
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
        
        //! @brief Subtracts a vector by a scalar and leaves the result in another.
        //! @details Subtracts a vector by a scalar and leaves the result in another. The size
        //! @details must be inferior or equal to the size of both vectors.
        //! @param size  The size of the vectors.
        //! @param value The value.
        //! @param in    The vector.
        //! @param out   The output vector.
        static inline void sub(const size_t size, Type const& value, const Type* in, Type* out) noexcept
        {
            assert("The vector can't be null." && in != nullptr && out != nullptr);
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
        
        //! @brief Multiplies two vectors.
        //! @details Multiplies two vectors. The size must be inferior or equal to the size of
        //! @details both vectors.
        //! @param size The size of the vectors.
        //! @param in   The input vector.
        //! @param out  The output vector.
        static inline void mul(const size_t size, const Type* in, Type* out) noexcept
        {
            assert("The vector can't be null." && in != nullptr && out != nullptr);
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
        //! @details Multiplies two vectors and leaves the result in another. The size must be
        //! @details inferior or equal to the size of all vectors.
        //! @param size The size of the vectors.
        //! @param in1  The first input vector.
        //! @param in2  The second input vector.
        //! @param out  The output vector.
        static inline void mul(const size_t size, const Type* in1, const Type* in2, Type* out) noexcept
        {
            assert("The vector can't be null." && in1 != nullptr && in2 != nullptr && out != nullptr);
            for(size_t i = size>>3; i; --i, in1 += 8, in2 += 8, out += 8)
            {
                const Type f0 = in1[0], f1 = in1[1], f2 = in1[2], f3 = in1[3];
                const Type f4 = in1[4], f5 = in1[5], f6 = in1[6], f7 = in1[7];
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
        
        //! @brief Multiplies a vector by a scalar.
        //! @details Multiplies a vector by a scalar. The size must be inferior or equal to
        //! @details the size of the vector.
        //! @param size  The size of the vector.
        //! @param value The value.
        //! @param in    The vector.
        static inline void mul(const size_t size, Type const& value, Type* in) noexcept
        {
            assert("The vector can't be null." && in != nullptr);
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
        
        //! @brief Multiplies a vector by a scalar and leaves the result in another vector.
        //! @details Multiplies a vector by a scalar and leaves the result in another vector.
        //! @details The size must be inferior or equal to the size of both vectors.
        //! @param size  The size of the vectors.
        //! @param value The value.
        //! @param in    The vector.
        //! @param out   The output vector.
        static inline void mul(const size_t size, Type const& value, const Type* in, Type* out) noexcept
        {
            assert("The vector can't be null." && in != nullptr && out != nullptr);
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
        
        //! @brief Divides a vector by another.
        //! @details Divides a vector by another vector. The size must be inferior or equal to
        //! @details the size of both vectors.
        //! @param size The size of the vectors.
        //! @param in   The input vector.
        //! @param out  The output vector.
        static inline void div(const size_t size, const Type* in, Type* out) noexcept
        {
            assert("The vector can't be null." && in != nullptr && out != nullptr);
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
        
        //! @brief Divides a vector by another and leaves the result in third one.
        //! @details Divides a vector by another and leaves the result in third one. The size
        //! @details must be inferior or equal to the size of all vectors.
        //! @param size The size of the vectors.
        //! @param in1  The first input vector.
        //! @param in2  The second input vector.
        //! @param out  The output vector.
        static inline void div(const size_t size, const Type* in1, const Type* in2, Type* out) noexcept
        {
            assert("The vector can't be null." && in1 != nullptr && in2 != nullptr && out != nullptr);
            for(size_t i = size>>3; i; --i, in1 += 8, in2 += 8, out += 8)
            {
                const Type f0 = in1[0], f1 = in1[1], f2 = in1[2], f3 = in1[3];
                const Type f4 = in1[4], f5 = in1[5], f6 = in1[6], f7 = in1[7];
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
        
        //! @brief Divides a vector by a scalar.
        //! @details Divides a vector by a scalar. The size must be inferior or equal to the
        //! @details size of the vector.
        //! @param size  The size of the vector.
        //! @param value The value.
        //! @param in    The vector.
        static inline void div(const size_t size, Type const& value, Type* in) noexcept
        {
            assert("The vector can't be null." && in != nullptr);
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
        
        //! @brief Divides a vector by a scalar and leaves the result in another vector.
        //! @details Divides a vector by a scalar and leaves the result in another vector. The
        //! @details size must be inferior or equal to the size of both vectors.
        //! @param size  The size of the vectors.
        //! @param value The value.
        //! @param in    The vector.
        //! @param out   The output vector.
        static inline void div(const size_t size, Type const& value, const Type* in, Type* out) noexcept
        {
            assert("The vector can't be null." && in != nullptr && out != nullptr);
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
        //! @details Computes the dot product of two vectors. The size must be inferior or
        //! @details equal to the size of both vectors.
        //! @param   size   The size of the vectors.
        //! @param   in1    The first vector.
        //! @param   in2    The second vector.
        //! @return The dot product of the two vectors.
        static inline Type dot(const size_t size, const Type* in1, const Type* in2) noexcept
        {
            assert("The vector can't be null." && in1 != nullptr && in2 != nullptr);
            Type result = 0;
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
}

#endif // KIWI_DSP_SAMPLES_H_INCLUDED
