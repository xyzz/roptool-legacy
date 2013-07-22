#ifndef __DEBUG_H__
#define __DEBUG_H__


#define debug(p, ...)                                                                           \
{                                                                                               \
    if (p <= Debug::instance()->level())                                                        \
    {                                                                                           \
        Debug::instance()->msg(typeid(*this).name() + 6, __FUNCTION__, __VA_ARGS__);            \
    }                                                                                           \
}

#include <set>
#include <string>

class Debug
{
    public:
        // singleton model
        static Debug *instance(void);

        // display debug message
        void msg(const char *class_name, const char *function, const char *fmt, ...);

        // add a class to the debug output
        void add_class(const char *class_name);

        // optionally, force to log everything
        void log_all(bool enable);

        // get the current debug level
        int level(void) { return m_level; }

        // set the current debug level
        void set_level(int level) { m_level = level; }
        
    private:
        // private constructor so it cannot be instantiated
        Debug(void) : m_override(true), m_level(0) { }

        // only instance of debug
        static Debug *m_instance;

        // our set of classes to debug
        std::set<std::string> m_class_list;

        bool m_override;
        int m_level;
};

#endif // __DEBUG_H__
