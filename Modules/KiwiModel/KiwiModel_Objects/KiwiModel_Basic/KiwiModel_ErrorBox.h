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

#include <KiwiModel/KiwiModel_Object.h>

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      ERRORBOX                                    //
        // ================================================================================ //
        
        class ErrorBox : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            ErrorBox(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            ErrorBox(std::string const& name, std::vector<Atom> const& args);
            
            //! @brief Set the number of inlets.
            //! @param inlets The number of inlets.
            void setInlets(flip::Array<Inlet> const& inlets);
            
            //! @brief Set the number of inlets.
            //! @param inlets The number of inlets.
            void setOutlets(flip::Array<Outlet> const& outlets);
            
            //! @brief Sets the message error that caused the errorbox construction.
            void setError(std::string const& error);
            
            //! @brief Returns the error that caused the errorbox construction.
            std::string getError() const;

            std::string getIODescription(bool is_inlet, size_t index) const override;
            
            //! @internal flip static declare method
            static void declare();
            
        private: // members
            
            std::string m_error;
        };
    }
}
