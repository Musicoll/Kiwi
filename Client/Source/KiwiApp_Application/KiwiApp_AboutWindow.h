/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
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

#include <juce_gui_basics/juce_gui_basics.h>

#include "../KiwiApp_Components/KiwiApp_Window.h"

namespace kiwi
{
    // ================================================================================ //
    //                                   ABOUT WINDOW                                   //
    // ================================================================================ //

    //! @brief Kiwi About window.
    //! @details Displays application informations.
    class AboutWindow : public Window
    {
    public: // methods
        
        //! @brief Constructor.
        AboutWindow();
        
        //! @brief Destructor.
        ~AboutWindow() = default;
     
    private: // classes
        
        class Content;
        
    private: // variables
        
    };
    
    //! @brief A list of HyperlinkButtons with optional description.
    class LinkList : public juce::Component
    {
    public: // methods
        
        struct Link {
            Link(juce::String text, juce::URL url, juce::String _description = "")
            : link(text, url)
            , description("description", _description)
            {}
            
            Link(Link const& rhs)
            : link(rhs.link.getButtonText(), rhs.link.getURL())
            , description("description", rhs.description.getText())
            {}
            
            Link(Link&&) = default;
            
            juce::HyperlinkButton link = {};
            juce::Label description = {};
        };
        
        //! @brief Constructor.
        LinkList(juce::String header, std::vector<Link> links);
        
        //! @brief Destructor.
        ~LinkList() = default;
        
        //! @brief juce::Component
        void resized() override;
        
    private: // variables
        
        juce::Label m_header = {};
        std::vector<Link> m_links;
    };
    
    class AboutWindow::Content : public juce::Component
    {
    public: // methods
        
        //! @brief Constructor.
        Content();
        
        //! @brief Destructor.
        ~Content() = default;
        
        //! @brief juce::Component
        void paint(juce::Graphics& g) override;
        
        //! @brief juce::Component
        void resized() override;
        
    private: // variables
        
        juce::Image m_kiwi_app_image;
        LinkList m_authors_links;
        LinkList m_credits_links;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Content)
    };
}
