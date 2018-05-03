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

#include <juce_gui_basics/juce_gui_basics.h>

#include <KiwiApp.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectFrame.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectView.h>

#include <KiwiModel/KiwiModel_DocumentManager.h>

namespace kiwi
{
    // ================================================================================ //
    //                                   OBJECT VIEW                                    //
    // ================================================================================ //
    
    ObjectView::ObjectView(model::Object & object_model):
    m_model(object_model),
    m_border_size(1.5),
    m_master(this, [](ObjectView*){})
    {
        object_model.addListener(*this);
        
        setColour(ObjectView::ColourIds::Error, juce::Colour::fromFloatRGBA(0.6, 0.1, 0.1, 0.));
        setColour(ObjectView::ColourIds::Background, juce::Colours::white);
        setColour(ObjectView::ColourIds::Text, juce::Colours::black);
        setColour(ObjectView::ColourIds::Outline, juce::Colours::black);
        setColour(ObjectView::ColourIds::Highlight, juce::Colour::fromFloatRGBA(0., 0.5, 1., 0.));
        setColour(ObjectView::ColourIds::Active, juce::Colour(0xff21ba90));
    }
    
    ObjectView::~ObjectView()
    {
        getModel().removeListener(*this);
    }
    
    void ObjectView::setAttribute(std::string const& name, tool::Parameter const& parameter)
    {
        model::Object & object_model = getModel();
        
        object_model.setAttribute(name, parameter);
        
        model::DocumentManager::commit(object_model);
    }
    
    void ObjectView::setParameter(std::string const& name, tool::Parameter const& parameter)
    {
        getModel().setParameter(name, parameter);
    }
    
    void ObjectView::modelAttributeChanged(std::string const& name, tool::Parameter const& param)
    {
        attributeChanged(name, param);
    }
    
    void ObjectView::modelParameterChanged(std::string const& name, tool::Parameter const& param)
    {
        parameterChanged(name, param);
    }
    
    void ObjectView::attributeChanged(std::string const& name, tool::Parameter const& param)
    {
    }
    
    void ObjectView::parameterChanged(std::string const& name, tool::Parameter const& param)
    {
    }
    
    model::Object& ObjectView::getModel() const
    {
        return m_model;
    }
    
    juce::Rectangle<int> ObjectView::getOutline() const
    {
        return getLocalBounds();
    }
    
    void ObjectView::drawOutline(juce::Graphics & g)
    {
        g.drawRect(getOutline(), m_border_size);
    }
    
    // ================================================================================ //
    //                                      SCHEDULER                                   //
    // ================================================================================ //
    
    tool::Scheduler<> & ObjectView::getScheduler() const
    {
        return KiwiApp::useScheduler();
    }
    
    void ObjectView::defer(std::function<void()> call_back)
    {
        std::weak_ptr<ObjectView> object(m_master);
        
        getScheduler().defer([object, cb = std::move(call_back)]()
        {
            if (!object.expired())
            {
                cb();
            }
        });
    }
    
    void ObjectView::schedule(std::function<void()> call_back, tool::Scheduler<>::duration_t delay)
    {
        std::weak_ptr<ObjectView> object(m_master);
        
        getScheduler().schedule([object, cb = std::move(call_back)]()
        {
            if (!object.expired())
            {
                cb();
            }
        }, delay);
    }
}
