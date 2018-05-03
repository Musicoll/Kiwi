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

#include <iostream>


#include "../catch.hpp"

#include <KiwiTool/KiwiTool_Matrix.h>

using namespace kiwi;

// ==================================================================================== //
//                                      MATRIX                                          //
// ==================================================================================== //

void fill(tool::Matrix<int> & matrix)
{
    for (size_t row = 0; row < matrix.getNumRows(); ++row)
    {
        for (size_t col = 0; col < matrix.getNumCols(); ++col)
        {
            matrix.at(row, col) = row * matrix.getNumCols() + col;
        }
    }
}

TEST_CASE("Matrix", "[Matrix]")
{
    tool::Matrix<int> matrix(3, 4);
    
    fill(matrix);
    
    SECTION("Constructor")
    {
        tool::Matrix<int> matrix_cons(3, 4);
        
        CHECK(matrix_cons.getNumRows() == 3);
        CHECK(matrix_cons.getNumCols() == 4);
        CHECK(matrix_cons.at(2, 3) == 0);
    }
    
    SECTION("Copy Constructor")
    {
        tool::Matrix<int> matrix_cp(matrix);
        
        CHECK(matrix_cp.getNumRows() == matrix.getNumRows());
        CHECK(matrix_cp.getNumCols() == matrix.getNumCols());
        CHECK(matrix_cp.at(2, 1)== matrix.at(2, 1));
    }
    
    SECTION("Move Constructors")
    {
        size_t rows = matrix.getNumRows();
        size_t cols = matrix.getNumCols();
        int value22 = matrix.at(2, 2);
        
        tool::Matrix<int> matrix_mv(std::move(matrix));
        
        CHECK(matrix.getNumRows() == 0);
        CHECK(matrix.getNumCols() == 0);
        CHECK(matrix_mv.getNumRows() == rows);
        CHECK(matrix_mv.getNumCols() == cols);
        CHECK(matrix_mv.at(2, 2) == value22);
    }
    
    SECTION("Assignement")
    {
        tool::Matrix<int> matrix_as(matrix.getNumRows(), matrix.getNumCols());
        matrix_as = matrix;
        
        CHECK(matrix_as.getNumRows() == matrix.getNumRows());
        CHECK(matrix_as.getNumCols() == matrix.getNumCols());
        CHECK(matrix_as.at(2, 2) == matrix.at(2, 2));
    }
    
    SECTION("Move assignement")
    {
        size_t rows = matrix.getNumRows();
        size_t cols = matrix.getNumCols();
        int value22 = matrix.at(2, 2);
        
        tool::Matrix<int> matrix_mv(matrix.getNumRows(), matrix.getNumCols());
        matrix_mv = std::move(matrix);
        
        CHECK(matrix.getNumRows() == 0);
        CHECK(matrix.getNumCols() == 0);
        CHECK(matrix_mv.getNumRows() == rows);
        CHECK(matrix_mv.getNumCols() == cols);
        CHECK(matrix_mv.at(2, 2) == value22);
    }
}
