/***************************************************************************
                          GrammarInfo.cpp
                          -------------------
    begin                : Sat Jun 1 2002
    copyright            : (C) 2002 by Manuel Astudillo
    email                : d00mas@efd.lth.se
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/
#include "stdafx.h"
#include "GrammarInfo.h"

GrammarInfo::GrammarInfo () {
	name = NULL;
	author = NULL;
	version = NULL;
	about = NULL;
}


GrammarInfo::~GrammarInfo () {
	delete [] name;
	delete [] author;
	delete [] version;
	delete [] about;
}

