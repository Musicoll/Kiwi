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

#include <KiwiDsp/KiwiDsp_Processor.h>

using namespace kiwi;
using namespace dsp;

// ==================================================================================== //
//                                      PROCESSORS                                      //
// ==================================================================================== //

// ==================================================================================== //
//                                          SIG                                         //
// ==================================================================================== //

class Sig : public Processor
{
public:
    Sig(sample_t value) noexcept : Processor(0ul, 1ul), m_value(value) {}
    ~Sig() = default;
private:
    
    void prepare(PrepareInfo const& infos) override final
    {
        setPerformCallBack(this, &Sig::perform);
    }
    
    void perform(Buffer const& /*input*/, Buffer& output)
    {
        output[0ul].fill(m_value);
    }
    sample_t m_value;
};

// ==================================================================================== //
//                                      PLUS SCALAR                                     //
// ==================================================================================== //

class PlusScalar : public Processor
{
public:
    PlusScalar(sample_t value) noexcept : Processor(1ul, 1ul), m_value(value) {}
    ~PlusScalar() = default;
private:
    
    void prepare(PrepareInfo const& infos) override final
    {
        m_sig_value.reset(new Signal(infos.vector_size, m_value));
        setPerformCallBack(this, &PlusScalar::perform);
    }
    
    void perform(Buffer const& input, Buffer& output) noexcept
    {
        Signal::add(input[0ul], *m_sig_value.get(), output[0ul]);
    }
    
    sample_t                        m_value;
    std::unique_ptr<const Signal>   m_sig_value;
};

// ==================================================================================== //
//                                      PLUS SIGNAL                                     //
// ==================================================================================== //

class PlusSignal : public Processor
{
public:
    PlusSignal() noexcept : Processor(2ul, 1ul) {}
    ~PlusSignal()  noexcept{}
    
private:
    
    void prepare(PrepareInfo const& infos) override final
    {
        setPerformCallBack(this, &PlusSignal::perform);
    }
    
    void perform(Buffer const& input, Buffer& output) noexcept
    {
        Signal::add(input[0ul], input[1ul], output[0ul]);
    }
};

// ==================================================================================== //
//                                       COPY THROW                                     //
// ==================================================================================== //

class CopyThrow : public Processor
{
public:
    CopyThrow() noexcept : Processor(1ul, 0ul) {}
    ~CopyThrow()  noexcept{}
private:
    
    void prepare(PrepareInfo const& infos) override final
    {
        if(infos.sample_rate != 44100ul || infos.vector_size != 64ul)
        {
            throw Error(std::string("CopyThrow wants a sample rate of 44100 and a vector size of 64."));
        }
        else
        {
            setPerformCallBack(this, &CopyThrow::perform);
        }
    }
    
    void perform(Buffer const& input, Buffer& output) noexcept
    {
        output[0ul].add(input[0ul]);
        output[0ul].add(input[1ul]);
    }
};

// ==================================================================================== //
//                                         PRINT                                        //
// ==================================================================================== //

class Print : public Processor
{
public:
    Print(std::string& string_to_write) noexcept : Processor(1ul, 0ul), m_string(string_to_write) {}
    ~Print() = default;
    
private:
    
    void prepare(PrepareInfo const& infos) override final
    {
        setPerformCallBack(this, &Print::perform);
    }
    
    void perform(Buffer const& input, Buffer&) noexcept
    {
        Signal const& sig = input[0];
        const size_t size = sig.size();
        
        m_string.clear();
        m_string.reserve(size*10);
        
        m_string += "[";
        if(size)
        {
            for(size_t i = 0; i < size - 1; ++i)
            {
                m_string += std::to_string(sig[i]) + ", ";
            }
            
            m_string += std::to_string(sig[size - 1]);
        }
        
        m_string += "]";
    }
    
    std::string& m_string;
};

// ==================================================================================== //
//                                         COUNT                                        //
// ==================================================================================== //

class Count : public Processor
{
public:
    Count() noexcept : Processor(0ul, 1ul) {}
    ~Count() = default;
    
private:
    
    void prepare(PrepareInfo const& infos) override final
    {
        setPerformCallBack(this, &Count::perform);
    }
    
    void perform(Buffer const&, Buffer& output) noexcept
    {
        Signal& sig = output[0ul];
        sample_t* sig_data = sig.data();
        size_t framesize = sig.size();
        while(framesize--)
        {
            *sig_data++ = m_count++;
        }
    }
    
    size_t m_count = 0ul;
};

// ==================================================================================== //
//                                         NULLPROCESSOR                                //
// ==================================================================================== //

class NullProcessor : public Processor
{
public:
    NullProcessor(size_t n_inlets, size_t n_outlets) noexcept : Processor(n_inlets, n_outlets) {}
    NullProcessor() = default;
    
private:
    
    void prepare(PrepareInfo const& infos) override final
    {
        setPerformCallBack(this, &NullProcessor::perform);
    }
    
    void perform(Buffer const&, Buffer& output) noexcept
    {
    }
};

// ==================================================================================== //
//                                         REMOVER                                      //
// ==================================================================================== //

class PlusScalarRemover : public Processor
{
public:
    PlusScalarRemover() noexcept : Processor(2ul, 1ul) {}
    ~PlusScalarRemover() = default;
private:
    
    void prepare(PrepareInfo const& info) override final
    {
        if (info.inputs[0])
        {
            setPerformCallBack(this, &PlusScalarRemover::perform);
        }
    };
    
    void perform(Buffer const& input, Buffer& output) noexcept
    {
        Signal::add(input[0ul], input[1ul], output[0ul]);
    }
};

// ==================================================================================== //
//                                     SHARED SIGNAL                                    //
// ==================================================================================== //

class SharedSignalsChecker : public Processor
{
public:
    SharedSignalsChecker() noexcept : Processor(3ul, 3ul) {}
    ~SharedSignalsChecker()  noexcept {}
    
    Buffer const* m_input;
    Buffer const* m_output;
    
private:
    
    void prepare(PrepareInfo const& info) override final
    {
        setPerformCallBack(this, &SharedSignalsChecker::perform);
    };
    
    void perform(Buffer const& input, Buffer& output) noexcept
    {
        m_input = &input;
        m_output = &output;
    }
};

// ==================================================================================== //
//                                     CONDITIONAL PERFORM                              //
// ==================================================================================== //

class CondPerform : public Processor
{
public:
    CondPerform() noexcept : Processor(2, 1) {}
    
    ~CondPerform() = default;
    
private:
    
    void perform1(Buffer const& input, Buffer& output) {output[0][0] = 1.; }
    
    void perform2(Buffer const& input, Buffer& output){ output[0][0] = 2.; }
    
    void perform0(Buffer const& input, Buffer& output){ output[0][0] = 0.; }
    
    void prepare(PrepareInfo const& infos) override final
    {
        if (infos.inputs[0])
        {
            setPerformCallBack(this, &CondPerform::perform1);
        }
        else if(infos.inputs[1])
        {
            setPerformCallBack(this, &CondPerform::perform2);
        }
        else
        {
            setPerformCallBack(this, &CondPerform::perform0);
        }
    }
};
