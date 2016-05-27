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

#include <memory>

#include "jFileBrowser.hpp"

namespace kiwi
{
    int deduceJuceFlag(kiwi::jFileBrowser::Mode mode)
    {
        using JuceBrowserFlag = juce::FileBrowserComponent::FileChooserFlags;
        
        int juce_flag = 0;
        
        switch (mode)
        {
            case kiwi::jFileBrowser::Mode::open:
            {
                juce_flag = JuceBrowserFlag::openMode |
                            JuceBrowserFlag::canSelectFiles |
                            JuceBrowserFlag::filenameBoxIsReadOnly;
                break;
            }
            case kiwi::jFileBrowser::Mode::save:
            {
                juce_flag = JuceBrowserFlag::saveMode |
                            JuceBrowserFlag::canSelectFiles |
                            JuceBrowserFlag::warnAboutOverwriting |
                            JuceBrowserFlag::doNotClearFileNameOnRootChange;
                break;
            }
        }
        
        return juce_flag;
    }
    
    class FileFilter final : public juce::FileFilter
    {
    public:
        FileFilter():juce::FileFilter("Kiwi file filter"){};
        
        bool isFileSuitable (const juce::File& file) const final override
        {
            return !file.isHidden() && file.getFileExtension() == ".kiwi";
        };
        
        bool isDirectorySuitable (const juce::File& file) const final override
        {
            return !file.isHidden();
        };
        
        ~FileFilter(){};
        
    private:
        FileFilter(FileFilter const& other) = delete;
        FileFilter(FileFilter && other) = delete;
        FileFilter& operator=(FileFilter const& other) = delete;
        FileFilter& operator=(FileFilter && other) = delete;
    };
    
    jFileBrowser::jFileBrowser(Mode mode):
    m_file_browser(deduceJuceFlag(mode), juce::File::nonexistent, new FileFilter(), nullptr)
    {
        setVisible(true);
        addAndMakeVisible(m_file_browser);
        setSize(500, 300);
    }
    
    void jFileBrowser::resized()
    {
        m_file_browser.setSize(getWidth(), getHeight());
    }
    
    kiwi::File jFileBrowser::getSelectedFile() const
    {
        return kiwi::File(m_file_browser.getSelectedFile(0).getFullPathName().toStdString());
    }
    
}