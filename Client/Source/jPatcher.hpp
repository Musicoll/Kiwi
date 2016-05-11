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
    class jObject;
    class jLink;
    
    //! @brief The juce Patcher Component.
    class jPatcher : public juce::Component
    {
    public:
        
        jPatcher();
        ~jPatcher();
        
        // juce::Component
        void paint(juce::Graphics& g) override;
        void mouseDown(juce::MouseEvent const& event) override;
        
        //! @internal flip::DocumentObserver<model::Patcher>::document_changed
        void patcherChanged(model::Patcher& patcher, model::Patcher::View& view);
        
        //! @brief Returns the jObject corresponding to a given Object model.
        jObject const* const getjObject(model::Object const& object) const;
        
        //! @brief Returns the jLink corresponding to a given Link model.
        //jLink const* const getjLink(model::Link const& link) const;
        
        //! @brief Returns the jLink corresponding to a given Link model.
        jLink* getjLink(model::Link const& link);
        
    private:
        
        void leftClick(juce::MouseEvent const& event);
        void rightClick(juce::MouseEvent const& event);
        
        //! @internal Object model has just been added to the document.
        void objectAdded(model::Object& object);
        
        //! @internal Object model is resident and internal value changed.
        void objectChanged(model::Patcher::View& view, model::Object& object);
        
        //! @internal Object model will be removed from the document.
        void objectRemoved(model::Object& object);
        
        //! @internal Link model has just been added to the document.
        void linkAdded(model::Link& link);
        
        //! @internal Link model is resident and internal value changed.
        void linkChanged(model::Link& link);
        
        //! @internal Link model will be removed from the document.
        void linkRemoved(model::Link& link);
        
        model::Patcher*                     m_model;
        
        std::set<std::unique_ptr<jObject>>  m_objects;
        std::set<std::unique_ptr<jLink>>    m_links;
    };
}

#endif // KIWI_JPATCHER_HPP_INCLUDED
