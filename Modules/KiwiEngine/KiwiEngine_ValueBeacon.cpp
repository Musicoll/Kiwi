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

#include "KiwiEngine_ValueBeacon.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      BEACON                                      //
        // ================================================================================ //
        
        ValueBeacon::ValueBeacon(std::string const& name) : m_name(name)
        {
            ;
        }
        
        void ValueBeacon::bind(Castaway& castaway)
        {
            m_castaways.insert(&castaway);
        }
        
        void ValueBeacon::unbind(Castaway& castaway)
        {
            m_castaways.erase(&castaway);
        }
        
        bool ValueBeacon::hasValue()
        {
            return (m_value != nullptr);
        }
        
        ValueBeacon::Value* ValueBeacon::getValue()
        {
            return m_value.get();
        }
        
        void ValueBeacon::setValue(std::unique_ptr<Value> new_value)
        {
            m_value = std::move(new_value);
        }
        
        // ================================================================================ //
        //                                  BEACON FACTORY                                  //
        // ================================================================================ //
        
        ValueBeacon& ValueBeacon::Factory::useValueBeacon(std::string const& name)
        {
            auto it = m_beacons.find(name);
            if(it != m_beacons.end())
            {
                return *it->second.get();
            }
            
            ValueBeacon* beacon = new ValueBeacon(name);
            if(beacon)
            {
                m_beacons[name] = std::unique_ptr<ValueBeacon>(beacon);
            }
            return *beacon;
        }
    };
};
