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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_FaustTilde.h>
#include <faust/gui/UI.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    
    //! @brief The User Interface Glue for FAUST
    class FaustTilde::UIGlue : public UI
    {
    public:
        typedef FAUSTFLOAT param_type;
        
        UIGlue(FaustTilde& owner) : m_owner(owner)
        {
            
        }
        
        // ================================================================================ //
        // The FAUST interface
        // ================================================================================ //
        
        // Active widgets
        // ================================================================================ //
        void addButton(const char* label, param_type* zone) override
        {
            addParameter(label, Parameter::Type::Button, zone, 0, 0, 0, 0);
        }
        
        void addCheckButton(const char* label, param_type* zone) override
        {
            addParameter(label, Parameter::Type::CheckButton, zone, 0.f, 1.f, 1.f, 0.f);
        }
        
        void addVerticalSlider(const char* label, param_type* zone, param_type init, param_type min, param_type max, param_type step) override
        {
            addParameter(label, Parameter::Type::Float, zone, min, max, step, init);
        }
        
        virtual void addHorizontalSlider(const char* label, param_type* zone, param_type init, param_type min, param_type max, param_type step) override
        {
            addParameter(label, Parameter::Type::Float, zone, min, max, step, init);
        }
        
        virtual void addNumEntry(const char* label, param_type* zone, param_type init, param_type min, param_type max, param_type step) override
        {
            addParameter(label, Parameter::Type::Float, zone, min, max, step, init);
        }
        
        // Passive widgets
        // ================================================================================ //
        void addHorizontalBargraph(const char* label, param_type* zone, param_type min, param_type max) override
        {
            addOutput(label, zone, min, max);
        }
        
        void addVerticalBargraph(const char* label, param_type* zone, param_type min, param_type max) override
        {
            addOutput(label, zone, min, max);
        }
        
        void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) override
        {
            
        }
        
        // Widget layout
        // ================================================================================ //
        void openTabBox(const char* label) override
        {
            m_params_path.push_back(label);
        }
        
        void openHorizontalBox(const char* label) override
        {
            m_params_path.push_back(label);
        }
        
        void openVerticalBox(const char* label) override
        {
            m_params_path.push_back(label);
        }
        
        void closeBox() override
        {
            m_params_path.pop_back();
        }
        
        // Meta data
        // ================================================================================ //
        void declare(param_type*, const char* key, const char* value) override
        {
            //m_owner.log(std::string("faust~: declare ") + std::string(key) + std::string(" - ") + std::string(value));
        }
        
        // ================================================================================ //
        // The Kiwi interface
        // ================================================================================ //
        void log()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            m_owner.log("faust~: number of parameters " + std::to_string(m_params_short.size()));
            for(auto const& param : m_params_short)
            {
                m_owner.log("faust~: parameter " + param.first + " " + param.second.toString());
            }
        }
        
        param_type getOutput(const std::string& name)
        {
            if(m_mutex_glue.try_lock())
            {
                auto it = m_outputs.find(name);
                if(it == m_outputs.end())
                {
                    m_mutex_glue.unlock();
                    m_owner.warning(std::string("no such FAUST interface \"") + name + std::string("\""));
                    return 0;
                }
                param_type value = *(it->second.zone);
                m_mutex_glue.unlock();
                return value;
            }
            return 0;
        }
        
        bool hasOutput(const std::string& name)
        {
            if(m_mutex_glue.try_lock())
            {
                auto it = m_outputs.find(name);
                if(it == m_outputs.end())
                {
                    m_mutex_glue.unlock();
                    return false;
                }
                m_mutex_glue.unlock();
                return true;
            }
            return false;
        }
        
        void setInput(const std::string& name, param_type value)
        {
            if(m_mutex_glue.try_lock())
            {
                auto it = m_params_short.find(name);
                if(it == m_params_short.end())
                {
                    it = m_params_long.find(name);
                    if(it == m_params_long.end())
                    {
                        m_mutex_glue.unlock();
                        m_owner.warning(std::string("no such FAUST interface \"") + name + std::string("\""));
                        return;
                    }
                }
                
                if(it->second.dirty)
                {
                    m_mutex_glue.unlock();
                    m_owner.warning(std::string("FAUST interfaces \"") + name + std::string("\" not valid anymore"));
                    return;
                }
                if(it->second.type == Parameter::Type::Button)
                {
                    *(it->second.zone) = 0;
                    *(it->second.zone) = 1;
                    m_mutex_glue.unlock();
                    m_owner.schedule([this, name]()
                                     {
                                         if(m_mutex_glue.try_lock())
                                         {
                                             auto it = m_params_short.find(name);
                                             if(it != m_params_short.end())
                                             {
                                                 *(it->second.zone) = 0;
                                             }
                                             m_mutex_glue.unlock();
                                         }
                                     } , std::chrono::milliseconds(static_cast<size_t>(std::floor(m_owner.getBlockSizeInMS()))));
                    return;
                }
                else if(it->second.type == Parameter::Type::CheckButton)
                {
                    *(it->second.zone) = static_cast<param_type>(value < std::numeric_limits<param_type>::epsilon());
                    m_mutex_glue.unlock();
                    return;
                }
                else if(it->second.type == Parameter::Type::Float)
                {
                    *(it->second.zone) = std::max(std::min(it->second.max, value), it->second.min);
                    m_mutex_glue.unlock();
                    return;
                }
                m_owner.warning(std::string("FAUST interface \"") + name + std::string("\" is doesn't requires a value"));
                m_mutex_glue.unlock();
                return;
            }
            m_owner.warning(std::string("FAUST interfaces being processed - please wait"));
        }
        
        void resetToDefault()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            for(auto& param : m_params_short)
            {
                *param.second.zone = param.second.dflt;
            }
        }
        
        void saveStates()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            for(auto& param : m_params_short)
            {
                if(param.second.type != Parameter::Type::Button)
                {
                    param.second.saved = *param.second.zone;
                }
            }
        }
        
        void recallStates()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            for(auto& param : m_params_short)
            {
                if(param.second.type != Parameter::Type::Button)
                {
                    *param.second.zone = param.second.saved;
                }
            }
        }
        
        void prepareChanges()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            for(auto& param : m_params_short)
            {
                param.second.dirty = true;
            }
            m_params_path.clear();
            m_params_long.clear();
            m_outputs.clear();
        }
        
        void finishChanges()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            for(auto it = m_params_short.begin(); it != m_params_short.end();)
            {
                if(it->second.dirty)
                {
                    it = m_params_short.erase(it);
                }
                else
                {
                    it++;
                }
            }
            m_params_path.clear();
        }
        
    private:
        
        //! @brief The Parameter
        class Parameter
        {
        public:
            enum class Type
            {
                Button,
                CheckButton,
                Float
            };
            
            Type        type;
            param_type* zone;
            param_type  min;
            param_type  max;
            param_type  step;
            param_type  dflt;
            param_type  saved;
            bool        dirty; // If the parameter should be deleted
            
            std::string toString() const
            {
                return std::string((type == Type::Button) ? " button" : ((type == Type::CheckButton) ? " check button" : "float")) + " [" + std::to_string(min) + " " + std::to_string(max) + "] " + std::to_string(dflt);
            }
        };
        
        std::string getLongName(const char* name)
        {
            std::string longname;
            for(auto const& step : m_params_path)
                longname += step + "/";
            return longname + name;
        }
        
    
        void addParameter(const char* name, Parameter::Type type, param_type* zone, param_type min, param_type max, param_type step, param_type init)
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            auto it = m_params_short.find(name);
            if(it != m_params_short.end())
            {
                if(it->second.type != type ||
                   it->second.min != min ||
                   it->second.max != max ||
                   it->second.step != step ||
                   it->second.dflt != init)
                {
                    it->second = Parameter({type, zone, min, max, step, init, init, false});
                    m_params_long[getLongName(name)] = it->second;
                    *zone = init;
                }
                else
                {
                    it->second.saved = *it->second.zone;
                    it->second.zone = zone;
                    it->second.dirty = false;
                    m_params_long[getLongName(name)] = it->second;
                    *zone = it->second.saved;
                }
            }
            else
            {
                m_params_short[name] = Parameter({type, zone, min, max, step, init, init, false});
                m_params_long[getLongName(name)] = it->second;
                *zone = init;
            }
        }
        
        void addOutput(const char* name, param_type* zone, param_type min, param_type max)
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            m_outputs[name] = Parameter({Parameter::Type::Float, zone, min, max, 0, 0, 0, false});
        }

        std::map<std::string, Parameter>    m_params_short;
        std::map<std::string, Parameter>    m_params_long;
        std::map<std::string, Parameter>    m_outputs;
        std::vector<std::string>            m_params_path;
        FaustTilde&                         m_owner;
        std::mutex                          m_mutex_glue;
    };
}}
