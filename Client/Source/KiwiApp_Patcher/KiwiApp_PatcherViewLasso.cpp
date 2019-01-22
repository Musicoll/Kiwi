/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectFrame.h>

#include "KiwiApp_PatcherViewLasso.h"
#include "KiwiApp_PatcherView.h"
#include "KiwiApp_LinkView.h"
#include "../KiwiApp.h"

namespace kiwi
{
    // ================================================================================ //
    //										LASSO                                       //
    // ================================================================================ //
    
    Lasso::Lasso(PatcherView& patcher) : m_patcher(patcher), m_dragging(false)
    {
        setInterceptsMouseClicks(false, false);
        setWantsKeyboardFocus(false);
        setAlwaysOnTop(true);
        setVisible(false);
        setBounds(0, 0, 1, 1);
    }
    
    Lasso::~Lasso()
    {
        ;
    }
    
    bool Lasso::isPerforming() const noexcept
    {
        return m_dragging;
    }
    
    void Lasso::paint(juce::Graphics& g)
    {
        const juce::Rectangle<int> bounds = getLocalBounds();
        const juce::Colour color = juce::Colour::fromFloatRGBA(0.96, 0.96, 0.96, 1.);
        
        g.setColour(color.withAlpha(0.5f));
        g.fillAll();
        g.setColour(color);
        g.drawRect(bounds, 1.);
    }
    
    void Lasso::begin(juce::Point<int> const& point, const bool preserve_selection)
    {
        assert(!m_dragging);
        
        if(!preserve_selection)
        {
            m_patcher.unselectAll();
        }
        else
        {
            m_objects = m_patcher.getSelectedObjects();
            m_links = m_patcher.getSelectedLinks();
        }

        m_start = point;
        m_dragging = true;
    }

    void Lasso::perform(juce::Point<int> const& point,
                         bool include_objects, bool include_links, const bool preserve)
    {
        bool selection_changed = false;
        
        juce::Rectangle<int> bounds = juce::Rectangle<int>(m_start, point);
        
        if(bounds.getWidth() == 0)
        {
            bounds.setWidth(1);
        }
        if(bounds.getHeight() == 0)
        {
            bounds.setHeight(1);
        }
        
        setBounds(bounds);
        
        m_dragging = true;
        setVisible(true);
        toFront(false);
        
        if(preserve)
        {
            if(include_objects)
            {
                PatcherView::ObjectFrames const& objects = m_patcher.getObjects();
                
                HitTester hit(m_patcher);
                std::vector<ObjectFrame*> lasso_objects;
                hit.testObjects(bounds, lasso_objects);
                
                for(auto& object_frame_uptr : objects)
                {
                    if(object_frame_uptr)
                    {
                        ObjectFrame& object = *object_frame_uptr.get();
                        
                        const bool is_selected = object.isSelected();
                        const bool was_selected = m_objects.find(object.getModel().ref()) != m_objects.end();
                        
                        const bool in_lasso = std::find(lasso_objects.begin(), lasso_objects.end(), &object) != lasso_objects.end();
                        
                        if (!is_selected && (was_selected != in_lasso))
                        {
                            m_patcher.selectObject(object);
                            selection_changed = true;
                        }
                        else if(is_selected && (was_selected == in_lasso))
                        {
                            m_patcher.unselectObject(object);
                            selection_changed = true;
                        }
                    }
                }
            }
            
            if(include_links)
            {
                PatcherView::LinkViews const& links = m_patcher.getLinks();
                
                HitTester hit(m_patcher);
                std::vector<LinkView*> lasso_links;
                hit.testLinks(bounds, lasso_links);
                
                for(auto& link_view_uptr : links)
                {
                    if(link_view_uptr)
                    {
                        LinkView& link = *link_view_uptr.get();
                        
                        const bool is_selected = link.isSelected();
                        const bool was_selected = m_links.find(link.getModel().ref()) != m_links.end();
                        
                        const bool in_lasso = std::find(lasso_links.begin(), lasso_links.end(), &link) != lasso_links.end();
                        
                        if (!is_selected && (was_selected != in_lasso))
                        {
                            m_patcher.selectLink(link);
                            selection_changed = true;
                        }
                        else if(is_selected && (was_selected == in_lasso))
                        {
                            m_patcher.unselectLink(link);
                            selection_changed = true;
                        }
                    }
                }
            }
            
            if(selection_changed)
            {
                //ctrl->selectionChanged();
            }
        }
        else
        {
            m_patcher.unselectAll();
            HitTester hit(m_patcher);
            
            if(include_objects)
            {
                std::vector<ObjectFrame*> objects;
                hit.testObjects(bounds, objects);
                m_patcher.selectObjects(objects);
            }
            
            if(include_links)
            {
                std::vector<LinkView*> links;
                hit.testLinks(bounds, links);
                m_patcher.selectLinks(links);
            }
        }
    }

    void Lasso::end()
    {
        m_dragging = false;
        setVisible(false);
        
        m_objects.clear();
        m_links.clear();
    }
}
