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

#include "jObject.hpp"

namespace kiwi
{
    jObject::jObject()
    {
        setSize(100, 20);
    }
    
    jObject::~jObject()
    {
        ;
    }
    
    void jObject::document_changed(model::Object& object)
    {
        if(object.added())
        {
            m_model = &object;
        }
        
        if(object.positionChanged())
        {
            setTopLeftPosition(juce::Point<int>(object.getX(), object.getY()));
            std::cout << "jObject ---> positionChanged" << '\n';
        }
        
        if(object.removed())
        {
            m_model = nullptr;
        }
    }
    
    void jObject::paint(juce::Graphics & g)
    {
        auto bounds = getLocalBounds();
        
        g.setColour(juce::Colours::white);
        g.fillRect(bounds);
        
        g.setColour(juce::Colours::black);
        g.drawRect(bounds);
        
        g.drawFittedText(m_model->getName(), bounds.reduced(5), juce::Justification::centredLeft, 1);
        
        drawInletsOutlets(g);
    }
    
    void jObject::drawInletsOutlets(juce::Graphics & g)
    {
        const unsigned int io_width = 5;
        const unsigned int io_height = 3;
        const juce::Colour io_color(0.3, 0.3, 0.3);
        const juce::Rectangle<int> bounds = getLocalBounds();
        
        const unsigned int ninlets = m_model->getNumberOfInlets();
        const unsigned int noutlets = m_model->getNumberOfOutlets();

        if(ninlets)
        {
            g.setColour(io_color);
            g.fillRect(bounds.getX(), bounds.getY(), io_width, io_height);
            
            if(ninlets > 1)
            {
                const double ratio = (bounds.getWidth() - io_width) / (double)(ninlets - 1);
                for(unsigned int i = 1; i < ninlets; ++i)
                {
                    g.fillRect(bounds.getX() + ratio * i, bounds.getY(), io_width, io_height);
                }
            }
        }
        
        if(noutlets)
        {
            g.setColour(io_color);
            g.fillRect(bounds.getX(), bounds.getY() + bounds.getHeight() - io_height, io_width, io_height);
            
            if(noutlets > 1)
            {
                const double ratio = (bounds.getWidth() - io_width) / (double)(noutlets - 1);
                for(unsigned int i = 1; i < noutlets; --i)
                {
                    juce::Rectangle<int> outlet(bounds.getX() + ratio * i,
                                                bounds.getY() + bounds.getHeight() - io_height,
                                                io_width, io_height);
                    g.fillRect(outlet);
                }
            }
        }
    }
    
    void jObject::mouseDown(juce::MouseEvent const& event)
    {
        // signal engine
    }
    
    void jObject::mouseDrag(juce::MouseEvent const& event)
    {
        ;
    }
}
