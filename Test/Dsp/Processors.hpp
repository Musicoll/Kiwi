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

#include <KiwiDsp/KiwiDsp_Processor.hpp>

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
    void perform(Buffer const& /*input*/, Buffer& output) noexcept final
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
    
    bool prepare(PrepareInfo const& infos) override final
    {
        m_sig_value.reset(new Signal(infos.vector_size, m_value));
        return true;
    }
    
    void perform(Buffer const& input, Buffer& output) noexcept final
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
    
    void perform(Buffer const& input, Buffer& output) noexcept final
    {
        Signal::add(input[0ul], input[1ul], output[0ul]);
    }
    
    bool prepare(PrepareInfo const& infos) override final
    {
        return true;
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
    bool prepare(PrepareInfo const& infos) final
    {
        if(infos.sample_rate != 44100ul || infos.vector_size != 64ul)
        {
            throw Error(std::string("CopyThrow wants a sample rate of 44100 and a vector size of 64."));
        }
        
        return true;
    }
    
    void perform(Buffer const& input, Buffer& output) noexcept final
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
    
    void perform(Buffer const& input, Buffer&) noexcept final
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
    
    void perform(Buffer const&, Buffer& output) noexcept final
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
    
    void perform(Buffer const&, Buffer& output) noexcept final
    {
    }
};

// ==================================================================================== //
//                                         REMOVER                                      //
// ==================================================================================== //

class PlusScalarRemover : public Processor
{
public:
    PlusScalarRemover() noexcept : Processor(1ul, 1ul) {}
    ~PlusScalarRemover() = default;
private:
    
    bool prepare(PrepareInfo const& info) override
    {
        if (info.inputs[0])
        {
            m_signal_1.reset(new Signal(info.vector_size, 1.));
            return true;
        }
        else
        {
            return false;
        }
    };
    
    void perform(Buffer const& input, Buffer& output) noexcept override final
    {
        Signal::add(output[0ul], input[0ul], *m_signal_1);
    }
    
    std::unique_ptr<Signal> m_signal_1;
};
