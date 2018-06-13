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

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectView.h>

namespace kiwi
{
    // ================================================================================ //
    //                                   SLIDER VIEW                                    //
    // ================================================================================ //
    
    class SliderView : public ObjectView, juce::Slider::Listener
    {
    private: // classes
        
        class Task;
        
    public: // methods
        
        SliderView(model::Object & object_model);
        
        ~SliderView();
        
        static void declare();
        
        static std::unique_ptr<ObjectView> create(model::Object & object);
        
    private: // methods
        
        //! @brief Validate the new width and height for the box
        void validateSize(int& new_width, int& new_height) override;
        
        void sliderValueChanged(juce::Slider * slider) override final;
        
        void paint(juce::Graphics & g) override final;
        
        void mouseDown(juce::MouseEvent const& e) override final;
        
        void mouseUp(juce::MouseEvent const& e) override final;
        
        void mouseDrag(juce::MouseEvent const& e) override final;
        
        void resized() override final;
        
        void parameterChanged(std::string const& name, tool::Parameter const& parameter) override final;
        
        void valueChanged(double new_value);
        
    private: // members
        
        juce::Slider        m_slider;
        flip::Signal<> &    m_output_value;
        
    private: // delted methods
        
        SliderView() = delete;
        SliderView(SliderView const& other) = delete;
        SliderView(SliderView && other) = delete;
        SliderView& operator=(SliderView const& other) = delete;
        SliderView& operator=(SliderView && other) = delete;
    };
}
