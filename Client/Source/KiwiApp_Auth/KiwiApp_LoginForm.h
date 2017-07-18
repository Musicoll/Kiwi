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

#include <juce_gui_extra/juce_gui_extra.h>

namespace kiwi
{
    class LoginForm : public juce::Component, private juce::ButtonListener
    {
    public: // methods
        
        //! @brief Constructor.
        //! @details Creates a login form.
        LoginForm();
        
        //! @brief Destructor.
        ~LoginForm();
        
        /** This is called when the form is dismissed (either cancelled or when registration
         succeeds).
         By default it will delete this, but you can override it to do other things.
         */
        virtual void dismiss();
        
    private: // methods
        
        //! @internal paint method
        void paint(juce::Graphics&) override;
        
        //! @internal resized method
        void resized() override;
        
        //! @internal lookAndFeelChanged method
        void lookAndFeelChanged() override;
        
    private: // variables
        
        juce::Label message;
        juce::TextEditor emailBox, passwordBox;
        juce::TextButton registerButton, cancelButton;
        
        juce::ScopedPointer<juce::BubbleMessageComponent> bubble;
        
        struct OverlayComp;
        friend struct OverlayComp;
        Component::SafePointer<Component> m_overlay;
        
        void buttonClicked(juce::Button*) override;
        void attemptRegistration();
        void showBubbleMessage(juce::String const&, juce::Component&);
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoginForm)
    };
}
