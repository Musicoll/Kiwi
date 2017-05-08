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

#include "../KiwiApp_Window.hpp"
#include "KiwiApp_PatcherManager.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                            PATCHER COMPONENT TOOLBAR                             //
    // ================================================================================ //
    
    class PatcherToolbar : public juce::Component
    {
    public: // methods
        
        //! @brief Constructor.
        PatcherToolbar(PatcherManager& patcher_manager);
        
        //! @brief juce::Component::resized
        void resized() override;
        
        //! @brief juce::Component::paint
        void paint(juce::Graphics& g) override;
        
    private: // classes
        
        //! @internal Toolbar item factory
        struct Factory : public juce::ToolbarItemFactory
        {
            Factory(PatcherManager& patcher_manager);
            
            enum ItemIds
            {
                lock_unlock         = 1,
                zoom_in             = 2,
                zoom_out            = 3,
                dsp_on_off          = 4,
                //users               = 5,
            };
            
            void getAllToolbarItemIds(juce::Array<int>& ids) override;
            
            void getDefaultItemSet(juce::Array<int>& ids) override;
            
            juce::ToolbarItemComponent* createItem(int itemId) override;
            
            PatcherManager& m_patcher_manager;
        };
        
        class UsersItemComponent;
        
    private: // variables
        
        PatcherManager& m_patcher_manager;
        
        juce::Toolbar   m_toolbar;
        Factory         m_factory;
    };
    
    // ================================================================================ //
    //                             TOOLBAR USER COMPONENT                               //
    // ================================================================================ //
    
    //! @brief A toolbar component that displays informations about the users of the patch
    class PatcherToolbar::UsersItemComponent :
    public juce::ToolbarItemComponent,
    public PatcherManager::Listener
    {
    public: // methods
        
        UsersItemComponent(const int toolbarItemId, PatcherManager& patcher_manager);
        
        ~UsersItemComponent();
        
        //! @brief Called when one or more users are connecting or disconnecting to the Patcher Document.
        void connectedUserChanged(PatcherManager& manager) override;
        
        bool getToolbarItemSizes(int toolbarDepth, bool isVertical,
                                 int& preferredSize, int& minSize, int& maxSize) override;
        
        void paintButtonArea(juce::Graphics&, int width, int height,
                             bool isMouseOver, bool isMouseDown) override;
        
        void contentAreaChanged(const juce::Rectangle<int>& newArea) override;
        
        void mouseEnter(juce::MouseEvent const& e) override;
        void mouseExit(juce::MouseEvent const& e) override;
        
    private: // variables
        
        PatcherManager&     m_patcher_manager;
        size_t              m_users;
        const juce::Image   m_users_img;
    };
    
    // ================================================================================ //
    //                                PATCHER COMPONENT                                 //
    // ================================================================================ //
    
    //! @brief The PatcherView Viewport
    class PatcherComponent :
    public juce::Component,
    public juce::ApplicationCommandTarget
    {
    public: // methods
        
        //! @brief Constructor
        PatcherComponent(PatcherView& patcherview);
        
        //! @brief Destructor
        ~PatcherComponent();
        
        //! @brief juce::Component::resized
        void resized() override;
        
        //! @brief Returns the patcher view.
        PatcherView& usePatcherView();
        
        //! @brief Returns the patcher manager.
        PatcherManager& usePatcherManager();
        
        //! @brief juce::Component::paint
        void paint(juce::Graphics& g) override;
        
        // -------------------------------------------------------------------------------- //
        //                              APPLICATION COMMAND TARGET                          //
        // -------------------------------------------------------------------------------- //
        
        juce::ApplicationCommandTarget* getNextCommandTarget() override;
        void getAllCommands(juce::Array<juce::CommandID>& commands) override;
        void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
        bool perform(const InvocationInfo& info) override;

    private: // members
        
        PatcherManager& m_patcher_manager;
        PatcherView&    m_patcherview;
        PatcherToolbar  m_toolbar;
    };
    
    // ================================================================================ //
    //                                PATCHER VIEW WINDOW                               //
    // ================================================================================ //
    
    class PatcherViewWindow : public Window
    {
    public: // methods
        
        PatcherViewWindow(PatcherManager& manager, PatcherView& patcherview);
        
        void closeButtonPressed() override;
        
        //! @brief returns the patcher manager.
        PatcherManager& getPatcherManager();
        
        //! @brief returns the PatcherView.
        PatcherView& getPatcherView();
        
    private: // variables
        
        PatcherComponent m_patcher_component;
    };
}
