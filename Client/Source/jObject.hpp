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

#ifndef KIWI_JOBJECT_HPP_INCLUDED
#define KIWI_JOBJECT_HPP_INCLUDED

#include <KiwiModel/KiwiObject.hpp>

#include "../JuceLibraryCode/JuceHeader.h"

namespace kiwi
{
    //! @brief The juce Patcher Component.
    class jObject : public juce::Component
    {
    public:
        
        jObject();
        ~jObject();
        
        void document_changed(model::Object& object);
        
        // juce::Component
        void paint(juce::Graphics& g) override;
        void mouseDown(juce::MouseEvent const& event) override;
        void mouseDrag(juce::MouseEvent const& event) override;
        
    private:
        
        void drawInletsOutlets(juce::Graphics & g);
        
        model::Object* m_model;
    };
}

#endif // KIWI_JOBJECT_HPP_INCLUDED
