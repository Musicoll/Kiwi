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


#include "KiwiEngineBeacon.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      BEACON                                      //
        // ================================================================================ //
        
        Beacon::Beacon(std::string const& name) : m_name(name)
        {
            ;
        }
        
        Beacon::~Beacon()
        {
            
        }
        
        void Beacon::bind(Castaway& castaway)
        {
            m_castaways.insert(&castaway);
        }
        
        void Beacon::unbind(Castaway& castaway)
        {
            m_castaways.erase(&castaway);
        }
        
        void Beacon::dispatch(std::vector<Atom> const& args)
        {
            for(auto castaway : m_castaways)
            {
                castaway->receive(args);
            }
        }
        
        // ================================================================================ //
        //                                  BEACON FACTORY                                  //
        // ================================================================================ //
        
        Beacon::Factory::Factory()
        {
            ;
        }
        
        Beacon::Factory::~Factory()
        {
            ;
        }
        
        Beacon& Beacon::Factory::getBeacon(std::string const& name)
        {
            auto it = m_beacons.find(name);
            if(it != m_beacons.end())
            {
                return *it->second.get();
            }
            Beacon* beacon = new Beacon(name);
            if(beacon)
            {
                m_beacons[name] = std::unique_ptr<Beacon>(beacon);
            }
            return *beacon;
        }
    };
};









