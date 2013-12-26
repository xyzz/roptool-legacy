#ifndef _XML_TARGET_MANIFEST_H_
#define _XML_TARGET_MANIFEST_H_

// roptool
#include "TargetManifest.h"

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
		bool parseFile(const std::string& file);
		
		const std::string& version(void) { return m_version; }
		
	private:
		class XmlTargetManifestVisitor;
		std::unique_ptr<XmlTargetManifestVisitor> m_visitor;
		std::unique_ptr<tinyxml2::XMLDocument> m_xmldoc;
		std::string m_version;
};

typedef std::shared_ptr<XmlTargetManifest> XmlTargetManifestPtr;

#endif // _XML_TARGET_MANIFEST_H_