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

#ifndef KIWI_APP_WINDOW_HPP_INCLUDED
#define KIWI_APP_WINDOW_HPP_INCLUDED

#include <juce_gui_basics/juce_gui_basics.h>

namespace kiwi
{
    // ================================================================================ //
    //                                      WINDOW                                      //
    // ================================================================================ //

    //! @brief A juce Window.
    class Window : public juce::DocumentWindow, public juce::ApplicationCommandTarget
    {
    public:
        Window(std::string const& name = "untitled",
                juce::Colour color = juce::Colours::lightgrey,
                int buttons = allButtons,
                bool addToDesktop = true);
        
        virtual ~Window();
        
        // ================================================================================ //
        //                              APPLICATION COMMAND TARGET                          //
        // ================================================================================ //
        
        ApplicationCommandTarget* getNextCommandTarget() override;
        void getAllCommands (juce::Array<juce::CommandID>& commands) override;
        void getCommandInfo (const juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
        bool perform (const InvocationInfo& info) override;
        
    protected:
        
        void closeButtonPressed() override;
    };
}

#endif // KIWI_APP_WINDOW_HPP_INCLUDED
