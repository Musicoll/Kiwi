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

#include "jPatcher.hpp"

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
                        objectHasBeenAdded(object);
                    }
                    else if(object.removed())
                    {
                        objectWillBeRemoved(object);
                    }
                    else // resident
                    {
                        objectChanged(object);
                    }
                }
            }
        }
        
        if(patcher.removed())
        {
            m_model = nullptr;
        }
    }
    
    void jPatcher::objectHasBeenAdded(model::Object& object)
    {
        std::cout << "jPatcher ---> object has been added" << '\n';
    }
    
    void jPatcher::objectChanged(model::Object& /*object*/)
    {
        
    }
    
    void jPatcher::objectWillBeRemoved(model::Object& object)
    {
        std::cout << "jPatcher ---> object will be removed" << '\n';
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
                patcher.addPlus();
                engine::DocumentManager::commit(patcher, "Add Plus Object");
                break;
            }
                
            case 2:
            {
                patcher.addPrint();
                engine::DocumentManager::commit(patcher, "Add Print Object");
                break;
            }
                
            default:break;
        }
    }
}
