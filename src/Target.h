#ifndef _TARGET_H_
#define _TARGET_H_

// std
#include <string>
#include <memory>

class Target
{
	public:
		virtual const std::string& name(void) = 0;
		virtual void setName(const std::string& name) = 0;
		/*TargetManifestPtr manifest(void) = 0;
		GadgetMapList gadgetMaps(void) = 0;
		GadgetList gadgets(void) = 0; */
};

typedef std::shared_ptr<Target> TargetPtr;

#endif // _TARGET_H_
