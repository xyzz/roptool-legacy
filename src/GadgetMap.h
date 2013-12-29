#ifndef _GADGET_MAP_H_
#define _GADGET_MAP_H_

// roptool
#include "Gadget.h"

// std
#include <string>
#include <memory>
#include <vector>

// boost
#include <boost/regex.hpp>

class GadgetMap
{
	public:
		virtual bool parse(const std::string& file) = 0;
		
		// gadget map information
		virtual boost::regex regex(void) = 0;
		
		// add gadgets
		virtual void addGadgets(GadgetPtrList gadgets) = 0;
};

typedef std::shared_ptr<GadgetMap> GadgetMapPtr;
typedef std::vector<GadgetMapPtr> GadgetMapPtrList;

#endif // _GADGET_MAP_H_
