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
        m_output_buffer[0].fill(0.);
        m_output_buffer[1].fill(0.);
        
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

// ==================================================================================== //
//                                        KIWISYNTH                                     //
// ==================================================================================== //

class KiwiSynth : public AudioExample
{
public:
    
    KiwiSynth() : AudioExample(0ul, 2ul),
    m_osc(new Osc(440)),
    m_dac(new Dac(m_output_buffer))
    {
    }
    
    void setup() override
    {
        m_chain.addProcessor(m_osc);
        m_chain.addProcessor(m_dac);
        
        m_chain.connect(*m_osc, 0, *m_dac, 0);
        m_chain.connect(*m_osc, 0, *m_dac, 1);
    }
    
    void setFrequency(sample_t const& freq)
    {
        m_osc->setFrequency(freq);
    }
    
private:
    std::shared_ptr<Osc> m_osc;
    std::shared_ptr<Dac> m_dac;
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
