/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <cstring>
#include <cassert>

#include <KiwiTool/KiwiTool_Matrix.h>

namespace kiwi { namespace tool {
    
    // ================================================================================ //
    //                                      MATRIX                                      //
    // ================================================================================ //
    
    template<class Type>
    void copy(Type * const dest, Type const * const src, size_t size)
    {
        for (size_t index = 0; index < size; ++index)
        {
            dest[index] = src[index];
        }
    }
    
    template<class Type>
    Matrix<Type>::Matrix(size_t num_rows, size_t num_cols):
    m_num_rows(num_rows),
    m_num_cols(num_cols),
    m_data(new Type[m_num_rows * m_num_cols]())
    {
    }
    
    template<class Type>
    Matrix<Type>::~Matrix()
    {
        delete[] m_data;
    }
    
    template<class Type>
    Matrix<Type>::Matrix(Matrix const& other):
    m_num_rows(other.m_num_rows),
    m_num_cols(other.m_num_cols),
    m_data(new Type[m_num_rows * m_num_cols]())
    {
        copy(m_data, other.m_data, m_num_rows * m_num_cols);
    }
    
    template<class Type>
    Matrix<Type>::Matrix(Matrix && other):
    m_num_rows(other.m_num_rows),
    m_num_cols(other.m_num_cols),
    m_data(new Type[m_num_rows * m_num_cols]())
    {
        copy(m_data, other.m_data, m_num_rows * m_num_cols);
        
        other.m_num_cols = 0;
        other.m_num_rows = 0;
        other.m_data = nullptr;
    }
    
    template<class Type>
    Matrix<Type> & Matrix<Type>::operator=(Matrix const& other)
    {
        assert((m_num_rows == other.m_num_rows && m_num_cols == other.m_num_cols)
               && "Assigning matrixes with different size");
        
        copy(m_data, other.m_data, m_num_rows * m_num_cols);
        
        return *this;
    }
    
    template<class Type>
    Matrix<Type> & Matrix<Type>::operator=(Matrix && other)
    {
        assert((m_num_rows == other.m_num_rows && m_num_cols == other.m_num_cols)
               && "Assigning matrixes with different size");
        
        copy(m_data, other.m_data, m_num_rows * m_num_cols);
        
        other.m_num_cols = 0;
        other.m_num_rows = 0;
        other.m_data = nullptr;
        
        return *this;
    }
    
    template<class Type>
    size_t Matrix<Type>::getNumRows() const
    {
        return m_num_rows;
    }
    
    template<class Type>
    size_t Matrix<Type>::getNumCols() const
    {
        return m_num_cols;
    }
    
    template<class Type>
    Type & Matrix<Type>::at(size_t row, size_t column)
    {
        assert((row < m_num_rows && column < m_num_cols) && "Accessing matrix out of bounds");
        return m_data[m_num_cols * row + column];
    }
    
    template<class Type>
    Type const& Matrix<Type>::at(size_t row, size_t column) const
    {
        assert((row < m_num_rows && column < m_num_cols) && "Accessing matrix out of bounds");
        return m_data[m_num_cols * row + column];
    }
}}
