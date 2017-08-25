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

#include "KiwiApp_LoginForm.h"
#include "../KiwiApp.h"

#include "../KiwiApp_Ressources/KiwiApp_BinaryData.h"

namespace kiwi
{
    AlertBox::AlertBox(std::string const& message,
                       AlertBox::Type type,
                       bool can_cancel,
                       std::function<void(void)> on_close)
    : m_message(message)
    , m_type(type)
    , m_close_fn(on_close)
    {
        if(can_cancel)
        {
            m_close_btn.reset(new juce::TextButton("X", "Close"));
            m_close_btn->setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
            addAndMakeVisible(*m_close_btn);
            m_close_btn->addListener(this);
        }
    }
    
    AlertBox::~AlertBox()
    {
        if(m_close_btn)
        {
            m_close_btn->removeListener(this);
        }
    }
    
    void AlertBox::paint(juce::Graphics& g)
    {
        const auto bgcolor = [](Type const& type) {
            switch (type) {
                case Type::Success: return juce::Colour::fromRGB(72, 175, 95);
                case Type::Error: return juce::Colour::fromRGB(252, 99, 93);
                default: return juce::Colours::grey;
            }
        }(m_type).withAlpha(1.f);
        
        g.setColour(bgcolor);
        g.fillRoundedRectangle(getLocalBounds().reduced(1).toFloat(), 0.f);
        
        g.setFont(16.f);
        g.setColour(bgcolor.contrasting(0.9));
        g.drawText(m_message, getLocalBounds().reduced(10), juce::Justification::verticallyCentred);
    }
    
    void AlertBox::resized()
    {
        if(m_close_btn)
        {
            const int margin = 5;
            const auto bounds = getLocalBounds();
            m_close_btn->setSize(20, 20);
            m_close_btn->setTopRightPosition(bounds.getWidth() - margin, margin);
        }
    }
    
    void AlertBox::buttonClicked(juce::Button* b)
    {
        if(b == m_close_btn.get())
        {
            if(m_close_fn)
            {
                m_close_fn();
            }
            else
            {
                // close alert box
            }
        }
    }
    
    struct Spinner : public juce::Component, private juce::Timer
    {
        Spinner()                       { startTimer(1000 / 50); }
        void timerCallback() override   { repaint(); }
        
        void paint(juce::Graphics& g) override
        {
            getLookAndFeel().drawSpinningWaitAnimation(g, juce::Colours::whitesmoke, 0, 0, getWidth(), getHeight());
        }
    };
    
    class LoginForm::OverlayComp : public juce::Component
    {
    public: // methods
        
        OverlayComp(juce::String message)
        : m_message(message)
        {
            addAndMakeVisible(spinner);
        }
        
        ~OverlayComp() = default;
        
        void paint(juce::Graphics& g) override
        {
            auto& lf = KiwiApp::useLookAndFeel();
            const auto bgcolor = lf.getCurrentColourScheme().getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::windowBackground);
            
            g.fillAll(bgcolor.withAlpha(0.97f));
            
            g.setColour(juce::Colours::whitesmoke);
            g.setFont(22.0f);
            
            g.drawFittedText(m_message,
                             getLocalBounds().reduced(20, 0)
                             .removeFromTop(proportionOfHeight(0.6f)),
                             juce::Justification::centred, 5);
        }
        
        void resized() override
        {
            const int spinnerSize = 40;
            spinner.setBounds((getWidth() - spinnerSize) / 2,
                              proportionOfHeight(0.5f),
                              spinnerSize, spinnerSize);
        }
        
    private: // variables
        
        Spinner spinner;
        juce::String m_message;
        
        JUCE_LEAK_DETECTOR(LoginForm::OverlayComp);
    };
    
    static juce::juce_wchar getDefaultPasswordChar() noexcept
    {
        #if JUCE_LINUX
        return 0x2022;
        #else
        return 0x25cf;
        #endif
    }
    
    LoginForm::LoginForm()
    : m_password_box(juce::String(), getDefaultPasswordChar())
    , m_submit_btn(TRANS("Login"))
    , m_cancel_btn(TRANS("Cancel"))
    , m_remember_me_checkbox("Remember me")
    , m_kiwi_app_image(juce::ImageCache::getFromMemory(binary_data::images::kiwi_icon_png,
                                                       binary_data::images::kiwi_icon_png_size))
    {
        //(force use of a default system font to make sure it has the password blob character)
        juce::Font font(juce::Font::getDefaultTypefaceForFont(juce::Font(juce::Font::getDefaultSansSerifFontName(),
                                                                         juce::Font::getDefaultStyle(), 5.0f)));
        
        font.setHeight(22);
        
        m_email_box.setInputRestrictions(512);
        m_email_box.setFont(font);
        m_email_box.setText(KiwiApp::getCurrentUser().getName(), false);
        
        m_password_box.setInputRestrictions(64);
        m_password_box.setFont(font);
        
        addAndMakeVisible(m_email_box);
        addAndMakeVisible(m_password_box);
        addAndMakeVisible(m_remember_me_checkbox);
        addAndMakeVisible(m_submit_btn);
        addAndMakeVisible(m_cancel_btn);
        
        m_remember_me_checkbox.setToggleState(getAppSettings().network().getRememberUserFlag(),
                                              juce::NotificationType::dontSendNotification);
        
        m_email_box.setEscapeAndReturnKeysConsumed(false);
        m_password_box.setEscapeAndReturnKeysConsumed(false);
        
        m_submit_btn.addShortcut(juce::KeyPress(juce::KeyPress::returnKey));
        
        m_submit_btn.addListener(this);
        m_cancel_btn.addListener(this);
        
        lookAndFeelChanged();
        setSize(400, 330);
        resized();
    }
    
    LoginForm::~LoginForm()
    {
        m_overlay.deleteAndZero();
    }
    
    void LoginForm::paint(juce::Graphics& g)
    {
        auto& lf = KiwiApp::useLookAndFeel();
        const auto bgcolor = lf.getCurrentColourScheme().getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::windowBackground);
        
        g.fillAll(bgcolor);
        
        float img_size = 100;
        int padding = 15;
        
        auto bounds = getLocalBounds();
        bounds.removeFromTop((m_alert_box != nullptr) ? m_alert_box->getHeight() : 0);
        
        g.drawImage(m_kiwi_app_image,
                    bounds.reduced(padding)
                    .removeFromTop(img_size).toFloat(),
                    juce::RectanglePlacement::xMid, false);
    }
    
    void LoginForm::resized()
    {
        const int alert_height = 50;
        const int button_height = 35;
        
        juce::Rectangle<int> r(getLocalBounds());
        
        if(m_alert_box)
        {
            m_alert_box->setBounds(r.removeFromTop(alert_height));
        }
        
        r.removeFromTop(120);
        
        r.reduce(10, 10);
        
        const int boxHeight = 32;
        
        m_email_box.setBounds(r.removeFromTop(boxHeight));
        
        r.removeFromTop(20);
        m_password_box.setBounds(r.removeFromTop(boxHeight));
        
        r.removeFromTop(10);
        m_remember_me_checkbox.setBounds(r.removeFromTop(boxHeight));
        
        r.removeFromTop(10);
        juce::Rectangle<int> buttonArea(r.removeFromTop(button_height));
        m_submit_btn.changeWidthToFitText(button_height);
        m_cancel_btn.changeWidthToFitText(button_height);
        
        const int gap = 20;
        buttonArea = buttonArea.withSizeKeepingCentre(m_submit_btn.getWidth()
                                                      +(m_cancel_btn.isVisible() ? gap + m_cancel_btn.getWidth() : 0),
                                                      button_height);
        
        m_submit_btn.setBounds(buttonArea.removeFromLeft(m_submit_btn.getWidth()));
        buttonArea.removeFromLeft(gap);
        m_cancel_btn.setBounds(buttonArea);
        
        if(m_overlay != nullptr)
        {
            m_overlay->setBounds(getLocalBounds());
        }
    }
    
    void LoginForm::lookAndFeelChanged()
    {
        juce::Colour labelCol(findColour(juce::TextEditor::backgroundColourId).contrasting(0.5f));
        
        m_email_box.setTextToShowWhenEmpty(TRANS("Username or Email"), labelCol);
        m_password_box.setTextToShowWhenEmpty(TRANS("Password"), labelCol);
    }
    
    void LoginForm::showOverlay()
    {
        if(m_overlay == nullptr)
        {
            addAndMakeVisible(m_overlay = new OverlayComp("login..."));
            resized();
        }
    }
    
    void LoginForm::hideOverlay()
    {
        if(m_overlay)
        {
            removeChildComponent(m_overlay);
            m_overlay.deleteAndZero();
        }
    }
    
    void LoginForm::showAlert(std::string const& message, AlertBox::Type type)
    {
        const bool was_showing = (m_alert_box != nullptr);
        if(was_showing)
        {
            removeChildComponent(m_alert_box.get());
        }
        
        m_alert_box.reset(new AlertBox(message, type, true, [this](){
            removeChildComponent(m_alert_box.get());
            m_alert_box.reset();
            setBounds(getBounds().withHeight(getHeight() - 50));
        }));
        
        addAndMakeVisible(*m_alert_box);
        
        if(!was_showing)
        {
            setBounds(getBounds().withHeight(getHeight() + 50));
        }
        else
        {
            resized();
        }
    }
    
    void LoginForm::buttonClicked(juce::Button* b)
    {
        if(b == &m_submit_btn)
        {
            attemptRegistration();
        }
        else if(b == &m_cancel_btn)
        {
            dismiss();
        }
    }
    
    
    void LoginForm::attemptRegistration()
    {
        if(m_overlay == nullptr)
        {
            if(m_email_box.getText().trim().length() < 3)
            {
                showAlert("Please enter a valid email address or username!", AlertBox::Type::Error);
                return;
            }
            
            if(m_password_box.getText().trim().length() < 3)
            {
                showAlert("Please enter a valid password!", AlertBox::Type::Error);
                return;
            }
            
            showOverlay();
            
            auto success_callback = [this]()
            {
                KiwiApp::useInstance().useScheduler().schedule([this]() {
                    getAppSettings().network().setRememberUserFlag(m_remember_me_checkbox.getToggleState());
                    dismiss();
                }, std::chrono::milliseconds(500));
            };
            
            auto error_callback = [this](Api::Error error)
            {
                KiwiApp::useInstance().useScheduler().schedule([this, message = error.getMessage()](){
                    
                    std::cout << "Error: " << message << "\n";
                    
                    hideOverlay();
                    showAlert(message, AlertBox::Type::Error);
                    
                }, std::chrono::milliseconds(500));
            };
            
            KiwiApp::login(m_email_box.getText().toStdString(),
                           m_password_box.getText().toStdString(),
                           std::move(success_callback),
                           std::move(error_callback));
        }
    }
    
    void LoginForm::dismiss()
    {
        if(Window* parent_window = findParentComponentOfClass<Window>())
        {
            parent_window->close();
        }
    }
}
