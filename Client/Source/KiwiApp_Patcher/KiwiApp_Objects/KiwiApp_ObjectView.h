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

#include <set>

#include <juce_gui_basics/juce_gui_basics.h>

#include <KiwiTool/KiwiTool_Parameter.h>
#include <KiwiTool/KiwiTool_Scheduler.h>

#include <KiwiModel/KiwiModel_Object.h>

namespace kiwi
{
    class ObjectFrame;
    
    // ================================================================================ //
    //                                   OBJECT VIEW                                    //
    // ================================================================================ //
    
    //! @brief Abstract for objects graphical representation.
    class ObjectView : public juce::Component, public model::Object::Listener
    {
    public: // classes
        
        enum ColourIds
        {
            Background =    0x1100004,
            Error =         0x1100005,
            Text =          0x1100006,
            Outline =       0x1100007,
            Highlight =     0x1100008,
            Active =        0x1100009
        };
        
    public: // methods
        
        //! @brief Constructor.
        ObjectView(model::Object& object_model);
        
        //! @brief Destructor.
        virtual ~ObjectView();
        
        //! @brief Returns the model represented by the graphical object.
        model::Object& getModel() const;
        
        //! @brief Called when one of the model's attributes has changed.
        void modelAttributeChanged(std::string const& name, tool::Parameter const& param) override final;
        
        //! @brief Called when a parameter has changed.
        void modelParameterChanged(std::string const& name, tool::Parameter const& param) override final;
        
    protected: // methods
        
        //! @biref Returns the main scheduler.
        tool::Scheduler<> & getScheduler() const;
        
        //! @brief Defers a task on the main thread.
        //! @details The task is automatically unscheduled when object is destroyed.
        void defer(std::function<void()> call_back);
        
        //! @brief Schedules a task on the main thread.
        //! @details The tasks is automatically unscheduled when object is destroyed.
        void schedule(std::function<void()> call_back, tool::Scheduler<>::duration_t delay);
        
        //! @brief Draws the outlines of the object.
        void drawOutline(juce::Graphics & g);
        
        //! @brief Changes one of the data model's attribute.
        void setAttribute(std::string const& name, tool::Parameter const& param);
        
        //! @brief Changes one of the data model's parameter.
        void setParameter(std::string const& name, tool::Parameter const& param);
        
    private: // methods
        
        //! @brief Override this function if you want it to have a customied outline.
        //! @details Used to draw the object's outline. Returns the object's bounds by default.
        //! @todo May make it return path instead.
        virtual juce::Rectangle<int> getOutline() const;
        
        //! @brief Called once one of the data model's attributes has changed.
        virtual void attributeChanged(std::string const& name, tool::Parameter const& param);
        
        //! @brief Called once the data model's parameters has changed.
        virtual void parameterChanged(std::string const& name, tool::Parameter const& param);
        
    private: // members
        
        model::Object&                  m_model;
        int                             m_border_size;
        std::shared_ptr<ObjectView>     m_master;
        
    private: // deleted methods
        
        ObjectView() = delete;
        ObjectView(ObjectView const& other) = delete;
        ObjectView(ObjectView && other) = delete;
        ObjectView& operator=(ObjectView const& other) = delete;
        ObjectView& operator=(ObjectView && other) = delete;
    };
}
