//
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//

#ifndef KIWI_DSP_LINK_H_INCLUDED
#define KIWI_DSP_LINK_H_INCLUDED

#include "KiwiDspProcessor.hpp"

namespace kiwi
{
namespace dsp
{
    // ==================================================================================== //
    //                                          LINK                                        //
    // ==================================================================================== //
    //! @brief The class owns the attributes of a connection between two Processor objects.
    //! @details The class acts like a simple structure with constant attributes to connect
    //! @details two Processor objects in a Chain object.
    //! @see Chain and Processor
    class Link
    {
    public:
        //! @brief The constructor.
        //! @details Allocates and initializes the Link object.
        //! @param from   The output Processor object.
        //! @param output The index of the output.
        //! @param to     The input Processor object.
        //! @param input  The index of the input.
        Link(Processor const& from, size_t const output, Processor const& to, size_t const input) noexcept;
        
        //! @brief The destructor.
        ~Link() noexcept;
        
        //! @brief Gets the output Processor object.
        inline Processor const& getOutputProcessor() const noexcept {return m_from;}
        
        //! @brief Gets the input Processor object.
        inline Processor const& getInputProcessor() const noexcept {return m_to;}
        
        //! @brief Gets the index of the output Processor object.
        inline size_t getOutputIndex() const noexcept {return m_output;}
        
        //! @brief Gets the index of the input Processor object.
        inline size_t getInputIndex() const noexcept {return m_input;}
    private:
        Processor const&    m_from;
        size_t const        m_output;
        Processor const&    m_to;
        size_t const        m_input;
    };
}
}

#endif // KIWI_DSP_LINK_H_INCLUDED
