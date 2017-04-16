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

#ifndef KIWI_APP_PATCHER_COMPONENT_HPP_INCLUDED
#define KIWI_APP_PATCHER_COMPONENT_HPP_INCLUDED

#include "KiwiApp_PatcherView.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                            PATCHER COMPONENT TOOLBAR                             //
    // ================================================================================ //
    
    class PatcherToolbarFactory : public juce::ToolbarItemFactory
    {
    public: // methods
        
        //! @brief Constructor.
        PatcherToolbarFactory();
        
        enum ItemIds
        {
            lock_unlock         = 1,
            zoom_in             = 2,
            zoom_out            = 3,
            dsp_on_off          = 4,
        };
        
        void getAllToolbarItemIds(juce::Array<int>& ids) override;
        
        void getDefaultItemSet(juce::Array<int>& ids) override;
        
        juce::ToolbarItemComponent* createItem(int itemId) override;
    };
    
    // ================================================================================ //
    //                                PATCHER COMPONENT                                 //
    // ================================================================================ //
    
    //! @brief The PatcherView Viewport
    class PatcherComponent : public juce::Component, public juce::ApplicationCommandTarget
    {
    public: // methods
        
        //! @brief Constructor
        PatcherComponent(PatcherView& patcherview);
        
        //! @brief Destructor
        ~PatcherComponent() = default;
        
        //! @brief juce::Component::resized
        void resized() override;
        
        //! @brief Returns the patcher view.
        PatcherView& usePatcherView();
        
        //! @brief juce::Component::paint
        void paint(juce::Graphics& g) override;
        
        // ================================================================================ //
        //                              APPLICATION COMMAND TARGET                          //
        // ================================================================================ //
        
        juce::ApplicationCommandTarget* getNextCommandTarget() override;
        void getAllCommands(juce::Array<juce::CommandID>& commands) override;
        void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
        bool perform(const InvocationInfo& info) override;

    private: // members
        
        PatcherView&            m_patcherview;
        juce::Toolbar           m_toolbar;
        PatcherToolbarFactory   m_toolbar_factory;
    };
}

#endif // KIWI_APP_PATCHER_COMPONENT_HPP_INCLUDED
