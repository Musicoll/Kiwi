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

#ifndef KIWI_NETWORK_QUERRY_HPP_INCLUDED
#define KIWI_NETWORK_QUERRY_HPP_INCLUDED

#include <memory>
#include <vector>

#include <juce_core/juce_core.h>

namespace kiwi
{
    namespace network
    {
        // ================================================================================ //
        //                                 QUERY                                            //
        // ================================================================================ //
        
        //! @brief A class to create a json query to be sent over network.
        //! @details A wrapper over juce::DynamicObject that held a reference counting pointer.
        
        class Query
        {
        public: // methods
            
            //! @brief Default Constructor.
            //! @details Initializes implementation with nullptr.
            Query();
            
            //! @brief  Assignment operator.
            //! @details Ownership transfered.
            Query(juce::DynamicObject::Ptr impl);
            
            //! @brief Constructor with message.
            //! @details Method parses the message the json message.
            //! Might throw if message is not json compliant.
            Query(std::string const& message);
            
            //! @brief Destructor.
            //! @details Pointer on implementation doesn't held reference
            ~Query() = default;
            
            //! @brief Implicite conversion to juce::var object.
            //! @details juce::var created now shares ownership of impl with query.
            operator juce::var() const;
            
            //! @brief Adds a property to the query.
            void setProperty(std::string const& key, juce::var const& var);
            
            //! @brief Retrieves property from query.
            juce::var const& getProperty(std::string const& key);
            
            //! @bref Checks if property is in query. Not recursive.
            bool hasProperty(std::string const& key) const;
            
            //! @brief Retrieves the Query in a json formated string.
            std::string getMessage() const;
            
        private: // members
            
            juce::DynamicObject::Ptr m_impl_obj {nullptr};
            
        private: // deleted methods
            
            Query(Query const& other) = delete;
            Query(Query&& other) = delete;
            Query& operator=(Query const& other) = delete;
            Query& operator=(Query&& other) = delete;
        };
    }
}

#endif // KIWI_NETWORK_QUERRY_HPP_INCLUDED
