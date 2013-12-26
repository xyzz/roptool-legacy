#ifndef _FOLDER_TARGET_H_
#define _FOLDER_TARGET_H_

// roptool
#include "Target.h"

// std
#include <string>

class FolderTarget : public Target
{
	public:
		FolderTarget(void) { }
		FolderTarget(const std::string& name) { setName(name); }
		
		// accessor/mutator
		const std::string& name(void) { return m_name; }
		void setName(const std::string& name) { m_name = name; }
		
	private:
		std::string m_name;
};

#endif // _FOLDER_TARGET_H_