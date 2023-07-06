#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "EventLog.h"


void readData(UC* dest, char* sur, long* offset, unsigned long size);
int processEventData(char* buf, long fileSize);
void readFileHeader(struct _fileHeader* fh, char* buf, long* offset);
void readChunkHeader(struct _chunkHeader* ch, char* buf, long* offset);
void printLog(const char* buf, UC* arg, int size);

union convertBuf {
	unsigned char byte;
	unsigned short word;
	unsigned int dword;
	unsigned long qword;

	unsigned char uc[8];
};

void readData(UC* dest, char* sur, long* offset, unsigned long size) {
	memcpy(dest, &sur[*offset], size);
	*offset = *offset + size;
}

void readFileHeader(struct _fileHeader* fh, char* buf, long* offset) {
	readData(fh->fileSignature, buf, offset, sizeof(fh->fileSignature));
	readData(fh->firstChunkNumber, buf, offset, sizeof(fh->firstChunkNumber));
	readData(fh->lastChunkNumber, buf, offset, sizeof(fh->lastChunkNumber));
	readData(fh->nextRecordIdentifier, buf, offset, sizeof(fh->nextRecordIdentifier));
	readData(fh->headerSize, buf, offset, sizeof(fh->headerSize));
	readData(fh->minorFormatVersion, buf, offset, sizeof(fh->minorFormatVersion));
	readData(fh->majorFormatVersion, buf, offset, sizeof(fh->majorFormatVersion));
	readData(fh->headerBlockSize, buf, offset, sizeof(fh->headerBlockSize));
	readData(fh->numberOfChunks, buf, offset, sizeof(fh->numberOfChunks));
	readData(fh->unknown_1, buf, offset, sizeof(fh->unknown_1));
	readData(fh->fileFlags, buf, offset, sizeof(fh->fileFlags));
	readData(fh->checksum, buf, offset, sizeof(fh->checksum));
	readData(fh->unknown_2, buf, offset, sizeof(fh->unknown_2));
}

void readChunkHeader(struct _chunkHeader* ch, char* buf, long* offset) {
	long finally_offset = *offset + _CHUNK_HEADER_SIZE;

	readData(ch->chunkSignature, buf, offset, sizeof(ch->chunkSignature));
	readData(ch->firstEventRecordNumber, buf, offset, sizeof(ch->firstEventRecordNumber));
	readData(ch->lastEventRecordNumber, buf, offset, sizeof(ch->lastEventRecordNumber));
	readData(ch->firstEventRecordIdentifier, buf, offset, sizeof(ch->firstEventRecordIdentifier));
	readData(ch->lastEventRecordIdentifier, buf, offset, sizeof(ch->lastEventRecordIdentifier));
	readData(ch->headerSize, buf, offset, sizeof(ch->headerSize));
	readData(ch->lastEventRecordDataOffset, buf, offset, sizeof(ch->lastEventRecordDataOffset));
	readData(ch->freeSpaceOffset, buf, offset, sizeof(ch->freeSpaceOffset));
	readData(ch->eventRecordsChecksum, buf, offset, sizeof(ch->eventRecordsChecksum));
	readData(ch->unknown_1, buf, offset, sizeof(ch->unknown_1));
	readData(ch->unknown_2, buf, offset, sizeof(ch->unknown_2));
	readData(ch->checksum, buf, offset, sizeof(ch->checksum));
	readData(ch->commonStringOffsetArray, buf, offset, sizeof(ch->commonStringOffsetArray));
	readData(ch->templatePtr, buf, offset, sizeof(ch->templatePtr));

	*offset = finally_offset;
	//printf("%x\n", finally_offset);
}

void printLog(const char* buf, UC* arg, int size) {
	union convertBuf _cb = { 0 };

	if (size <= 8)
		for (int i = 0; i < size; i++) {
			_cb.uc[i] = arg[i];
		}

	switch (size) {
	case 1:
		printf("%s : 0x%lx\n", buf, _cb.byte);
		break;
	case 2:
		printf("%s : 0x%lx\n", buf, _cb.word);
		break;
	case 4:
		printf("%s : 0x%lx\n", buf, _cb.dword);
		break;
	case 8:
		printf("%s : 0x%lx\n", buf, _cb.qword);
		break;
	default:
		printf("%s : Big Data\n", buf);
		break;
	}

}

int processEventData(char* buf, long fileSize) {
	struct _fileHeader fh = { 0, };
	struct _chunkHeader** chs = NULL;
	long buf_offset = 0;

	union convertBuf _cb = { 0 };


	readFileHeader(&fh, buf, &buf_offset);

	for (int i = 0; i < sizeof(fh.numberOfChunks); i++) {
		_cb.uc[i] = fh.numberOfChunks[i];
	}
	//printf("Chunk 숫자 : %d | %xh\n", _cb.word, _cb.word);

	chs = (struct _chunkHeader**)malloc(_cb.word * sizeof(struct _chunkHeader*));
	memset(chs, 0, (_cb.word));
	for (unsigned short i = 0; i < _cb.word; i++) {
		chs[i] = (struct _chunkHeader*)malloc(_CHUNK_HEADER_SIZE);
		if (chs[i] == NULL)
			exit(1);
		memset(chs[i], 0, _CHUNK_HEADER_SIZE);
	}

	for (unsigned short i = 0; i < _cb.word; i++) {
		readChunkHeader(chs[i], buf, &buf_offset);
	}

	// print
	{
		printf("파일명 : Security.evtx\n");
		printf("파일 사이즈 : 0x%lx\n\n", fileSize);

		printf("=== File Header ===\n");
		printLog("File Signature", fh.fileSignature, sizeof(fh.fileSignature));
		printLog("First Chunk Number", fh.firstChunkNumber, sizeof(fh.firstChunkNumber));
		printLog("Last Chunk Number", fh.lastChunkNumber, sizeof(fh.lastChunkNumber));
		printLog("Next Record Identifier", fh.nextRecordIdentifier, sizeof(fh.nextRecordIdentifier));
		printLog("Header Size", fh.headerSize, sizeof(fh.headerSize));
		printLog("Minor Format Version", fh.minorFormatVersion, sizeof(fh.minorFormatVersion));
		printLog("Major Format Version", fh.minorFormatVersion, sizeof(fh.majorFormatVersion));
		printLog("Header Block Size", fh.headerBlockSize, sizeof(fh.headerBlockSize));
		printLog("Number Of Chunks", fh.numberOfChunks, sizeof(fh.numberOfChunks));
		printLog("unknown_1", fh.unknown_1, sizeof(fh.unknown_1));
		printLog("File Flag", fh.fileFlags, sizeof(fh.fileFlags));
		printLog("Checksum", fh.checksum, sizeof(fh.checksum));
		printLog("unknown_2", fh.unknown_2, sizeof(fh.unknown_2));
		printf("\n");



		for (int i = 0; i < sizeof(fh.numberOfChunks); i++) {
			_cb.uc[i] = fh.numberOfChunks[i];
		}
		unsigned short numberOfChunks = _cb.word;

		for (unsigned short i = 0; i < numberOfChunks; i++) {
			printf("=== 0x%x. Chunk Header ===\n", i);
			printLog("Chunk Signature", chs[i]->chunkSignature, sizeof(chs[i]->chunkSignature));
			printLog("First Event Record Number", chs[i]->firstEventRecordNumber, sizeof(chs[i]->firstEventRecordNumber));
			printLog("Last Event Record Number", chs[i]->lastEventRecordNumber, sizeof(chs[i]->firstEventRecordNumber));
			printLog("First Event Record Idenifier", chs[i]->firstEventRecordIdentifier, sizeof(chs[i]->firstEventRecordIdentifier));
			printLog("Last Event Record Idenifier", chs[i]->lastEventRecordIdentifier, sizeof(chs[i]->lastEventRecordIdentifier));
			printLog("Header Size", chs[i]->headerSize, sizeof(chs[i]->headerSize));
			printLog("Last Event Record Data Offset", chs[i]->lastEventRecordDataOffset, sizeof(chs[i]->lastEventRecordDataOffset));
			printLog("Free Space Offset", chs[i]->freeSpaceOffset, sizeof(chs[i]->freeSpaceOffset));
			printLog("Event Records Checksum", chs[i]->eventRecordsChecksum, sizeof(chs[i]->eventRecordsChecksum));
			printLog("unknown_1", chs[i]->unknown_1, sizeof(chs[i]->unknown_1));
			printLog("unknown_2", chs[i]->unknown_2, sizeof(chs[i]->unknown_2));
			printLog("Checksum", chs[i]->checksum, sizeof(chs[i]->checksum));
			printLog("Common String Offset Array", chs[i]->commonStringOffsetArray, sizeof(chs[i]->commonStringOffsetArray));
			printLog("Template Ptr", chs[i]->templatePtr, sizeof(chs[i]->templatePtr));

			printf("\n");
		}


	}

	for (int i = 0; i < sizeof(fh.numberOfChunks); i++) {
		_cb.uc[i] = fh.numberOfChunks[i];
	}
	unsigned short numberOfChunks = _cb.word;
	
	for (unsigned short i = 0; i < numberOfChunks; i++) {
		free(chs[i]);
	}

	free(chs);

	return 0;
}

