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

#ifndef KIWI_JFILE_BROWSER_HPP_INCLUDED
#define KIWI_JFILE_BROWSER_HPP_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include <KiwiCore/KiwiFile.hpp>

namespace kiwi
{
    //! @brief Component used to browse file.
    //! @details The mode of the file browser determines its behavior.
    class jFileBrowser final : public juce::Component
    {
    public:
        enum class Mode
        {
            open, // Used to initialize a FileBrowser to open a file
            save  // Used to initialize a FileBrowser to save file
        };
        
        //! @brief Initializes the file browser depending on the mode.
        jFileBrowser(Mode mode);
        
        //! @brief Returns the file selected file.
        kiwi::File getSelectedFile() const;
        
        void resized() final override;
        
        ~jFileBrowser() = default;
        
    private:
        juce::FileBrowserComponent m_file_browser;
        
    private:
        jFileBrowser(jFileBrowser& other) = delete;
        jFileBrowser(jFileBrowser && other) = delete;
        jFileBrowser& operator=(jFileBrowser const& other) = delete;
        jFileBrowser& operator=(jFileBrowser && other) = delete;
    };
}

#endif // KIWI_JFILE_BROWSER_HPP_INCLUDED
