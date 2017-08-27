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

#include "KiwiApp_FormComponent.h"

#include "../KiwiApp.h"
#include "../KiwiApp_Ressources/KiwiApp_BinaryData.h"

namespace kiwi
{
    // ================================================================================ //
    //                                    ALERT BOX                                     //
    // ================================================================================ //
    
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
    
    // ================================================================================ //
    //                                     SPINNER                                      //
    // ================================================================================ //
    
    struct Spinner : public juce::Component, private juce::Timer
    {
        Spinner()                       { startTimer(1000 / 50); }
        void timerCallback() override   { repaint(); }
        
        void paint(juce::Graphics& g) override
        {
            getLookAndFeel().drawSpinningWaitAnimation(g, juce::Colours::whitesmoke, 0, 0, getWidth(), getHeight());
        }
    };
    
    // ================================================================================ //
    //                               FORM COMPONENT OVERLAY                             //
    // ================================================================================ //
    
    class FormComponent::OverlayComp : public juce::Component
    {
    public: // methods
        
        OverlayComp(juce::String message)
        : m_message(message)
        , m_show_success(false)
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
        
        void showSuccess(juce::String message)
        {
            m_show_success = true;
            m_message = message;
            spinner.setVisible(false);
            repaint();
        }
        
    private: // variables
        
        Spinner spinner;
        juce::String m_message;
        bool m_show_success = false;
        
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
    
    // ================================================================================ //
    //                              FORM COMPONENT FIELD                                //
    // ================================================================================ //
    
    FormComponent::Field::Field(std::string name)
    : juce::Component(name)
    , m_name(name)
    {
        ;
    }
    
    std::string const& FormComponent::Field::getName() const
    {
        return m_name;
    }
    
    int FormComponent::Field::getPreferedHeight()
    {
        return 32;
    }
    
    // ================================================================================ //
    //                               SINGLE LINE FIELD                                  //
    // ================================================================================ //
    
    FormComponent::Field::SingleLineText::SingleLineText(std::string name,
                                                         std::string text,
                                                         std::string placeholder_text)
    : Field(name)
    , m_text_editor()
    {
        //(force use of a default system font to make sure it has the password blob character)
        static juce::Font font(juce::Font::getDefaultTypefaceForFont(juce::Font(juce::Font::getDefaultSansSerifFontName(),
                                                                                juce::Font::getDefaultStyle(), 5.0f)));
        m_text_editor.setFont(font.withHeight(22));
        
        m_text_editor.setInputRestrictions(512);
        m_text_editor.setText(text, false);
        m_text_editor.setEscapeAndReturnKeysConsumed(false);
        
        juce::Colour labelCol(findColour(juce::TextEditor::backgroundColourId).contrasting(0.5f));
        m_text_editor.setTextToShowWhenEmpty(placeholder_text, labelCol);
        
        addAndMakeVisible(m_text_editor);
    }
    
    juce::Value& FormComponent::Field::SingleLineText::getValue()
    {
        return m_text_editor.getTextValue();
    }
    
    void FormComponent::Field::SingleLineText::resized()
    {
        m_text_editor.setBounds(getLocalBounds());
    }
    
    // ================================================================================ //
    //                                 PASWWORD FIELD                                   //
    // ================================================================================ //
    
    FormComponent::Field::Password::Password(std::string name,
                                             std::string text,
                                             std::string placeholder_text)
    : SingleLineText(name, text, placeholder_text)
    {
        m_text_editor.setPasswordCharacter(getDefaultPasswordChar());
        m_text_editor.setInputRestrictions(64);
    }
    
    // ================================================================================ //
    //                               TOGGLE BUTTON FIELD                                //
    // ================================================================================ //
    
    FormComponent::Field::ToggleButton::ToggleButton(std::string name,
                                                     std::string text,
                                                     bool _default)
    : Field(name)
    , m_button(text)
    {
        m_button.setToggleState(_default, juce::NotificationType::dontSendNotification);
        addAndMakeVisible(m_button);
    }
    
    juce::Value& FormComponent::Field::ToggleButton::getValue()
    {
        return m_button.getToggleStateValue();
    }
    
    void FormComponent::Field::ToggleButton::resized()
    {
        m_button.setBounds(getLocalBounds());
    }
    
    int FormComponent::Field::ToggleButton::getPreferedHeight()
    {
        return 20;
    }
    
    // ================================================================================ //
    //                                KIWI LOGO FIELD                                   //
    // ================================================================================ //
    
    FormComponent::Field::KiwiLogo::KiwiLogo()
    : Field("KiwiLogo")
    , m_kiwi_app_image(juce::ImageCache::getFromMemory(binary_data::images::kiwi_icon_png,
                                                       binary_data::images::kiwi_icon_png_size))
    {
        ;
    }
    
    void FormComponent::Field::KiwiLogo::paint(juce::Graphics& g)
    {
        int padding = 0;
        
        auto bounds = getLocalBounds();
        
        g.drawImage(m_kiwi_app_image,
                    bounds.reduced(padding).toFloat(),
                    juce::RectanglePlacement::xMid, false);
    }
    
    juce::Value& FormComponent::Field::KiwiLogo::getValue()
    {
        return m_useless_value;
    }
    
    int FormComponent::Field::KiwiLogo::getPreferedHeight()
    {
        return 100;
    }
    
    // ================================================================================ //
    //                                  FORM COMPONENT                                  //
    // ================================================================================ //
    
    FormComponent::FormComponent(std::string const& submit_button_text,
                                 std::string const& overlay_text)
    : m_submit_button_text(submit_button_text)
    , m_overlay_text(overlay_text)
    , m_submit_btn(m_submit_button_text)
    , m_cancel_btn(TRANS("Cancel"))
    {
        addAndMakeVisible(m_submit_btn);
        addAndMakeVisible(m_cancel_btn);
        m_submit_btn.addShortcut(juce::KeyPress(juce::KeyPress::returnKey));
        m_submit_btn.addListener(this);
        m_cancel_btn.addListener(this);
    }
    
    FormComponent::~FormComponent()
    {
        m_overlay.deleteAndZero();
    }
    
    void FormComponent::paint(juce::Graphics& g)
    {
        auto& lf = KiwiApp::useLookAndFeel();
        const auto bgcolor = lf.getCurrentColourScheme().getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::windowBackground);
        
        g.fillAll(bgcolor);
        
        auto bounds = getLocalBounds();
        bounds.removeFromTop((m_alert_box != nullptr) ? m_alert_box->getHeight() : 0);
        
        g.setColour(bgcolor.contrasting(0.05));
        g.fillRect(getButtonArea());
    }
    
    void FormComponent::resized()
    {
        const auto bounds(getLocalBounds());
        auto r(bounds);
        
        if(m_alert_box)
        {
            const int alert_height = 50;
            m_alert_box->setBounds(r.removeFromTop(alert_height));
        }
        
        r.reduce(10, 10);
        
        for(auto& field : m_fields)
        {
            field->setBounds(r.removeFromTop(field->getPreferedHeight()));
            r.removeFromTop(20);
        }
        
        const int button_height = 30;
        m_submit_btn.changeWidthToFitText(button_height);
        m_cancel_btn.changeWidthToFitText(button_height);
        
        const int gap = 15;
        const int sep = 5;
        auto buttonArea = getButtonArea().reduced(gap);
        auto submit_bounds = buttonArea.removeFromLeft(buttonArea.getWidth()*0.5);
        m_submit_btn.setTopRightPosition(submit_bounds.getRight() - sep, submit_bounds.getY());
        m_cancel_btn.setTopLeftPosition(buttonArea.getX() + sep, buttonArea.getY());
        
        if(m_overlay != nullptr)
        {
            m_overlay->setBounds(bounds);
        }
    }
    
    juce::Rectangle<int> FormComponent::getButtonArea() const
    {
        const int button_height = 30;
        return {getLocalBounds().removeFromBottom(button_height*2)};
    }
    
    void FormComponent::showOverlay()
    {
        if(m_overlay == nullptr)
        {
            addAndMakeVisible(m_overlay = new OverlayComp(m_overlay_text));
            resized();
        }
    }
    
    void FormComponent::showSuccessOverlay(juce::String const& message)
    {
        showOverlay();
        m_overlay->showSuccess(message);
    }
    
    void FormComponent::hideOverlay()
    {
        if(m_overlay)
        {
            removeChildComponent(m_overlay);
            m_overlay.deleteAndZero();
        }
    }
    
    bool FormComponent::hasOverlay()
    {
        return (m_overlay != nullptr);
    }
    
    void FormComponent::showAlert(std::string const& message, AlertBox::Type type)
    {
        const bool was_showing = (m_alert_box != nullptr);
        if(was_showing)
        {
            removeChildComponent(m_alert_box.get());
        }
        
        const int alert_height = 50;
        
        m_alert_box.reset(new AlertBox(message, type, true, [this](){
            removeChildComponent(m_alert_box.get());
            m_alert_box.reset();
            setBounds(getBounds().withHeight(getHeight() - alert_height));
        }));
        
        addAndMakeVisible(*m_alert_box);
        
        if(!was_showing)
        {
            setBounds(getBounds().withHeight(getHeight() + alert_height));
        }
        else
        {
            resized();
        }
    }
    
    void FormComponent::buttonClicked(juce::Button* b)
    {
        if(b == &m_submit_btn)
        {
            if(!hasOverlay())
            {
                onUserSubmit();
            }
        }
        else if(b == &m_cancel_btn)
        {
            onUserCancelled();
        }
    }
    
    void FormComponent::onUserCancelled()
    {
        dismiss();
    }
    
    void FormComponent::dismiss()
    {
        if(Window* parent_window = findParentComponentOfClass<Window>())
        {
            parent_window->close();
        }
    }
    
    juce::Value FormComponent::getFieldValue(std::string const& name)
    {
        for(auto& field : m_fields)
        {
            if(field->getName() == name)
            {
                return field->getValue();
            }
        }
        
        return {};
    }
    
    int FormComponent::getFieldsHeight()
    {
        int total = 0;
        for(auto& field : m_fields)
        {
            total += field->getPreferedHeight();
        }
        
        return total;
    }
    
    int FormComponent::getBestHeight()
    {
        // TODO: add margin concept in Field to replace this messy hard typed height computation...
        
        int best_height = 0;
        best_height += (m_alert_box != nullptr) ? m_alert_box->getHeight() : 0;
        best_height += 10;
        best_height += getFieldsHeight();
        
        // field margin bottom
        best_height += (m_fields.size() * 20);
        
        best_height += 10;
        best_height += getButtonArea().getHeight();
        
        best_height += 20;
        
        return best_height;
    }
}
