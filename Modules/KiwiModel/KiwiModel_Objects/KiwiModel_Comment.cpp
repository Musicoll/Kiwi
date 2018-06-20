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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Comment.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT COMMENT                                  //
    // ================================================================================ //
    
    void Comment::declare()
    {
        // Objectclass
        std::unique_ptr<ObjectClass> comment_class(new ObjectClass("comment", &Comment::create));
        
        // Parameters
        std::unique_ptr<ParameterClass> param_text(new ParameterClass(tool::Parameter::Type::String));
        
        comment_class->addAttribute("text", std::move(param_text));
        
        // Flags
        comment_class->setFlag(ObjectClass::Flag::DefinedSize);
        
        // DataModel
        flip::Class<Comment> & comment_model = DataModel::declare<Comment>()
                                                          .name(comment_class->getModelName().c_str())
                                                          .inherit<Object>()
                                                          .member<flip::String, &Comment::m_comment_text>("comment_text");
        
        Factory::add<Comment>(std::move(comment_class), comment_model);
    }
    
    std::unique_ptr<Object> Comment::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Comment>(args);
    }
    
    Comment::Comment(std::vector<tool::Atom> const& args)
    : Object()
    , m_comment_text("")
    {
        if (args.size() > 0)
            throw Error("comment too many arguments");
        
        setWidth(120.);
        setHeight(20.);
    }
    
    Comment::Comment(flip::Default& d)
    : Object(d)
    , m_comment_text("")
    {}
    
    void Comment::writeAttribute(std::string const& name, tool::Parameter const& parameter)
    {
        if (name == "text")
        {
            m_comment_text = parameter[0].getString();
        }
    }
    
    void Comment::readAttribute(std::string const& name, tool::Parameter & parameter) const
    {
        std::string const& message = m_comment_text;
        
        parameter = tool::Parameter(tool::Parameter::Type::String, {message});
    }
    
    bool Comment::attributeChanged(std::string const& name) const
    {
        bool changed = false;
        
        if (name == "text" && m_comment_text.changed())
        {
            changed = true;
        }
        
        return changed;
    }
    
    std::string Comment::getIODescription(bool is_inlet, size_t index) const
    {
        return "";
    }
}}
