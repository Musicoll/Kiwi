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

#include "KiwiApp_BeaconDispatcher.hpp"

#include "KiwiApp.hpp"
#include "KiwiApp_BinaryData.hpp"
#include "KiwiApp_CommandIDs.hpp"
#include "KiwiApp_StoredSettings.hpp"

namespace kiwi
{
    BeaconDispatcherElem::TextValueComponent::TextValueComponent(BeaconDispatcherElem& owner) :
    m_owner(owner),
    m_message_editor("message_editor"),
    m_send_button("Send")
    {
        // message field
        m_message_editor.setReturnKeyStartsNewLine(false);
        m_message_editor.setMultiLine(false, false);
        m_message_editor.setTextToShowWhenEmpty("Message to send", findColour(juce::TextEditor::ColourIds::textColourId).withAlpha(0.3f));
        m_message_editor.setColour(juce::TextEditor::backgroundColourId,
                                   juce::Colours::transparentBlack);
        
        //button
        m_send_button.addListener(this);
        
        addAndMakeVisible(m_message_editor);
        addAndMakeVisible(m_send_button);
    }
    
    BeaconDispatcherElem::TextValueComponent::~TextValueComponent()
    {
        m_send_button.removeListener(this);
    }
    
    void BeaconDispatcherElem::TextValueComponent::buttonClicked(juce::Button* btn)
    {
        if(btn == &m_send_button)
        {
            std::string args = m_message_editor.getText().toStdString();
            m_owner.sendValue(AtomHelper::parse(args));
        }
    }
    
    void BeaconDispatcherElem::TextValueComponent::resized()
    {
        int margin_top = 6;
        auto bounds = getLocalBounds();
        bounds.removeFromTop(margin_top);
        m_send_button.setBounds(bounds.removeFromRight(60));
        bounds.removeFromRight(4);
        m_message_editor.setBounds(bounds);
    }
    
    // ================================================================================ //
    //                               BEACON DISPATCHER ELEM                             //
    // ================================================================================ //
    
    BeaconDispatcherElem::BeaconDispatcherElem(engine::Instance& instance) :
    m_instance(instance),
    m_beacon_name_editor("beacon_name_editor"),
    m_text_value(*this),
    m_message_slider("message_slider"),
    m_toggle_value("On/Off"),
    m_message_tab(juce::TabbedButtonBar::Orientation::TabsAtTop)
    {
        setInterceptsMouseClicks(false, true);
        
        // beacon name field
        m_beacon_name_editor.setReturnKeyStartsNewLine(false);
        m_beacon_name_editor.setMultiLine(false, false);
        m_beacon_name_editor.setTextToShowWhenEmpty("Symbol name", findColour(juce::TextEditor::ColourIds::textColourId).withAlpha(0.3f));
        
        // slider value
        m_message_slider.setRange(0., 1., 0.001);
        m_message_slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        m_message_slider.addListener(this);
        
        // toggle value
        m_toggle_value.addListener(this);
        
        // message value tab
        m_message_tab.addTab("text", juce::Colours::black.withAlpha(0.f), &m_text_value, false);
        m_message_tab.addTab("slider", juce::Colours::black.withAlpha(0.f), &m_message_slider, false);
        m_message_tab.addTab("toggle", juce::Colours::black.withAlpha(0.f), &m_toggle_value, false);
        
        m_message_tab.setOutline(0);
        m_message_tab.setTabBarDepth(20);
        
        addAndMakeVisible(m_beacon_name_editor);
        addAndMakeVisible(m_message_tab);
        
        setSize(310, 100);
    }
    
    BeaconDispatcherElem::~BeaconDispatcherElem()
    {
        m_message_slider.removeListener(this);
        m_toggle_value.removeListener(this);
    }
    
    void BeaconDispatcherElem::restoreState(juce::XmlElement* saved_state)
    {
        if (saved_state->hasTagName("BEACON"))
        {
            juce::String name = saved_state->getStringAttribute("name");
            int tab_idx = saved_state->getIntAttribute("value_tab_idx");
            
            if(name.isNotEmpty())
            {
                m_beacon_name_editor.setText(name);
            }
            
            m_message_tab.setCurrentTabIndex(tab_idx);
        }
    }
    
    void BeaconDispatcherElem::saveState(juce::XmlElement* saved_state)
    {
        if(saved_state->hasTagName("BEACON"))
        {
            saved_state->setAttribute("name", m_beacon_name_editor.getText());
            saved_state->setAttribute("value_tab_idx", m_message_tab.getCurrentTabIndex());
        }
    }
    
    void BeaconDispatcherElem::setBackgroundColor(juce::Colour const& color)
    {
        m_bgcolor = color;
        m_beacon_name_editor.setColour(juce::TextEditor::backgroundColourId,
                                       juce::Colours::black.withAlpha(0.1f));
        repaint();
    }
    
    void BeaconDispatcherElem::paint(juce::Graphics& g)
    {
        g.fillAll(m_bgcolor);
    }
    
    void BeaconDispatcherElem::resized()
    {
        int padding = 4;
        int texteditor_height = 24;
        int margin_top = 6;
        int margin_bottom = 6;
        
        auto bounds = getLocalBounds().reduced(padding);
        
        bounds.removeFromTop(margin_top);
        auto texteditor_bounds = bounds.removeFromTop(texteditor_height);

        texteditor_bounds.reduce(padding, 0);
        
        m_beacon_name_editor.setBounds(texteditor_bounds);
        bounds.removeFromTop(margin_bottom);
        
        bounds.reduce(padding, 0);
        m_message_tab.setBounds(bounds.removeFromTop(50));
    }
    
    void BeaconDispatcherElem::buttonClicked(juce::Button* btn)
    {
        if(btn == &m_toggle_value)
        {
            std::string name = m_beacon_name_editor.getText().toStdString();
            send(name, {m_toggle_value.getToggleState()});
        }
    }
    
    void BeaconDispatcherElem::sliderValueChanged(juce::Slider* slider)
    {
        if(slider == &m_message_slider)
        {
            std::string name = m_beacon_name_editor.getText().toStdString();
            send(name, {m_message_slider.getValue()});
        }
    }
    
    void BeaconDispatcherElem::sendValue(std::vector<Atom> const& args) const
    {
        send(m_beacon_name_editor.getText().toStdString(), args);
    }
    
    void BeaconDispatcherElem::send(std::string const& name, std::vector<Atom> const& args) const
    {
        if(!name.empty() && !args.empty())
        {
            engine::Beacon& beacon = m_instance.getBeacon(name);
            beacon.dispatch(args);
        }
    }
    
    // ================================================================================ //
    //                                   CONSOLE TOOLBAR                                //
    // ================================================================================ //
    
    BeaconDispatcherToolbarFactory::BeaconDispatcherToolbarFactory()
    {
        
    }
    
    void BeaconDispatcherToolbarFactory::getAllToolbarItemIds(juce::Array<int>& ids)
    {
        ids.add(addItem);
        ids.add(removeItem);
        ids.add(separatorBarId);
        ids.add(spacerId);
        ids.add(flexibleSpacerId);
    }
    
    void BeaconDispatcherToolbarFactory::getDefaultItemSet(juce::Array<int>& ids)
    {
        ids.add(flexibleSpacerId);
        ids.add(addItem);
        ids.add(removeItem);
    }
    
    juce::ToolbarItemComponent* BeaconDispatcherToolbarFactory::createItem(int itemId)
    {
        juce::ToolbarItemComponent* btn = nullptr;
        
        if(itemId == addItem)
        {
            btn = new juce::ToolbarButton(itemId, "Add item", juce::Drawable::createFromImageData(binary_data::images::plus_png, binary_data::images::plus_png_size), nullptr);
            
            btn->setCommandToTrigger(&KiwiApp::getCommandManager(), CommandIDs::addBeaconDispatcher, true);
        }
        else if(itemId == removeItem)
        {
            btn = new juce::ToolbarButton(itemId, "Remove item", juce::Drawable::createFromImageData(binary_data::images::minus_png, binary_data::images::minus_png_size), nullptr);
            
            btn->setCommandToTrigger(&KiwiApp::getCommandManager(), CommandIDs::removeBeaconDispatcher, true);
        }
        
        return btn;
    }
    
    // ================================================================================ //
    //                                  BEACON DISPATCHER                               //
    // ================================================================================ //
    
    BeaconDispatcher::BeaconDispatcher(engine::Instance& instance) :
    m_instance(instance),
    m_toolbar()
    {
        m_toolbar.setVertical(false);
        m_toolbar.setStyle(juce::Toolbar::ToolbarItemStyle::iconsOnly);
        m_toolbar.setColour(juce::Toolbar::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
        addAndMakeVisible(m_toolbar);
        m_toolbar.addDefaultItems(m_toolbar_factory);
        
        KiwiApp::bindToCommandManager(this);
        
        if(!restoreState())
        {
            addElem();
        }
        
        updateLayout();
    }
    
    BeaconDispatcher::~BeaconDispatcher()
    {
        saveState();
    }
    
    void BeaconDispatcher::addElem()
    {
        auto it = m_components.emplace(m_components.end(),
                                       std::make_unique<BeaconDispatcherElem>(m_instance));
        
        addAndMakeVisible(it->get());
        updateLayout();
    }
    
    void BeaconDispatcher::removeElem()
    {
        if(m_components.size() > 1)
        {
            const auto it = m_components.end() - 1;
            removeChildComponent(it->get());
            m_components.erase(it);
            updateLayout();
        }
    }
    
    void BeaconDispatcher::resized()
    {
        auto bounds = getLocalBounds();
        m_toolbar.setBounds(bounds.removeFromTop(24));
        
        for(auto& elem : m_components)
        {
            elem->setSize(getWidth(), m_elem_height);
        }
    }
    
    void BeaconDispatcher::updateLayout()
    {
        const auto nelems = m_components.size();
        juce::Rectangle<int> bounds(0, 0, getWidth(),
                                    m_elem_height * nelems + m_toolbar.getHeight() + (nelems - 1) * 2);
        setBounds(bounds);
        
        bounds.removeFromTop(m_toolbar.getHeight());
        
        auto& lf = KiwiApp::useLookAndFeel();
        const auto bgcolor = lf.getCurrentColourScheme().getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::windowBackground).contrasting(0.2);
        
        for(size_t i = 0; i < nelems; ++i)
        {
            if(i > 0)
            {
                bounds.removeFromTop(2);
            }
            
            auto& elem = *m_components[i];
            elem.setBounds(bounds.removeFromTop(m_elem_height));
            elem.setBackgroundColor(bgcolor);
        }
    }
    
    bool BeaconDispatcher::restoreState()
    {
        bool success = false;
        m_components.clear();
        
        std::unique_ptr<juce::XmlElement> saved_state(getGlobalProperties()
                                                      .getXmlValue("beacon_dispatcher_state"));
        if(saved_state)
        {
            if (saved_state->hasTagName("BEACONS"))
            {
                int count = 0;
                forEachXmlChildElement(*saved_state, e)
                {
                    addElem();
                    m_components[count++]->restoreState(e);
                    success = true;
                }
            }
        }
        
        return success;
    }
    
    void BeaconDispatcher::saveState()
    {
        auto saved_state = std::make_unique<juce::XmlElement>("BEACONS");
        for(auto& elem : m_components)
        {
            auto* child_state = saved_state->createNewChildElement("BEACON");
            elem->saveState(child_state);
        }
        
        getGlobalProperties().setValue("beacon_dispatcher_state", saved_state.get());
    }
    
    // ================================================================================ //
    //                              APPLICATION COMMAND TARGET                          //
    // ================================================================================ //
    
    juce::ApplicationCommandTarget* BeaconDispatcher::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void BeaconDispatcher::getAllCommands(juce::Array<juce::CommandID>& commands)
    {
        commands.add(CommandIDs::addBeaconDispatcher);
        commands.add(CommandIDs::removeBeaconDispatcher);
    }
    
    void BeaconDispatcher::getCommandInfo(const juce::CommandID commandID, juce::ApplicationCommandInfo& result)
    {
        switch (commandID)
        {
            case CommandIDs::addBeaconDispatcher:
            {
                result.setInfo(TRANS("Add a new beacon dispatcher"), TRANS("Add a new beacon dispatcher"), CommandCategories::editing, 0);
                result.setActive(m_components.size() <= 5);
                break;
            }
            case CommandIDs::removeBeaconDispatcher:
                result.setInfo(TRANS("Remove a beacon dispatcher"), TRANS("Remove a beacon dispatcher"), CommandCategories::windows, 0);
                result.setActive(m_components.size() > 1);
                break;
                
            default: break;
        }
    }
    
    bool BeaconDispatcher::perform(InvocationInfo const& info)
    {
        switch (info.commandID)
        {
            case CommandIDs::addBeaconDispatcher:       addElem(); break;
            case CommandIDs::removeBeaconDispatcher:    removeElem(); break;
            default: return false;
        }
        return true;
    }
}
