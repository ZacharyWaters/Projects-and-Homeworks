#!/bin/bash

if [ `cat list.h | md5sum | cut -f1 -d' '` != "f252a54aac3439531bb0c38b12ff4465" ]; then
	echo "list.h has been modified. Please re-download it from assignments"
	exit 1
fi

if [ `cat circ_list.h | md5sum | cut -f1 -d' '` != "e185c514dab99727662ed500299a905a" ]; then
	echo "circ_list.h has been modified. Please re-download it from assignments"
	exit 1
fi

if [ `cat array_list.h | md5sum | cut -f1 -d' '` != "e978ab57efb3f10cfa3172d8ab82e7b5" ]; then
	echo "array_list.h has been modified. Please re-download it from assignments"
	exit 1
fi

