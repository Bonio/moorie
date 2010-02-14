#ifndef _LIBMOOR_H_
#define _LIBMOOR_H_

#include <string>

#include "Hash.h"
#include "Mailbox.h"
#include "Mailboxes.h"
#include "Status.h"

class CLibMoor {
	public:
		CLibMoor();
		~CLibMoor();

	public:
		bool Dehash(const std::string& hashcode);
		int selectDownloadMailBox(int MailBox, std::string path = "");
		int startDownload();
		bool downloadDone;
                bool downloadPaused;
                bool uploadDone;

                Status getDownloadStatus();
                Status getUploadStatus();
                void pauseDownload();
                void unpauseDownload();
                int getMyUploadNumOfSeg();
                std::string getMyUploadFileCRC();

		// upload
                int selectUploadMailBox(std::string login, std::string passwd, std::string adressee, std::string downPasswd, std::string editPasswd);
		int splitFile(std::string filename, int size);
		int startUpload(unsigned int fromseg);
		std::string addMirror(std::string editpass, std::string orighash, std::string mboxaddr, std::string mboxpass); // adding mirror to hashcode
                std::string generateCleanHashcode(); // clear hashcode generator

	private:
		/**
		 * Gets number of last downloaded segment.
		 * @param filePath Path to downloaded file.
		 * @return Last downloaded segment.
		 */
		unsigned int getLastSegment(const std::string& filePath);

		boost::shared_ptr<Hash> myHash;
		CMailBox* myMailBox;
		int mySeg;
		std::string myPath;
                Status::State state;

                // upload
		std::string myUploadFilename;
		int myUploadFilesize;
		int myUploadSegSize;
		std::string myUploadAccessPasswd;
		std::string myUploadEditPasswd;
		int myUploadNumOfSeg;
		std::string myUploadFileCRC;

		int selected;
		int segments;
		std::string myUploadMailbox;
		std::string myLogin;
                std::string myPasswd;
                std::string myDownPasswd;
                std::string myEditPasswd ;
                std::vector<std::string> address;

};

#endif
