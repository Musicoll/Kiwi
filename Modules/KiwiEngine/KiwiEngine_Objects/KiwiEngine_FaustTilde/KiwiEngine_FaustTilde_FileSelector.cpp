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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_FaustTilde.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    
    class FaustTilde::FileSelector
    {
    public:
        
        FileSelector(FaustTilde& owner) :
        m_owner(owner)
        {
            
        }
        
        void show()
        {
            auto directory = juce::File(m_last_directory);
            if(!directory.exists())
            {
                directory = juce::File::getSpecialLocation(juce::File::userMusicDirectory);
            }
            m_file_chooser = std::make_unique<juce::FileChooser>("Choose a DSP file to read...",
                                                     directory,
                                                     juce::String("*.dsp"), true);
            m_owner.deferMain([this]() {
                
                m_file_chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles, [this](juce::FileChooser const& chooser)
                                      {
                                          auto file = chooser.getResult();
                                          if(file.getFullPathName().isNotEmpty())
                                          {
                                              m_owner.openFile(file.getFullPathName().toStdString());
                                              m_last_directory = file.getParentDirectory().getFullPathName().toStdString();
                                          }
                                      });
            });
        }
        
    private:
        
        FaustTilde&                        m_owner;
        std::unique_ptr<juce::FileChooser> m_file_chooser;
        std::string                        m_last_directory;
    };
}}
