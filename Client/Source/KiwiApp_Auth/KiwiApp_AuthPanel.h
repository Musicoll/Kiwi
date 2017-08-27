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

#include "../KiwiApp_Components/KiwiApp_FormComponent.h"

namespace kiwi
{
    // ================================================================================ //
    //                                   LOGIN FORM                                     //
    // ================================================================================ //
    
    class LoginForm : public FormComponent
    {
    public: // methods
        
        //! @brief Constructor.
        //! @details Creates a login form.
        LoginForm();
        
        //! @brief Destructor.
        ~LoginForm() = default;
        
    private: // methods
        
        void onUserSubmit() override;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoginForm)
    };
    
    // ================================================================================ //
    //                                  SIGNUP FORM                                     //
    // ================================================================================ //
    
    class SignUpForm : public FormComponent
    {
    public: // methods
        
        //! @brief Constructor.
        //! @details Creates a signup form.
        SignUpForm();
        
        //! @brief Destructor.
        ~SignUpForm() = default;
        
    private: // methods
        
        void onUserSubmit() override;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SignUpForm)
    };
    
    // ================================================================================ //
    //                                   AUTH PANEL                                     //
    // ================================================================================ //
    
    class AuthPanel
    : public juce::TabbedComponent
    , public juce::ComponentListener
    {
    public: // methods
        
        enum FormType
        {
            Login = 0,
            SignUp,
            UserProfile,
            None,
        };
            
        AuthPanel(FormType form_type = FormType::None);
        
        ~AuthPanel();
        
    private: // methods
        
        void updateSize(FormType type);
        
        //! @brief Callback method to indicate the selected tab has been changed.
        void currentTabChanged(int new_tab_index, juce::String const& new_tab_name) override;
        
        void componentMovedOrResized(juce::Component& component,
                                     bool wasMoved,
                                     bool wasResized) override;
        
    private: // variables
        
        LoginForm m_login_form;
        SignUpForm m_signup_form;
    };
}
