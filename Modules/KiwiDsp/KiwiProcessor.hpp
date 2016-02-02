/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#ifndef __DEF_OSP_PROCESSOR__
#define __DEF_OSP_PROCESSOR__

#if (__cplusplus <= 199711L)
#define noexcept
#define nullptr NULL
#define constexpr
#endif

namespace osp
{
    class Node;
    // ==================================================================================== //
    //                                      PROCESSOR                                       //
    // ==================================================================================== //
    //! @brief The pure virtual class that processes digital signal in a Chain object.
    //! @details The class is pure virtual and allows to implement digital signal processing.
    //! @details You should implement the pure virtual methods prepare(), perform() and
    //! @details release().
    //! @see Chain, Node, Link and Signal
    class Processor
    {
    public:
        //! @brief The constructor.
        //! @details Allocates and initializes a default Processor object.
        inline constexpr Processor() noexcept :
        m_ninputs(0ul), m_noutputs(0ul), m_inplace(false), m_valid(false), m_running(false) {}
        
        //! @brief The destructor.
        virtual ~Processor() noexcept {};
        
        //! @brief Retrieves the current number of inputs.
        //! @return The number of inputs of the Processor object.
        inline size_t getNumberOfInputs() const noexcept {return m_ninputs;}
        
        //! @brief Retrieves the current number of outputs.
        //! @return The number of outputs of the Processor object.
        inline size_t getNumberOfOutputs() const noexcept {return m_noutputs;}
        
        //! @brief Retrieves if the DSP is inplace.
        //! @return true if the DSP is inplace, otherwise false.
        inline bool isInplace() const noexcept {return m_inplace;}
        
        //! @brief Retrieves if the DSP is running.
        //! @return true if the DSP is running, otherwise false.
        inline bool isRunning() const noexcept {return m_running;}
        
    protected:
        //! @brief Sets the number of inputs.
        //! @details You should use this method while the DSP ins't running.
        //! @param nins The number of inputs.
        //! @see setNumberOfOutlets() and getNumberOfInputs()
        inline void setNumberOfInlets(const size_t nins) noexcept {if(!m_running){m_ninputs = nins;}}
        
        //! @brief Sets the number of outputs.
        //! @details You should use this method while the DSP ins't running.
        //! @param nouts The number of outputs.
        //! @see setNumberOfInlets() and getNumberOfOutputs()
        inline void setNumberOfOutlets(const size_t nouts) noexcept {if(!m_running){m_noutputs = nouts;}}
        
        //! @brief Sets if the DSP should be inplace.
        //! @details You should use this method before the perform method's call to set
        //! @details if the inputs and outputs Signal objects own the same samples vectors.
        //! @param status The inplace status.
        //! @see isInplace()
        inline void setInplace(const bool status) noexcept {m_inplace = status;}
        
        //! @brief Sets if the perform method should be called.
        //! @details You should always use this method during the call of the prepare method'
        //! @details to notify the Chain object that the perform method can be called. Usually
        //! @details this method is called with true but sometimes you should want to avoid
        //! @details the perform if the sample rate, the vector size or the connected inputs
        //! @details and outputs don't match with the needs of your perform.
        //! @param status The perform status.
        //! @see prepare()
        inline void shouldPerform(const bool status) noexcept {m_running  = status;}
        
    private:
        //! @brief Prepares everything for the perform method.
        //! @details This is a pure virtual method. You should use this method to check the
        //! @details vector size, the sample rate, the connected inputs and outputs and to
        //! @details allocate memory if need. You should always call the shouldPerform method
        //! @details to notify that everything is well setted to process or not.
        //! @see shouldPerform() and release()
        virtual void prepare(Node const& node) = 0;
        
        //! @brief Performs the digital signal processing.
        //! @details This is a pure virtual method. You can use the methods in the following
        //! @details section but you should avoid to allocate memory or do it asynchronously.
        //! @see getInputsSamples(), getOutputsSamples(), getInputSamples(),
        //! @see  getOutputSamples(), getNumberOfInputs(), getNumberOfOutputs(),
        //! @see getVectorSize() and getSampleRate()
        virtual void perform() noexcept = 0;
        
        //! @brief Releases everything after the digital signal processing.
        //! @details You can use this method to free the memory allocated during the call of
        //! @details the prepare method for example.
        //! @see prepare()
        virtual void release() {};
        
        size_t              m_ninputs;
        size_t              m_noutputs;
        bool                m_inplace;
        std::atomic_bool    m_valid;
        std::atomic_bool    m_running;
        friend Node;
    };
}

#endif
