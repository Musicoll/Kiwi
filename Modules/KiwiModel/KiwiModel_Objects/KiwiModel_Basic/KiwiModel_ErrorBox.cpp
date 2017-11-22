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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Basic/KiwiModel_ErrorBox.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      ERRORBOX                                    //
        // ================================================================================ //
        
        void ErrorBox::declare()
        {
            Factory::add<ErrorBox>("errorbox").setInternal(true);
        }
        
        ErrorBox::ErrorBox(std::string const& name, std::vector<tool::Atom> const& args)
        {
        }
        
        void ErrorBox::setInlets(flip::Array<Inlet> const& inlets)
        {
            model::Object::setInlets(inlets);
        }
        
        void ErrorBox::setOutlets(flip::Array<Outlet> const& outlets)
        {
            model::Object::setOutlets(outlets);
        }
        
        std::string ErrorBox::getIODescription(bool is_inlet, size_t index) const
        {
            return "(nothing here)";
        }
        
        void ErrorBox::setError(std::string const& error_message)
        {
            m_error = error_message;
        }
        
        std::string ErrorBox::getError() const
        {
            return m_error;
        }
    }
}
