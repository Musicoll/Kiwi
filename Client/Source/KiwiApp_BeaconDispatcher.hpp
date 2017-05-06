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

#ifndef KIWI_APP_BEACON_DISPATCHER_HPP_INCLUDED
#define KIWI_APP_BEACON_DISPATCHER_HPP_INCLUDED

#include <KiwiEngine/KiwiEngine_Instance.hpp>

#include "KiwiApp_Window.hpp"

namespace kiwi
{
    class BeaconDispatcher;
    
    // ================================================================================ //
    //                                  BEACON DISPATCHER                               //
    // ================================================================================ //
    
    //! @brief A Component that allows to dispatch messages to Beacon::Castaway objects.
    class BeaconDispatcherElem : public juce::Component, public juce::Button::Listener, juce::Slider::Listener
    {
    public: // methods
        
        //! @brief Constructor.
        BeaconDispatcherElem(engine::Instance& instance);
        
        //! @brief Destructor.
        ~BeaconDispatcherElem();
        
        //! @brief Called when resized.
        void resized() override;
        
        //! @brief juce::Component.
        void paint(juce::Graphics& g) override;
        
        //! @brief Called when the button is clicked.
        void buttonClicked(juce::Button*) override;
        
        //! @brief Set the background colour.
        void setBackgroundColor(juce::Colour const& color);
        
        void restoreState(juce::XmlElement* saved_state);
        
        void saveState(juce::XmlElement* saved_state);
    
    private: // methods
        
        class TextValueComponent : public juce::Component, public juce::Button::Listener
        {
        public:
            TextValueComponent(BeaconDispatcherElem& owner);
            ~TextValueComponent();
            
            void buttonClicked(juce::Button*) override;
            void resized() override;
            
        private:
            BeaconDispatcherElem& m_owner;
            juce::TextEditor m_message_editor;
            juce::TextButton m_send_button;
        };
        
        //! @brief dispatch message to castaways
        void sendValue(std::vector<Atom> const& args) const;
        
        //! @brief dispatch message to castaways
        void send(std::string const& name, std::vector<Atom> const& args) const;
        
        //! @brief Called when a slider value changed.
        void sliderValueChanged(juce::Slider* slider) override;
        
    private: // members
        
        engine::Instance&       m_instance;
        juce::TextEditor        m_beacon_name_editor;
        TextValueComponent      m_text_value;
        juce::Slider            m_message_slider;
        juce::ToggleButton      m_toggle_value;
        juce::TabbedComponent   m_message_tab;
        
        juce::Colour            m_bgcolor;
    };
    
    // ================================================================================ //
    //                                   CONSOLE TOOLBAR                                //
    // ================================================================================ //
    
    class BeaconDispatcherToolbarFactory : public juce::ToolbarItemFactory
    {
    public: // methods
        
        //! @brief Constructor.
        BeaconDispatcherToolbarFactory();
        
        enum ItemIds
        {
            addItem     = 1,
            removeItem  = 2,
        };
        
        void getAllToolbarItemIds(juce::Array<int>& ids) override;
        void getDefaultItemSet(juce::Array<int>& ids) override;
        
        juce::ToolbarItemComponent* createItem(int itemId) override;
    };
    
    // ================================================================================ //
    //                                  BEACON DISPATCHER                               //
    // ================================================================================ //
    
    class BeaconDispatcher : public juce::Component, public juce::ApplicationCommandTarget
    {
    public: // methods
        
        //! @brief Constructor.
        BeaconDispatcher(engine::Instance& instance);
        
        //! @brief Destructor.
        ~BeaconDispatcher();
        
        //! @brief Called when resized.
        void resized() override;
        
        void addElem();
        
        void removeElem();
        
        bool restoreState();
        
        void saveState();
        
        // ================================================================================ //
        //                              APPLICATION COMMAND TARGET                          //
        // ================================================================================ //
        
        juce::ApplicationCommandTarget* getNextCommandTarget() override;
        void getAllCommands(juce::Array<juce::CommandID>& commands) override;
        void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
        bool perform(const InvocationInfo& info) override;
        
    private: // variables
        
        void updateLayout();
        
        engine::Instance&                                   m_instance;
        std::vector<std::unique_ptr<BeaconDispatcherElem>>  m_components;
        juce::Toolbar                                       m_toolbar;
        BeaconDispatcherToolbarFactory                      m_toolbar_factory;
        int                                                 m_elem_height = 100;
    };
}

#endif // KIWI_APP_BEACON_DISPATCHER_HPP_INCLUDED
