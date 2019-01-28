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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Unpack.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    UNPACK                                        //
    // ================================================================================ //
    
    void Unpack::declare()
    {
        Factory::add<Unpack>("unpack", &Unpack::create);
    }
    
    std::unique_ptr<Object> Unpack::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Unpack>(model, patcher);
    }
    
    Unpack::Unpack(model::Object const& model, Patcher& patcher) :
    Object(model, patcher),
    m_list(model.getArguments())
    {
    }
    
    void Unpack::output_list()
    {
        for (int i = m_list.size() - 1; i >= 0; --i)
        {
            send(i, {m_list[i]});
        }
    }
    
    void Unpack::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (args[0].isBang())
        {
            output_list();
        }
        else
        {
            for (int i = std::max(m_list.size() - 1, args.size() - 1); i >= 0; --i)
            {
                if (!args[i].isBang())
                {
                    switch (m_list[i].getType())
                    {
                        case tool::Atom::Type::Float:
                        {
                            m_list[i] = args[i].getFloat();
                            send(i, {m_list[i]});
                            break;
                        }
                        case tool::Atom::Type::Int:
                        {
                            m_list[i] = args[i].getInt();
                            send(i, {m_list[i]});
                            break;
                        }
                        case tool::Atom::Type::String:
                        {
                            if (args[i].isString())
                            {
                                m_list[i] = args[i].getString();
                                send(i, {m_list[i]});
                            }
                            break;
                        }
                        default: break;
                    }
                }
                else if (i > 0)
                {
                    warning("unpack list element " + std::to_string(i) + " bang not taken into account");
                }
            }
        }
    }
    
}}
