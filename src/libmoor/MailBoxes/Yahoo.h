/*
 * This file is a part of Moorie.
 *
 * Copyright (C) 2007 Pawel Stolowski <pawel.stolowski@wp.pl>
 *
 * Moorie is free software; you can redestribute it and/or modify it
 * under terms of GNU General Public License by Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY. See GPL for more details.
 */

#ifndef __YAHOO_H
#define __YAHOO_H

#include "../Mailbox.h"
#include <string>

//! Mailbox implementation for yahoo.com
class YahooMailbox: public CMailBox
{
	private:
		std::string auth;
		std::string uvar;
		std::string challenge;
		std::string mserv;
		int totalEmails;
		int curEmail;
		int pgcnt;

	public:
		YahooMailbox(const std::string &usr, const std::string &passwd);
		~YahooMailbox();
		int loginRequest();
		void logoutRequest();
		void getHeadersRequest();
		int downloadRequest(int seg);
		int uploadRequest(std::string filename, std::string to);

		void parseResponse();
};

#endif

