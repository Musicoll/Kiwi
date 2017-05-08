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

#include <KiwiModel/KiwiModel_DataModel.hpp>

#include "KiwiApp_AboutWindow.hpp"
#include "KiwiApp.hpp"

#include "KiwiApp_BinaryData.hpp"

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
    //                               ABOUT WINDOW CONTENT                               //
    // ================================================================================ //
    
    AboutWindow::Content::Content() :
    m_kiwi_app_image(juce::ImageCache::getFromMemory(binary_data::images::kiwi_icon_png,
                                                     binary_data::images::kiwi_icon_png_size))
    {
        setSize(400, 380);
    }
    
    void AboutWindow::Content::paint(juce::Graphics& g)
    {
        const auto bounds = getLocalBounds();
        
        g.fillAll(juce::Colours::whitesmoke);
        
        float img_size = 50;
        int padding = 10;
        
        g.setColour(juce::Colour::fromRGB(88, 88, 90));
        const auto header_bounds = bounds.withBottom(img_size + padding * 2);
        g.fillRect(header_bounds);
        
        g.drawImage(m_kiwi_app_image,
                    juce::Rectangle<float>(getWidth() - img_size - padding, padding, img_size, img_size),
                    juce::RectanglePlacement::centred, false);
        
        const auto app_version = KiwiApp::use().getApplicationVersion();
        const auto model_version = model::DataModel::use().version();
        const auto compile_date = juce::Time::getCompilationDate().toString(true, false);
        
        g.setColour(juce::Colours::whitesmoke);
        
        const auto info_text =
          "- Kiwi app version: " + app_version + '\n'
        + "- Kiwi model version: " + model_version + '\n'
        + "- Compilation date: " + compile_date;
        
        g.drawFittedText(info_text, header_bounds.withLeft(padding), juce::Justification::centredLeft, 5);
        
        g.setColour(juce::Colours::black);
        
        std::string licence_text = std::string("Kiwi © CICM - ANR MUSICOLL 2016-2018.\n\n");
        
        licence_text += "Authors:\n";
        licence_text += "- Eliott Paris\n";
        licence_text += "- Pierre Guillot\n";
        licence_text += "- Jean Millot\n";
        licence_text += "\n";
        
        licence_text += "Credits:\n";
        licence_text += "- Application icons are designed by Freepik and distributed by Flaticon\n";
        
        g.drawFittedText(licence_text,
                         bounds.withTop(header_bounds.getBottom()).reduced(padding),
                         juce::Justification::topLeft, 100);
    }
}
