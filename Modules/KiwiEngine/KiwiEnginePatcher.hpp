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

#ifndef KIWI_ENGINE_PATCHER_HPP_INCLUDED
#define KIWI_ENGINE_PATCHER_HPP_INCLUDED

#include "KiwiEngineDef.hpp"

namespace kiwi
{    
    namespace engine
    {
        // ================================================================================ //
        //                                      PATCHER                                     //
        // ================================================================================ //
        
        //! @brief The Patcher manages a set of Object and Link.
        class Patcher
        {
        public:
            
            //! @brief Constructor.
            Patcher(model::Patcher const& model) noexcept;
            
            //! @brief Destructor.
            ~Patcher();
            
            //! @brief Returns the objects.
            std::vector<Object const*> getObjects() const;
            
            //! @brief Returns the objects.
            std::vector<Object*> getObjects();
            
            //! @brief Returns the links.
            std::vector<Link const*> getLinks() const;
            
            //! @internal The model changed.
            void modelChanged();
            
            //! @brief Adds a link to the current stack overflow list (or create a new list if there is no).
            //! @internal Only the Object should use this method.
            void addStackOverflow(Link const& link);
            
            //! @brief Ends a list of stack overflow.
            //! @internal Only the Object should use this method.
            void endStackOverflow();
            
            //! @brief Gets the lists of stack overflow.
            std::vector<std::queue<Link const*>> getStackOverflow() const;
            
            //! @brief Clears the lists of stack overflow.
            void clearStackOverflow();
            
        private: // methods
            
            //! @internal Object model has just been added to the document.
            void objectAdded(model::Object& object);
            
            //! @internal Object model has changed.
            void objectChanged(model::Object& object);
            
            //! @internal Object model will be removed from the document.
            void objectRemoved(model::Object& object);
            
            //! @internal Link model has just been added to the document.
            void linkAdded(model::Link& link);
            
            //! @internal Link model has changed.
            void linkChanged(model::Link& link);
            
            //! @internal Link model will be removed from the document.
            void linkRemoved(model::Link& link);
        
        private: // members
            
            using SoLinks = std::queue<Link const*>;
            
            model::Patcher const&   m_model;
            std::vector<SoLinks>    m_so_links;
            
        private: // deleted methods
            
            Patcher(Patcher const&) = delete;
            Patcher(Patcher&&) = delete;
            Patcher& operator=(Patcher const&) = delete;
            Patcher& operator=(Patcher&&) = delete;
        };
    }
}


#endif // KIWI_ENGINE_PATCHER_HPP_INCLUDED
