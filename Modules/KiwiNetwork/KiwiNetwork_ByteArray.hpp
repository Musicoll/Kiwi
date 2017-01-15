/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_NETWORK_BYTEARRAY_HPP_INCLUDED
#define KIWI_NETWORK_BYTEARRAY_HPP_INCLUDED

#include <cstddef>
#include <iostream>

#include "KiwiNetwork_Error.hpp"

namespace kiwi
{
    namespace network
    {
        
        // ==================================================================================== //
        //                                    BYTE ARRAY                                        //
        // ==================================================================================== //
        
        //! @brief A low level array of bytes that be used to represent any type of data.
        //! @details It used by the network layer to send data.
        //! @see TcpSocket
        
        class ByteArray final
        {
        public: // methods
            
            // @brief Default constructor.
            ByteArray() = default;
            
            // @brief Creates of array of bytes of size size filled with 0.
            ByteArray(size_t size);
            
            //! @brief Copy constructor.
            ByteArray(ByteArray const& other);
            
            //! @brief Destructor.
            ~ByteArray();
            
            //! @brief Assignment operator.
            ByteArray& operator=(ByteArray const& other);
            
            //! @brief Comprison operator
            //! @details Compares each bytes present in ByteArray.
            bool operator==(ByteArray const& other) const;
            
            //! @brief Comprison operator
            //! @details Compares each bytes present in ByteArray.
            bool operator!=(ByteArray const& other) const;
            
            //! @brief acces data one specific byte in the byte array.
            char& operator[](size_t offset) const;
            
            //! @brief Returns the number of bytes.
            size_t getSize() const;
            
            //! @brief Set a new size to the bytes array.
            //! @details Former written bytes will keep their value if not clipped out.
            //! New bytes will be set to 0.
            void setSize(size_t new_size);
            
            //! @brief Returns a raw pointer the data pointed by the ByteArray.
            void* getData() const;
            
            //! @brief Copy data into the ByteArry.
            //! @details Throw an error before writting if it goes the byte array's bound.
            //! Data pointer cannot be nullptr.
            void writeData(void const * const data, size_t offset, size_t num_bytes);
            
            //! @brief Copy data from the ByteArray.
            //! @details Throw an error before writting if it goes out of bound.
            //! data pointer cannot be nullptr.
            void readData(void* data, size_t offset, size_t num_bytes) const;
            
            //! @brief Convenient operator to append string to the ByteArray.
            friend ByteArray &operator<<(ByteArray& byte_array, std::string const& input);
            
            //! @brief Convenient operator to retrieve the ByteArray into an std::string.
            friend ByteArray &operator>>(ByteArray& byte_array, std::string & ouput);
            
        private: // members
            
            size_t m_size {0};
            char* m_data {nullptr};
            
        private: // deleted methods
            
            ByteArray(ByteArray && other) = delete;
            ByteArray& operator=(ByteArray && other) = delete;
        };
        
        // ==================================================================================== //
        //                                    BYTE ARRAY ERROR                                  //
        // ==================================================================================== //
        
        //! @brief An exception that a ByteArray operation might throw.
        
        class ByteArrayError final : public network::Error
        {
        public:
            explicit ByteArrayError(const std::string& message) :
            Error(std::string("ByteArray:") + message) {}
            
            explicit ByteArrayError(const char* message) :
            Error(std::string("ByteArray:") + std::string(message)) {}
            
            ~ByteArrayError() noexcept override = default;
        };
        
    }
}

#endif // KIWI_NETWORK_BYTEARRAY_HPP_INCLUDED
