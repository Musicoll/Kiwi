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
    // ================================================================================ //
    //                                    ALERT BOX                                     //
    // ================================================================================ //
    
    class AlertBox : public juce::Component, private juce::Button::Listener
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
    
    // ================================================================================ //
    //                                  FORM COMPONENT                                  //
    // ================================================================================ //
    
    class FormComponent : public juce::Component, private juce::Button::Listener
    {
    public: // methods
        
        class Field;
        
        //! @brief Constructor.
        //! @details Creates a login form.
        FormComponent(std::string const& submit_button_text,
                      std::string const& overlay_text = "");
        
        //! @brief Destructor.
        virtual ~FormComponent();
        
        //! @brief This is called when the form is dismissed
        //! @details (either cancelled or when registration succeeds).
        virtual void dismiss();
        
        //! @brief Computes and returns the best height for this form.
        //! @details It will be based on the number of fields and form components.
        int getBestHeight();
        
    protected: // methods
        
        //! @brief Add a new field.
        template<class FieldType, class... Args>
        FieldType& addField(Args&&... args);
        
        //! @brief Returns a field Value.
        juce::Value getFieldValue(std::string const& name);
        
        //! @brief Remove fields by name.
        void removeField(std::string const& name);
        
        //! @brief Remove all fields.
        void clearFields();
        
        //! @brief Show an Alert on the top of the form;
        void showAlert(std::string const& message,
                       AlertBox::Type type = AlertBox::Type::Error);
        
        //! @brief Show overlay
        void showOverlay();
        
        //! @brief Show success overlay
        void showSuccessOverlay(juce::String const& message);
        
        //! @brief Hide overlay
        void hideOverlay();
        
        //! @brief Changes the submit button text
        void setSubmitText(std::string const& submit_text);
        
        //! @brief Returns true if the overlay component is visible.
        bool hasOverlay();
        
        //! @brief Called when the user clicked the submit button.
        //! @details Subclasses need to override this to perform form submission.
        virtual void onUserSubmit() = 0;
        
        //! @brief Called when the user clicked the cancel button.
        //! @details The default implementation will call the dismiss() method,
        //! Subclasses can override this to react to the user cancel action.
        virtual void onUserCancelled();
        
        //! @internal resized method
        void resized() override;
        
        //! @brief Returns the height of all fields
        int getFieldsHeight();
        
    private: // methods
        
        //! @internal paint method
        void paint(juce::Graphics&) override final;
        
        void buttonClicked(juce::Button*) override;
        
        juce::Rectangle<int> getButtonArea() const;
        
    private: // variables
        
        std::string m_overlay_text;
        
        juce::TextButton m_submit_btn, m_cancel_btn;
        
        std::unique_ptr<AlertBox> m_alert_box;
        
        std::vector<std::unique_ptr<Field>> m_fields;
        
        int m_alert_height;
        
        class OverlayComp;
        friend class OverlayComp;
        Component::SafePointer<OverlayComp> m_overlay;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FormComponent)
    };
    
    // ================================================================================ //
    //                              FORM COMPONENT FIELD                                //
    // ================================================================================ //
    
    class FormComponent::Field : public juce::Component
    {
    public: // methods
        
        class SingleLineText;
        class Password;
        class ToggleButton;
        class KiwiLogo;
        class TextButton;
        
        Field(std::string name);
        
        virtual ~Field() = default;
        
        std::string const& getName() const;
        
        virtual juce::Value& getValue() = 0;
        
        virtual int getPreferedHeight();
        
    private: // variables
        
        std::string m_name;
    };
    
    // ================================================================================ //
    //                               SINGLE LINE FIELD                                  //
    // ================================================================================ //
    
    class FormComponent::Field::SingleLineText : public Field
    {
    public: // methods
        
        SingleLineText(std::string name, std::string text, std::string placeholder_text);
        
        virtual ~SingleLineText() = default;
        
        juce::Value& getValue() override;
        
        void resized() override;
        
    protected: // variables
        
        juce::TextEditor m_text_editor;
    };
    
    // ================================================================================ //
    //                                 PASWWORD FIELD                                   //
    // ================================================================================ //
    
    class FormComponent::Field::Password : public SingleLineText
    {
    public: // methods
        
        Password(std::string name, std::string text, std::string placeholder_text);
        ~Password() = default;
    };
    
    // ================================================================================ //
    //                               TOGGLE BUTTON FIELD                                //
    // ================================================================================ //
    
    class FormComponent::Field::ToggleButton : public Field
    {
    public: // methods
        
        ToggleButton(std::string name, std::string text, bool _default = false);
        
        ~ToggleButton() = default;
        
        juce::Value& getValue() override;
        
        void resized() override;
        
        int getPreferedHeight() override;
        
    private: // variables
        
        juce::ToggleButton m_button;
    };
    
    // ================================================================================ //
    //                                TEXT BUTTON FIELD                                 //
    // ================================================================================ //
    
    class FormComponent::Field::TextButton : public Field, public juce::Button::Listener
    {
    public:
        
        TextButton(std::string const& name,
                   std::string const& buton_text,
                   std::function<void()> call_back);
        
        ~TextButton();
        
        juce::Value& getValue() override final;
        
        int getPreferedHeight() override final;
        
        void resized() override;
        
        void buttonClicked(juce::Button *) override final;
        
    private:
        
        juce::TextButton        m_button;
        juce::Value             m_value;
        std::function<void()>   m_call_back;
    };
    
    // ================================================================================ //
    //                                KIWI LOGO FIELD                                   //
    // ================================================================================ //
    
    class FormComponent::Field::KiwiLogo : public Field
    {
    public: // methods
        
        KiwiLogo();
        
        ~KiwiLogo() = default;
        
        void paint(juce::Graphics& g) override;
        
        juce::Value& getValue() override;
        
        int getPreferedHeight() override;
        
    private: // variables
        
        juce::Image m_kiwi_app_image;
        juce::Value m_useless_value;
    };
    
    
    // ================================================================================ //
    //                        FORM COMPONENT - template definitions                     //
    // ================================================================================ //
    
    template<class FieldType, class... Args>
    FieldType& FormComponent::addField(Args&&... args)
    {
        auto field = std::make_unique<FieldType>(std::forward<Args>(args)...);
        addAndMakeVisible(*field);
        auto it = m_fields.emplace(m_fields.end(), std::move(field));
        return dynamic_cast<FieldType&>(*(it->get()));
    }
}
