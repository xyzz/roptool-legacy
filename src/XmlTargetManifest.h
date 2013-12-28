#ifndef _XML_TARGET_MANIFEST_H_
#define _XML_TARGET_MANIFEST_H_

// roptool
#include "TargetManifest.h"
#include "XmlActionVisitor.h"

// std
#include <memory>

// forward declare class
namespace tinyxml2
{
	class XMLDocument;
}

class XmlTargetManifest : public TargetManifest
{
	public:
		XmlTargetManifest(void);
		~XmlTargetManifest(void);
		
		// file setting
		bool parse(const std::string& file);
		
		const std::string& version(void) { return m_version; }
		
	private:
		std::unique_ptr<XmlActionVisitor> m_visitor;
		std::unique_ptr<tinyxml2::XMLDocument> m_xmldoc;
		std::string m_version;
		
		void set_version(const std::string& version);
};

typedef std::shared_ptr<XmlTargetManifest> XmlTargetManifestPtr;

#endif // _XML_TARGET_MANIFEST_H_