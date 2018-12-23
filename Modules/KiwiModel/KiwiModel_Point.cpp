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

#include <KiwiModel/KiwiModel_Point.h>
#include <KiwiModel/KiwiModel_DataModel.h>

namespace kiwi { namespace model {
    
    void Point::declare()
    {
        if(DataModel::has<model::Point>())
            return; // abort
        
        DataModel::declare<model::Point>()
        .name("cicm.kiwi.Point")
        .member<flip::Float, &Point::m_x>("x")
        .member<flip::Float, &Point::m_y>("y");
    }
    
    Point::Point(double x, double y)
    : m_x(x)
    , m_y(y)
    {}
    
    double Point::getX() const
    {
        return m_x;
    }
    
    double Point::getY() const
    {
        return m_y;
    }
    
    void Point::setX(double x)
    {
        m_x = x;
    }
    
    void Point::setY(double y)
    {
        m_y = y;
    }
    
    void Point::setPosition(double x, double y)
    {
        setX(x);
        setY(y);
    }
    
}}

