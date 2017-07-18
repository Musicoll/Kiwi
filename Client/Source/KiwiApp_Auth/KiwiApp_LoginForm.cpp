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

namespace kiwi
{
    struct Spinner : public juce::Component, private juce::Timer
    {
        Spinner()                       { startTimer(1000 / 50); }
        void timerCallback() override   { repaint(); }
        
        void paint(juce::Graphics& g) override
        {
            getLookAndFeel().drawSpinningWaitAnimation(g, juce::Colours::whitesmoke, 0, 0, getWidth(), getHeight());
        }
    };
    
    struct LoginForm::OverlayComp : public juce::Component
    {
        OverlayComp()
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
            g.setFont(18.0f);
            
            g.drawFittedText(TRANS("login..."), getLocalBounds().reduced(20, 0).removeFromTop(proportionOfHeight(0.6f)), juce::Justification::centred, 5);
        }
        
        void resized() override
        {
            const int spinnerSize = 40;
            spinner.setBounds((getWidth() - spinnerSize) / 2,
                              proportionOfHeight(0.5f),
                              spinnerSize, spinnerSize);
        }
        
        Spinner spinner;
        
        JUCE_LEAK_DETECTOR(LoginForm::OverlayComp)
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
    : message(juce::String(), "Enter an Email/username and password to log in")
    , passwordBox(juce::String(), getDefaultPasswordChar())
    , registerButton(TRANS("Register"))
    , cancelButton(TRANS("Cancel"))
    {
        setOpaque(true);
        
        //emailBox.setText(status.getUserEmail());
        emailBox.setText("");
        message.setJustificationType(juce::Justification::centred);
        
        addAndMakeVisible(message);
        addAndMakeVisible(emailBox);
        addAndMakeVisible(passwordBox);
        
        addAndMakeVisible(registerButton);
        addAndMakeVisible(cancelButton);
        
        emailBox.setEscapeAndReturnKeysConsumed(false);
        passwordBox.setEscapeAndReturnKeysConsumed(false);
        
        registerButton.addShortcut(juce::KeyPress(juce::KeyPress::returnKey));
        
        registerButton.addListener(this);
        cancelButton.addListener(this);
        
        lookAndFeelChanged();
        setSize(500, 250);
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
    }
    
    void LoginForm::resized()
    {
        const int buttonHeight = 22;
        
        juce::Rectangle<int> r(getLocalBounds().reduced(10, 20));
        
        juce::Rectangle<int> buttonArea(r.removeFromBottom(buttonHeight));
        registerButton.changeWidthToFitText(buttonHeight);
        cancelButton.changeWidthToFitText(buttonHeight);
        
        const int gap = 20;
        buttonArea = buttonArea.withSizeKeepingCentre(registerButton.getWidth()
                                                       +(cancelButton.isVisible() ? gap + cancelButton.getWidth() : 0),
                                                       buttonHeight);
        registerButton.setBounds(buttonArea.removeFromLeft(registerButton.getWidth()));
        buttonArea.removeFromLeft(gap);
        cancelButton.setBounds(buttonArea);
        
        r.removeFromBottom(20);
        
        //(force use of a default system font to make sure it has the password blob character)
        juce::Font font(juce::Font::getDefaultTypefaceForFont(juce::Font(juce::Font::getDefaultSansSerifFontName(), juce::Font::getDefaultStyle(), 5.0f)));
        
        const int boxHeight = 24;
        passwordBox.setBounds(r.removeFromBottom(boxHeight));
        passwordBox.setInputRestrictions(64);
        passwordBox.setFont(font);
        
        r.removeFromBottom(20);
        emailBox.setBounds(r.removeFromBottom(boxHeight));
        emailBox.setInputRestrictions(512);
        emailBox.setFont(font);
        
        r.removeFromBottom(20);
        
        message.setBounds(r);
        
        if(m_overlay != nullptr)
            m_overlay->setBounds(getLocalBounds());
    }
    
    void LoginForm::lookAndFeelChanged()
    {
        juce::Colour labelCol(findColour(juce::TextEditor::backgroundColourId).contrasting(0.5f));
        
        emailBox.setTextToShowWhenEmpty(TRANS("Email Address"), labelCol);
        passwordBox.setTextToShowWhenEmpty(TRANS("Password"), labelCol);
    }
    
    void LoginForm::showBubbleMessage(juce::String const& text, Component& target)
    {
        bubble = new juce::BubbleMessageComponent(500);
        addChildComponent(bubble);
        
        juce::AttributedString attString;
        attString.append(text, juce::Font(16.0f));
        
        bubble->showAt(getLocalArea(&target, target.getLocalBounds()),
                        attString, 2000,  // numMillisecondsBeforeRemoving
                        true,  // removeWhenMouseClicked
                        false); // deleteSelfAfterUse
    }
    
    void LoginForm::buttonClicked(juce::Button* b)
    {
        if(b == &registerButton)
            attemptRegistration();
        else if(b == &cancelButton)
            dismiss();
    }
    
    void LoginForm::attemptRegistration()
    {
        if(m_overlay == nullptr)
        {
            if(emailBox.getText().trim().length() < 3)
            {
                showBubbleMessage(TRANS("Please enter a valid email address!"), emailBox);
                return;
            }
            
            if(passwordBox.getText().trim().length() < 3)
            {
                showBubbleMessage(TRANS("Please enter a valid password!"), passwordBox);
                return;
            }
            
            //status.setUserEmail(emailBox.getText());
            addAndMakeVisible(m_overlay = new OverlayComp());
            
            resized();
        }
    }
    
    void LoginForm::dismiss()
    {
        delete this;
    }
}
