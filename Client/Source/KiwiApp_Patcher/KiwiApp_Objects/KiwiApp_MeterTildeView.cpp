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

#include <cmath>
#include <functional>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_MeterTilde.h>

#include <KiwiApp.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_MeterTildeView.h>
#include <KiwiApp_Patcher/KiwiApp_Factory.h>

namespace kiwi
{
    // ================================================================================ //
    //                                  METER~ VIEW                                     //
    // ================================================================================ //
    
    void MeterTildeView::declare()
    {
        Factory::add<MeterTildeView>("meter~", &MeterTildeView::create);
    }
    
    std::unique_ptr<ObjectView> MeterTildeView::create(model::Object & model)
    {
        return std::make_unique<MeterTildeView>(model);
    }
    
    MeterTildeView::MeterTildeView(model::Object & object_model)
    : ObjectView(object_model)
    , m_connection(object_model.getSignal<float>(model::MeterTilde::Signal::PeakChanged)
                   .connect([this](float new_peak){ peakChanged(new_peak); }))
    {
        const size_t num_leds = 12;
        m_leds.resize(num_leds);
        
        for(size_t i = 0; i < num_leds; ++i)
        {
            auto& led = m_leds[i];
            led.min_db = -3.f * (num_leds - (i + 1.f));
            led.colour = computeGradientColour(static_cast<float>(i) / (num_leds - 1.f));
        }
        
        computeActiveLed(-120.f);
    }
    
    MeterTildeView::~MeterTildeView()
    {}
    
    void MeterTildeView::validateSize(int& new_width, int& new_height)
    {
        const int min = 40;
        const bool vertical = (new_width <= new_height);
        if(vertical)
        {
            new_height = std::max<int>(min, new_height);
        }
        else
        {
            new_width = std::max<int>(min, new_width);
        }
    }
    
    void MeterTildeView::resized()
    {
        const auto bounds = getLocalBounds().toFloat();
        const bool vertical = (bounds.getWidth() <= bounds.getHeight());
        
        const float num_leds = m_leds.size();
        const float padding = m_padding;
        const float sep = m_led_distance;
        
        auto space = bounds.reduced(padding, padding);
        
        if(vertical)
        {
            const auto led_space = (space.getHeight() / num_leds);
            for(auto& led : m_leds)
            {
                led.bounds = (space.removeFromBottom(led_space)
                              .removeFromTop(led_space - sep));
            }
        }
        else
        {
            const auto led_space = (space.getWidth() / num_leds);
            for(auto& led : m_leds)
            {
                led.bounds = (space.removeFromLeft(led_space)
                              .removeFromLeft(led_space - sep));
            }
        }
    }
    
    juce::Colour MeterTildeView::computeGradientColour(float delta) const
    {
        assert((delta >= 0 && delta <= 1.) && "meter wrong index for gradient");
        
        float hue = m_cold_colour.getHue() +
                    delta * (m_hot_colour.getHue() - m_cold_colour.getHue());
        
        float saturation = m_cold_colour.getSaturation() +
                           delta * (m_hot_colour.getSaturation() - m_cold_colour.getSaturation());
        
        float brightness = m_cold_colour.getBrightness() +
                           delta * (m_hot_colour.getBrightness() - m_cold_colour.getBrightness());
        
        float alpha = 1;

        return juce::Colour(hue, saturation, brightness, alpha);
    }
    
    void MeterTildeView::computeActiveLed(float peak_db)
    {
        auto it = std::find_if(m_leds.rbegin(), m_leds.rend(), [peak_db](Led const& led)
        {
            return peak_db >= led.min_db;
        });
        
        m_active_led = it != m_leds.rend() ? m_leds.rend() - (it + 1) : -1;
    }
    
    void MeterTildeView::peakChanged(float new_peak)
    {
        defer([this, new_peak]()
        {
            float peak_db = 20. * log10(new_peak);
            computeActiveLed(peak_db);
            repaint();
        });
    }
    
    void MeterTildeView::paint(juce::Graphics & g)
    {
        g.fillAll(findColour(ObjectView::ColourIds::Background));
        const auto led_border_size = 0.5f;
        const auto outline_color = findColour(ObjectView::ColourIds::Outline);
        
        for(int i = 0; i < m_leds.size(); i++)
        {
            const auto& led = m_leds[i];
            if (i <= m_active_led)
            {
                g.setColour(led.colour);
                g.fillRect(led.bounds);
            }
            else
            {
                g.setColour(outline_color);
                g.drawRect(led.bounds, led_border_size);
            }
        }
        
        g.setColour(outline_color);
        drawOutline(g);
    }
}
