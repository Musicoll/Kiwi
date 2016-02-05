/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef __DEF_OSP_LINK__
#define __DEF_OSP_LINK__

#if (__cplusplus <= 199711L)
#define noexcept
#define nullptr NULL
#define constexpr
#endif

namespace osp
{
    // ==================================================================================== //
    //                                          LINK                                        //
    // ==================================================================================== //
    //! @brief The class owns the attributes of a connection between two Node objects.
    //! @details The class acts like a simple structure with constant attributes to connect
    //! @details two Node objects in a Chain object. The member attributes are private to
    //! @details allow future changes in the class. You should refer to Chain to perform the
    //! @details connection.
    //! @see Chain and Node
    class Link
    {
    public:
        //! @brief The constructor.
        //! @details Allocates and initializes the object. The validity of the object is
        //! @details examined during the compilation of the Chain object by connecting the
        //! @details Nodes objects.
        //! @param from   The output Node object.
        //! @param output The index of the output.
        //! @param to     The input Node object.
        //! @param input  The index of the input.
        inline Link(Node* from, size_t output, Node* to, size_t input) noexcept :
        m_from(from), m_output(output), m_to(to), m_input(input) {};
        
        //! @brief The destructor.
        inline ~Link() noexcept {};
        
        //! @brief Retrieves the output Node object.
        inline Node* getOutpuNode() const noexcept {return m_from;}
        
        //! @brief Retrieves the input Node object.
        inline Node* getInputNode() const noexcept {return m_to;}
        
        //! @brief Retrieves the index of the output Node object.
        inline size_t getOutputIndex() const noexcept {return m_output;}
        
        //! @brief Retrieves the index of the input Node object.
        inline size_t getInputIndex() const noexcept {return m_input;}
        
    private:
        Node*   m_from;
        size_t  m_output;
        Node*   m_to;
        size_t  m_input;
    };
}

#endif
