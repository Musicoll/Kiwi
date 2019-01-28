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
    
    ObjectView::ObjectView(model::Object & object_model)
    : m_model(object_model)
    , m_master(this, [](ObjectView*){})
    , m_resizing_flags(HitTester::Border::None)
    {
        object_model.addListener(*this);
        canGrowHorizontally(true);
        canGrowVertically(true);
    }
    
    ObjectView::~ObjectView()
    {
        getModel().removeListener(*this);
    }
    
    void ObjectView::setAttribute(std::string const& name, tool::Parameter const& parameter)
    {
        model::Object& object_model = getModel();
        object_model.setAttribute(name, parameter);
        
        const auto commit_msg = object_model.getName() + " \"" + name + "\" changed";
        model::DocumentManager::commit(object_model, commit_msg);
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
        g.setColour(findColour(ObjectView::ColourIds::Outline));
        const auto border_size = KiwiApp::useLookAndFeel().getObjectBorderSize();
        g.drawRect(getOutline().toFloat(), border_size);
    }
    
    void ObjectView::lockStatusChanged(bool is_locked)
    {
        // nothing to do by default
    }

    int ObjectView::getResizingFlags() const
    {
        return m_resizing_flags;
    }
    
    bool ObjectView::canGrowHorizontally() const
    {
        return ((m_resizing_flags & HitTester::Border::Left)
                || m_resizing_flags & HitTester::Border::Right);
    }
    
    bool ObjectView::canGrowVertically() const
    {
        return ((m_resizing_flags & HitTester::Border::Top)
                || m_resizing_flags & HitTester::Border::Bottom);
    }
    
    void ObjectView::canGrowHorizontally(bool can)
    {
        if(can)
        {
            m_resizing_flags |= HitTester::Border::Left;
            m_resizing_flags |= HitTester::Border::Right;
        }
        else
        {
            m_resizing_flags &= ~HitTester::Border::Left;
            m_resizing_flags &= ~HitTester::Border::Right;
        }
    }
    
    void ObjectView::canGrowVertically(bool can)
    {
        if(can)
        {
            m_resizing_flags |= HitTester::Border::Top;
            m_resizing_flags |= HitTester::Border::Bottom;
        }
        else
        {
            m_resizing_flags &= ~HitTester::Border::Top;
            m_resizing_flags &= ~HitTester::Border::Bottom;
        }
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
