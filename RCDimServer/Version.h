/*
 * Version.h
 *
 *  Created on: 18 Jun 2014
 *      Author: ncl
 */

#ifndef VERSION_H_
#define VERSION_H_

typedef struct NA62Version {
	const int fMajor;
	const int fMinor;
	const int fPatch;
} NA62Version_t;

static NA62Version_t server_version = {2,3,0};

#endif /* VERSION_H_ */
