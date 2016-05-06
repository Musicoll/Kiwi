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

#ifndef KIWI_JPATCHER_HPP_INCLUDED
#define KIWI_JPATCHER_HPP_INCLUDED

#include <KiwiModel/KiwiPatcher.hpp>

#include "flip/DocumentObserver.h"

#include "../JuceLibraryCode/JuceHeader.h"

namespace kiwi
{
    //! @brief The juce Patcher Component.
    class jPatcher : public juce::Component, public flip::DocumentObserver<model::Patcher>
    {
    public:
        
        jPatcher();
        ~jPatcher();
        
        // juce::Component
        void paint(juce::Graphics& g) override;
        void mouseDown(juce::MouseEvent const& event) override;
        
        //! @internal flip::DocumentObserver<model::Patcher>::document_changed
        void document_changed(model::Patcher& patcher) override final;
        
    private:
        
        void leftClick(juce::MouseEvent const& event);
        void rightClick(juce::MouseEvent const& event);
        
        //! @internal Object has just been added to the document.
        void objectModelAdded(model::Object& object);
        
        //! @internal Object is resident and internal value changed.
        void objectModelChanged(model::Object& object);
        
        //! @internal Object will be removed from the document.
        void objectModelRemoved(model::Object& object);
        
        //! @internal Link has just been added to the document.
        void linkModelAdded(model::Link& link);
        
        //! @internal Link is resident and internal value changed.
        void linkModelChanged(model::Link& link);
        
        //! @internal Link will be removed from the document.
        void linkModelRemoved(model::Link& link);
        
        model::Patcher* m_model;
    };
}

#endif // KIWI_JPATCHER_HPP_INCLUDED
