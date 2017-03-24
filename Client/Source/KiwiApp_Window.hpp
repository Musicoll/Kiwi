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

    //! @brief Common interface for all windows held by the application.
    class Window : public juce::DocumentWindow, public juce::ApplicationCommandTarget
    {
    public: // methods
        Window(std::string const& name, bool addToDesktop);
        
        //! @brief Window destructor. Called whenever buttonPressed is called.
        virtual ~Window() = default;
        
        // ================================================================================ //
        //                              APPLICATION COMMAND TARGET                          //
        // ================================================================================ //
        
        ApplicationCommandTarget* getNextCommandTarget() override;
        void getAllCommands (juce::Array<juce::CommandID>& commands) override;
        void getCommandInfo (const juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
        bool perform (const InvocationInfo& info) override;
    };
    
    // ================================================================================ //
    //                                      APPWINDOW                                   //
    // ================================================================================ //
    
    //! @brief A class used for application windows as settings windows and managers.
    //! @details Distinction is made between AppWindow and Patcher Windows that countain PatcherViews.
    class AppWindow : public Window
    {
    public: // methods
        
        //! @brief Constructor restore AppWindow position from stored settings it they exist.
        AppWindow(std::string const& name);
        
        //! @brief Initialise AppWindow bounds with either stored settings if there or specified bounds if not.
        void initBounds(juce::Rectangle<int> bounds);
        
        //! @brief Return true if window shall be a main window of kiwi.
        //! @details On windows platform, the application quit when all main windows are closed.
        virtual bool isMainWindow() = 0;
        
        //! @brief Destructor store position into settings.
        virtual ~AppWindow();
        
    protected: // methods
        
        //! @biref Called when close button is pressed. Request instance to close the window.
        void closeButtonPressed() override final;
    };
}

#endif // KIWI_APP_WINDOW_HPP_INCLUDED
