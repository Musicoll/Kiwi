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
        
        jPatcher(model::Patcher& patcher, model::Patcher::View& view);
        ~jPatcher();
        
        // juce::Component
        void paint(juce::Graphics& g) override;
        void mouseDown(juce::MouseEvent const& event) override;
        
        //! @internal flip::DocumentObserver<model::Patcher>::document_changed
        void patcherChanged(model::Patcher& patcher, model::Patcher::View& view);
        
        //! @brief Returns the jObject corresponding to a given Object model.
        jObject* getjObject(model::Object const& object) const;
        
        //! @brief Returns the jLink corresponding to a given Link model.
        jLink* getjLink(model::Link const& link) const;
        
    private:
        
        std::set<std::unique_ptr<jObject>>::iterator findjObject(model::Object const& object) const;
        std::set<std::unique_ptr<jLink>>::iterator findjLink(model::Link const& link) const;
        
        //! @brief Load object and links.
        void loadPatcher();
        
        void leftClick(juce::MouseEvent const& event);
        void rightClick(juce::MouseEvent const& event);
        
        //! @internal Object model has just been added to the document.
        void addjObject(model::Object& object);
        
        //! @internal Object model is resident and internal value changed.
        void objectChanged(model::Patcher::View& view, model::Object& object);
        
        //! @internal Object model will be removed from the document.
        void removejObject(model::Object& object);
        
        //! @internal Link model has just been added to the document.
        void addjLink(model::Link& link);
        
        //! @internal Link model is resident and internal value changed.
        void linkChanged(model::Link& link);
        
        //! @internal Link model will be removed from the document.
        void removejLink(model::Link& link);
        
        model::Patcher&                     m_patcher_model;
        model::Patcher::View&               m_view_model;
        
        std::set<std::unique_ptr<jObject>>  m_objects;
        std::set<std::unique_ptr<jLink>>    m_links;
    };
}

#endif // KIWI_JPATCHER_HPP_INCLUDED
