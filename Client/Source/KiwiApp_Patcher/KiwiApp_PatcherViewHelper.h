/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#pragma once

#include "flip/Ref.h"

#include <juce_gui_extra/juce_gui_extra.h>

#include <set>

namespace kiwi
{
    class PatcherView;
    class ObjectView;
    
    // ================================================================================ //
    //                                   IOLET HILIGHTER                                //
    // ================================================================================ //
    
    class IoletHighlighter : public juce::Component
    {
    public:
        
        //! @brief Constructor.
        IoletHighlighter();
        
        //! @brief Destructor.
        ~IoletHighlighter() = default;
        
        //! @brief The paint method.
        void paint(juce::Graphics& g) override;
        
        //! @brief Stop highlighting.
        void hide();
        
        //! @brief Highlight inlet
        void highlightInlet(ObjectView const& object, const size_t index);
        
        //! @brief Highlight outlet
        void highlightOutlet(ObjectView const& object, const size_t index);
        
    private: // members
        
        bool            m_is_inlet;
        std::string     m_text;
    };
    
    // ================================================================================ //
    //										JLASSO                                      //
    // ================================================================================ //
    
    class Lasso : public juce::Component
    {
    public:
        
        //! @brief Contructor.
        Lasso(PatcherView& patcher);
        
        //! @grief Destructor.
        ~Lasso();
        
        //! @brief The paint method.
        void paint(juce::Graphics& g) override;
        
        //! @brief Begins the selection of the links and the boxes.
        //! @param point The starting point.
        //! @param preserve_selection The lasso should preserve the current selection.
        void begin(juce::Point<int> const& point, const bool preserve_selection);
        
        //! @brief Perform the selection of the links and the boxes.
        //! @param point The draging point.
        //! @param objects The lasso should add objects to the selection.
        //! @param links The lasso should add links to the selection.
        //! @param preserve The lasso should preserve the last selection.
        void perform(juce::Point<int> const& point, bool objects, bool links, const bool preserve);
        
        //! @brief Ends the selection of the links and the boxes.
        void end();

        //! Retrieve Returns true if the Lasso is performing the selection.
        bool isPerforming() const noexcept;
        
    private: // members
        
        PatcherView&        m_patcher;
        
        std::set<flip::Ref> m_objects;
        std::set<flip::Ref> m_links;
        
        juce::Point<int>    m_start;
        bool				m_dragging;
    };
    
}
