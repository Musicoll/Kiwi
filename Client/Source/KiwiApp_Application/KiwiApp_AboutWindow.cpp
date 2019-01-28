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

#include "KiwiApp_AboutWindow.h"

#include <KiwiModel/KiwiModel_DataModel.h>

#include "../KiwiApp.h"

#include "../KiwiApp_Resources/KiwiApp_BinaryData.h"

namespace kiwi
{
    // ================================================================================ //
    //                                   ABOUT WINDOW                                   //
    // ================================================================================ //
    
    AboutWindow::AboutWindow() :
    Window("About Kiwi", std::make_unique<Content>(), false, false, "about_kiwi_window")
    {
        setTitleBarButtonsRequired(juce::DocumentWindow::closeButton, true);
        
        // force fixed size
        getContentComponent()->setSize(400, 380);
    }
    
    // ================================================================================ //
    //                                    LINK LIST                                     //
    // ================================================================================ //
    
    LinkList::LinkList(juce::String header, std::vector<Link> links)
    : m_header("header", std::move(header))
    , m_links(std::move(links))
    {
        addAndMakeVisible(m_header);
        m_header.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        
        for(auto& link: m_links)
        {
            addAndMakeVisible(link.link);
            link.link.setColour(juce::HyperlinkButton::ColourIds::textColourId, juce::Colours::black.withAlpha(0.8f));
            
            addAndMakeVisible(link.description);
            link.description.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        }
    }
    
    void LinkList::resized()
    {
        auto bounds = getLocalBounds();
        m_header.setBounds(bounds.removeFromTop(22));
        bounds.reduce(10, 0);
        for(auto& link: m_links)
        {
            const auto link_bounds = bounds.removeFromTop(20);
            link.link.setBounds(link_bounds);
            link.link.changeWidthToFitText();
            link.description.setBounds(link_bounds.withLeft(link.link.getRight()));
        }
    }
    
    // ================================================================================ //
    //                               ABOUT WINDOW CONTENT                               //
    // ================================================================================ //
    
    AboutWindow::Content::Content()
    : m_kiwi_app_image(juce::ImageCache::getFromMemory(binary_data::images::kiwi_icon_png,
                                                     binary_data::images::kiwi_icon_png_size))
    , m_authors_links("Developers:", {
        {"Eliott Paris", juce::URL("https://github.com/eliottparis")},
        {"Pierre Guillot", juce::URL("https://github.com/pierreguillot")},
        {"Jean Millot", juce::URL("https://github.com/jean-millot")},
    })
    , m_credits_links("Credits: ", {
        {"Juce", juce::URL("https://github.com/WeAreROLI/JUCE"), juce::CharPointer_UTF8("- © Roli")},
        {"Flip", juce::URL("http://developer.irisate.com/"), juce::CharPointer_UTF8("- © irisate")},
        {"Beast", juce::URL("https://github.com/boostorg/Beast/"), juce::CharPointer_UTF8("- boost")},
        {"concurrentqueue", juce::URL("https://github.com/cameron314/concurrentqueue"), juce::CharPointer_UTF8("- Cameron Desrochers")},
        {"json", juce::URL("https://github.com/nlohmann/json"), juce::CharPointer_UTF8("- Niels Lohmann")},
        {"Faust", juce::URL("https://github.com/grame-cncm/faust"), juce::CharPointer_UTF8("- GRAME")},
        {"Icons", juce::URL("https://www.flaticon.com/"), juce::CharPointer_UTF8("- by Freepik")}
    })
    {
        addAndMakeVisible(m_authors_links);
        addAndMakeVisible(m_credits_links);
        
        setSize(400, 380);
    }
    
    void AboutWindow::Content::resized()
    {
        auto bounds = getLocalBounds().reduced(5, 0);
        bounds.removeFromTop(70); // header
        bounds.removeFromTop(30); // text
        m_authors_links.setBounds(bounds.removeFromTop(90));
        m_credits_links.setBounds(bounds);
    }
    
    void AboutWindow::Content::paint(juce::Graphics& g)
    {
        auto bounds = getLocalBounds();
        
        auto header_bounds = bounds.removeFromTop(70);

        g.setColour(juce::Colour::fromRGB(88, 88, 90));
        g.fillRect(header_bounds);
        
        const auto padding = 10;
        
        header_bounds.reduce(padding, padding);

        g.drawImage(m_kiwi_app_image,
                    header_bounds.removeFromRight(header_bounds.getHeight()).toFloat(),
                    juce::RectanglePlacement::centred, false);
        
        g.setColour(juce::Colours::lightgrey);
        g.fillRect(bounds);
        
        #if JUCE_DEBUG
        const auto app_version = KiwiApp::use().getApplicationVersion() + " (debug)";
        #else
        const auto app_version = KiwiApp::use().getApplicationVersion();
        #endif
        
        const auto model_version = model::DataModel::use().version();
        const auto compilation_date = juce::Time::getCompilationDate().toString(true, false);
        
        g.setColour(juce::Colours::whitesmoke);
        
        const auto header =
          "App version: " + app_version + '\n'
        + "model version: " + model_version + '\n'
        + "Build date: " + compilation_date;
        
        g.drawFittedText(header, header_bounds, juce::Justification::centredLeft, 5);
        
        g.setColour(juce::Colours::black);
        
        g.drawFittedText(juce::CharPointer_UTF8 ("Kiwi © CICM ANR MUSICOLL - 2016-2018.\n\n"),
                         bounds.reduced(padding),
                         juce::Justification::topLeft, 1);
    }
}
