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

#include <cassert>

#include "KiwiNetwork_Query.hpp"

#include <juce_core/juce_core.h>

namespace kiwi
{
    namespace network
    {
        
        // ================================================================================ //
        //                                     QUERY                                        //
        // ================================================================================ //
        
        Query::Query():
        m_impl_obj(new juce::DynamicObject())
        {
        }
        
        Query::Query(std::string const& message)
        {
            juce::var json;
            
            if (juce::JSON::parse(message, json).wasOk())
            {
                m_impl_obj = json.getDynamicObject();
            }
            else
            {
                throw "wrong json representation";
            }
        }
        
        Query::Query(juce::DynamicObject::Ptr impl):
        m_impl_obj(impl)
        {
        }
        
        Query::operator juce::var() const
        {
            juce::DynamicObject::Ptr impl_obj(m_impl_obj);
            return juce::var(impl_obj);
        }
        
        void Query::setProperty(std::string const& key, juce::var const& var)
        {
            m_impl_obj->setProperty(juce::String(key), var);
        }
        
        juce::var const& Query::getProperty(std::string const& key)
        {
            return m_impl_obj->getProperty(juce::String(key));
        }
        
        bool Query::hasProperty(std::string const& key) const
        {
            return m_impl_obj.get()->hasProperty(juce::String(key));
        }
        
        std::string Query::getMessage() const
        {
            juce::var json(m_impl_obj);
            return juce::JSON::toString(json).toStdString();
        }
    }
}
