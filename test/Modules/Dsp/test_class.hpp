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
    
    void perform(Buffer const& /*input*/, Buffer& output) noexcept final{
        Samples< sample_t >::fill(output.getVectorSize(), m_value, output[0ul]);}
    sample_t m_value;
};

class PlusScalar : public Processor
{
public:
    PlusScalar(sample_t value) noexcept : m_value(value){setNumberOfInlets(1);setNumberOfOutlets(1);}
    ~PlusScalar() noexcept{}
private:
    bool prepare(Infos const& infos) final {
        return infos.isInputConnected(0ul) && infos.isOutputConnected(0ul);}
    
    void perform(Buffer const& input, Buffer& output) noexcept final {
        Samples< sample_t >::add(input.getVectorSize(), m_value, input[0ul], output[0ul]);}
    sample_t m_value;
};

class PlusSignal : public Processor
{
public:
    PlusSignal() noexcept {setNumberOfInlets(2); setNumberOfOutlets(1);}
    ~PlusSignal()  noexcept{}
private:
    bool prepare(Infos const& infos) final {
        return infos.isInputConnected(0ul) && infos.isInputConnected(1ul) && infos.isOutputConnected(0ul);}
    
    void perform(Buffer const& input, Buffer& output) noexcept final{
        Samples< sample_t >::add(input.getVectorSize(), input[0ul], input[1ul], output[0ul]);}
};

#endif /* test_class_hpp */
