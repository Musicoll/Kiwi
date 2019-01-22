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

#include <KiwiModel/KiwiModel_Bounds.h>
#include <KiwiModel/KiwiModel_DataModel.h>

namespace kiwi { namespace model {
    
    void Bounds::declare()
    {
        if(DataModel::has<model::Bounds>())
            return; // abort
        
        DataModel::declare<model::Bounds>()
        .name("cicm.kiwi.Bounds")
        .member<flip::Float, &Bounds::m_x>("x")
        .member<flip::Float, &Bounds::m_y>("y")
        .member<flip::Float, &Bounds::m_width>("width")
        .member<flip::Float, &Bounds::m_height>("height");
    }
    
    Bounds::Bounds(double x, double y, double width, double height)
    : m_x(x)
    , m_y(y)
    {
        setSize(width, height);
    }
    
    double Bounds::getX() const
    {
        return m_x;
    }
    
    double Bounds::getY() const
    {
        return m_y;
    }
    
    double Bounds::getWidth() const
    {
        return m_width;
    }
    
    double Bounds::getHeight() const
    {
        return m_height;
    }
    
    void Bounds::setX(double x)
    {
        m_x = x;
    }
    
    void Bounds::setY(double y)
    {
        m_y = y;
    }
    
    void Bounds::setWidth(double width)
    {
        m_width = std::max(0., width);
    }
    
    void Bounds::setHeight(double height)
    {
        m_height = std::max(0., height);
    }
    
    void Bounds::setPosition(double x, double y)
    {
        setX(x);
        setY(y);
    }
    
    void Bounds::setSize(double width, double height)
    {
        setWidth(width);
        setHeight(height);
    }
    
}}

