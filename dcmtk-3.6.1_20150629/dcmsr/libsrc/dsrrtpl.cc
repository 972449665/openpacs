/*
 *
 *  Copyright (C) 2015, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *
 *  This software and supporting documentation were developed by
 *
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *
 *  Module: dcmsr
 *
 *  Author: Joerg Riesmeier
 *
 *  Purpose:
 *    classes: DSRRootTemplate
 *
 */


#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */

#include "dcmtk/dcmsr/dsrrtpl.h"
#include "dcmtk/dcmsr/dsrstpl.h"
#include "dcmtk/dcmsr/dsrxmld.h"


DSRRootTemplate::DSRRootTemplate(const E_DocumentType documentType,
                                 const OFString &templateIdentifier,
                                 const OFString &mappingResource,
                                 const OFString &mappingResourceUID)
  : DSRDocumentTree(documentType),
    DSRTemplateCommon(templateIdentifier, mappingResource, mappingResourceUID)
{
}


DSRRootTemplate::DSRRootTemplate(const DSRRootTemplate &rootTemplate)
  : DSRDocumentTree(rootTemplate),
    DSRTemplateCommon(rootTemplate)
{
}


DSRRootTemplate::~DSRRootTemplate()
{
}


DSRRootTemplate *DSRRootTemplate::clone() const
{
    return new DSRRootTemplate(*this);
}


DSRDocumentTree *DSRRootTemplate::cloneTree() const
{
    DSRDocumentTree *tree = DSRDocumentTree::clone();
    if (tree != NULL)
    {
        /* mark root CONTAINER with template identification */
        tree->setTemplateIdentification(getTemplateIdentifier(), getMappingResource(), getMappingResourceUID());
    }
    return tree;
}


void DSRRootTemplate::clear()
{
    DSRDocumentTree::clear();
}


OFBool DSRRootTemplate::isValid() const
{
    /* check whether both base classes are valid */
    return DSRDocumentTree::isValid() && DSRTemplateCommon::isTemplateIdentificationValid();
}


const DSRDocumentTree &DSRRootTemplate::getTree()
{
    /* update the document tree for output (if needed) */
    updateTreeForOutput();
    return *this;
}


OFCondition DSRRootTemplate::insertTemplate(const DSRSubTemplate &subTemplate,
                                            const E_AddMode addMode,
                                            const E_RelationshipType defaultRelType)
{
    /* call the functions doing the real work */
    return insertSubTree(subTemplate.cloneTree(), addMode, defaultRelType, OFTrue /*deleteIfFail*/);
}


// protected methods

OFCondition DSRRootTemplate::read(DcmItem & /*dataset*/,
                                  const E_DocumentType /*documentType*/,
                                  const size_t /*flags*/)
{
    /* not implemented (yet) */
    return EC_IllegalCall;
}


OFCondition DSRRootTemplate::readXML(const DSRXMLDocument & /*doc*/,
                                     DSRXMLCursor /*cursor*/,
                                     const size_t /*flags*/)
{
    /* not implemented (yet) */
    return EC_IllegalCall;
}


void DSRRootTemplate::updateTreeForOutput()
{
    /* mark root CONTAINER with template identification (if applicable) */
    OFCondition result = DSRDocumentTree::setTemplateIdentification(getTemplateIdentifier(), getMappingResource(), getMappingResourceUID());
    /* in case of error, output some useful debug information */
    if (result.bad())
    {
        DCMSR_DEBUG("DSRRootTemplate::updateTreeForOutput() Problem with setting TID "
            << getTemplateIdentifier() << " (" << getMappingResource() << "): " << result.text());
    }
}
