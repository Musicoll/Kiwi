/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#include <KiwiEngine/KiwiDocumentManager.hpp>

#include "jInstance.hpp"
#include "jPatcher.hpp"

namespace kiwi
{
    jInstance::jInstance() :
    m_user_id(123456789ULL),
    m_instance(new engine::Instance(m_user_id)),
    m_console_window(new jConsoleWindow())
    {
        ;
    }
    
    jInstance::~jInstance()
    {
        m_console_window.reset();
        m_patcher_manager.reset();
    }
    
    uint64_t jInstance::getUserId() const noexcept
    {
        return m_user_id;
    }
    
    void jInstance::setUserId(uint64_t user_id)
    {
        m_user_id = user_id;
    }
    
    void jInstance::newPatcher()
    {
        m_patcher_manager.reset();
        m_patcher_manager = std::make_unique<jPatcherManager>(*this);
        
        m_patcher_manager->newView();
        m_patcher_manager->newView();
    }
    
    //------------------------------------------------------------
    // Settings component
    //------------------------------------------------------------
    
    class jSettings final : public juce::Component
    {
    public:
        jSettings(uint64_t client_id);
        
        uint64_t getClientId();
        
        ~jSettings() = default;
        
    private:
        juce::TextEditor m_client_id;
        
    private:
        jSettings(jSettings const & other) = delete;
        jSettings(jSettings && other) = delete;
        jSettings& operator=(jSettings const& other) = delete;
        jSettings& operator=(jSettings && other) = delete;
    };
    
    jSettings::jSettings(uint64_t client_id)
    {
        setSize(300, 100);
        setVisible(true);
        
        addAndMakeVisible(m_client_id);
        m_client_id.setBoundsRelative((1./5), (5./12), (3./5), (3./12));
        m_client_id.setText(juce::String(client_id));
    }
    
    uint64_t jSettings::getClientId()
    {
        return m_client_id.getText().getFloatValue();
    }
    
    void jInstance::openSettings()
    {
        jSettings set_cmp(getUserId());
        juce::OptionalScopedPointer<Component> settings_component(&set_cmp, false);
        
        juce::DialogWindow::LaunchOptions option;
        option.dialogTitle = juce::String("Settings");
        option.content = settings_component;
        option.resizable = false;
        
        option.runModal();
        
        setUserId(set_cmp.getClientId());
    }
    
    void jInstance::showConsoleWindow()
    {
        m_console_window->setVisible(true);
        m_console_window->toFront(true);
    }
}
