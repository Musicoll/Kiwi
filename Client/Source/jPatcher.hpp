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
        
        //! @internal flip::DocumentObserver<model::Patcher>::document_changed
        void document_changed(model::Patcher& patcher) override final;
        
        // juce::Component
        void paint(juce::Graphics& g) override;
        void mouseDown(juce::MouseEvent const& event) override;
        
    private:
        
        void leftClick(juce::MouseEvent const& event);
        void rightClick(juce::MouseEvent const& event);
        
        void objectHasBeenAdded(model::Object& object);
        void objectChanged(model::Object& /*object*/);
        void objectWillBeRemoved(model::Object& object);
        
        model::Patcher* m_model;
    };
}

#endif // KIWI_JPATCHER_HPP_INCLUDED
