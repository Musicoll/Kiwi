/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
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
    m_output_buffer(outputs, vectorsize, 0.)
    {
        
    }
    
    virtual ~AudioExample()
    {
        m_chain.release();
    }
    
    void play()
    {
        m_chain.release();
        setup();
        
        m_chain.prepare(getSampleRate(), getVectorSize());
        
        start();
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
    
    //! @brief Add nodes and connect them here.
    virtual void setup() = 0;
    
protected: // members
    Chain                   m_chain;
    Buffer                  m_output_buffer;
};

class KiwiSynth : public AudioExample
{
public:
    
    KiwiSynth() : AudioExample(0ul, 2ul)
    {
        
    }
    
    void setup() override
    {
        m_chain.addProcessor(0, std::unique_ptr<Processor>(new Osc(440)));
        m_chain.addProcessor(1, std::unique_ptr<Processor>(new Osc(10.)));
        m_chain.addProcessor(2, std::unique_ptr<Processor>(new Osc(10.5)));
        m_chain.addProcessor(3, std::unique_ptr<Processor>(new MultiplySignal()));
        m_chain.addProcessor(4, std::unique_ptr<Processor>(new Dac(m_output_buffer)));
        
        //connect(m_osc, 0ul, m_multiply, 0ul);
        //connect(m_osc_gain_1, 0ul, m_multiply, 1ul);
        //connect(m_osc_gain_2, 0ul, m_multiply, 1ul);
        //connect(m_multiply, 0ul, getDac(), 0ul);
        //connect(m_multiply, 0ul, getDac(), 1ul);
        //connect(m_osc, 0ul, getDac(), 0ul);
        
        m_chain.connect(0, 0, 4, 0);
        m_chain.connect(0, 0, 4, 1);
    }
    
    void setFrequency(sample_t const& freq)
    {
        std::dynamic_pointer_cast<Osc>(m_chain.getProcessor(0))->setFrequency(freq);
    }
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
