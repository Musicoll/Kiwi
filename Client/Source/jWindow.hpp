/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_JWINDOW_HPP_INCLUDED
#define KIWI_JWINDOW_HPP_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

namespace kiwi
{
    // ================================================================================ //
    //                                      JWINDOW                                     //
    // ================================================================================ //

    //! @brief A juce Window.
    class jWindow : public juce::DocumentWindow, public ApplicationCommandTarget
    {
    public:
        jWindow(std::string const& name = "untitled",
                juce::Colour color = juce::Colours::lightgrey,
                int buttons = allButtons,
                bool addToDesktop = true);
        
        virtual ~jWindow();
        
        // ================================================================================ //
        //                              APPLICATION COMMAND TARGET                          //
        // ================================================================================ //
        
        ApplicationCommandTarget* getNextCommandTarget() override;
        void getAllCommands (Array <CommandID>& commands) override;
        void getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result) override;
        bool perform (const InvocationInfo& info) override;
        
    protected:
        
        void closeButtonPressed() override;
    };
}

#endif // KIWI_JWINDOW_HPP_INCLUDED
