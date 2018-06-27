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
#include <flip/Message.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT FAUST~                                   //
    // ================================================================================ //
    
    class FaustTilde : public model::Object
    {
    public:

        static void declare();
        
        static std::unique_ptr<Object> create(std::vector<tool::Atom> const& args);
        
        FaustTilde(flip::Default& d);
        
        FaustTilde(std::vector<tool::Atom> const& args);
        
        std::string getIODescription(bool is_inlet, size_t index) const override;
        
        void setDSPCode(std::string const& newcode);
        
        std::string getDSPCode() const;
        
        void setEditCode(std::string const& newcode);
        
        std::string getEditCode() const;
        
        void writeAttribute(std::string const& name, tool::Parameter const& parameter) override;
        
        void readAttribute(std::string const& name, tool::Parameter & parameter) const override;
        
        bool attributeChanged(std::string const& name) const override;
    private:
        
        flip::String               m_dsp_code;
        flip::Message<std::string> m_dsp_code_changed;
        flip::String               m_edit_code;
        flip::Message<std::string> m_edit_code_changed;
    };
    
}}
