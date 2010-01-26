#include "LibMoor.h"

#include <iostream>

#include "Log.h"
#include "MailboxFactory.h"
#include "HashManager.h"

const int segDownloadTries = 2;

CLibMoor::CLibMoor()
	: mySeg(0),
		selected(0),
		downloadDone(false),
		uploadDone(false)
{
}

CLibMoor::~CLibMoor() { }

bool CLibMoor::Dehash(const std::string& hashcode) {
	myHash = boost::shared_ptr<Hash>(HashManager::fromString(hashcode));
	if (myHash->getInfo().valid) {
		int vector_size = myHash->getInfo().accounts.size();
		for (int i = 0; i < vector_size; ++i) {
			std::cout << i+1 << " " << myHash->getInfo().accounts[i].name << std::endl;
		}
	}

	return myHash->getInfo().valid;
}

int CLibMoor::selectDownloadMailBox(int MailBox, std::string path) {
	if((path.find_last_of("/") != 0) && (path.length() > 1))
		path += "/";

	LOG(Log::Info, boost::format("Pobieranie do %1%") %path);
        mySeg = getLastSegment(path + myHash->getInfo().fileName);
        if (mySeg == myHash->getInfo().numOfSegments) {
            LOG(Log::Info, "Plik pobrano w calosci, przerywam...");
            return 1;
        }

	int vector_size = myHash->getInfo().accounts.size();

	if (MailBox > vector_size -1) {
		selected = 0;
		LOG(Log::Info ,"Skrzynka nie istnieje, wybieram pierwsza z listy");
	}
	else
		selected = MailBox;
	/*for (int i = 0; i < vector_size; ++i) {
		LOG(Log::Debug, boost::format( "%1%. ID: %2% L: %3% P: %4%" ) %i
										%myHash->getInfo().accounts[i].name
										%myHash->getInfo().accounts[i].login
										%myHash->getInfo().accounts[i].password);
	}*/

	int tries = 1;
	while (tries <= myHash->getInfo().accounts.size() && downloadDone == false) {
		std::string mailbox = myHash->getInfo().accounts[selected].name;
		std::string login = myHash->getInfo().accounts[selected].login;
		std::string passwd = myHash->getInfo().accounts[selected].password;

//		validMailbox = true;

		myMailBox = MailboxFactory::Instance().Create(mailbox, login, passwd);
		if (myMailBox) {
			LOG(Log::Info, boost::format( "Logowanie do:  %1%" )
			               %myHash->getInfo().accounts[selected].name);
			if (myMailBox->loginRequest() == 0) {
				myMailBox->setFileName(path + myHash->getInfo().fileName);
				myMailBox->setFileCRC(myHash->getInfo().crc);
				LOG(Log::Info, "Zalogowano pomyslnie...");
				LOG(Log::Info, "Sprawdzanie listy segmentow...");
				myMailBox->getHeadersRequest();
				unsigned int segments = myMailBox->countAvailableSegments(mySeg);
				if (segments == 0) {
					LOG(Log::Info, "Found none new segments.");
				}
				else {
					LOG(Log::Info, "Found new segments. Downloading...");
					if (startDownload() == 0) {
						LOG(Log::Info, boost::format("Pobranie segmentu %1% nie powiodlo sie... Przelaczanie skrzynki...") %(mySeg + 1) );
					}
				}
			}
			else {
				LOG(Log::Info, "Logowanie nie powiodlo sie..." );
			}
		}
		// Program should never reach this execution path, if decoder is
		// implemented properly!
		else {
			LOG(Log::Info, "Wybrana skrzynka nie jest obsługiwana w tej wersji programu.");
		}

		if (tries >= myHash->getInfo().accounts.size()) {
			LOG(Log::Info, "Nie udalo sie pobrac pliku z zadnej ze skrzynek... Koncze program." );
			downloadDone = true;
			delete myMailBox;
			break;
		}

		if (++selected >= vector_size)
				selected = 0;
		++tries;
	}

//	myMailBox -> Login();

	return 0;
}

int CLibMoor::startDownload() {
	bool segValid = true;
	int curSeg = mySeg;
	while (segValid && (mySeg < myHash->getInfo().numOfSegments)) {
		++curSeg;
		LOG(Log::Info, boost::format( "Sciaganie segmentu: %1%/%2%" )
		               %curSeg
		               %myHash->getInfo().numOfSegments);
		segValid = false;
		for (int i = 0; i < segDownloadTries && !segValid; ++i) {
			if (myMailBox->downloadRequest(curSeg) == 0) {
				segValid = true;
				mySeg = curSeg;
			}
		}
	}

	if (segValid) {
		LOG(Log::Info, "Wszystkie segmenty sciagnieto pomyslnie... Koncze pobieranie.");
		downloadDone = true;
	}

	return segValid;
}

int CLibMoor::selectUploadMailBox(int id, std::string login, std::string passwd) {
// 	myUploadMailbox = getMailboxName(id);

	myLogin = login;
	myPasswd = passwd;
	myUploadMailbox = "gmail.com"; // TODO - wybieranie skrzynki po id;
	return 0;
}

int CLibMoor::splitFile(std::string filename, int size) {
	myUploadFilename = filename;
	LOG(Log::Info, boost::format("Dzielenie pliku %1% na segmenty") % filename);
	int mysegsize = size*1024*1024;
	int bytes = 0; int read = 0;
	segments = 1;
	char buffer[128*1024];
	std::stringstream ss;
	ss << filename << "." << segments;
	std::cout << ss.str() << std::endl;

	std::ifstream in(filename.c_str(), std::ifstream::binary);
	std::ofstream *out = new std::ofstream(ss.str().c_str(), std::ios::out | std::ofstream::binary);
	LOG(Log::Debug, boost::format( "Seg: %1%" )	%segments);

	while (!in.eof()) {
		in.read(buffer, 128*1024);
		read = in.gcount();
		out->write(buffer, read);
		bytes += read;

// 		std::cout << "Read: " << read << std::endl;

		if (bytes == mysegsize) {
			bytes = 0;
			segments++;
			delete out;
 			ss.str("");
			ss << filename << "." << segments;
			out = new std::ofstream(ss.str().c_str(), std::ios::out | std::ofstream::binary);
			LOG(Log::Debug, boost::format( "Seg: %1%" )	%segments);
		}
	}
	return 0;

}

int CLibMoor::startUpload() {
	LOG(Log::Info, boost::format("Zaczynam upload"));

	std::stringstream ss;
	std::string address = myLogin+"@"+myUploadMailbox;

	myMailBox = MailboxFactory::Instance().Create(myUploadMailbox, myLogin, myPasswd); // TODO - zmienic "mail.ru" na wybrana skrzynke
	if (myMailBox) {
// 		LOG(Log::Info, boost::format( "Logowanie do:  %1%" ) myUploadMailbox);
		LOG(Log::Info, boost::format( "Logowanie do: ...") );
 		if (myMailBox->loginRequest() == 0) {
			myMailBox->calculateFileCRC(myUploadFilename);
			LOG(Log::Info, boost::format( "CRC Pliku: %1%" )	%myMailBox->getFileCRC());
			for (int i=1; i <= segments; i++) {
				LOG(Log::Info, boost::format( "Upload segmentu: %1%" )	%i);

				ss.str("");
				ss << myUploadFilename << "." << i;
				if (myMailBox->uploadRequest(ss.str(), address, i) == 0)
					LOG(Log::Info, boost::format( "Segment %1% wrzucony" )	%i);
				else
					LOG(Log::Error, boost::format( "Nie udalo sie wrzucic segmentu nr %1% " )	%i);
			}
			LOG(Log::Info, boost::format( "Upload zakonczony!" ));
 		} else
 			LOG(Log::Info, boost::format( "Logowanie nie powiodlo sie, przerywam." ));
	}

	return 0;
}


Status CLibMoor::getStatus() {
	Status s(mySeg, myMailBox->getSpeed(), myMailBox->getBytesRead(),
	         myHash->getInfo().accounts[selected].name);
	return s;
}

unsigned int CLibMoor::getLastSegment(const std::string& filePath) {
	unsigned int segment = 0;
	if (boost::filesystem::exists(filePath)) {
		unsigned int filesize = boost::filesystem::file_size(filePath);
		if (myHash->getInfo().segmentSize != 0) {
			segment = filesize / myHash->getInfo().segmentSize;
		}
		else {
			int currSize = 0;
			std::vector<int>::const_iterator it = myHash->getInfo().segmentSizes.begin();
			for (; it != myHash->getInfo().segmentSizes.end(); ++it) {
				if ((currSize += *it) < filesize)	break;
				else ++segment;
			}
		}
	}

	return segment;
}
