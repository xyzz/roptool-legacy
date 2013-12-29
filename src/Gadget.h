#ifndef _GADGET_H_
#define _GADGET_H_

// std
#include <string>
#include <memory>
#include <vector>

class Gadget
{
	public:
		virtual bool parse(const std::string& file) = 0;
		
		virtual const std::string& name(void) = 0;
};

typedef std::shared_ptr<Gadget> GadgetPtr;
typedef std::vector<GadgetPtr> GadgetPtrList;

#endif // _GADGET_H_
