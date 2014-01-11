#ifndef _TARGET_MANIFEST_H_
#define _TARGET_MANIFEST_H_

// std
#include <string>
#include <memory>

class TargetManifest
{
	public:
		virtual bool parse(const std::string& target) = 0;
		
		virtual const std::string& version(void) const = 0;
		virtual int arch_bitlen(void) const = 0;
};

typedef std::shared_ptr<TargetManifest> TargetManifestPtr;

#endif // _TARGET_MANIFEST_H_