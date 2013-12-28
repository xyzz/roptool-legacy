#ifndef _TARGET_MANIFEST_H_
#define _TARGET_MANIFEST_H_

// std
#include <string>
#include <memory>

class TargetManifest
{
	public:
		virtual const std::string& version(void) = 0;
		virtual bool parse(const std::string& target) = 0;
};

typedef std::shared_ptr<TargetManifest> TargetManifestPtr;

#endif // _TARGET_MANIFEST_H_