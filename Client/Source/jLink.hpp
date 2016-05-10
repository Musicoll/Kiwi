/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_JLINK_HPP_INCLUDED
#define KIWI_JLINK_HPP_INCLUDED

#include <KiwiModel/KiwiLink.hpp>

#include "jObject.hpp"

namespace kiwi
{
    //! @brief The juce link Component.
    class jLink : public juce::Component
    {
    public:
        
        jLink();
        ~jLink();
        
        void linkChanged(model::Link& link);
        void objectChanged(model::Object& object);
        
        // juce::Component
        void paint(juce::Graphics& g) override;
        
    private:
        
        void updateBounds();
        
        model::Link* m_model;
        
        juce::Point<int> m_last_inlet_pos, m_last_outlet_pos;
    };
}

#endif // KIWI_JLINK_HPP_INCLUDED
