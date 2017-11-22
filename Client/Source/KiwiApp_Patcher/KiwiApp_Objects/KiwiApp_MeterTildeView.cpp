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
    
    MeterTildeView::MeterTildeView(model::Object & object_model):
    ObjectView(object_model),
    m_leds(),
    m_active_led(-1),
    m_cold_colour(juce::Colour(0xff04047f)),
    m_hot_colour(juce::Colour(0xffca2423)),
    m_border(5),
    m_padding(1),
    m_connection(object_model.getSignal<float>(model::MeterTilde::Signal::PeakChanged)
                 .connect(std::bind(&MeterTildeView::peakChanged, this, std::placeholders::_1)))
    {
        size_t num_leds = 12;
        
        for(int i = 0; i < num_leds; ++i)
        {
            float min_db = - 3. * (num_leds - (i + 1));
            
            m_leds.push_back({min_db,
                              computeGradientColour(( static_cast<float>(i) / (num_leds - 1))),
                              juce::Rectangle<int>()});
        }
    }
    
    MeterTildeView::~MeterTildeView()
    {
    }
    
    void MeterTildeView::resized()
    {
        juce::AffineTransform transform;
        
        if (getHeight() > getWidth())
        {
            transform = juce::AffineTransform::translation(-getLocalBounds().getCentre());
            transform = transform.followedBy(juce::AffineTransform(0, -1, 0, 1, 0, 0));
            transform = transform.followedBy(juce::AffineTransform::translation(getLocalBounds().getCentre()));
        }
        
        juce::Rectangle<int> bounds = getLocalBounds().transformedBy(transform);
        
        float led_width = (bounds.getWidth() - (m_leds.size() - 1.) * m_padding - 2. * m_border) / m_leds.size();
        
        int led_height = bounds.getHeight() - 2 * m_border;
        
        int led_y = bounds.getY() + (bounds.getHeight() / 2.) - (led_height / 2.);
        
        for(int i = 0; i < m_leds.size(); ++i)
        {
            juce::Rectangle<int> led_bounds(bounds.getX() + m_border + (i + 1) * m_padding + i * led_width,
                                            led_y,
                                            led_width,
                                            led_height);
            
            m_leds[i].m_bounds = led_bounds.transformedBy(transform.inverted());
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
            return peak_db >= led.m_min_db;
        });
        
        m_active_led = it != m_leds.rend() ? m_leds.rend() - (it + 1) : -1;
    }
    
    void MeterTildeView::peakChanged(float new_peak)
    {
        defer([this, new_peak]()
        {
            float peak_db = 20 * log10(new_peak);
            computeActiveLed(peak_db);
            repaint();
        });
    }
    
    void MeterTildeView::paint(juce::Graphics & g)
    {
        g.fillAll(findColour(ObjectView::ColourIds::Background));
        
        for(int i = 0; i < m_leds.size(); ++i)
        {
            g.setColour(findColour(ObjectView::ColourIds::Outline));
            g.drawRect(m_leds[i].m_bounds.toFloat(), 1);

            if (i <= m_active_led)
            {
                g.setColour(m_leds[i].m_colour);
                g.fillRect(m_leds[i].m_bounds.reduced(1).toFloat());
            }
        }
        
        g.setColour(findColour(ObjectView::ColourIds::Outline));
        
        drawOutline(g);
    }
}
