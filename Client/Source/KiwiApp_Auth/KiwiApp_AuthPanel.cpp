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

#include "KiwiApp_AuthPanel.h"

#include "../KiwiApp.h"
#include "../KiwiApp_General/KiwiApp_CommandIDs.h"

namespace kiwi
{
    // ================================================================================ //
    //                                   LOGIN FORM                                     //
    // ================================================================================ //
    
    LoginForm::LoginForm()
    : FormComponent("Login", "login...")
    {
        setState(State::Login);
    }
    
    void LoginForm::setState(State state)
    {
        clearFields();
        
        m_state = state;
        
        switch (state)
        {
            case State::Login:
            {
                addField<Field::KiwiLogo>();
                
                addField<Field::SingleLineText>("username",
                                                KiwiApp::getCurrentUser().getName(),
                                                "Username or Email");
                
                addField<Field::Password>("password", "", "Password");
                
                addField<Field::ToggleButton>("remember_me",
                                              "Remember me",
                                              getAppSettings().network().getRememberUserFlag());
                
                addField<Field::TextButton>("forgot_pass",
                                            "Forgot password ?",
                                            [this]()
                                            {
                                                setState(State::Request);
                                                showAlert("Enter email to request reset token", AlertBox::Type::Info);
                                            });
                
                setSubmitText("Login");
                
                break;
            }
            case State::Request:
            {
                addField<Field::KiwiLogo>();
                
                addField<Field::SingleLineText>("email", "", "Email");
                addField<Field::TextButton>("already_have",
                                            "Already have token ?",
                                            [this]()
                                            {
                                                setState(State::Reset);
                                                showAlert("Enter reset token.", AlertBox::Type::Info);
                                            });
                
                setSubmitText("Send");
                
                break;
            }
            case State::Reset:
            {
                addField<Field::KiwiLogo>();
                
                addField<Field::SingleLineText>("token", "", "Token");
                addField<Field::SingleLineText>("newpass", "", "New Password");
                
                setSubmitText("Send");
                
                break;
            }
        }
        
        setSize(getWidth(), getBestHeight());
    }
    
    void LoginForm::performPassReset()
    {
        auto success_callback = [this](std::string const& message)
        {
            KiwiApp::useInstance().useScheduler().schedule([this]() {
                setState(State::Login);
                showAlert("Password reset. Try login again.", AlertBox::Type::Info);
            });
        };
        
        auto error_callback = [this](Api::Error error)
        {
            std::string error_message = error.getMessage();
            
            KiwiApp::useInstance().useScheduler().schedule([this, error_message]() {
                showAlert(error_message, AlertBox::Type::Error);
            });
        };
        
        const std::string token = getFieldValue("token").toString().toStdString();
        const std::string newpass = getFieldValue("newpass").toString().toStdString();
        
        KiwiApp::useApi().resetPassword(token, newpass, success_callback, error_callback);
    }
    
    void LoginForm::performPassRequest()
    {
        auto success_callback = [this](std::string const& message)
        {
            KiwiApp::useInstance().useScheduler().schedule([this]() {
                setState(State::Reset);
                showAlert("Reset token sent. Check your email.", AlertBox::Type::Info);
            });
        };
        
        auto error_callback = [this](Api::Error error)
        {
            std::string error_message = error.getMessage();
            
            KiwiApp::useInstance().useScheduler().schedule([this, error_message]() {
                showAlert(error_message, AlertBox::Type::Error);
            });
        };
        
        const std::string email = getFieldValue("email").toString().toStdString();
        
        KiwiApp::useApi().requestPasswordToken(email, success_callback, error_callback);
    }
    
    void LoginForm::performLogin()
    {
        const auto username = getFieldValue("username").toString();
        const auto password = getFieldValue("password").toString();
        const bool remember_me = getFieldValue("remember_me").getValue();
        
        if(username.trim().length() < 3)
        {
            showAlert("Please enter a valid email address or username!", AlertBox::Type::Error);
            return;
        }
        
        if(password.trim().length() < 3)
        {
            showAlert("Please enter a valid password!", AlertBox::Type::Error);
            return;
        }
        
        showOverlay();
        
        auto success_callback = [this, remember_me]()
        {
            KiwiApp::useInstance().useScheduler().schedule([this, remember_me]() {
                
                showSuccessOverlay("Login success !");
                
                KiwiApp::useInstance().useScheduler().schedule([this, remember_me]() {
                    
                    getAppSettings().network().setRememberUserFlag(remember_me);
                    dismiss();
                    
                }, std::chrono::milliseconds(1000));
                
            }, std::chrono::milliseconds(500));
        };
        
        auto error_callback = [this](Api::Error error)
        {
            KiwiApp::useInstance().useScheduler().schedule([this, message = error.getMessage()]() {
                
                hideOverlay();
                showAlert(message, AlertBox::Type::Error);
                
            }, std::chrono::milliseconds(500));
        };
        
        KiwiApp::login(username.toStdString(),
                       password.toStdString(),
                       std::move(success_callback),
                       std::move(error_callback));
    }
    
    void LoginForm::onUserSubmit()
    {
        switch (m_state)
        {
            case State::Login:
            {
                performLogin();
                break;
            }
            case State::Request:
            {
                performPassRequest();
                break;
            }
            case State::Reset:
            {
                performPassReset();
                break;
            }
        }
    }
    
    void LoginForm::onUserCancelled()
    {
        switch(m_state)
        {
            case State::Login:
            {
                dismiss();
                break;
            }
            case State::Request:
            {
                setState(State::Login);
                break;
            }
            case State::Reset:
            {
                setState(State::Login);
                break;
            }
        }
    }
    
    // ================================================================================ //
    //                                  SIGNUP FORM                                     //
    // ================================================================================ //
    
    SignUpForm::SignUpForm()
    : FormComponent("Register", "register...")
    {
        addField<Field::KiwiLogo>();
        addField<Field::SingleLineText>("username", "", "Username");
        addField<Field::SingleLineText>("email", "", "Email");
        addField<Field::Password>("password", "", "Password");
        //addField<Field::Password>("password_check", "", "Password again...");
    }
    
    void SignUpForm::onUserSubmit()
    {
        const auto username = getFieldValue("username").toString();
        const auto email = getFieldValue("email").toString();
        const auto password = getFieldValue("password").toString();
        //const auto password_check = getFieldValue("password_check").toString();
        
        if(username.trim().length() < 3)
        {
            showAlert("Please enter a valid user name!", AlertBox::Type::Error);
            return;
        }
        
        if(email.trim().length() < 3)
        {
            showAlert("Please enter a valid email address!", AlertBox::Type::Error);
            return;
        }
        
        if(password.trim().length() < 3)
        {
            showAlert("Please enter a valid password!", AlertBox::Type::Error);
            return;
        }
        
        /*
        if(password != password_check)
        {
            showAlert("The two passwords must be the same!", AlertBox::Type::Error);
            return;
        }
        */
        
        showOverlay();
        
        auto success_callback = [this](std::string message)
        {
            KiwiApp::useInstance().useScheduler().schedule([this, message]() {
                
                showSuccessOverlay(message);
                
                KiwiApp::useInstance().useScheduler().schedule([this]() {
                    
                    if(AuthPanel* auth_panel = findParentComponentOfClass<AuthPanel>())
                    {
                        hideOverlay();
                        auth_panel->setCurrentTabIndex(AuthPanel::FormType::Login);
                    }
                    
                }, std::chrono::milliseconds(1000));
                
            }, std::chrono::milliseconds(500));
        };
        
        auto error_callback = [this](Api::Error error)
        {
            KiwiApp::useInstance().useScheduler().schedule([this, message = error.getMessage()](){
                
                hideOverlay();
                showAlert(message, AlertBox::Type::Error);
                
            }, std::chrono::milliseconds(500));
        };
        
        KiwiApp::signup(username.toStdString(),
                        email.toStdString(),
                        password.toStdString(),
                        std::move(success_callback),
                        std::move(error_callback));
    }
    
    // ================================================================================ //
    //                                   AUTH PANEL                                     //
    // ================================================================================ //
    
    AuthPanel::AuthPanel(FormType form_type)
    : juce::TabbedComponent(juce::TabbedButtonBar::Orientation::TabsAtTop)
    , m_login_form()
    , m_signup_form()
    {
        auto& lf = KiwiApp::useLookAndFeel();
        const auto bgcolor = lf.getCurrentColourScheme().getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::windowBackground);
        
        addTab("Login", bgcolor, &m_login_form, false, static_cast<int>(FormType::Login));
        addTab("Register", bgcolor, &m_signup_form, false, static_cast<int>(FormType::SignUp));
        
        m_login_form.addComponentListener(this);
        m_signup_form.addComponentListener(this);
        setCurrentTabIndex(static_cast<int>(form_type));
        
        setColour(juce::TabbedComponent::ColourIds::outlineColourId, juce::Colours::transparentBlack);
        setColour(juce::TabbedComponent::ColourIds::backgroundColourId, bgcolor.contrasting(0.05));
        
        const int width = 440;
        m_login_form.setSize(width, m_login_form.getBestHeight());
        m_signup_form.setSize(width, m_signup_form.getBestHeight());
        
        auto& current_comp = dynamic_cast<FormComponent&>(*getCurrentContentComponent());
        setSize(width, current_comp.getBestHeight());
    }
    
    AuthPanel::~AuthPanel()
    {
        ;
    }
    
    void AuthPanel::updateSize(FormType type)
    {
        switch(type)
        {
            case FormType::Login:
            {
                setSize(getWidth(), m_login_form.getBestHeight());
                break;
            }
            case FormType::SignUp:
            {
                setSize(getWidth(), m_signup_form.getBestHeight());
                break;
            }
            default: break;
        }
    }
    
    void AuthPanel::currentTabChanged(int new_tab_index, juce::String const& /*new_tab_name*/)
    {
        updateSize(FormType(new_tab_index));
    }
    
    void AuthPanel::componentMovedOrResized(juce::Component& /*component*/, bool /*moved*/, bool resized)
    {
        if(resized)
        {
            updateSize(FormType(getCurrentTabIndex()));
        }
    }
}
