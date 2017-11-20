/**
 * @author See Contributors.txt for code contributors and overview of BadgerDB.
 *
 * @section LICENSE
 * Copyright (c) 2012 Database Group, Computer Sciences Department, University of Wisconsin-Madison.
 */

#include "btree.h"
#include "filescan.h"
#include "exceptions/bad_index_info_exception.h"
#include "exceptions/bad_opcodes_exception.h"
#include "exceptions/bad_scanrange_exception.h"
#include "exceptions/no_such_key_found_exception.h"
#include "exceptions/scan_not_initialized_exception.h"
#include "exceptions/index_scan_completed_exception.h"
#include "exceptions/file_not_found_exception.h"
#include "exceptions/end_of_file_exception.h"


//#define DEBUG

namespace badgerdb
{
	// -----------------------------------------------------------------------------
	// BTreeIndex::BTreeIndex -- Constructor
	// -----------------------------------------------------------------------------
	BTreeIndex::BTreeIndex(
		const std::string & relationName,
		std::string & outIndexName,
		BufMgr *bufMgrIn,
		const int attrByteOffset,
		const Datatype attrType) {

			// Create index file name
			std::ostringstream idxStr;
			idxStr << relationName << '.' << attrByteOffset;
			outIndexName = idxStr.str();

			// initialize btree index variables
			bufMgr = bufMgrIn;
			headerPageNum = 1;
			attributeType = attrType;
			this->attrByteOffset = attrByteOffset;
			leafOccupancy = 0;
			nodeOccupancy = 0;
			scanExecuting = false;

			IndexMetaInfo* metadata;
			Page* headerPage, rootPage;

			try {
				// Create file, check if it exists
				file = new BlobFile(outIndexName, true);
				// File does not exist, so new index file has been created

				// Allocate index meta info page and btree root page
				bufMgr->allocPage(file, headerPageNum, headerPage);
				bufMgr->allocPage(file, rootPageNum, rootPage);

				// Set up index meta info
				metadata = (IndexMetaInfo*) headerPage;
				strcpy(metadata->relationName, relationName.c_str());
				metadata->attrByteOffset = attrByteOffset;
				metadata->attributeType = attrType;
				metadata->rootPageNo = rootPageNum;

				// Set up the root of the btree
				NonLeafNodeInt* root = (NonLeafNodeInt*) rootPage;
				root->level = 1;
				for (int i = 0; i < INTARRAYNONLEAFSIZE; i++) {
					root->keyArray[i] = 0;
					root->pageNoArray[i] = 0;
				}
				root->pageNoArray[INTARRAYNONLEAFSIZE] = 0;

				// Scan relation and insert entries for all tuples into index
				try {
					FileScan fileScan(relationName, bufMgr);
					RecordId rid;
					while (true) {
						fileScan.scanNext(rid);
						insertEntry((int*) fileScan.getRecord().c_str() + attrByteOffset, rid);
					}
				} catch (EndOfFileException e) {
					// Do nothing. Finished scanning file.
				}
			} catch (FileExistsException e) {  // File exists
				// Open the file
				file = new BlobFile(outIndexName, false);

				// Get index meta info for value checking
				bufMgr->readPage(file, headerPageNum, headerPage);
				metadata = (IndexMetaInfo*) headerPage;

				// Check that values in (relationName, attribute byte, attribute type etc.) match parameters
				if (strcmp(metadata->relationName, relationName.c_str()) != 0
					|| metadata->attrByteOffset != attrByteOffset
					|| metadata->attributeType != attrType) {
						// Metadata does not match the parameters
						throw BadIndexInfoException();
				}
				// Metatdata matches
			}
		}


		// -----------------------------------------------------------------------------
		// BTreeIndex::~BTreeIndex -- destructor
		// -----------------------------------------------------------------------------

		// TODO: sreejita
		BTreeIndex::~BTreeIndex() {
		}

		// -----------------------------------------------------------------------------
		// BTreeIndex::insertEntry
		// -----------------------------------------------------------------------------

		// TODO: sahib, haylee
		void BTreeIndex::insertEntry(const void *key, const RecordId rid) {

		}

		// -----------------------------------------------------------------------------
		// BTreeIndex::startScan
		// -----------------------------------------------------------------------------

		// TODO: sreejita
		void BTreeIndex::startScan(const void* lowValParm,
			const Operator lowOpParm,
			const void* highValParm,
			const Operator highOpParm) {

		}

		// -----------------------------------------------------------------------------
		// BTreeIndex::scanNext
		// -----------------------------------------------------------------------------

		// TODO: sahib
		void BTreeIndex::scanNext(RecordId& outRid) {

		}

		// -----------------------------------------------------------------------------
		// BTreeIndex::endScan
		// -----------------------------------------------------------------------------
		//
		// TODO: sreejita
		void BTreeIndex::endScan() {

		}

}
