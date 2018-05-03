/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#pragma once

#include <cstddef>

namespace kiwi { namespace tool {

    // ================================================================================ //
    //                                      MATRIX                                      //
    // ================================================================================ //
    
    //! @brief A matrix data structure.
    //! @details Implementation of basic matrix operation.
    //! @todo Adds more usefull operation.
    template<class Type>
    class Matrix final
    {
    public: // methods
        
        //! @brief Constructor.
        //! @details Initializes matrix with default constructor.
        Matrix(size_t num_rows, size_t num_cols);
        
        //! @brief Destructor.
        ~Matrix();
        
        //! @brief Copy constructor.
        Matrix(Matrix const& other);
        
        //! @brief Move constructor.
        Matrix(Matrix && other);
        
        //! @brief Assignment operator.
        //! @details Number of rows and columns must match.
        Matrix& operator=(Matrix const& other);
        
        //! @brief Move assignement operator.
        //! @details Number of rows and columns must match.
        Matrix& operator=(Matrix && other);
        
        //! @brief Returns the number of rows.
        size_t getNumRows() const;
        
        //! @brief Returns the number of colunms.
        size_t getNumCols() const;
        
        //! @brief Gets a value stored in matrix.
        //! @details row (res column) must be inferior to number of rows (res num columns).
        Type & at(size_t row, size_t colunm);
        
        //! @brief Gets a value stored in matrix.
        //! @details row (res column) must be inferior to number of rows (res num columns).
        Type const& at(size_t row, size_t column) const;
        
    private: // members
        
        size_t  m_num_rows;
        size_t  m_num_cols;
        Type*   m_data;
        
    private: // deleted methods
        
        Matrix() = delete;
    };
}}

#include <KiwiTool/KiwiTool_Matrix.hpp>
