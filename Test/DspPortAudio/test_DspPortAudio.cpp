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

#include <KiwiDsp/KiwiDsp_Chain.hpp>

#include "PortAudioExample.hpp"
#include "Processors.hpp"

#include <thread>

// ==================================================================================== //
//                                          SYNTH                                       //
// ==================================================================================== //

class AudioExample : public PortAudioExample
{
public: // methods
    
    AudioExample(size_t inputs      = 2,
                 size_t outputs     = 2,
                 size_t samplerate  = 44100ul,
                 size_t vectorsize  = 1024ul) :
    PortAudioExample(samplerate, vectorsize),
    m_output_buffer(outputs, vectorsize, 0.),
    m_dac(m_output_buffer)
    {
        
    }
    
    virtual ~AudioExample()
    {
        m_chain.release();
    }
    
    void play()
    {
        m_chain.release();
        m_links.clear();
        setup();
        
        add(m_dac);
        std::set<Link*> links;
        for(auto& link_uptr : m_links)
        {
            links.emplace(link_uptr.get());
        }
        
        m_chain.compile(getSampleRate(), getVectorSize(), m_processes, links);
        
        start();
    }
    
    void add(Processor& processor)
    {
        m_processes.insert(&processor);
    }
    
    void connect(Processor const& from, size_t from_index, Processor const& to, size_t to_index)
    {
        //Link link(from, from_index, to, to_index);
        m_links.emplace(new Link(from, from_index, to, to_index));
    }
    
    void perform(float*	out, unsigned long vectorsize) override
    {
        m_output_buffer[0].clear();
        m_output_buffer[1].clear();
        
        m_chain.tick();
        
        for(unsigned long i = 0; i < vectorsize; i++ )
        {
            // interleaved output
            *out++ = m_output_buffer[0][i];
            *out++ = m_output_buffer[1][i];
        }
    }
    
protected: // methods
    
    Dac const& getDac() const noexcept
    {
        return m_dac;
    }
    
    //! @brief Add nodes and connect them here.
    virtual void setup() = 0;
    
private: // members
    
    Chain                   m_chain;
    std::set<Processor*>    m_processes;
    std::set<std::unique_ptr<Link>> m_links;
    Buffer                  m_output_buffer;
    Dac                     m_dac;
};

class KiwiSynth : public AudioExample
{
public:
    
    KiwiSynth() : AudioExample(0ul, 2ul),
    m_osc(440.),
    m_osc_gain_1(10.),
    m_osc_gain_2(10.5)
    {
        
    }
    
    void setup() override
    {
        add(m_osc);
        add(m_osc_gain_1);
        add(m_osc_gain_2);
        add(m_multiply);
        
        //connect(m_osc, 0ul, m_multiply, 0ul);
        //connect(m_osc_gain_1, 0ul, m_multiply, 1ul);
        //connect(m_osc_gain_2, 0ul, m_multiply, 1ul);
        //connect(m_multiply, 0ul, getDac(), 0ul);
        //connect(m_multiply, 0ul, getDac(), 1ul);
        //connect(m_osc, 0ul, getDac(), 0ul);
        
        connect(m_osc, 0ul, getDac(), 0ul);
        connect(m_osc, 0ul, getDac(), 1ul);
    }
    
    void setFrequency(sample_t const& freq)
    {
        m_osc.setFrequency(freq);
    }
    
private:

    Osc             m_osc;
    Osc             m_osc_gain_1;
    Osc             m_osc_gain_2;
    MultiplySignal  m_multiply;
};

// ==================================================================================== //
//                                          MAIN                                        //
// ==================================================================================== //

int main(int argc, const char * argv[])
{
    std::cout << "~ KiwiDspPortAudioExample\n";
    
    KiwiSynth synth;
    
    std::thread t([&synth]()
    {
        synth.play();
    });
    
    size_t nloops = 10ul;
    std::vector<sample_t> frequencies
    {
        329., 369., 493., 554., 587., 369.,
        329., 554., 493., 369., 587., 554.
    };
    
    while(nloops--)
    {
        for(size_t i = 0; i < frequencies.size(); i++)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(120));
            
            synth.setFrequency(frequencies[i]);
        }
    }
    
    synth.stop();
    
    t.join();
    
    return 0;
}
