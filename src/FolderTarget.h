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
		void setName(const std::string& name);
		
		// 
		TargetManifestPtr manifest(void) { return m_manifest; }
		
	private:
		std::string m_name;
		TargetManifestPtr m_manifest;
};

#endif // _FOLDER_TARGET_H_