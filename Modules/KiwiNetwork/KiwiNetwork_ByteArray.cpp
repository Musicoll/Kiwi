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

#include <cstdio>
#include <cstring>

#include "KiwiNetwork_ByteArray.hpp"

namespace kiwi
{
    namespace network
    {
        ByteArray::ByteArray(size_t size):
        m_size(size),
        m_data(static_cast<char*>(std::calloc(size, sizeof(char))))
        {
        }
        
        ByteArray::ByteArray(ByteArray const& other):
        m_size(other.m_size)
        {
            if (other.m_size != 0)
            {
                m_data = static_cast<char*>(std::malloc(other.m_size * sizeof(char)));
                std::memcpy(m_data, other.m_data, other.m_size);
            }
        }
        
        ByteArray::~ByteArray()
        {
            if (m_size != 0)
            {
                std::free(m_data);
                m_data = nullptr;
            }
        }
        
        ByteArray& ByteArray::operator=(ByteArray const& other)
        {
            if (&other != this)
            {
                if (other.m_size == 0)
                {
                    std::free(m_data);
                    m_data = nullptr;
                }
                else
                {
                    if (m_size != other.m_size)
                    {
                        m_data = static_cast<char*>(std::realloc(m_data, other.m_size));
                    }
                    
                    std::memcpy(m_data, other.m_data, other.m_size);
                }
                
                m_size = other.m_size;
            }
            
            return *this;
        }
        
        bool ByteArray::operator==(ByteArray const& other) const
        {
            bool equal = true;
            
            if (m_size == other.m_size)
            {
                if (m_size != 0)
                {
                    for(int i = 0; i < m_size && equal == true; ++i)
                    {
                        if (m_data[i] != other.m_data[i])
                        {
                            equal = false;
                        }
                    }
                }
            }
            else
            {
                equal = false;
            }
            
            return equal;
        }
        
        bool ByteArray::operator!=(ByteArray const& other) const
        {
            return !(*this == other);
        }
        
        char& ByteArray::operator[](size_t offset) const
        {
            if (offset >= m_size)
            {
                throw ByteArrayError("Accessing byte array out of bound");
            }
            else
            {
                return m_data[offset];
            }
        }
        
        size_t ByteArray::getSize() const
        {
            return m_size;
        }
        
        void ByteArray::setSize(size_t new_size)
        {
            m_data = static_cast<char *>(std::realloc(m_data, new_size));
            
            if (new_size > m_size)
            {
                std::memset(m_data + m_size, 0, new_size - m_size);
            }
            
            m_size = new_size;
        }
        
        void* ByteArray::getData() const
        {
            return m_data;
        }
        
        void ByteArray::writeData(const void * const data, size_t offset, size_t num_bytes)
        {
            if (data == nullptr)
            {
                throw ByteArrayError("Writing data with nullptr");
            }
            else if(offset + num_bytes > m_size)
            {
                throw ByteArrayError("Writing data out of bounds");
            }
            else
            {
                std::memcpy(m_data + offset, data, num_bytes);
            }
        }
        
        void ByteArray::readData(void * const data, size_t offset, size_t num_bytes) const
        {
            if (data == nullptr)
            {
                throw ByteArrayError("Reading data with nullptr");
            }
            else if(offset + num_bytes > m_size)
            {
                throw ByteArrayError("Reading data out of bounds");
            }
            else
            {
                std::memcpy(data, m_data + offset, num_bytes);
            }
        }
        
        ByteArray &operator<<(ByteArray& byte_array, std::string const& input)
        {
            size_t old_size = byte_array.getSize();
            byte_array.setSize(byte_array.getSize() + input.size());
            byte_array.writeData(input.data(), old_size, input.size());
            return byte_array;
        }
        
        ByteArray &operator>>(ByteArray& byte_array, std::string & output)
        {
            output.append(static_cast<char*>(byte_array.getData()), byte_array.getSize());
            return byte_array;
        }
    }
}
