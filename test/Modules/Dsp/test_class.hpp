//
//  test_class.hpp
//  KiwiTestDsp
//
//  Created by Guillot Pierre on 19/02/2016.
//  Copyright © 2016 CICM. All rights reserved.
//

#ifndef test_class_hpp
#define test_class_hpp

#include "KiwiDsp.hpp"

using namespace kiwi::dsp;

class Sig : public Processor
{
public:
    Sig(sample_t value) noexcept : m_value(value) {setNumberOfInlets(0); setNumberOfOutlets(1);}
    ~Sig()  noexcept {}
private:
    bool prepare(Infos const& infos) final {return infos.isOutputConnected(0ul);}
    
    void perform(Buffer const& /*input*/, Buffer& output) noexcept final
    {
        Samples< sample_t >::fill(output.getVectorSize(), m_value, output[0ul]);
    }
    sample_t m_value;
};
/*
class Plus : public Processor
{
public:
    Plus() noexcept {setNumberOfInlets(2); setNumberOfOutlets(1);}
    Plus(sample_t value) noexcept : m_value(value){setNumberOfInlets(1);setNumberOfOutlets(1);}
    ~Plus()  noexcept{}
private:
    bool prepare(Infos const& infos) final
    {
        return infos.isInputConnected(0ul) || (getNumberOfInputs() > 1 && infos.isInputConnected(1ul));
    }
    void perform(Buffer const& buffer) noexcept final
    {
        if(getNumberOfInputs() > 1)
        {
            Samples< sample_t >::add(buffer.getVectorSize(), buffer.getInputSamples(0ul),
                                     buffer.getInputSamples(1ul), buffer.getOutputSamples(0ul));
        }
        else
        {
            Samples< sample_t >::add(buffer.getVectorSize(), m_value,
                                     buffer.getInputSamples(0ul), buffer.getOutputSamples(0ul));;
        }
    }
    sample_t m_value;
};
*/

#endif /* test_class_hpp */
