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
#include <KiwiModel/KiwiConsole.hpp>

#include "jPatcher.hpp"
#include "jObject.hpp"
#include "jLink.hpp"

namespace kiwi
{
    jPatcher::jPatcher()
    {
        setSize(600, 400);
    }
    
    jPatcher::~jPatcher()
    {
        ;
    }
    
    void jPatcher::paint(juce::Graphics & g)
    {
        const bool locked = false;
        const juce::Colour bgcolor = juce::Colours::lightgrey;
        const int grid_size = 20;
        const juce::Rectangle<int> bounds(g.getClipBounds());
        
        g.setColour(bgcolor);
        g.fillAll();
        
        if (!locked)
        {
            g.setColour(bgcolor.contrasting(0.5));
            for(int x = bounds.getX() - (bounds.getX() % grid_size); x < bounds.getRight(); x += grid_size)
            {
                for(int y = bounds.getY() - (bounds.getY() % grid_size) ; y < bounds.getBottom(); y += grid_size)
                {
                    g.setPixel(x, y);
                }
            }
        }
    }
    
    void jPatcher::mouseDown(juce::MouseEvent const& event)
    {
        if(event.mods.isLeftButtonDown())
        {
            leftClick(event);
        }
        else if(event.mods.isRightButtonDown())
        {
            rightClick(event);
        }
    }
    
    void jPatcher::leftClick(juce::MouseEvent const& event)
    {
        ;
    }

    void jPatcher::rightClick(juce::MouseEvent const& event)
    {
        assert(m_model != nullptr);
        
        auto& patcher = *m_model;

        juce::PopupMenu m;
        m.addItem(1, "Add Plus (1)");
        m.addItem(2, "Add Print");
        m.addSeparator();

        auto r = m.show();
        
        switch(r)
        {
            case 1:
            {
                auto& obj = patcher.addObject("plus");
                obj.setPosition(event.x, event.y);
                engine::DocumentManager::commit(patcher, "Add Plus Object");
                break;
            }
                
            case 2:
            {
                auto& obj = patcher.addObject("print");
                obj.setPosition(event.getMouseDownX(), event.getMouseDownY());
                engine::DocumentManager::commit(patcher, "Add Print Object");
                break;
            }
                
            default: break;
        }
    }
    
    void jPatcher::document_changed(model::Patcher& patcher)
    {
        if(patcher.added())
        {
            m_model = &patcher;
        }
        
        if(patcher.objectsChanged())
        {
            for(auto& object : patcher.getObjects())
            {
                if(object.changed())
                {
                    if(object.added())
                    {
                        objectModelAdded(object);
                    }
                    
                    objectModelChanged(object);
                    
                    if(object.removed())
                    {
                        objectModelRemoved(object);
                    }
                }
            }
        }
        
        if(patcher.linksChanged())
        {
            for(auto& link : patcher.getLinks())
            {
                if(link.changed())
                {
                    if(link.added())
                    {
                        linkModelAdded(link);
                    }
                    
                    linkModelChanged(link);
                    
                    if(patcher.objectsChanged())
                    {
                        for(auto& object : patcher.getObjects())
                        {
                            if(object.changed())
                            {
                                auto& jlink = link.entity().use<jLink>();
                                jlink.objectModelChanged(object);
                            }
                        }
                    }
                    
                    if(link.removed())
                    {
                        linkModelRemoved(link);
                    }
                }
            }
        }
        
        if(patcher.removed())
        {
            m_model = nullptr;
        }
    }
    
    void jPatcher::objectModelAdded(model::Object& object)
    {
        auto& jobj = object.entity().emplace<jObject>();
        addAndMakeVisible(jobj);
    }
    
    void jPatcher::objectModelChanged(model::Object& object)
    {
        auto& jobject = object.entity().use<jObject>();
        jobject.objectModelChanged(object);
    }
    
    void jPatcher::objectModelRemoved(model::Object& object)
    {
        auto& jobj = object.entity().use<jObject>();
        removeChildComponent(&jobj);
        
        object.entity().erase<jObject>();
    }
    
    void jPatcher::linkModelAdded(model::Link& link)
    {
        auto& jlink = link.entity().emplace<jLink>();
        addAndMakeVisible(jlink);
    }
    
    void jPatcher::linkModelChanged(model::Link& link)
    {
        auto& jlink = link.entity().use<jLink>();
        jlink.linkModelChanged(link);
    }
    
    void jPatcher::linkModelRemoved(model::Link& link)
    {
        auto& jlink = link.entity().use<jLink>();
        removeChildComponent(&jlink);
        
        link.entity().erase<jLink>();
    }
}
