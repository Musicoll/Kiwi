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

#include "flip/Object.h"
#include "flip/Float.h"

namespace kiwi { namespace model {
    
    //! @brief A simple data structure that represent a bounds with 4 floating-point numbers.
    class Bounds
    : public flip::Object
    {
    public: // methods
        
        Bounds() = default;
        ~Bounds() = default;
        
        Bounds(double x, double y, double width, double height);
        
        double getX() const;
        double getY() const;
        double getWidth() const;
        double getHeight() const;
        
        void setX(double x);
        void setY(double y);
        void setWidth(double width);
        void setHeight(double height);
        
        void setPosition(double x, double y);
        void setSize(double width, double height);
        
    public: // internal
        
        static void declare();
        
    public: // members
        
        flip::Float m_x, m_y, m_width, m_height;
    };
}}

