/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
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

#include <KiwiTool/KiwiTool_Atom.h>

namespace kiwi { namespace tool {
    
    //! @brief Parameter is a class designed to represent any type of data.
    //! @details It's implemented as a vector of Atom.
    //! @todo Use virtual classes that implements check of atoms, copy and default initialization instead of using
    //! switches. See in juce::variant.
    class Parameter
    {
    public: // classes
        
        //! @brief The different types of data represented.
        enum class Type
        {
            Int,
            Float,
            String
        };
        
    public: // methods
        
        //! @brief Default Constructor.
        //! @details Initialises data according to type.
        Parameter(Type type);
        
        //! @brief Constructor.
        //! @details Atoms must be well formated for the construction to succeed.
        Parameter(Type type, std::vector<Atom> const atoms);
        
        //! @brief Copy constructor.
        Parameter(Parameter const& other);
        
        //! @brief Assignment operator.
        Parameter& operator=(Parameter const& other);
        
        //! @brief Desctructor.
        ~Parameter();
        
        //! @brief Returns the type of the parameter.
        Type getType() const;
        
        //! @brief Returns the underlying data of the parameter.
        Atom const& operator[](size_t index) const;
        
    private: // members
        
        Type                m_type;
        std::vector<Atom>   m_atoms;
        
    private: // deleted methods
        
        Parameter() = delete;
    };
    
}}
