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

class GadgetMap;
typedef std::shared_ptr<GadgetMap> GadgetMapPtr;
typedef std::vector<GadgetMapPtr> GadgetMapPtrList;

class GadgetMap
{
	public:
		virtual bool parse(const std::string& file) = 0;
		
		// gadget map information
		virtual boost::regex regex(void) = 0;
		
		// add gadgets
		virtual void addGadgets(GadgetPtrList gadgets) = 0;
		
		// get size
		virtual int size(void) = 0;
		
		// clone
		virtual GadgetMapPtr clone(void) const = 0;
};

#endif // _GADGET_MAP_H_
