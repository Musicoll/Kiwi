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
        
        //! @internal User has just been added to the document.
        void userAdded(model::Patcher::User& user);
        
        //! @internal User is resident and internal value changed.
        void userChanged(model::Patcher::User& user);
        
        //! @internal User will be removed from the document.
        void userRemoved(model::Patcher::User& user);
        
        //! @internal Object model has just been added to the document.
        void objectAdded(model::Object& object);
        
        //! @internal Object model is resident and internal value changed.
        void objectChanged(model::Object& object);
        
        //! @internal Object model will be removed from the document.
        void objectRemoved(model::Object& object);
        
        //! @internal Link model has just been added to the document.
        void linkAdded(model::Link& link);
        
        //! @internal Link model is resident and internal value changed.
        void linkChanged(model::Link& link);
        
        //! @internal Link model will be removed from the document.
        void linkRemoved(model::Link& link);
        
        model::Patcher* m_model;
        model::Patcher::User* m_user;
    };
}

#endif // KIWI_JPATCHER_HPP_INCLUDED
