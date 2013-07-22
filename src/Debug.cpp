#include "Debug.h"
#include <cstdio>
#include <cstdarg>
#include <iostream>

Debug *Debug::m_instance = NULL;

void Debug::msg(const char *class_name, const char *func, const char *fmt, ...)
{
	// check for an override or the class name
	if (m_override || (m_class_list.count(class_name)))
	{
		// container
		char output[256];
		
		// resolve into string
		va_list argl;
		va_start(argl, fmt);
		vsnprintf(output, sizeof(output), fmt, argl);
		va_end(argl);

		// TODO:
		// allow file output too

		// display message 
        std::cout << "DEBUG: " << func << " " << output;
	}
}

void Debug::add_class(const char *class_name)
{
	// add string
	m_class_list.insert(class_name);
}

void Debug::log_all(bool enable)
{
	// set the override
	m_override = enable;
}

Debug *Debug::instance(void)
{
	// check if we have a debug object
	if (!m_instance)
	{
		// return debug obj
		m_instance = new Debug();
	}
	
	// return the instance
	return m_instance;
}
