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

#include "KiwiModelObject.hpp"

#include "KiwiModelDataModel.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                  OBJECT::declare                                 //
        // ================================================================================ //
        
        void Object::declare()
        {
            if(DataModel::has<model::Object>()) return;
            
            DataModel::declare<model::Object>()
            .name("cicm.kiwi.Object")
            .member<flip::String, &Object::m_name>("name")
            .member<flip::String, &Object::m_text>("text")
            .member<flip::Int, &Object::m_inlets>("inlets")
            .member<flip::Int, &Object::m_outlets>("outlets")
            .member<flip::Float, &Object::m_position_x>("pos_x")
            .member<flip::Float, &Object::m_position_y>("pos_y")
            .member<flip::Float, &Object::m_width>("width")
            .member<flip::Float, &Object::m_height>("height");
        }
        
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        Object::Object(flip::Default&)
        {
            ;
        }
        
        Object::Object() :
        m_name("noobj"),
        m_inlets(1),
        m_outlets(0),
        m_position_x(0.),
        m_position_y(0.),
        m_width(60.),
        m_height(20.)
        {
            ;
        }
        
        Object::Object(model::Object const& other) :
        m_name(other.m_name),
        m_inlets(other.m_inlets),
        m_outlets(other.m_outlets),
        m_position_x(other.m_position_x),
        m_position_y(other.m_position_y),
        m_width(other.m_width),
        m_height(other.m_height)
        {
            ;
        }
        
        Object::~Object()
        {
            ;
        }
        
        std::string Object::getName() const
        {
            return m_name;
        }
        
        std::string Object::getText() const
        {
            return m_text;
        }
        
        size_t Object::getNumberOfInlets() const
        {
            return static_cast<size_t>(m_inlets);
        }
        
        bool Object::inletsChanged() const noexcept
        {
            return m_inlets.changed();
        }
        
        size_t Object::getNumberOfOutlets() const
        {
            return static_cast<size_t>(m_outlets);
        }
        
        bool Object::outletsChanged() const noexcept
        {
            return m_outlets.changed();
        }
        
        void Object::setPosition(double x, double y)
        {
            m_position_x = x;
            m_position_y = y;
        }
        
        bool Object::positionChanged() const noexcept
        {
            return (m_position_x.changed() || m_position_y.changed());
        }
        
        bool Object::sizeChanged() const noexcept
        {
            return (m_width.changed() || m_height.changed());
        }
        
        bool Object::boundsChanged() const noexcept
        {
            return (positionChanged() || sizeChanged());
        }
        
        double Object::getX() const noexcept
        {
            return !removed() ? m_position_x.value() : m_position_x.before();
        }
        
        double Object::getY() const noexcept
        {
            return !removed() ? m_position_y.value() : m_position_y.before();
        }
        
        void Object::setWidth(double new_width)
        {
            m_width = std::max(0., new_width);
        }
        
        void Object::setHeight(double new_height)
        {
            m_height = std::max(0., new_height);
        }
        
        double Object::getWidth() const noexcept
        {
            return !removed() ? m_width.value() : m_width.before();
        }
        
        double Object::getHeight() const noexcept
        {
            return !removed() ? m_height.value() : m_height.before();
        }
        
        void Object::setNumberOfInlets(size_t inlets)
        {
            m_inlets = static_cast<flip::Int::internal_type>(inlets);
        }
        
        void Object::setNumberOfOutlets(size_t outlets)
        {
            m_outlets = static_cast<flip::Int::internal_type>(outlets);
        }
    }
}
