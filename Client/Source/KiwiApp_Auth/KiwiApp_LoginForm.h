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
    class AlertBox : public juce::Component, private juce::ButtonListener
    {
    public: // methods
        
        enum class Type : uint8_t
        {
            Info = 0,
            Success = 1,
            Error = 2
        };
        
        //! @brief Constructor.
        AlertBox(std::string const& message,
                 Type type = Type::Info,
                 bool can_cancel = true,
                 std::function<void(void)> on_close = nullptr);
        
        //! @brief Destructor.
        ~AlertBox();
        
    private: // methods
        
        //! @internal paint method
        void paint(juce::Graphics&) override;
        
        //! @internal resized method
        void resized() override;
        
        //! @internal juce::ButtonListener
        void buttonClicked(juce::Button*) override;
        
    private: // variables
        
        std::string m_message;
        Type m_type = Type::Info;
        std::unique_ptr<juce::TextButton> m_close_btn;
        std::function<void(void)> m_close_fn;
    };
    
    class LoginForm : public juce::Component, private juce::ButtonListener
    {
    public: // methods
        
        //! @brief Constructor.
        //! @details Creates a login form.
        LoginForm();
        
        //! @brief Destructor.
        ~LoginForm();
        
        //! @brief This is called when the form is dismissed
        //! @details (either cancelled or when registration succeeds).
        void dismiss();
        
    private: // methods
        
        //! @internal paint method
        void paint(juce::Graphics&) override;
        
        //! @internal resized method
        void resized() override;
        
        //! @internal lookAndFeelChanged method
        void lookAndFeelChanged() override;
        
        //! @internal Show overlay
        void showOverlay();
        
        //! @internal Hide overlay
        void hideOverlay();
        
        void showAlert(std::string const& message, AlertBox::Type type = AlertBox::Type::Error);
        
        void buttonClicked(juce::Button*) override;
        void attemptRegistration();
        
    private: // variables
        
        juce::TextEditor m_email_box, m_password_box;
        juce::TextButton m_submit_btn, m_cancel_btn;
        
        juce::ToggleButton m_remember_me_checkbox;
        
        std::unique_ptr<AlertBox> m_alert_box;
        
        juce::Image m_kiwi_app_image;
        
        class OverlayComp;
        friend class OverlayComp;
        Component::SafePointer<Component> m_overlay;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoginForm)
    };
}
