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

#include <vector>

#include <juce_gui_basics/juce_gui_basics.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectView.h>

namespace kiwi {
    
    // ================================================================================ //
    //                                  METER~ VIEW                                     //
    // ================================================================================ //
    
    class MeterTildeView : public ObjectView
    {
    private: // classes
        
        struct Led
        {
            float                   m_min_db;
            juce::Colour            m_colour;
            juce::Rectangle<int>    m_bounds;
        };
        
    public: // methods
        
        static void declare();
        
        static std::unique_ptr<ObjectView> create(model::Object & model);
        
        MeterTildeView(model::Object & object_model);
        
        ~MeterTildeView();
        
    private: // methods
        
        void resized() override final;
        
        void paint(juce::Graphics & g) override final;
        
        void peakChanged(float new_peak);
        
        juce::Colour computeGradientColour(float delta) const;
        
        void computeActiveLed(float peak_db);
        
    private: // members
        
        std::vector<Led>                                m_leds;
        int                                             m_active_led;
        juce::Colour                                    m_cold_colour;
        juce::Colour                                    m_hot_colour;
        int                                             m_border;
        int                                             m_padding;
        flip::SignalConnection                          m_connection;
        
    private: // deleted methods
        
        MeterTildeView() = delete;
        MeterTildeView(MeterTildeView const& other) = delete;
        MeterTildeView(MeterTildeView && other) = delete;
        MeterTildeView& operator=(MeterTildeView const& other) = delete;
        MeterTildeView& operator=(MeterTildeView && other) = delete;
    };
}
