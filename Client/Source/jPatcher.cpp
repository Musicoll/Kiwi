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
                DocumentManager::commit(patcher, "Add Plus Object");
                break;
            }
                
            case 2:
            {
                auto& obj = patcher.addObject("print");
                obj.setPosition(event.getMouseDownX(), event.getMouseDownY());
                DocumentManager::commit(patcher, "Add Print Object");
                break;
            }
                
            default: break;
        }
    }
    
    void jPatcher::patcherChanged(model::Patcher& patcher, model::Patcher::View& view)
    {
        if(view.added())
        {
            m_model = &patcher;
        }
        
        // send changes to objects
        for(auto& object : patcher.getObjects())
        {
            if(object.changed())
            {
                if(object.added())
                {
                    objectAdded(object);
                }
                
                objectChanged(view, object);
                
                if(object.removed())
                {
                    objectRemoved(object);
                }
            }
        }
        
        for(auto& link : patcher.getLinks())
        {
            if(link.added())
            {
                linkAdded(link);
            }
            
            linkChanged(link);
            
            if(patcher.objectsChanged())
            {
                for(auto& object : patcher.getObjects())
                {
                    if(object.changed())
                    {
                        auto jlink = getjLink(link);
                        if(jlink)
                        {
                            jlink->objectChanged(object);
                        }
                    }
                }
            }
            
            if(link.removed())
            {
                linkRemoved(link);
            }
        }
        
        if(view.removed())
        {
            m_model = nullptr;
        }
    }
    
    void jPatcher::objectAdded(model::Object& object)
    {
        auto result = m_objects.emplace(new jObject(object));
        if(result.second)
        {
            jObject& jobj = *result.first->get();
            addAndMakeVisible(jobj);
        }
    }
    
    void jPatcher::objectChanged(model::Patcher::View& view, model::Object& object)
    {
        const auto find_jobj = [&object](std::unique_ptr<jObject> const& jobj)
        {
            return (&object == &jobj->getModel());
        };
        
        const auto it = std::find_if(m_objects.begin(), m_objects.end(), find_jobj);
        
        if(it != m_objects.cend())
        {
            jObject& jobject = *it->get();
            jobject.objectChanged(view, object);
        }
    }
    
    void jPatcher::objectRemoved(model::Object& object)
    {
        const auto find_jobj = [&object](std::unique_ptr<jObject> const& jobj)
        {
            return (&object == &jobj->getModel());
        };
        
        const auto it = std::find_if(m_objects.begin(), m_objects.end(), find_jobj);
        
        if(it != m_objects.cend())
        {
            removeChildComponent(it->get());
            m_objects.erase(it);
        }
    }
    
    void jPatcher::linkAdded(model::Link& link)
    {
        auto result = m_links.emplace(new jLink(*this, link));
        
        if(result.second)
        {
            jLink& jlink = *result.first->get();
            addAndMakeVisible(jlink);
        }
    }
    
    void jPatcher::linkChanged(model::Link& link)
    {
        const auto find_jlink = [&link](std::unique_ptr<jLink> const& jlink)
        {
            return (&link == &jlink->getModel());
        };
        
        const auto it = std::find_if(m_links.begin(), m_links.end(), find_jlink);
        
        if(it != m_links.cend())
        {
            jLink& jlink = *it->get();
            jlink.linkChanged(link);
        }
    }
    
    void jPatcher::linkRemoved(model::Link& link)
    {
        const auto find_jlink = [&link](std::unique_ptr<jLink> const& jlink)
        {
            return (&link == &jlink->getModel());
        };
        
        const auto it = std::find_if(m_links.begin(), m_links.end(), find_jlink);
        
        if(it != m_links.cend())
        {
            removeChildComponent(it->get());
            m_links.erase(it);
        }
    }
    
    jObject const* const jPatcher::getjObject(model::Object const& object) const
    {
        const auto find_jobj = [&object](std::unique_ptr<jObject> const& jobj)
        {
            return (&object == &jobj->getModel());
        };
        
        const auto it = std::find_if(m_objects.begin(), m_objects.end(), find_jobj);
        
        if(it != m_objects.cend())
        {
            return it->get();
        }
        
        return nullptr;
    }
    
    jLink* jPatcher::getjLink(model::Link const& link)
    {
        const auto find_jlink = [&link](std::unique_ptr<jLink> const& jlink)
        {
            return (&link == &jlink->getModel());
        };
        
        const auto it = std::find_if(m_links.begin(), m_links.end(), find_jlink);
        
        if(it != m_links.cend())
        {
            return it->get();
        }
        
        return nullptr;
    }
}
